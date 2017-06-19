#ifndef _SIALEMBIC_OBJECT_H_
#define _SIALEMBIC_OBJECT_H_

#include "AlembicFoundation.h"
#include "SIAlembicRegister.h"
#include "SIAlembicWriteJob.h"

class SIAlembicOObject : public AlembicOObject
{
protected:
   XSI::CRef _ref;
   SIAlembicWriteJob * _job;

public:
	//SIOAlembicObject(){};
   SIAlembicOObject(const XSI::CRef & in_ref, SIAlembicWriteJob * in_job);
   ~SIAlembicOObject(){};

   const XSI::CRef & GetRef() { return _ref; }
   SIAlembicWriteJob* GetAlembicJob(){return _job;};
   //ABCStatus Save(double time);
};

class SIAlembicIObject : public AlembicIObject
{
protected:
   XSI::CRef _ref;

public:
	SIAlembicIObject(){_init = false;};
	SIAlembicIObject(const XSI::CRef & in_Ref);
   ~SIAlembicIObject();

   const XSI::CRef & GetRef() { return _ref; }

   Alembic::Abc::IObject* GetIObject(){return _abcobject;};
   bool Initialized(){return _init;}; 
   bool IsDirty(ICENodeContext& in_ctxt);
   void Init(ICENodeContext& in_ctxt);
   void Init(ICENodeContext& in_ctxt, const Alembic::Abc::MetaData& metadata);
   virtual void Check(){};

   void Clean();
   virtual void Update(OperatorContext& in_ctxt){};
};

typedef boost::shared_ptr < SIAlembicOObject > SIAlembicOObjectPtr;
typedef boost::shared_ptr < SIAlembicIObject > SIAlembicIObjectPtr;

#include "AlembicWriteJob.h"

#endif