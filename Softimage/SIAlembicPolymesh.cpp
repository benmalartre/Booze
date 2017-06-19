//-----------------------------------------------------------------
// SIAlembicPolymesh Custom Operator
//-----------------------------------------------------------------
#include "SIAlembicRegister.h"
#include "SIAlembicPolymesh.h"
#include "SIAlembicXForm.h"

SIAlembicOPolymesh::SIAlembicOPolymesh(const XSI::CRef & in_ref, SIAlembicWriteJob * in_job)
:SIAlembicOObject(in_ref, in_job)
{
	Primitive prim = GetRef();
	CString xformName(prim.GetParent3DObject().GetName());
	CString meshName(xformName+L"Shape");

	Alembic::AbcGeom::OXform xform(GetAlembicJob()->GetArchive().getTop(),xformName.GetAsciiString(),GetAlembicJob()->GetAnimatedTs());
	Alembic::AbcGeom::OPolyMesh mesh(xform,meshName.GetAsciiString(),GetAlembicJob()->GetAnimatedTs());

	_xformschema = xform.getSchema();
	_meshschema = mesh.getSchema();
	_numsamples = 0;

}


void SIAlembicIPolymesh::Check()
{
	if(!Alembic::AbcGeom::IPolyMesh::matches(AlembicIObject::GetIObject()->getMetaData()))
	{
		Clean();
	}
}
struct SortEnvelopeWeights{
    bool operator()(const std::pair<LONG,float> &left, const std::pair<LONG,float> &right) {
        return left.second > right.second;
    }
};
void EncodeEnvelope(CFloatArray& weights,Imath::C4c idx, Imath::C4f wgt,LONG ID,LONG nb_deformers)
{
	std::vector<std::pair<LONG,float>> weightID;
	weightID.resize(nb_deformers);
	CString s_weight = L"Vertex ID "+(CString)ID+L" :: ";
	for(LONG i=0;i<nb_deformers;i++){
		weightID[i].first = i;
		weightID[i].second = weights[ID*nb_deformers+i];
		s_weight +=(CString)weightID[i].second+L",";
	}
	//sort by value using std::sort
	std::sort(weightID.begin(), weightID.end(),SortEnvelopeWeights() );
	idx.r = (char)weightID[0].first;
	idx.g = (char)weightID[1].first;
	idx.b = (char)weightID[2].first;
	idx.a = (char)weightID[3].first;

	wgt.r = weightID[0].second;
	wgt.g = weightID[1].second;
	wgt.b = weightID[2].second;
	wgt.a = weightID[3].second;

	Application().LogMessage(s_weight);
}

