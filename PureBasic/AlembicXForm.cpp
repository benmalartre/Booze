#include "AlembicUtil.h"
#include "AlembicXForm.h"
#include "AlembicObject.h"

BOOZE_NAMESPACE_OPEN_SCOPE

AlembicIXForm::AlembicIXForm(AbcG::IObject& object) :AlembicIObject(object)
{
	_type = GeometricType_XForm;
}

bool AlembicIXForm::Initialize(){
	return true;
}

BOOZE_EXPORT bool ABC_ObjectIsXForm(AlembicIObject* obj)
{
	return Alembic::AbcGeom::IXform::matches(obj->Get().getMetaData());
}


BOOZE_EXPORT void ABC_SaveXFormSample(float* xform, Alembic::AbcGeom::OXformSchema & schema, Alembic::AbcGeom::XformSample & sample, float time)
{

	// extract the x, y, z axes
	Imath::V3f x(xform[0], xform[1], xform[2]);
	Imath::V3f y(xform[4], xform[5], xform[6]);
	Imath::V3f z(xform[8], xform[9], xform[10]);

	// scale
	Imath::V3f s(x.length(), y.length(), z.length());

	// compute quaternion
	float qx, qy, qz, qw, qw4;
	float tr = xform[0] + xform[5] + xform[10];
	float S;
	if (tr > 0.0f){
		S = sqrtf(tr + 1.0f)  * 2.0f;
		qw = 0.25 * S;
		qx = (xform[9] - xform[6]) / S;
		qy = (xform[2] - xform[8]) / S;
		qz = (xform[4] - xform[1]) / S;
	}
	else if (xform[0] > xform[5] && xform[0] > xform[10]){
		S = sqrtf(1.0f + xform[0] - xform[5] - xform[10]) * 2.0f;
		qw = (xform[9] - xform[6]) / S;
		qx = 0.25f * S;
		qy = (xform[1] + xform[4]) / S;
		qz = (xform[2] + xform[8]) / S;
	}
	else if (xform[5] > xform[10]){
		S = sqrtf(1.0f + xform[5] - xform[0] - xform[10]) * 2.0f;
		qw = (xform[2] - xform[8]) / S;
		qx = (xform[1] + xform[4]) / S;
		qy = 0.25f * S;
		qz = (xform[6] + xform[9]) / S;
	}
	else{
		S = sqrtf(1.0f + xform[10] - xform[0] - xform[5]) * 2.0f;
		qw = (xform[4] - xform[1]) / S;
		qx = (xform[2] + xform[8]) / S;
		qy = (xform[6] + xform[9]) / S;
		qz = 0.25f * S;
	}
	// set the rotation!
	Imath::Quatf r(qw, qx, qy, qz);

	// finally set the position!
	Imath::V3f t(xform[12], xform[13], xform[14]);

   sample.setTranslation(t);
   sample.setRotation(Imath::V3d(qx, qy, qz), qw * RADIANTODEGREE);
   sample.setScale(s);

   // save the sample
   schema.set(sample);
}

/*
XSIPLUGINCALLBACK CStatus Alembic_XForm_Init( CRef& in_ctxt )
{
	OperatorContext ctxt(in_ctxt);
	CString path = ctxt.GetParameterValue(L"path");
	CString identifier = ctxt.GetParameterValue(L"identifier");
	IAlembicArchive* archive = abc_archive_manager.GetArchiveFromID(path.GetAsciiString());
	Alembic::Abc::IObject* obj = archive->GetObjectFromID(identifier.GetAsciiString());
	ctxt.PutUserData((CValue::siPtrType)obj);
}
*/
BOOZE_EXPORT const char* ABC_TestXForm(AlembicIObject* obj, XForm_Sample* io_sample)
{
	//return ABC_PassStringToPureBasic(std::string("Hey!!"));
	return NULL;
}

BOOZE_EXPORT void ABC_GetXFormSample(AlembicIObject* obj, float frame, XForm_Sample* io_sample)
{
	if(!obj->Get().valid()||!ABC_ObjectIsXForm(obj))
		return;

	Alembic::AbcGeom::IXform xform(obj->Get(),Alembic::Abc::kWrapExisting);
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

/*
   CTransformation transform;
   transform.SetTranslationFromValues(sample.getTranslation().x,sample.getTranslation().y,sample.getTranslation().z);
   transform.SetRotationFromAxisAngle(
      CVector3(sample.getAxis().x,sample.getAxis().y,sample.getAxis().z),DegreesToRadians(sample.getAngle()));
   transform.SetScalingFromValues(sample.getScale().x,sample.getScale().y,sample.getScale().z);

   KinematicState state(ctxt.GetOutputTarget());
   state.PutTransform(transform);
*/
}

BOOZE_NAMESPACE_CLOSE_SCOPE

