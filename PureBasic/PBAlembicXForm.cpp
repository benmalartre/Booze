#include "PBAlembicRegister.h"
#include "PBAlembicXForm.h"
#include "PBAlembicObject.h"
#include "PBAlembicHelpers.h"

struct XForm_Sample
{
	float pos[3];
	float ori[4];
	float scl[3];
};

namespace AbcX = ::Alembic::AbcCoreAbstract::ALEMBIC_VERSION_NS;
using namespace AbcX;

EXPORT bool ABC_ObjectIsXForm(AlembicIObject* obj)
{
	return Alembic::AbcGeom::IXform::matches(obj->GetIObject()->getMetaData());
}
/*
void SaveXFormSample(XSI::CRef kinestateRef, Alembic::AbcGeom::OXformSchema & schema, Alembic::AbcGeom::XformSample & sample, double time)
{
   CTransformation global = KinematicState(kinestateRef).GetTransform(time);

   // store the transform
   CVector3 trans = global.GetTranslation();
   CVector3 axis;
   double angle = global.GetRotationAxisAngle(axis);
   CVector3 scale = global.GetScaling();
   sample.setTranslation(Imath::V3d(trans.GetX(),trans.GetY(),trans.GetZ()));
   sample.setRotation(Imath::V3d(axis.GetX(),axis.GetY(),axis.GetZ()),RadiansToDegrees(angle));
   sample.setScale(Imath::V3d(scale.GetX(),scale.GetY(),scale.GetZ()));

   // save the sample
   schema.set(sample);
}

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
EXPORT TCHAR* ABC_TestXForm(AlembicIObject* obj,XForm_Sample* io_sample)
{
	//return ABC_PassStringToPureBasic(std::string("Hey!!"));
	return NULL;
}

EXPORT void ABC_GetXFormSample(AlembicIObject* obj,float frame,XForm_Sample* io_sample)
{
	if(!obj->GetIObject()->valid()||!ABC_ObjectIsXForm(obj))
		return;

	Alembic::AbcGeom::IXform xform(*obj->GetIObject(),Alembic::Abc::kWrapExisting);
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