ABCStatus SIAlembicOPolymesh::Save(double time)
{
	Application app;
	app.LogMessage(L" Save Polymesh At FRame "+(CString)time);
   // store the transform
   Primitive prim(GetRef());
   app.LogMessage(L" Primitive ;: "+prim.GetFullName());

   SaveXFormSample(prim.GetParent3DObject().GetKinematics().GetGlobal().GetRef(),_xformschema,_xformsample,time);

   // access the mesh
   PolygonMesh mesh = prim.GetGeometry(time);
   CVector3Array pos = mesh.GetVertices().GetPositionArray();
   CPolygonFaceRefArray faces = mesh.GetPolygons();
   LONG vertCount = pos.GetCount();
   LONG faceCount = faces.GetCount();
   LONG sampleCount = mesh.GetSamples().GetCount();

   Alembic::AbcGeom::OPolyMeshSchema schema = _meshschema;
	Alembic::Abc::OCompoundProperty argGeomParamsProp = _meshschema.getArbGeomParams();

   app.LogMessage(L"Vertices count : "+(CString)vertCount);
   app.LogMessage(L"Faces count : "+(CString)faceCount);
   app.LogMessage(L"Samples count : "+(CString)sampleCount);

   // allocate the points and normals
   std::vector<Alembic::Abc::V3f> posVec(vertCount);
   for(LONG i=0;i<vertCount;i++)
   {
      posVec[i].x = (float)pos[i].GetX();
      posVec[i].y = (float)pos[i].GetY();
      posVec[i].z = (float)pos[i].GetZ();
   }

   LONG offset = 0;
   std::vector<Alembic::Abc::N3f> normalVec;
   std::vector<Alembic::Abc::V2f> uvVec;
   SIAlembicWriteJob* job = this->GetAlembicJob();

   if(job->GetOption("exportNormals") == "true")
   {
	   Application().LogMessage(L"Export Normal Enabled");
      CVector3Array normals = mesh.GetVertices().GetNormalArray();
      if(job->GetOption("vertexNormals") == "true")
      {
         normalVec.resize(vertCount);
         for(LONG i=0;i<vertCount;i++)
         {
            normalVec[i].x = (float)normals[i].GetX();
            normalVec[i].y = (float)normals[i].GetY();
            normalVec[i].z = (float)normals[i].GetZ();
         }
      }
      else
      {
         CGeometryAccessor accessor = mesh.GetGeometryAccessor(siConstructionModeSecondaryShape);
         CFloatArray shadingNormals;
         accessor.GetNodeNormals(shadingNormals);
         normalVec.resize(shadingNormals.GetCount() / 3);

         // we need to loop the faces, there is nothing we can do
         LONG offset = 0;
         for(LONG i=0;i<faces.GetCount();i++)
         {
            PolygonFace face(faces[i]);
            CLongArray samples = face.GetSamples().GetIndexArray();
            for(LONG j=samples.GetCount()-1;j>=0;j--)
            {
               LONG sampleIndex = samples[j];
               normalVec[sampleIndex].x = (float)shadingNormals[offset++];
               normalVec[sampleIndex].y = (float)shadingNormals[offset++];
               normalVec[sampleIndex].z = (float)shadingNormals[offset++];
            }
         }
      }
   }

   // allocate for the points and normals
   Alembic::Abc::P3fArraySample posSample(&posVec.front(),posVec.size());

   // if we are the first frame!
   if(_numsamples == 0)
   {
      // we also need to store the face counts as well as face indices
	   std::vector<Alembic::Abc::int32_t> faceCountVec(faceCount);
      std::vector<Alembic::Abc::int32_t> faceIndicesVec(sampleCount);
      offset = 0;
      for(LONG i=0;i<faceCount;i++)
      {
         PolygonFace face(faces[i]);
         CLongArray indices = face.GetVertices().GetIndexArray();
         faceCountVec[i] = indices.GetCount();
         for(LONG j=indices.GetCount()-1;j>=0;j--)
            faceIndicesVec[offset++] = indices[j];
      }

      Alembic::Abc::Int32ArraySample faceCountSample(&faceCountVec.front(),faceCountVec.size());
      Alembic::Abc::Int32ArraySample faceIndicesSample(&faceIndicesVec.front(),faceIndicesVec.size());

      _meshsample.setPositions(posSample);
      if(normalVec.size() > 0)
      {
         Alembic::AbcGeom::ON3fGeomParam::Sample normalSample(Alembic::Abc::N3fArraySample(&normalVec.front(),normalVec.size()),Alembic::AbcGeom::kFacevaryingScope);
         _meshsample.setNormals(normalSample);
      }
      _meshsample.setFaceCounts(faceCountSample);
      _meshsample.setFaceIndices(faceIndicesSample);

      // also check if we need to store UV
      CRefArray clusters = mesh.GetClusters();
	  
	  if(job->GetOption("exportUVs")=="true")

      {
         CRef uvPropRef;
         for(LONG i=0;i<clusters.GetCount();i++)
         {
            Cluster cluster(clusters[i]);
            if(!cluster.GetType().IsEqualNoCase(L"sample"))
               continue;
            CRefArray props(cluster.GetLocalProperties());
            for(LONG k=0;k<props.GetCount();k++)
            {
               ClusterProperty prop(props[k]);
               if(prop.GetType().IsEqualNoCase(L"uvspace"))
               {
                  uvPropRef = props[k];
                  break;
               }
            }
            if(uvPropRef.IsValid())
               break;
         }
         if(uvPropRef.IsValid())
         {
			 Application().LogMessage(L"UV Prop Valid : Export It!!!");
            // ok, great, we found UVs, let's set them up
            uvVec.resize(sampleCount);
            CDoubleArray uvValues = ClusterProperty(uvPropRef).GetElements().GetArray();

            LONG offset = 0;
            for(LONG i=0;i<faces.GetCount();i++)
            {
               PolygonFace face(faces[i]);
               CLongArray samples = face.GetSamples().GetIndexArray();
               for(LONG j=samples.GetCount()-1;j>=0;j--)
               {
                  LONG sampleIndex = samples[j];
                  uvVec[sampleIndex].x = (float)uvValues[offset++];
                  uvVec[sampleIndex].y = (float)uvValues[offset++];
                  //offset++;
               }
            }

            Alembic::AbcGeom::OV2fGeomParam::Sample uvSample(Alembic::Abc::V2fArraySample(&uvVec.front(),uvVec.size()),Alembic::AbcGeom::kFacevaryingScope);
            _meshsample.setUVs(uvSample);
         }
      }
	  else{
		Application().LogMessage(L"Export UV Disabled...");
	  }

      // sweet, now let's have a look at face sets
	  std::vector<std::vector<Alembic::Abc::int32_t>> faceSetVecs;
      if(job->GetOption("exportFaceSets")=="true")
      {
         for(LONG i=0;i<clusters.GetCount();i++)
         {
            Cluster cluster(clusters[i]);
            if(!cluster.GetType().IsEqualNoCase(L"poly"))
               continue;

            CLongArray elements = cluster.GetElements().GetArray();
            if(elements.GetCount() == 0)
               continue;

            std::string name(cluster.GetName().GetAsciiString());

			faceSetVecs.push_back(std::vector<Alembic::Abc::int32_t>());
			std::vector<Alembic::Abc::int32_t> & faceSetVec = faceSetVecs.back();
            for(LONG j=0;j<elements.GetCount();j++)
               faceSetVec.push_back(elements[j]);

            Alembic::AbcGeom::OFaceSet faceSet = _meshschema.createFaceSet(name);
            Alembic::AbcGeom::OFaceSetSchema::Sample faceSetSample(Alembic::Abc::Int32ArraySample(&faceSetVec.front(),faceSetVec.size()));
            faceSet.getSchema().set(faceSetSample);
         }
      }

	  std::string export_color = job->GetOption("exportColors");
	   if(export_color=="1" || export_color=="2")
	   {
		   Application().LogMessage(L"Export Colorz!!!");
		   
		   
		   Alembic::Abc::OC4fArrayProperty colorProp(argGeomParamsProp, "Colors", this->AlembicOObject::GetMetaData(), job->GetAnimatedTs());

		   // Export ICE Color Attribute
		   if(export_color=="1"){
			   Application().LogMessage(L"Export ICE Color Attribute");
			   ICEAttribute color = prim.GetICEAttributeFromName("Color");
			   

			   if(color.IsDefined() && color.IsValid()){
				   CICEAttributeDataArrayColor4f data;
					color.GetDataArray( data );
					std::vector<Alembic::Abc::C4f> outputColorArray;
					
					for( ULONG i=0; i<data.GetCount( ); i++ )
					{
						Alembic::Abc::C4f c;
						c.r = data[i].GetR();
						c.g = data[i].GetG();
						c.b = data[i].GetB();
						c.a = 1.0;
						outputColorArray.push_back(c);
	
					}
					
					colorProp.set(Alembic::Abc::C4fArraySample(outputColorArray));
					
			   }
			   else{
				   Application().LogMessage(L"No COLOR ICE Attribute on "+prim.GetParent3DObject().GetFullName()+L" -----> Write Colors Skipped...");
			   }
		   }
		   // Export Vertex Color Map(if any...)
		   else{
				Application().LogMessage(L"Export Vertex Color Map");
				CGeometryAccessor accessor = mesh.GetGeometryAccessor(siConstructionModeSecondaryShape);
				CRefArray  colors = accessor.GetVertexColors();
				std::vector<Alembic::Abc::C4f> outputColorArray;

				if(colors.GetCount()>0){
					ClusterProperty cp = colors[0];
					CBitArray elemSet;
					CFloatArray colorValues;
					cp.GetValues(colorValues,elemSet);

					std::vector<Alembic::Abc::C4f> outputColorArray;

					outputColorArray.resize(sampleCount);;

					LONG offset = 0;
					for(LONG i=0;i<faces.GetCount();i++)
					{
					   PolygonFace face(faces[i]);
					   CLongArray samples = face.GetSamples().GetIndexArray();
					   for(LONG j=samples.GetCount()-1;j>=0;j--)
					   {
						  LONG sampleIndex = samples[j];
						  outputColorArray[sampleIndex].r = (float)colorValues[offset++];
						  outputColorArray[sampleIndex].g = (float)colorValues[offset++];
						  outputColorArray[sampleIndex].b = (float)colorValues[offset++];
						  outputColorArray[sampleIndex].a = (float)colorValues[offset++];
					   }
					}
						
					//}
				
					colorProp.set(Alembic::Abc::C4fArraySample(outputColorArray));
				}

				else{
				   Application().LogMessage(L"No Vertex Color Map on "+prim.GetParent3DObject().GetFullName()+L" -----> Write Colors Skipped...");
				}
		   }

	   }

	   if(job->GetOption("exportEnvelope") == "true")
	   {
		   // find envelope
			CGeometryAccessor accessor = mesh.GetGeometryAccessor();
			if(accessor.GetEnvelopeWeights().GetCount()>0)
			{
				EnvelopeWeight envelope = accessor.GetEnvelopeWeights().GetItem(0);
				CRefArray deformers = envelope.GetDeformers();
				CFloatArray weights;
				envelope.GetValues(weights);

				LONG out;
				Application().GetUIToolkit().MsgBox(L"Weights Size : "+(CString)weights.GetCount(),XSI::siMsgInformation,L"Export Envelope",out);
				Alembic::AbcGeom::OPolyMeshSchema schema = _meshschema;
				Alembic::Abc::OCompoundProperty argGeomParamsProp = _meshschema.getArbGeomParams();
				Alembic::AbcCoreAbstract::MetaData metadata = this->AlembicOObject::GetMetaData();
				Alembic::Abc::OC4cArrayProperty indicesProp(argGeomParamsProp, "EnvelopeIndices", metadata, job->GetAnimatedTs());
				Alembic::Abc::OC4fArrayProperty weightsProp(argGeomParamsProp, "EnvelopeWeights", metadata, job->GetAnimatedTs());
				Alembic::Abc::OUInt32Property nbDeformersProp(argGeomParamsProp, "EnvelopeNbDeformers", metadata, job->GetAnimatedTs());

				std::vector<Alembic::Abc::C4c> indicesArray;
				std::vector<Alembic::Abc::C4f> weightsArray;

				LONG nbp = weights.GetCount()/deformers.GetCount();
				indicesArray.resize(nbp);
				weightsArray.resize(nbp);

				Alembic::Abc::C4c idx;
				Alembic::Abc::C4f wgt;
				CString s_idx;
				CString s_wgt;
				for(LONG i=0;i<nbp;i++){
					EncodeEnvelope(weights,indicesArray[i], weightsArray[i],i,deformers.GetCount());
					s_idx = L"Vertex "+(CString)i+L" IDs :: ("+(CString)(LONG)indicesArray[i].r+L","+(CString)(LONG)indicesArray[i].g+L","+(CString)(LONG)indicesArray[i].b+L","+(CString)(LONG)indicesArray[i].a+L")";
					s_wgt = L"Weights "+(CString)i+L" Weights :: ("+(CString)weightsArray[i].r+L","+(CString)weightsArray[i].g+L","+(CString)weightsArray[i].b+L","+(CString)weightsArray[i].a+L")";
					Application().LogMessage(s_idx);
					Application().LogMessage(s_wgt);
				
				}

				indicesProp.set(Alembic::Abc::C4cArraySample(indicesArray));
				weightsProp.set(Alembic::Abc::C4fArraySample(weightsArray));
				nbDeformersProp.set(Alembic::Abc::uint32_t(deformers.GetCount()));
			}
		   /*
			LONG out;
		   Application().GetUIToolkit().MsgBox(L"Export Envelope",XSI::siMsgInformation,L"hohoho",out);
		   */
	   }

      _meshschema.set(_meshsample);
   }
   else
   {
		_meshsample.reset();
		_meshsample.setPositions(posSample);
		if(normalVec.size() > 0)
		{
			Alembic::AbcGeom::ON3fGeomParam::Sample normalSample(Alembic::Abc::N3fArraySample(&normalVec.front(),normalVec.size()),Alembic::AbcGeom::kFacevaryingScope);
			_meshsample.setNormals(normalSample);
		}
		_meshschema.set(_meshsample);
	
   }
   _numsamples++;


   return Status_OK;
}
/*
//---------------------------------------------------------
// Input Alembic Polymesh
//---------------------------------------------------------
SIIAlembicPolymesh::Init(ICENodeContext& in_ctxt)
{
	
}
*/
void SIAlembicIPolymesh::Update(ICENodeContext& in_ctxt)
{

}
void SIAlembicIPolymesh::Update(OperatorContext& in_ctxt)
{

}

