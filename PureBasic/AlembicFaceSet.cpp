#include "AlembicFaceSet.h"
#include "AlembicWriteJob.h"

BOOZE_NAMESPACE_OPEN_SCOPE


AlembicIFaceSet::AlembicIFaceSet(AbcG::IObject& object) :AlembicIObject(object)
{
	m_type = GeometricType_FaceSet;
}

bool AlembicIFaceSet::Initialize()
{
	AbcG::IFaceSet _faceset(m_object);
	GetProperties();
	return true;
}

/*
AlembicOCurves::AlembicOCurves(AlembicWriteJob * in_job)
:AlembicOObject(in_job)
{
	Primitive prim = GetRef();
	CString xformName(prim.GetParent3DObject().GetName());
	CString pName(xformName+L"Shape");

	//Alembic::AbcGeom::OXform xform(GetAlembicJob()->GetArchive().getTop(),xformName.GetAsciiString(),GetAlembicJob()->GetAnimatedTs());
	Alembic::AbcGeom::OCurves curves(xform,pName.GetAsciiString(),GetAlembicJob()->GetAnimatedTs());

	_curveschema = curves.getSchema();
	_numsamples = 0;

}
*/

ABCStatus AlembicOFaceSet::Save(double time, ABC_FaceSet_Sample_Infos* infos, ABC_FaceSet_Sample* sample)
{
	/*
	Application app;
	app.LogMessage(L" Save Curves At Frame "+(CString)time);

   // store the transform
   Primitive prim(GetRef());
   app.LogMessage(L" Primitive ;: "+prim.GetFullName());

   SaveXFormSample(prim.GetParent3DObject().GetKinematics().GetGlobal().GetRef(),_xformschema,_xformsample,time);

   // access the point cloud
   Geometry geom = prim.GetGeometry(time);

   NurbsCurveList crvlist(geom);
   CNurbsCurveDataArray datas;
   crvlist.Get(XSI::siSINurbs,datas);
   app.LogMessage(L" Nb Sub Curves : "+(CString)datas.GetCount());

   std::vector<Imath::V3f> pos;
   std::vector<Alembic::Abc::int32_t> N;

   for(int i=0;i<datas.GetCount();i++){
	  CVector4Array cp =datas[i].m_aControlPoints;
	  for(int j=0;j<cp.GetCount();j++){
		  pos.push_back(Imath::V3f((float)cp[j].GetX(),(float)cp[j].GetY(),(float)cp[j].GetZ()));
	  }
	  N.push_back(cp.GetCount());
	  _nbp += cp.GetCount();
	  _nbcurves++;
   }   
   
   Alembic::AbcGeom::OCurvesSchema::Sample sample(
	   Alembic::Abc::P3fArraySample((const Alembic::Abc::V3f*)&pos.front(), _nbp),
		Alembic::Abc::Int32ArraySample((const Alembic::Abc::int32_t*)&N.front(),_nbcurves)
		);
	
   //sample.setSelfBounds( Imath::Box3d(Imath::V3f(-1000,-1000,-1000),Imath::V3f(1000,1000,1000)));

	_curveschema.set( sample );
	_numsamples++;
	   Alembic::Abc::P3fArraySample(

	   Alembic::AbcGeom::OCurvesSchema::Sample sample(
			Alembic::Abc::P3fArraySample( (const Alembic::Abc::V3f*)&p.front(), nbp ),
			Alembic::Abc::UInt64ArraySample( &ids[0], nbp )
			);
	
   //_curveschema.getArbGeomParams().getN
   
   ICEAttribute position = prim.GetICEAttributeFromName(L"PointPosition");
   bool has_position = position.IsValid();
   ICEAttribute rotation = prim.GetICEAttributeFromName(L"Rotation"); 
   bool has_rotation = rotation.IsValid();
   ICEAttribute scale = prim.GetICEAttributeFromName(L"Scale"); 
   bool has_scale = scale.IsValid();
   ICEAttribute color = prim.GetICEAttributeFromName(L"Scale"); 
   bool has_color = color.IsValid();

   CICEAttributeDataArrayVector3f  pos;
   CICEAttributeDataArrayQuaternionf  rot;
   CICEAttributeDataArrayVector3f  scl;
   CICEAttributeDataArrayColor4f  col;

   if(has_position)position.GetDataArray(pos);
	if(has_rotation)rotation.GetDataArray(rot);
	if(has_scale)scale.GetDataArray(scl);
	if(has_color)color.GetDataArray(col);

   ULONG nbp = pos.GetCount();
	Application().LogMessage(L"There are "+(CString)nbp+L" Vertices at frame "+(CString)time);
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
	
	Alembic::AbcGeom::OPointsSchema::Sample sample(
		Alembic::Abc::P3fArraySample( (const Alembic::Abc::V3f*)&p.front(), nbp ),
		Alembic::Abc::UInt64ArraySample( &ids[0], nbp )
		);
	//

	sample.setSelfBounds( Imath::Box3d(Imath::V3f(-1000,-1000,-1000),Imath::V3f(1000,1000,1000)));

	_cloudschema.set( sample );


	_numsamples++;
	*/
	return Status_OK;
}

/*
void AlembicICurves::Update(float time)
{
	AlembicICurves* c = (AlembicICurves*)this;
	Alembic::AbcGeom::ICurves curves((c->Get()),Alembic::Abc::kWrapExisting);	
	Alembic::AbcGeom::ICurvesSchema schema = curves.getSchema();
	// object has no samples, bail early
    if (schema.getNumSamples() == 0)
    {
		Application().LogMessage("No Samples!!!");
		return CStatus::Fail;
    }
    

    Alembic::AbcCoreAbstract::index_t index =
        schema.getTimeSampling()->getNearIndex(
            time, schema.getNumSamples()).first;

	Alembic::AbcGeom::ICurvesSchema::Sample sample;
    schema.get(sample, index);

    //_nbp = sample.getPositions()->size();
	
	// prepare it
	Alembic::Abc::P3fArraySamplePtr pos = _cloudsample.getPositions();
	_nbp = pos->size();
	//_nbp = _cloudsample.getPositions().getSize();

   Alembic::AbcGeom::IPointsSample sample;
   cloud.getSchema().get(sample,sampleIndex);
	Alembic::Abc::P3fArraySamplePtr pos = sample.getPositions();

	return CStatus::OK;
}
*/


BOOZE_NAMESPACE_CLOSE_SCOPE