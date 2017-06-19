#include "SIAlembicRegister.h"
#include "SIAlembicCamera.h"
#include "SIAlembicXform.h"

/*
#include <xsi_application.h>
#include <xsi_x3dobject.h>
#include <xsi_primitive.h>
#include <xsi_context.h>
#include <xsi_operatorcontext.h>
#include <xsi_customoperator.h>
#include <xsi_factory.h>
#include <xsi_parameter.h>
#include <xsi_ppglayout.h>
#include <xsi_ppgitem.h>
#include <xsi_math.h>
#include <xsi_kinematics.h>
#include <xsi_kinematicstate.h>

using namespace XSI;
using namespace MATH;

*/

namespace AbcA = ::Alembic::AbcCoreAbstract::ALEMBIC_VERSION_NS;
using namespace AbcA;

SIAlembicOCamera::SIAlembicOCamera(const XSI::CRef & in_ref, SIAlembicWriteJob * in_job)
: SIAlembicOObject(in_ref, in_job)
{
   Primitive prim(GetRef());
   CString xformName(prim.GetParent3DObject().GetName());
   CString cameraName(xformName+L"Shape");
   Alembic::AbcGeom::OXform xform(in_job->GetArchive().getTop(),xformName.GetAsciiString(),GetAlembicJob()->GetAnimatedTs());
   Alembic::AbcGeom::OCamera camera(xform,cameraName.GetAsciiString(),GetAlembicJob()->GetAnimatedTs());

   _xformschema = xform.getSchema();
   _cameraschema = camera.getSchema();
}

void SIAlembicICamera::Check()
{
	Application().LogMessage(L" Check MetaData Called...");
	if(!Alembic::AbcGeom::ICamera::matches(SIAlembicIObject::GetIObject()->getMetaData()))
	{
		Clean();
		Application().LogMessage(L"Meta Data Doesn't Match!!");
	}
	else
		Application().LogMessage(L"Meta Data Does Match!!");
}

ABCStatus SIAlembicOCamera::Save(double time)
{
   // access the camera
   Primitive prim(GetRef());

   // store the transform
   SaveXFormSample(prim.GetParent3DObject().GetKinematics().GetGlobal().GetRef(),_xformschema,_xformsample,time);

   // store the camera data
   _camerasample.setFocalLength(prim.GetParameterValue(L"projplanedist",time));
   _camerasample.setVerticalAperture(prim.GetParameterValue(L"projplaneheight",time));
   _camerasample.setNearClippingPlane(prim.GetParameterValue(L"near",time));
   _camerasample.setFarClippingPlane(prim.GetParameterValue(L"far",time));
   _camerasample.setLensSqueezeRatio(prim.GetParameterValue(L"aspect",time));

   // save the samples
   _cameraschema.set(_camerasample);

   return Status_OK;
}

XSIPLUGINCALLBACK CStatus Alembic_Camera_Define( CRef& in_ctxt )
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

XSIPLUGINCALLBACK CStatus Alembic_Camera_DefineLayout( CRef& in_ctxt )
{
   Context ctxt( in_ctxt );
   PPGLayout oLayout;
   PPGItem oItem;
   oLayout = ctxt.GetSource();
   oLayout.Clear();
   return CStatus::OK;
}


XSIPLUGINCALLBACK CStatus Alembic_Camera_Update( CRef& in_ctxt )
{
   OperatorContext ctxt( in_ctxt );
	AlembicIArchive* archive = (AlembicIArchive*)(CValue::siPtrType)ctxt.GetUserData();

   CString path = ctxt.GetParameterValue(L"path");
   CString identifier = ctxt.GetParameterValue(L"identifier");
   Alembic::AbcCoreAbstract::index_t sampleIndex = (Alembic::AbcCoreAbstract::index_t)int(ctxt.GetParameterValue(L"frame"))-1;
   Alembic::Abc::IObject* obj = archive->GetObjectFromID(identifier.GetAsciiString());
   
   if(!obj->valid())
      return CStatus::OK;

   Alembic::AbcGeom::ICamera camera(*obj,Alembic::Abc::kWrapExisting);
   if(!camera.valid())return CStatus::OK;

   // clamp the sample
   if(sampleIndex < 0)
      sampleIndex = 0;
   else if(sampleIndex >= (Alembic::AbcCoreAbstract::index_t)camera.getSchema().getNumSamples())
      sampleIndex = int(camera.getSchema().getNumSamples()) - 1;

   Alembic::AbcGeom::CameraSample sample;
   camera.getSchema().get(sample,sampleIndex);

   Primitive prim(ctxt.GetOutputTarget());
   prim.PutParameterValue(L"std",0l);
   prim.PutParameterValue(L"aspect",sample.getLensSqueezeRatio());
   prim.PutParameterValue(L"projplane",true);
   prim.PutParameterValue(L"projplanedist",sample.getFocalLength());
   prim.PutParameterValue(L"projplaneheight",sample.getVerticalAperture());
   prim.PutParameterValue(L"near",sample.getNearClippingPlane());
   prim.PutParameterValue(L"far",sample.getFarClippingPlane());
   prim.PutParameterValue(L"far",sample.getFarClippingPlane());

   return CStatus::OK;
}
