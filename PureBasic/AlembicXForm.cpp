#include "AlembicUtil.h"
#include "AlembicXForm.h"
#include "AlembicObject.h"
#include "AlembicWriteJob.h"

#include "Alembic/AbcGeom/XformOp.h"
#include "Alembic/AbcGeom/XformSample.h"

BOOZE_NAMESPACE_OPEN_SCOPE
//------------------------------------------------------------------------------------------------
// Utils
//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
AlembicIXForm::AlembicIXForm(AbcG::IObject& object) :AlembicIObject(object)
{
	m_type = GeometricType_XForm;
}

bool AlembicIXForm::initialize(){
	return true;
}

BOOZE_EXPORT bool ABC_ObjectIsIXForm(AlembicIObject* obj)
{
	return Alembic::AbcGeom::IXform::matches(obj->get().getMetaData());
}

//================================================================================================
// Alembic Export
//================================================================================================
// Constructor
//------------------------------------------------------------------------------------------------
AlembicOXForm::AlembicOXForm(AlembicOArchive* archive, AlembicOObject* parent, void* customData, const char* name)
: AlembicOObject(archive, parent, customData, name, GeometricType_XForm){
	ABCOXformPtr xfo(new AbcG::OXform(parent->get(), name, getJob()->getTimeSampling()->getPtr()));
	m_xform = xfo;
}

// Save
//------------------------------------------------------------------------------------------------
void AlembicOXForm::save(AbcA::TimeSamplingPtr time){

	// add a couple of ops
	AbcG::XformOp transop(AbcG::kTranslateOperation, AbcG::kTranslateHint);
	AbcG::XformOp scaleop(AbcG::kScaleOperation, AbcG::kScaleHint);

	AbcG::XformSample sample;
	sample.addOp(transop, AbcG::V3f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX));
	float s = (float)rand() / (float)RAND_MAX * 0.5f + 0.5f;
	sample.addOp(scaleop, AbcG::V3f(s, s, s));

	AbcU::shared_ptr< AbcG::OXform > xfoPtr = AbcU::dynamic_pointer_cast< AbcG::OXform >(getPtr());
	Alembic::AbcGeom::OXformSchema &schema = xfoPtr->getSchema();

	schema.set(sample);

	/*
	MessageBox(NULL, L"SAVE", L"SAVE", MB_OK);
	ABCOXformPtr xfoPtr = AbcU::dynamic_pointer_cast< AbcG::OXform> (m_object);
	AbcG::OXform xfo = *xfoPtr;
	AbcG::OXformSchema schema = xfo.getSchema();
	Alembic::AbcGeom::XformSample sample;
	sample.setTranslation(Imath::V3f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX));
	//sample.setRotation(Imath::V3d(qx, qy, qz), qw * RADIANTODEGREE);
	sample.setScale(Imath::V3f(1,1,1));
	
	// save the sample
	schema.set(sample);
	*/
	for (int i = 0; i < m_children.size(); ++i)m_children[i]->save(time);
	
}


BOOZE_EXPORT void ABC_SaveXFormSample(Imath::M44f& xform, Alembic::AbcGeom::OXformSchema & schema, Alembic::AbcGeom::XformSample & sample, float time)
{
	xform[0][0] = 0.0;
	// extract the x, y, z axes
	Imath::V3f x(xform[0][0], xform[0][1], xform[0][2]);
	Imath::V3f y(xform[1][0], xform[1][1], xform[1][2]);
	Imath::V3f z(xform[2][0], xform[2][1], xform[2][2]);

	// scale
	Imath::V3f s(x.length(), y.length(), z.length());

	// compute quaternion
	float qx, qy, qz, qw, qw4;
	float tr = xform[0][0] + xform[1][1] + xform[2][2];
	float S;
	if (tr > 0.0f){
		S = sqrtf(tr + 1.0f)  * 2.0f;
		qw = 0.25 * S;
		qx = (xform[2][1] - xform[1][2]) / S;
		qy = (xform[0][2] - xform[2][0]) / S;
		qz = (xform[1][0] - xform[0][1]) / S;
	}
	else if (xform[0][0] > xform[1][1] && xform[0][0] > xform[2][2]){
		S = sqrtf(1.0f + xform[0][0] - xform[1][1] - xform[2][2]) * 2.0f;
		qw = (xform[2][1] - xform[1][2]) / S;
		qx = 0.25f * S;
		qy = (xform[0][1] + xform[1][0]) / S;
		qz = (xform[0][2] + xform[2][0]) / S;
	}
	else if (xform[1][1] > xform[2][2]){
		S = sqrtf(1.0f + xform[1][1] - xform[0][0] - xform[2][2]) * 2.0f;
		qw = (xform[0][2] - xform[2][0]) / S;
		qx = (xform[0][1] + xform[1][0]) / S;
		qy = 0.25f * S;
		qz = (xform[1][2] + xform[2][1]) / S;
	}
	else{
		S = sqrtf(1.0f + xform[2][2] - xform[0][0] - xform[1][1]) * 2.0f;
		qw = (xform[1][0] - xform[0][1]) / S;
		qx = (xform[0][2] + xform[2][0]) / S;
		qy = (xform[1][2] + xform[2][1]) / S;
		qz = 0.25f * S;
	}
	// set the rotation!
	Imath::Quatf r(qw, qx, qy, qz);

	// finally set the position!
	Imath::V3f t(xform[3][0], xform[3][1], xform[3][2]);

   sample.setTranslation(t);
   sample.setRotation(Imath::V3d(qx, qy, qz), qw * RADIANTODEGREE);
   sample.setScale(s);

   // save the sample
   schema.set(sample);
}

BOOZE_EXPORT void ABC_GetXFormSample(AlembicIObject* obj, float frame, XForm_Sample* io_sample)
{
	if(!obj->get().valid()||!ABC_ObjectIsIXForm(obj))
		return;

	Alembic::AbcGeom::IXform xform(obj->get(),Alembic::Abc::kWrapExisting);
	//CString path = ctxt.GetParameterValue(L"path");
	//CString identifier = ctxt.GetParameterValue(L"identifier");
	Alembic::AbcCoreAbstract::index_t sampleIndex = (Alembic::AbcCoreAbstract::index_t)int(frame)-1;

   if(!xform.valid())
	   return ;

   // clamp the sample
   if(sampleIndex < 0)
      sampleIndex = 0;
   else if(sampleIndex >= (Alembic::AbcCoreAbstract::index_t)xform.getSchema().getNumSamples())
      sampleIndex = int(xform.getSchema().getNumSamples()) - 1;

   Alembic::AbcGeom::XformSample sample;
   xform.getSchema().get(sample,sampleIndex);

   io_sample->pos[0] = sample.getTranslation().x;
   io_sample->pos[1] = sample.getTranslation().y;
   io_sample->pos[2] = sample.getTranslation().z;
   io_sample->ori[0] = sample.getAxis().x;
   io_sample->ori[1] = sample.getAxis().y;
   io_sample->ori[2] = sample.getAxis().z;
   io_sample->ori[3] = sample.getAngle();
   io_sample->scl[0] = sample.getScale().x;
   io_sample->scl[1] = sample.getScale().y;
   io_sample->scl[2] = sample.getScale().z;
}

BOOZE_NAMESPACE_CLOSE_SCOPE