// Define
//-----------------------------------------------------------------
XSIPLUGINCALLBACK CStatus Alembic_Polymesh_Define( CRef& in_ctxt )
{
   Context ctxt( in_ctxt );
   CustomOperator oCustomOperator;

   Parameter oParam;
   CRef oPDef;

   Factory oFactory = Application().GetFactory();
   oCustomOperator = ctxt.GetSource();

   oPDef = oFactory.CreateParamDef(L"frame",CValue::siInt4,siAnimatable | siPersistable,L"frame",L"frame",1,-100000,100000,0,1);
   oCustomOperator.AddParameter(oPDef,oParam);
   oPDef = oFactory.CreateParamDef(L"path",CValue::siString,siReadOnly | siPersistable,L"path",L"path",L"",L"",L"",L"",L"");
   oCustomOperator.AddParameter(oPDef,oParam);
   oPDef = oFactory.CreateParamDef(L"identifier",CValue::siString,siReadOnly | siPersistable,L"identifier",L"identifier",L"",L"",L"",L"",L"");
   oCustomOperator.AddParameter(oPDef,oParam);

   oCustomOperator.PutAlwaysEvaluate(false);
   oCustomOperator.PutDebug(0);

   return CStatus::OK;

}

// Define Layout
//-----------------------------------------------------------------
XSIPLUGINCALLBACK CStatus Alembic_Polymesh_DefineLayout( CRef& in_ctxt )
{
   Context ctxt( in_ctxt );
   PPGLayout oLayout;
   PPGItem oItem;
   oLayout = ctxt.GetSource();
   oLayout.Clear();
   return CStatus::OK;
}


