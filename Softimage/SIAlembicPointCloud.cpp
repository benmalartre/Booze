//-----------------------------------------------------------------
// SIAlembicPointCloud.cpp
//-----------------------------------------------------------------
#include "SIAlembicRegister.h"
#include "SIAlembicPointCloud.h"
#include "SIAlembicXForm.h"
#include "SIAlembicAttribute.h"

SIAlembicOPointCloud::SIAlembicOPointCloud(const XSI::CRef & in_ref, SIAlembicWriteJob * in_job)
:SIAlembicOObject(in_ref, in_job)
{
	Primitive prim = GetRef();
	CString xformName(prim.GetParent3DObject().GetName());
	CString pName(xformName+L"Shape");

	Alembic::AbcGeom::OXform xform(GetAlembicJob()->GetArchive().getTop(),xformName.GetAsciiString(),GetAlembicJob()->GetAnimatedTs());
	Alembic::AbcGeom::OPoints points(xform,pName.GetAsciiString(),GetAlembicJob()->GetAnimatedTs());

	_xformschema = xform.getSchema();
	_pointsschema = points.getSchema();
	
	_numsamples = 0;

}

SIAlembicOPointCloud::~SIAlembicOPointCloud(){
	for(int i=0;i<attributes.size();i++)
	{
		delete attributes[i];
	}
}

void SIAlembicIPointCloud::Check()
{
	
	if(!Alembic::AbcGeom::IPoints::matches((*AlembicIObject::GetIObject()).getMetaData()))
	{
		Clean();
	}

}


ABCStatus SIAlembicOPointCloud::Save(double time)
{
	Application app;
	app.LogMessage(L" Save PointCloud At Frame "+(CString)time);

	SIAlembicWriteJob* job = this->GetAlembicJob();
	const std::string t = job->GetOption("pointcloudAttributes");
	std::vector<std::string>attr_names = split(t,',');
	
   // store the transform
   Primitive prim(GetRef());
   app.LogMessage(L" Primitive ;: "+prim.GetFullName());

   SaveXFormSample(prim.GetParent3DObject().GetKinematics().GetGlobal().GetRef(),_xformschema,_xformsample,time);

   // access the point cloud
   Geometry geom = prim.GetGeometry(time);

   ICEAttribute position = geom.GetICEAttributeFromName(L"PointPosition");
   CICEAttributeDataArrayVector3f  pos;
	position.GetDataArray(pos);
   
   ULONG nbp = pos.GetCount();
   // allocate the points and normals

	std::vector<Imath::V3f> p;
	std::vector<Alembic::Abc::uint64_t> ids;
	p.resize(nbp);
	ids.resize(nbp);

	for(ULONG i=0;i<nbp;i++)
   {
		ids[i] = i;
		p[i].x = (float)pos[i].GetX();
		p[i].y = (float)pos[i].GetY();
		p[i].z = (float)pos[i].GetZ();
   }
	

	_pointssample.reset();
	_pointssample.setPositions(Alembic::Abc::P3fArraySample( (const Alembic::Abc::V3f*)&p.front(), nbp ));
	_pointssample.setIds(Alembic::Abc::UInt64ArraySample( &ids[0], nbp ));
	_pointssample.setSelfBounds( Imath::Box3d(Imath::V3f(-1000,-1000,-1000),Imath::V3f(1000,1000,1000)));
	_pointsschema.set( _pointssample );

	if(_numsamples == 0)
	{

		Alembic::Abc::OCompoundProperty prop = _pointsschema.getArbGeomParams();
		Alembic::AbcCoreAbstract::MetaData metadata;
		Alembic::AbcGeom::SetGeometryScope(metadata,Alembic::AbcGeom::kVaryingScope);

		for(int i=0;i<attr_names.size();i++)
		{
			bool exists = CheckAttribute(prim,(CString)attr_names[i].c_str());

			CString name = (CString)attr_names[i].c_str();
			if(exists==true && prop.valid())
			{
				ICEAttribute attr = geom.GetICEAttributeFromName(name);
				SIAlembicOAttribute* out = new SIAlembicOAttribute(prop,attr,metadata,job->GetAnimatedTs());
				attributes.push_back(out);
				out->Get(attr);

			}
			else
			{
				Application().LogMessage(L"Attribute NOT Exists :: "+name);
			}
		}
	}
	else
	{
		Application().LogMessage(L"-------------------000000-------------------------");
		for(int i=0;i<attributes.size();i++)
		{
			Application().LogMessage(L"Save Attribute "+attributes[i]->GetName()+L" At Time "+(CString)time);
			ICEAttribute attr = geom.GetICEAttributeFromName(attributes[i]->GetName());
			attributes[i]->Get(attr);
			Application().LogMessage(L" >>>>>>>  Num Samples : "+(CString)attributes[i]->GetNumSamples());
		}
	}
	
	_numsamples++;

	return Status_OK;
}

