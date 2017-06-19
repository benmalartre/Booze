//----------------------------------------------------
// SIAlembicXForm.cpp
//----------------------------------------------------
#include "SIAlembicXForm.h"

#include <xsi_application.h>
#include <xsi_kinematics.h>
#include <xsi_kinematicstate.h>
#include <xsi_x3dobject.h>
#include <xsi_math.h>
#include <xsi_context.h>
#include <xsi_operatorcontext.h>
#include <xsi_customoperator.h>
#include <xsi_factory.h>
#include <xsi_parameter.h>
#include <xsi_ppglayout.h>
#include <xsi_ppgitem.h>

using namespace XSI;
using namespace MATH;

namespace AbcA = ::Alembic::AbcCoreAbstract::ALEMBIC_VERSION_NS;
using namespace AbcA;

SIAlembicOXForm::SIAlembicOXForm(const XSI::CRef & in_ref, SIAlembicWriteJob * in_job)
:SIAlembicOObject(in_ref,in_job)
{
	_ref = in_ref;
	_job = in_job;

	KinematicState kine = GetRef();
	CString xformName(kine.GetParent3DObject().GetName());

	Alembic::AbcGeom::OXform xform(GetAlembicJob()->GetArchive().getTop(),xformName.GetAsciiString(),GetAlembicJob()->GetAnimatedTs());

	_xformschema = xform.getSchema();
	_numsamples = 0;
}

void SIAlembicIXForm::Check()
{
	if(!Alembic::AbcGeom::IXform::matches(AlembicIObject::GetIObject()->getMetaData()))
	{
		Clean();
	}
}


ABCStatus SIAlembicOXForm::Save(double time)
{
	KinematicState kine(GetRef());
	CTransformation global = kine.GetTransform(time);

   // store the transform
   CVector3 trans = global.GetTranslation();
   CVector3 axis;
   double angle = global.GetRotationAxisAngle(axis);
   CVector3 scale = global.GetScaling();
   
   _xformsample.setTranslation(Imath::V3d(trans.GetX(),trans.GetY(),trans.GetZ()));
   _xformsample.setRotation(Imath::V3d(axis.GetX(),axis.GetY(),axis.GetZ()),RadiansToDegrees(angle));
   _xformsample.setScale(Imath::V3d(scale.GetX(),scale.GetY(),scale.GetZ()));
	
   // save the sample
   _xformschema.set(_xformsample);

   return Status_OK;
}

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

//-------------------------------------------------------------
// Custom Operator
//-------------------------------------------------------------
XSIPLUGINCALLBACK CStatus Alembic_XForm_Define( CRef& in_ctxt )
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

XSIPLUGINCALLBACK CStatus Alembic_XForm_DefineLayout( CRef& in_ctxt )
{
   Context ctxt( in_ctxt );
   PPGLayout oLayout;
   PPGItem oItem;
   oLayout = ctxt.GetSource();
   oLayout.Clear();
   return CStatus::OK;
}

XSIPLUGINCALLBACK CStatus Alembic_XForm_Init( CRef& in_ctxt )
{
	OperatorContext ctxt(in_ctxt);
	CString path = ctxt.GetParameterValue(L"path");
	CString identifier = ctxt.GetParameterValue(L"identifier");
	AlembicIArchive* archive = abc_archive_manager.GetArchiveFromID(path.GetAsciiString());

	Alembic::Abc::IObject* obj = archive->GetObjectFromID(identifier.GetAsciiString());
	ctxt.PutUserData((CValue::siPtrType)obj);
	return CStatus::OK;
}

XSIPLUGINCALLBACK CStatus Alembic_XForm_Update( CRef& in_ctxt )
{
	OperatorContext ctxt( in_ctxt );
	Alembic::Abc::IObject* obj = (Alembic::Abc::IObject*)(CValue::siPtrType)ctxt.GetUserData();
	Alembic::AbcGeom::IXform xform(*obj,Alembic::Abc::kWrapExisting);
	Alembic::AbcCoreAbstract::index_t sampleIndex = (Alembic::AbcCoreAbstract::index_t)int(ctxt.GetParameterValue(L"frame"))-1;

   if(!xform.valid())
      return CStatus::OK;

   // clamp the sample
   if(sampleIndex < 0)
      sampleIndex = 0;
   else if(sampleIndex >= (Alembic::AbcCoreAbstract::index_t)xform.getSchema().getNumSamples())
      sampleIndex = int(xform.getSchema().getNumSamples()) - 1;

   Alembic::AbcGeom::XformSample sample;
   xform.getSchema().get(sample,sampleIndex);

   CTransformation transform;
   transform.SetTranslationFromValues(sample.getTranslation().x,sample.getTranslation().y,sample.getTranslation().z);
   transform.SetRotationFromAxisAngle(
      CVector3(sample.getAxis().x,sample.getAxis().y,sample.getAxis().z),DegreesToRadians(sample.getAngle()));
   transform.SetScalingFromValues(sample.getScale().x,sample.getScale().y,sample.getScale().z);

   KinematicState state(ctxt.GetOutputTarget());
   state.PutTransform(transform);

   return CStatus::OK;
}


//-------------------------------------------------------------
// ICE Node
//-------------------------------------------------------------

