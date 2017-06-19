#include "SIAlembicRegister.h"

using namespace Alembic;

SICALLBACK Alembic_Import_Init( CRef& in_ctxt )
{
	Context ctxt( in_ctxt );
	Command oCmd;
	oCmd = ctxt.GetSource();
	oCmd.PutDescription(L"");
	oCmd.EnableReturnValue(true);

	ArgumentArray oArgs;
	oArgs = oCmd.GetArguments();
	oArgs.Add(L"filename");
	oArgs.Add(L"normals");
	oArgs.Add(L"uvs");
	oArgs.Add(L"clusters");
	oArgs.Add(L"colors");
	return CStatus::OK;
}

SICALLBACK Alembic_Import_Execute( CRef& in_ctxt )
{
	Context ctxt( in_ctxt );
	CValueArray args = ctxt.GetAttribute(L"Arguments");
	
   // take care of the filename
   CString filename = (CString)args[0].GetAsText();
   if(filename.IsEmpty())
   {
      // let's see if we are in interactive mode
      if(Application().IsInteractive())
      {
         CComAPIHandler toolkit;
         toolkit.CreateInstance(L"XSI.UIToolkit");
         CComAPIHandler filebrowser(toolkit.GetProperty(L"FileBrowser"));
         filebrowser.PutProperty(L"InitialDirectory",Application().GetActiveProject().GetPath());
         filebrowser.PutProperty(L"Filter",L"Alembic Files(*.abc)|*.abc||");
         CValue returnVal;
         filebrowser.Call(L"ShowOpen",returnVal);
         filename = filebrowser.GetProperty(L"FilePathName").GetAsText();
         if(filename.IsEmpty())
            return CStatus::Abort;
      }
      else
      {
         Application().LogMessage(L"[Alembic 1.1] No filename specified.",siErrorMsg);
         return CStatus::InvalidArgument;
      }
   }
   Application().LogMessage(L"[Alembic 1.1] filename used: "+filename);

   // check if we have arguments
   if(args[1].GetAsText().IsEmpty())
   {
      // let's setup the property
      CustomProperty settings = Application().GetActiveSceneRoot().AddProperty(L"Alembic_Import_Settings");

      // inspect it
      CValueArray inspectArgs(5);
      CValue inspectResult;
      inspectArgs[0] = settings.GetFullName();
      inspectArgs[1] = L"";
      inspectArgs[2] = L"Alembic Import Settings";
      inspectArgs[3] = siModal;
      inspectArgs[4] = false;
      Application().ExecuteCommand(L"InspectObj",inspectArgs,inspectResult);
      
      // prepare for deletion
      inspectArgs.Resize(1);
      inspectArgs[0] = settings.GetFullName();
      if((bool)inspectResult)
      {
         Application().ExecuteCommand(L"DeleteObj",inspectArgs,inspectResult);
         return CStatus::Abort;
      }

      // retrieve the options
      args[1] = settings.GetParameterValue(L"normals");
      args[2] = settings.GetParameterValue(L"uvs");
      args[3] = settings.GetParameterValue(L"facesets");
   }

   // let's try to read this
   AlembicIArchive* archive = new AlembicIArchive(filename.GetAsciiString());

   // prepare values for the setexpr command
   CValueArray setExprArgs(2);
   CValue setExprReturn;
   setExprArgs[1] = L"fc";

   bool importNormals = (bool)args[1];
   bool importUVs = (bool)args[2];
   bool importClusters = (bool)args[3];
   bool importColors = (bool)args[4];

   // let's figure out which objects we have
   CRefArray ops;
   std::vector<Alembic::Abc::IObject> objects;
   objects.push_back(archive->GetArchive()->getTop());
   for(size_t i=0;i<objects.size();i++)
   {
      // first, let's recurse
      for(size_t j=0;j<objects[i].getNumChildren();j++)
         objects.push_back(objects[i].getChild(j));
   }

   ProgressBar prog;
   prog = Application().GetUIToolkit().GetProgressBar();
   prog.PutMinimum(0);
   prog.PutMaximum((LONG)objects.size());
   prog.PutValue(0);
   prog.PutCancelEnabled(true);
   prog.PutVisible(true);

   Application().LogMessage(L" Nb Objects in Archive : "+(CString)(ULONG)objects.size());

   Alembic::AbcCoreAbstract::TimeSamplingPtr timeSampling;
	int nSamples=0;


   for(size_t i=0;i<objects.size();i++)
   {
      prog.PutCaption(L"Importing "+CString(objects[i].getFullName().c_str())+L" ...");
	  Application().LogMessage(L"Importing "+CString(objects[i].getFullName().c_str())+L" ...");

      // now let's see what we have here
	  if(Alembic::AbcGeom::ICamera::matches(objects[i].getMetaData()))
      {
         Abc::IObject parent = objects[i].getParent();
         std::string name = objects[i].getName();
         if(AbcGeom::IXform::matches(parent.getMetaData()))
            name = parent.getName();

         // let's create a camera
         Camera camera;
         Application().GetActiveSceneRoot().AddCamera(L"Camera",name.c_str(),camera);

         // delete the interest
         CValueArray deleteArgs(1);
         deleteArgs[0] = camera.GetInterest().GetFullName();
         Application().ExecuteCommand(L"DeleteObj",deleteArgs,setExprReturn);

         // let's setup the xform op
         if(AbcGeom::IXform::matches(parent.getMetaData()))
         {
            CustomOperator op = Application().GetFactory().CreateObject(L"Alembic_XForm");
            op.AddOutputPort(camera.GetKinematics().GetGlobal().GetRef());
            op.AddInputPort(camera.GetKinematics().GetGlobal().GetRef());
            op.Connect();
            ops.Add(op.GetRef());
            op.PutParameterValue(L"path",filename);
            op.PutParameterValue(L"identifier",CString(parent.getFullName().c_str()));
            setExprArgs[0] = op.GetFullName()+L".frame";
            Application().ExecuteCommand(L"SetExpr",setExprArgs,setExprReturn);
         }
         
         // let's setup the camera op
         CustomOperator op = Application().GetFactory().CreateObject(L"Alembic_Camera");
         op.AddOutputPort(camera.GetActivePrimitive().GetRef());
         op.AddInputPort(camera.GetActivePrimitive().GetRef());
         op.Connect();
         ops.Add(op.GetRef());
         op.PutParameterValue(L"path",filename);
         op.PutParameterValue(L"identifier",CString(objects[i].getFullName().c_str()));
         setExprArgs[0] = op.GetFullName()+L".frame";
         Application().ExecuteCommand(L"SetExpr",setExprArgs,setExprReturn);
      }
      else if(AbcGeom::IPolyMesh::matches(objects[i].getMetaData()))
      {
		 
         Abc::IObject parent = objects[i].getParent();
         std::string name = objects[i].getName();
         if(AbcGeom::IXform::matches(parent.getMetaData()))
            name = parent.getName();

         // let's create a mesh
         AbcGeom::IPolyMesh meshIObject(objects[i],Abc::kWrapExisting);
         AbcGeom::IPolyMeshSchema meshSchema = meshIObject.getSchema();
         AbcGeom::IPolyMeshSchema::Sample meshSample = meshSchema.getValue();

		 if(importColors){
			 AbcGeom::ICompoundProperty argGeomParamsProp = meshSchema.getArbGeomParams();
			
			 Application().LogMessage(L">>>>>>>>>>> Alembic Import "+(CString)name.c_str());
			 Application().LogMessage(L" -Nb Properties : "+(CString)argGeomParamsProp.getNumProperties());
			 for(ULONG i=0;i<(ULONG)argGeomParamsProp.getNumProperties();i++){
				  AbcGeom::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);
				  Application().LogMessage(L"Property Name : "+(CString)h.getName().c_str());
				  Application().LogMessage(L"Data Type Num Bytes : "+(CString)h.getDataType().getNumBytes());
				  AbcCoreAbstract::DataType dt = h.getDataType();

				  if(h.getName() == "Colors"){
					Alembic::Abc::IC4fArrayProperty colors(argGeomParamsProp,h.getName());
					Application().LogMessage(L"Colors Num Samples : "+(CString) colors.getNumSamples());
					Alembic::Abc::C4fArraySamplePtr propPtr1 = colors.getValue(0);
					ULONG nbp = (ULONG)propPtr1->size();
					Application().LogMessage(L"Sample Datas Size : "+(CString)nbp);
					Imath::Color4f color;
					for(ULONG i=0;i<nbp;i++){
						color = propPtr1->get()[i];
						Application().LogMessage(L"Vertex ID "+(CString)i+L" -------> ("+(CString)color.r+L","+(CString)color.g+L","+(CString)color.b+L")");
					}
				  }
			 }
		 }
		 

         // prepare the mesh
         Abc::P3fArraySamplePtr meshPos = meshSample.getPositions();
         Abc::Int32ArraySamplePtr meshFaceCount = meshSample.getFaceCounts();
         Abc::Int32ArraySamplePtr meshFaceIndices = meshSample.getFaceIndices();

         CVector3Array pos((LONG)meshPos->size());
         CLongArray polies((LONG)(meshFaceCount->size() + meshFaceIndices->size()));
         for(size_t j=0;j<meshPos->size();j++)
         {
            pos[(LONG)j].Set(meshPos->get()[j].x,meshPos->get()[j].y,meshPos->get()[j].z);
#ifdef _DEBUG
            if(j == 0)
               Application().LogMessage(L"first pos: "+CString(pos[(LONG)j].GetX())+L" : "+CString(pos[(LONG)j].GetY())+L" : "+CString(pos[(LONG)j].GetZ()));
#endif
         }
         LONG offset1 = 0;
         Abc::int32_t offset2 = 0;
#ifdef _DEBUG
         LONG meshFaceCountLong = (LONG)meshFaceCount->size();
         LONG meshFaceIndicesLong = (LONG)meshFaceIndices->size();
#endif
         for(size_t j=0;j<meshFaceCount->size();j++)
         {
            Abc::int32_t singleFaceCount = meshFaceCount->get()[j];
            polies[offset1++] = singleFaceCount;
            offset2 += singleFaceCount;
            for(size_t k=0;k<singleFaceCount;k++)
            {
               polies[offset1++] = meshFaceIndices->get()[(size_t)offset2 - 1 - k];
            }
         }

         X3DObject meshObj;
         Application().GetActiveSceneRoot().AddPolygonMesh(pos,polies,name.c_str(),meshObj);
         PolygonMesh meshGeo = meshObj.GetActivePrimitive().GetGeometry();

		 AbcGeom::IV2fGeomParam meshUVsParam = meshSchema.getUVsParam();
		if(meshUVsParam.valid())
         {
            Abc::V2fArraySamplePtr meshUVs = meshUVsParam.getExpandedValue(0).getVals();
            if(meshUVs->size() > 0)
            {
				//meshUVs->
			}
		}
		 /*
         // check the face sets
         if(importClusters)
         {
            std::vector<std::string> faceSetNames;
            meshSchema.getFaceSetNames(faceSetNames);
            for(size_t j=0;j<faceSetNames.size();j++)
            {
               AbcGeom::IFaceSetSchema faceSet = meshSchema.getFaceSet(faceSetNames[j]).getSchema();
               AbcGeom::IFaceSetSchema::Sample faceSetSample = faceSet.getValue();
               Abc::Int32ArraySamplePtr faces = faceSetSample.getFaces();
               CLongArray elements((LONG)faces->size());
               for(size_t k=0;k<faces->size();k++)
                  elements[(LONG)k] = (LONG)faces->get()[k];
               Cluster cluster;
               meshGeo.AddCluster(L"poly",CString(faceSetNames[j].c_str()),elements,cluster);
            }
         }

         // let's setup the xform op
         if(AbcGeom::IXform::matches(parent.getMetaData()))
         {
            CustomOperator op = Application().GetFactory().CreateObject(L"alembic_xform");
            op.AddOutputPort(meshObj.GetKinematics().GetGlobal().GetRef());
            op.AddInputPort(meshObj.GetKinematics().GetGlobal().GetRef());
            op.Connect();
            ops.Add(op.GetRef());
            op.PutParameterValue(L"path",filename);
            op.PutParameterValue(L"identifier",CString(parent.getFullName().c_str()));
            setExprArgs[0] = op.GetFullName()+L".frame";
            Application().ExecuteCommand(L"SetExpr",setExprArgs,setExprReturn);
         }
         
         // let's setup the positions op
         CustomOperator op = Application().GetFactory().CreateObject(L"alembic_polymesh");
         op.AddOutputPort(meshObj.GetActivePrimitive().GetRef());
         op.AddInputPort(meshObj.GetActivePrimitive().GetRef());
         op.Connect();
         ops.Add(op.GetRef());
         op.PutParameterValue(L"path",filename);
         op.PutParameterValue(L"identifier",CString(objects[i].getFullName().c_str()));
         setExprArgs[0] = op.GetFullName()+L".frame";
         Application().ExecuteCommand(L"SetExpr",setExprArgs,setExprReturn);

         // let's setup the normals op
         AbcGeom::IN3fGeomParam meshNormalsParam = meshSchema.getNormalsParam();
         if(meshNormalsParam.valid() && importNormals)
         {
            Abc::N3fArraySamplePtr meshNormals = meshNormalsParam.getExpandedValue(0).getVals();
            if(meshNormals->size() > meshPos->size()) // don't do this for vertex normals
            {
               // create user normals
               CValueArray createUserNormalArgs(1);
               createUserNormalArgs[0] = meshObj.GetFullName();
               Application().ExecuteCommand(L"CreateUserNormals",createUserNormalArgs,setExprReturn);
               ClusterProperty userNormalProp;
               CRefArray clusters = meshObj.GetActivePrimitive().GetGeometry().GetClusters();
               for(LONG j=0;j<clusters.GetCount();j++)
               {
                  Cluster cluster(clusters[j]);
                  if(!cluster.GetType().IsEqualNoCase(L"sample"))
                     continue;
                  CRefArray props(cluster.GetLocalProperties());
                  for(LONG k=0;k<props.GetCount();k++)
                  {
                     ClusterProperty prop(props[k]);
                     if(prop.GetType().IsEqualNoCase(L"normal"))
                     {
                        userNormalProp = props[k];
                        break;
                     }
                  }
                  if(userNormalProp.IsValid())
                     break;
               }
               if(userNormalProp.IsValid())
               {
                  // we found it, and we need to attach the op
                  CustomOperator op = Application().GetFactory().CreateObject(L"alembic_normals");
                  op.AddOutputPort(userNormalProp.GetRef());
                  op.AddInputPort(userNormalProp.GetRef());
                  op.Connect();
                  ops.Add(op.GetRef());
                  op.PutParameterValue(L"path",filename);
                  op.PutParameterValue(L"identifier",CString(objects[i].getFullName().c_str()));
                  setExprArgs[0] = op.GetFullName()+L".frame";
                  Application().ExecuteCommand(L"SetExpr",setExprArgs,setExprReturn);
               }
            }
         }
         // let's setup the normals op
         AbcGeom::IV2fGeomParam meshUVsParam = meshSchema.getUVsParam();
         if(meshUVsParam.valid() && importUVs)
         {
            Abc::V2fArraySamplePtr meshUVs = meshUVsParam.getExpandedValue(0).getVals();
            if(meshUVs->size() > 0)
            {
               // create user normals
               CValueArray createProjectionArgs(3);
               createProjectionArgs[0] = meshObj.GetFullName();
               createProjectionArgs[1] = siTxtSpatial;
               createProjectionArgs[2] = siTxtDefaultPlanarXY;
               Application().ExecuteCommand(L"CreateProjection",createProjectionArgs,setExprReturn);
               ClusterProperty uvProp;
               CRefArray clusters = meshObj.GetActivePrimitive().GetGeometry().GetClusters();
               for(LONG j=0;j<clusters.GetCount();j++)
               {
                  Cluster cluster(clusters[j]);
                  if(!cluster.GetType().IsEqualNoCase(L"sample"))
                     continue;
                  CRefArray props(cluster.GetLocalProperties());
                  for(LONG k=0;k<props.GetCount();k++)
                  {
                     ClusterProperty prop(props[k]);
                     if(prop.GetType().IsEqualNoCase(L"uvspace"))
                     {
                        uvProp = props[k];
                        break;
                     }
                  }
                  if(uvProp.IsValid())
                     break;
               }
               if(uvProp.IsValid())
               {
                  // we found it, and we need to attach the op
                  CustomOperator op = Application().GetFactory().CreateObject(L"alembic_uvs");
                  op.AddOutputPort(uvProp.GetRef());
                  op.AddInputPort(uvProp.GetRef());
                  op.Connect();
                  ops.Add(op.GetRef());
                  op.PutParameterValue(L"path",filename);
                  op.PutParameterValue(L"identifier",CString(objects[i].getFullName().c_str()));
                  // we don't put an expression, since UVs typically don't animate
               }
            }
         }*/
      }
	
      if(prog.IsCancelPressed())
         break;
      prog.Increment();
   }

   prog.PutVisible(false);

   // check if we have any ops, if so let's set them up
   if(ops.GetCount() > 0)
      abc_archive_manager.AddArchive(archive);
   else
      delete(archive);

   return CStatus::OK;
}