// Init
//-----------------------------------------------------------------
XSIPLUGINCALLBACK CStatus AlembicPolymesh_Init( CRef& in_ctx )
{
	Context ctx(in_ctx) ;	
	SIAlembicIPolymesh* pThis = new SIAlembicIPolymesh();	
	ctx.PutUserData( (CValue::siPtrType)pThis );
	CustomOperator oOp = ctx.GetSource();
	return CStatus::OK;
}


// Term
//-----------------------------------------------------------------
XSIPLUGINCALLBACK CStatus AlembicPolymesh_Term( CRef& in_ctx)
{
	Context ctx(in_ctx) ;
	CValue::siPtrType pUserData = ctx.GetUserData();
	SIAlembicIPolymesh* pThis = (SIAlembicIPolymesh*)pUserData;
	pThis->Clean();
	delete pThis;
	return CStatus::OK;
}


// Update
//-----------------------------------------------------------------
XSIPLUGINCALLBACK CStatus AlembicPolyMesh_Update(CRef&	in_ctx)
{
	OperatorContext ctx(in_ctx) ;
	CValue::siPtrType pUserData = ctx.GetUserData();
	SIAlembicIPolymesh* pThis = (SIAlembicIPolymesh*)pUserData;	
	pThis->Update(ctx);
	return CStatus::OK;
}