CStatus RegisterAlembicIXForm( PluginRegistrar& in_reg )
{
	ICENodeDef nodeDef;
	nodeDef = Application().GetFactory().CreateICENodeDef(L"AlembicIXForm",L"AlembicIXForm");

	CStatus st;
	st = nodeDef.PutColor(AlembicNodeR,AlembicNodeG,AlembicNodeB);
	st.AssertSucceeded( ) ;

	st = nodeDef.PutThreadingModel(siICENodeSingleThreading);
	st.AssertSucceeded( ) ;

	// Add input ports and groups.
	st = nodeDef.AddPortGroup(ID_G_100);
	st.AssertSucceeded( ) ;

	st = nodeDef.AddInputPort(ID_IN_Path,ID_G_100,siICENodeDataString,siICENodeStructureSingle,siICENodeContextSingleton,L"Path",L"Path",L"",ID_UNDEF,ID_UNDEF,ID_CTXT_CNS);
	st.AssertSucceeded( ) ;

	st = nodeDef.AddInputPort(ID_IN_Identifier,ID_G_100,siICENodeDataString,siICENodeStructureSingle,siICENodeContextSingleton,L"Identifier",L"Identifier",L"",ID_UNDEF,ID_UNDEF,ID_CTXT_CNS);
	st.AssertSucceeded( ) ;

	st = nodeDef.AddInputPort(ID_IN_Frame,ID_G_100,siICENodeDataFloat,siICENodeStructureSingle,siICENodeContextSingleton,L"Frame",L"Frame",0);
	st.AssertSucceeded( ) ;

	// Add output ports.
	st = nodeDef.AddOutputPort(ID_OUT_XForm,siICENodeDataMatrix44,siICENodeStructureSingle,siICENodeContextSingleton,L"XForm",L"XForm",ID_UNDEF,ID_UNDEF,ID_CTXT_CNS);
	st.AssertSucceeded( ) ;

	PluginItem nodeItem = in_reg.RegisterICENode(nodeDef);
	nodeItem.PutCategories(L"Alembic");

	return CStatus::OK;
}

SICALLBACK AlembicIXForm_Evaluate( ICENodeContext& in_ctxt )
{
	CValue userData = in_ctxt.GetUserData();
	SIAlembicIXForm* obj = (SIAlembicIXForm*)(CValue::siPtrType)in_ctxt.GetUserData( );
	//obj->GetMetaDatas()

	if(obj != NULL )
	{
		if(!obj->SIAlembicIObject::Initialized() || obj->IsDirty(in_ctxt))
		{
			obj->SIAlembicIObject::Init(in_ctxt);
			obj->SIAlembicIObject::Check();
		}
		if(obj->SIAlembicIObject::Initialized())
		{
			//obj->Update(in_ctxt);
			CDataArrayFloat frameData(in_ctxt, ID_IN_Frame);
			float frame = frameData[0];
			int iFrame = int(frame)-1;
		
			Application().LogMessage(L" Current Frame : "+(CString)iFrame);
			Alembic::AbcGeom::IXform xform((*obj->AlembicIObject::GetIObject()),Alembic::Abc::kWrapExisting);
			
			//Alembic::AbcGeom::IXform xform(obj->GetPtr(),Alembic::Abc::kWrapExisting);
			Alembic::AbcCoreAbstract::index_t sampleIndex = (Alembic::AbcCoreAbstract::index_t)iFrame;

			if(!xform.valid())
			return CStatus::OK;
			
		   // clamp the sample
		   if(sampleIndex < 0)
			  sampleIndex = 0;
		   else if(sampleIndex >= (Alembic::AbcCoreAbstract::index_t)xform.getSchema().getNumSamples())
			  sampleIndex = int(xform.getSchema().getNumSamples()) - 1;

		   Alembic::AbcGeom::XformSample sample;
		   xform.getSchema().get(sample,sampleIndex);

		   CTransformation transform;
		   transform.SetTranslationFromValues(sample.getTranslation().x,sample.getTranslation().y,sample.getTranslation().z);
		   transform.SetRotationFromAxisAngle(
			  CVector3(sample.getAxis().x,sample.getAxis().y,sample.getAxis().z),DegreesToRadians(sample.getAngle()));
		   transform.SetScalingFromValues(sample.getScale().x,sample.getScale().y,sample.getScale().z);
			CMatrix4f m;
			m.SetIdentity();

			CMatrix4 matrix = transform.GetMatrix4();
			for(int x=0;x<4;x++)
			{
				for(int y=0;y<4;y++)
				{
					m.SetValue(x,y,matrix.GetValue(x,y));
				}
			}
			
			// Get the output port array ...			
			CDataArrayMatrix4f outData( in_ctxt );
			outData[0] = m;			
			
		}
	}
	else
	{
		Application().LogMessage(L"User Data Empty...");
	}

	return CStatus::OK;
}

SICALLBACK AlembicIXForm_Init( CRef& in_ctxt )
{
	//Get Context from CRef
	Context ctxt(in_ctxt);

	// Build a new SIIAlembicObject
	SIAlembicIXForm* obj = new SIAlembicIXForm();
	
	// Store Object in User Data
	ctxt.PutUserData((CValue::siPtrType)obj);
	return CStatus::OK;
}

SICALLBACK AlembicIXForm_Term( CRef& in_ctxt )
{
	//Get Context from CRef
	Context ctxt(in_ctxt);

	//Get User Data
	CValue userData = ctxt.GetUserData();
	SIAlembicIXForm* obj = (SIAlembicIXForm*)(CValue::siPtrType)userData;
	delete obj;
	
	// Empty User Data
	ctxt.PutUserData((CValue::siPtrType)NULL);
	return CStatus::OK;
}