SICALLBACK Alembic_Import_Settings_Define( CRef& in_ctxt )
{
	Context ctxt( in_ctxt );
	CustomProperty oCustomProperty;
	Parameter oParam;
	oCustomProperty = ctxt.GetSource();

   // get the current frame in an out
	oCustomProperty.AddParameter(L"normals",CValue::siBool,siPersistable,L"",L"",0,0,1,0,1,oParam);
   oCustomProperty.AddParameter(L"uvs",CValue::siBool,siPersistable,L"",L"",1,0,1,0,1,oParam);
   oCustomProperty.AddParameter(L"facesets",CValue::siBool,siPersistable,L"",L"",1,0,1,0,1,oParam);
	return CStatus::OK;
}

SICALLBACK Alembic_Import_Settings_DefineLayout( CRef& in_ctxt )
{
	Context ctxt( in_ctxt );
	PPGLayout oLayout;
	PPGItem oItem;
	oLayout = ctxt.GetSource();
	oLayout.Clear();

   CValueArray normalItems(6);
   oLayout.AddGroup(L"Geometry");
   oLayout.AddItem(L"normals",L"Normals");
   oLayout.AddItem(L"uvs",L"UVs");
   oLayout.AddItem(L"facesets",L"Clusters");
   oLayout.EndGroup();

	return CStatus::OK;
}