CStatus SIAlembicIPointCloud::Update(int iFrame)
{
	Alembic::AbcGeom::IPoints points((*SIAlembicIObject::GetIObject()),Alembic::Abc::kWrapExisting);	
	Alembic::AbcGeom::IPointsSchema schema = points.getSchema();
	// object has no samples, bail early
    if (schema.getNumSamples() == 0)
    {
		Application().LogMessage("No Samples!!!");
		return CStatus::Fail;
    }
    

    Alembic::AbcCoreAbstract::index_t index =
        schema.getTimeSampling()->getNearIndex(
            iFrame, schema.getNumSamples()).first;

	Alembic::AbcGeom::IPointsSchema::Sample sample;
    schema.get(sample, index);

    _nbp = (int)sample.getPositions()->size();
	

	/*

	// prepare it
	Alembic::Abc::P3fArraySamplePtr pos = _cloudsample.getPositions();
	_nbp = pos->size();
	//_nbp = _cloudsample.getPositions().getSize();
*/
	/*
   Alembic::AbcGeom::IPointsSample sample;
   cloud.getSchema().get(sample,sampleIndex);
	Alembic::Abc::P3fArraySamplePtr pos = sample.getPositions();
	*/
	return CStatus::OK;
}

//-------------------------------------------------------------
// ICE Node
//-------------------------------------------------------------
enum ABCPointCloudIDs
{
	ID_OUT_PointPosition = 201,
	ID_OUT_PointRotation = 202,
	ID_OUT_PointScale = 203,
	ID_OUT_PointColor = 204
};

CStatus RegisterAlembicIPointCloud( PluginRegistrar& in_reg )
{
	ICENodeDef nodeDef;
	nodeDef = Application().GetFactory().CreateICENodeDef(L"AlembicIPointCloud",L"AlembicIPointCloud");

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
	st = nodeDef.AddOutputPort(ID_OUT_PointPosition,siICENodeDataVector3,siICENodeStructureArray,siICENodeContextSingleton,L"PointPosition",L"PointPosition");
	st.AssertSucceeded( ) ;

	st = nodeDef.AddOutputPort(ID_OUT_PointRotation,siICENodeDataQuaternion,siICENodeStructureArray,siICENodeContextSingleton,L"PointRotation",L"PointRotation");
	st.AssertSucceeded( ) ;

	st = nodeDef.AddOutputPort(ID_OUT_PointScale,siICENodeDataVector3,siICENodeStructureArray,siICENodeContextSingleton,L"PointScale",L"PointScale");
	st.AssertSucceeded( ) ;

	st = nodeDef.AddOutputPort(ID_OUT_PointColor,siICENodeDataColor4,siICENodeStructureArray,siICENodeContextSingleton,L"PointColor",L"PointColor");
	st.AssertSucceeded( ) ;

	PluginItem nodeItem = in_reg.RegisterICENode(nodeDef);
	nodeItem.PutCategories(L"Alembic");

	return CStatus::OK;
}