//-------------------------------------------------------------
// ICE Node
//-------------------------------------------------------------
enum ABCPolymeshTopoCloudIDs
{
	ID_OUT_Vertices = 201,
	ID_OUT_PolygonData,
	ID_OUT_UVs,
	ID_OUT_Colors
};

CStatus RegisterAlembicIPolymeshTopo( PluginRegistrar& in_reg )
{
	ICENodeDef nodeDef;
	nodeDef = Application().GetFactory().CreateICENodeDef(L"AlembicIPolymeshTopo",L"AlembicIPolymeshTopo");

	CStatus st;
	st = nodeDef.PutColor(AlembicNodeR,AlembicNodeG,AlembicNodeB);
	st.AssertSucceeded( ) ;

	/*
	st = nodeDef.PutThreadingModel(siICENodeSingleThreading);
	st.AssertSucceeded( ) ;
	*/

	// Add input ports and groups.
	st = nodeDef.AddPortGroup(ID_G_100);
	st.AssertSucceeded( ) ;

	st = nodeDef.AddInputPort(ID_IN_Path,ID_G_100,siICENodeDataString,siICENodeStructureSingle,siICENodeContextSingleton,L"Path",L"Path",L"");
	st.AssertSucceeded( ) ;

	st = nodeDef.AddInputPort(ID_IN_Identifier,ID_G_100,siICENodeDataString,siICENodeStructureSingle,siICENodeContextSingleton,L"Identifier",L"Identifier",L"");
	st.AssertSucceeded( ) ;

	st = nodeDef.AddInputPort(ID_IN_Frame,ID_G_100,siICENodeDataFloat,siICENodeStructureSingle,siICENodeContextSingleton,L"Frame",L"Frame",0);
	st.AssertSucceeded( ) ;

	// Add output ports.
	st = nodeDef.AddOutputPort(ID_OUT_Vertices,siICENodeDataVector3,siICENodeStructureArray,siICENodeContextSingleton,L"Vertices",L"Vertices",ID_UNDEF,ID_UNDEF,ID_CTXT_CNS);
	st.AssertSucceeded( ) ;

	st = nodeDef.AddOutputPort(ID_OUT_PolygonData,siICENodeDataLong,siICENodeStructureArray,siICENodeContextSingleton,L"PolygonData",L"PolygonData",ID_UNDEF,ID_UNDEF,ID_CTXT_CNS);
	st.AssertSucceeded( ) ;

	PluginItem nodeItem = in_reg.RegisterICENode(nodeDef);
	nodeItem.PutCategories(L"Alembic");

	return CStatus::OK;
}

SICALLBACK AlembicIPolymeshTopo_Evaluate( ICENodeContext& in_ctxt )
{
	// Get User Data
	SIAlembicIPolymesh* mesh = (SIAlembicIPolymesh*)(CValue::siPtrType)in_ctxt.GetUserData( );
	Application().LogMessage(L"Polymesh Evaluate Called...");

	if(mesh->IsDirty(in_ctxt)||!mesh->SIAlembicIObject::Initialized())
	{
		Application().LogMessage(L"Init Polymesh");
		mesh->SIAlembicIObject::Init(in_ctxt);
		mesh->SIAlembicIObject::Check();
	}

	if(mesh && mesh->SIAlembicIObject::Initialized())
	{
		// The current output port being evaluated...
		LONG out_portID = in_ctxt.GetEvaluatedOutputPortID( );

		CDataArrayFloat frameData(in_ctxt, ID_IN_Frame);
		float frame = frameData[0];
		int iFrame = int(frame)-1;

		
		switch( out_portID )
		{	
			case ID_OUT_Vertices :
			{
				// let's create a mesh
				Application().LogMessage(L" Current Frame : "+(CString)iFrame);

				//Application().LogMessage(L"Object Name : "+(CString)obj->getFullName().c_str());
				
				Alembic::AbcGeom::IPolyMesh meshobject((*mesh->SIAlembicIObject::GetIObject()),Alembic::Abc::kWrapExisting);
				
				Alembic::AbcGeom::IPolyMeshSchema meshschema = meshobject.getSchema();
				Alembic::AbcGeom::IPolyMeshSchema::Sample meshsample = meshschema.getValue();
				
				// prepare the mesh
				Alembic::Abc::P3fArraySamplePtr meshPos = meshsample.getPositions();
				ULONG nbv = ((LONG)meshPos->size());

				CDataArray2DVector3f outData( in_ctxt );
				CDataArray2DVector3f::Accessor outSubData = outData.Resize(0,nbv);

				for(ULONG j=0;j<nbv;j++)
				{
					outSubData[j].Set(meshPos->get()[j].x,meshPos->get()[j].y,meshPos->get()[j].z);
				}
				
				
			}break;
			
			case ID_OUT_PolygonData :
			{

				// let's create a mesh
				Alembic::AbcGeom::IPolyMesh meshobject((*mesh->SIAlembicIObject::GetIObject()),Alembic::Abc::kWrapExisting);
				Alembic::AbcGeom::IPolyMeshSchema meshschema = meshobject.getSchema();
				Alembic::AbcGeom::IPolyMeshSchema::Sample meshsample = meshschema.getValue();

				Alembic::Abc::Int32ArraySamplePtr meshFaceCount = meshsample.getFaceCounts();
				 Alembic::Abc::Int32ArraySamplePtr meshFaceIndices = meshsample.getFaceIndices();

				 LONG nbpd = ((LONG)(meshFaceCount->size() + meshFaceIndices->size()));

				 CDataArray2DLong outData( in_ctxt );
				CDataArray2DLong::Accessor outSubData = outData.Resize(0,nbpd);

				 LONG offset1 = 0;
				Alembic::Abc::int32_t offset2 = 0;

				 for(size_t j=0;j<meshFaceCount->size();j++)
				 {
					Alembic::Abc::int32_t singleFaceCount = meshFaceCount->get()[j];
					offset2 += singleFaceCount;
					for(size_t k=0;k<singleFaceCount;k++)
					{
					   outSubData[offset1++] = meshFaceIndices->get()[(size_t)offset2 - 1 - k];
					}
					 outSubData[offset1++] = -2;
				 }
				

			}break;
			
		}
	}

	return CStatus::OK;
}