SICALLBACK AlembicIPointCloud_Evaluate( ICENodeContext& in_ctxt )
{
	// Get User Data
	SIAlembicIPointCloud* cloud = (SIAlembicIPointCloud*)(CValue::siPtrType)in_ctxt.GetUserData( );
	Application().LogMessage(L"Point Cloud Evaluate Called...");
	if(!cloud)return CStatus::Undefined;

	if(cloud->IsDirty(in_ctxt)||!cloud->SIAlembicIObject::Initialized())
	{
		Application().LogMessage(L"Init Point Cloud");
		cloud->SIAlembicIObject::Init(in_ctxt);
		cloud->SIAlembicIObject::Check();
	}

	LONG out_portID = in_ctxt.GetEvaluatedOutputPortID( );
	switch(out_portID)
	{
		case ID_OUT_PointRotation:
			{
				Alembic::AbcGeom::IPoints points((*cloud->SIAlembicIObject::GetIObject()),Alembic::Abc::kWrapExisting);	
			Alembic::AbcGeom::IPointsSchema schema = points.getSchema();

			Alembic::AbcGeom::ICompoundProperty argGeomParamsProp = schema.getArbGeomParams();

			if(argGeomParamsProp.valid()){
				Application().LogMessage(L"Point Rotation ArgGeomParams Valid!!");
				//offset = 0;	
				 for(ULONG i=0;i<(ULONG)argGeomParamsProp.getNumProperties();i++){
					 Alembic::AbcGeom::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);
					Application().LogMessage(h.getName().c_str());
					  if(h.getName() == "Orientation"){
							Application().LogMessage(L">>>>>>>>>>>> Found Rotation Attribute!");
							Alembic::Abc::IC4fArrayProperty rot(argGeomParamsProp,h.getName());
							Alembic::Abc::C4fArraySamplePtr propPtr1 = rot.getValue(0);
							int nbp = (int)propPtr1->size();
							//memcpy(io_sample->_weights,propPtr1->get(),nbp * sizeof(Imath::Color4f));
							CDataArray2DQuaternionf outData( in_ctxt );
							CDataArray2DQuaternionf::Accessor outSubData = outData.Resize(0,nbp);

							for(int i =0;i<nbp;i++)
							{
								outSubData[i].Set(
									propPtr1->get()[i].r,
									propPtr1->get()[i].g,
									propPtr1->get()[i].b,
									propPtr1->get()[i].a
								);
								
								//outData[it] = data->_cloudsample._pos[it.GetAbsoluteIndex()]; 
							}
							break;
					  }
				 }
			}

			break;
			}

		case ID_OUT_PointScale:
			{
	
				Alembic::AbcGeom::IPoints points((*cloud->SIAlembicIObject::GetIObject()),Alembic::Abc::kWrapExisting);	
			Alembic::AbcGeom::IPointsSchema schema = points.getSchema();

			Alembic::AbcGeom::ICompoundProperty argGeomParamsProp = schema.getArbGeomParams();

			if(argGeomParamsProp.valid()){
				Application().LogMessage(L" Point SCale ArgGeomParams Valid!!");
				//offset = 0;	
				 for(ULONG i=0;i<(ULONG)argGeomParamsProp.getNumProperties();i++){
					 
					 Alembic::AbcGeom::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);

					Application().LogMessage(+h.getName().c_str());
					  if(h.getName() == "Scale"){
						  Application().LogMessage(L"############ Found Scale Attribute!");
							Alembic::Abc::IV3fArrayProperty scl(argGeomParamsProp,h.getName());
							Alembic::Abc::V3fArraySamplePtr propPtr1 = scl.getValue(0);
							ULONG nbp = (ULONG)propPtr1->size();
							//memcpy(io_sample->_weights,propPtr1->get(),nbp * sizeof(Imath::Color4f));
							CDataArray2DVector3f outData( in_ctxt );
							CDataArray2DVector3f::Accessor outSubData = outData.Resize(0,nbp);

							for(int i =0;i<nbp;i++)
							{
								outSubData[i].Set(
									propPtr1->get()[i].x,
									propPtr1->get()[i].y,
									propPtr1->get()[i].z
								);
								
								//outData[it] = data->_cloudsample._pos[it.GetAbsoluteIndex()]; 
							}
							break;
					  }
				 }
			}

			break;
			}
		case ID_OUT_PointPosition:
			{
			CDataArrayFloat frameData(in_ctxt, ID_IN_Frame);
			float frame = frameData[0];
			int iFrame = int(frame)-1;
			cloud->Update(iFrame);

			Application().LogMessage("Initialized!!!");
			
			Alembic::AbcGeom::IPoints points((*cloud->SIAlembicIObject::GetIObject()),Alembic::Abc::kWrapExisting);	
			Alembic::AbcGeom::IPointsSchema schema = points.getSchema();
			Alembic::AbcGeom::IPointsSchema::Sample sample = schema.getValue();

			Alembic::Abc::P3fArraySamplePtr meshPos = sample.getPositions();
			int nbp = meshPos->size();
			//Alembic::Abc::N3fArraySamplePtr nrm = sample.getNormals();
			
			CDataArray2DVector3f outData( in_ctxt );
			CDataArray2DVector3f::Accessor outSubData = outData.Resize(0,nbp);

			for(int i =0;i<nbp;i++)
			{
				outSubData[i].Set(
					meshPos->get()[i].x,
					meshPos->get()[i].y,
					meshPos->get()[i].z
				);
				
				//outData[it] = data->_cloudsample._pos[it.GetAbsoluteIndex()]; 
			}
			break;
			}

		case ID_OUT_PointColor:
			{
	
				Alembic::AbcGeom::IPoints points((*cloud->SIAlembicIObject::GetIObject()),Alembic::Abc::kWrapExisting);	
			Alembic::AbcGeom::IPointsSchema schema = points.getSchema();

			Alembic::AbcGeom::ICompoundProperty argGeomParamsProp = schema.getArbGeomParams();

			if(argGeomParamsProp.valid()){
				Application().LogMessage(L" Parent Property ArgGeomParams Valid!!");
				//offset = 0;	
				 for(ULONG i=0;i<(ULONG)argGeomParamsProp.getNumProperties();i++){
					 
					 Alembic::AbcGeom::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);

					Application().LogMessage(+h.getName().c_str());
					  if(h.getName() == "Color"){
						  Application().LogMessage(L"############ Found Color Attribute!");
							Alembic::Abc::IC4fArrayProperty col(argGeomParamsProp,h.getName());
							Alembic::Abc::C4fArraySamplePtr propPtr1 = col.getValue(0);
							ULONG nbp = (ULONG)propPtr1->size();
							//memcpy(io_sample->_weights,propPtr1->get(),nbp * sizeof(Imath::Color4f));
							CDataArray2DColor4f outData( in_ctxt );
							CDataArray2DColor4f::Accessor outSubData = outData.Resize(0,nbp);

							for(int i =0;i<nbp;i++)
							{
								outSubData[i].Set(
									propPtr1->get()[i].r,
									propPtr1->get()[i].g,
									propPtr1->get()[i].b,
									propPtr1->get()[i].a
								);
								
								//outData[it] = data->_cloudsample._pos[it.GetAbsoluteIndex()]; 
							}
							break;
					  }
				 }
			}

			break;
			}
		

	}

	return CStatus::OK;
}


SICALLBACK AlembicIPointCloud_Init( CRef& in_ctxt )
{
	//Get Context from CRef
	Context ctxt(in_ctxt);

	// Build a new SIIAlembicObject
	SIAlembicIPointCloud* cloud = new SIAlembicIPointCloud();
	
	// Store Object in User Data
	ctxt.PutUserData((CValue::siPtrType)cloud);
	return CStatus::OK;
}

SICALLBACK AlembicIPointCloud_Term( CRef& in_ctxt )
{
	//Get Context from CRef
	Context ctxt(in_ctxt);

	//Get User Data
	SIAlembicIPointCloud* cloud = (SIAlembicIPointCloud*)(CValue::siPtrType)ctxt.GetUserData();;
	delete cloud;
	
	// Empty User Data
	ctxt.PutUserData((CValue::siPtrType)NULL);
	return CStatus::OK;
}