SICALLBACK AlembicIPolymeshTopo_Init( CRef& in_ctxt )
{
	//Get Context from CRef
	Context ctxt(in_ctxt);

	// Build a new SIIAlembicObject
	SIAlembicIPolymesh* mesh = new SIAlembicIPolymesh();

	// Store Object in User Data
	ctxt.PutUserData((CValue::siPtrType)mesh);
	return CStatus::OK;
}

SICALLBACK AlembicIPolymeshTopo_Term( CRef& in_ctxt )
{
	//Get Context from CRef
	Context ctxt(in_ctxt);

	//Get User Data
	SIAlembicIPolymesh* mesh = (SIAlembicIPolymesh*)(CValue::siPtrType)ctxt.GetUserData();;
	delete mesh;
	
	// Empty User Data
	ctxt.PutUserData((CValue::siPtrType)NULL);
	return CStatus::OK;
}

//------------------------------------------------------------
// Polymesh Update ICE Node
//------------------------------------------------------------
enum ABCPolymeshIDs
{
	ID_IN_PointID = 3,
	ID_OUT_Position = 201

};

CStatus RegisterAlembicIPolymesh( PluginRegistrar& in_reg )
{
	ICENodeDef nodeDef;
	nodeDef = Application().GetFactory().CreateICENodeDef(L"AlembicIPolymesh",L"AlembicIPolymesh");

	CStatus st;
	st = nodeDef.PutColor(AlembicNodeR,AlembicNodeG,AlembicNodeB);
	st.AssertSucceeded( ) ;

	// Add input ports and groups.
	st = nodeDef.AddPortGroup(ID_G_100);
	st.AssertSucceeded( ) ;

	st = nodeDef.AddInputPort(ID_IN_Path,ID_G_100,siICENodeDataString,siICENodeStructureSingle,siICENodeContextSingleton,L"Path",L"Path",L"");
	st.AssertSucceeded( ) ;

	st = nodeDef.AddInputPort(ID_IN_Identifier,ID_G_100,siICENodeDataString,siICENodeStructureSingle,siICENodeContextSingleton,L"Identifier",L"Identifier",L"");
	st.AssertSucceeded( ) ;

	st = nodeDef.AddInputPort(ID_IN_Frame,ID_G_100,siICENodeDataFloat,siICENodeStructureSingle,siICENodeContextSingleton,L"Frame",L"Frame",0);
	st.AssertSucceeded( ) ;

	st = nodeDef.AddInputPort(ID_IN_PointID,ID_G_100,siICENodeDataLong,siICENodeStructureSingle,siICENodeContextComponent0D,L"PointID",L"PointID",0,ID_UNDEF,ID_UNDEF,ID_CTXT_CNS);
	st.AssertSucceeded( ) ;

	// Add output ports.
	st = nodeDef.AddOutputPort(ID_OUT_Position,siICENodeDataVector3,siICENodeStructureSingle,XSI::siICENodeContextComponent0D,L"Position",L"Position",ID_UNDEF,ID_UNDEF,ID_CTXT_CNS);
	st.AssertSucceeded( ) ;

	PluginItem nodeItem = in_reg.RegisterICENode(nodeDef);
	nodeItem.PutCategories(L"Alembic");

	return CStatus::OK;
}

SICALLBACK AlembicIPolymesh_Evaluate( ICENodeContext& in_ctxt )
{
	// Get User Data
	SIAlembicIPolymesh* mesh = (SIAlembicIPolymesh*)(CValue::siPtrType)in_ctxt.GetUserData( );

	if(mesh->IsDirty(in_ctxt)||!mesh->SIAlembicIObject::Initialized())
	{
		Application().LogMessage(L"Update Polymesh");
		mesh->SIAlembicIObject::Init(in_ctxt);
		mesh->Check();
	}

	if(mesh && mesh->SIAlembicIObject::Initialized())
	{
		// The current output port being evaluated...
		LONG out_portID = in_ctxt.GetEvaluatedOutputPortID( );

		CDataArrayFloat frameData(in_ctxt, ID_IN_Frame);
		float frame = frameData[0];
		int iFrame = int(frame)-1;

		switch( out_portID )
		{	
			case ID_OUT_Position :
			{
				// let's create a mesh
				Application().LogMessage(L" Current Frame : "+(CString)iFrame);

				//Application().LogMessage(L"Object Name : "+(CString)obj->getFullName().c_str());
				
				Alembic::AbcGeom::IPolyMesh meshobject((*mesh->SIAlembicIObject::GetIObject()),Alembic::Abc::kWrapExisting);
				
				Alembic::AbcGeom::IPolyMeshSchema meshschema = meshobject.getSchema();
				Alembic::AbcGeom::IPolyMeshSchema::Sample meshsample = meshschema.getValue();
				Alembic::Abc::P3fArraySamplePtr meshPos = meshsample.getPositions();

				CDataArrayLong PointIDData(in_ctxt,ID_IN_PointID);

				CDataArrayVector3f outData( in_ctxt );
				CIndexSet indexSet(in_ctxt);
				for(CIndexSet::Iterator it = indexSet.Begin(); it.HasNext(); it.Next())
				{
					ULONG id = PointIDData[it];

					/*
					//outData[j].Set(meshPos->get()[j].x,meshPos->get()[j].y,meshPos->get()[j].z);
					ULONG aid = it.GetAbsoluteIndex();
					outData[it].Set(meshPos->get()[aid].x,meshPos->get()[aid].y,meshPos->get()[aid].z);
					*/
					Application().LogMessage(L"Absolute Index : "+(CString)id);
				}
				
			}break;
			
		}
	
	}

	return CStatus::OK;
}


SICALLBACK AlembicIPolymesh_Init( CRef& in_ctxt )
{
	//Get Context from CRef
	Context ctxt(in_ctxt);

	// Build a new SIIAlembicObject
	SIAlembicIPolymesh* mesh = new SIAlembicIPolymesh();
	
	// Store Object in User Data
	ctxt.PutUserData((CValue::siPtrType)mesh);
	return CStatus::OK;
}

SICALLBACK AlembicIPolymesh_Term( CRef& in_ctxt )
{
	//Get Context from CRef
	Context ctxt(in_ctxt);

	//Get User Data
	SIAlembicIPolymesh* mesh = (SIAlembicIPolymesh*)(CValue::siPtrType)ctxt.GetUserData();;
	delete mesh;
	
	// Empty User Data
	ctxt.PutUserData((CValue::siPtrType)NULL);
	return CStatus::OK;
}