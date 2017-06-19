#ifndef _SIALEMBIC_POLYMESH_H_
#define _SIALEMBIC_POLYMESH_H_

#include "AlembicFoundation.h"
#include "AlembicPolymesh.h"
#include "SIAlembicRegister.h"
#include "SIAlembicObject.h"

class SIAlembicOPolymesh : public SIAlembicOObject, public AlembicOPolymesh
{

public:
	//SIAlembicOPolymesh():SIAlembicOPolymesh(){};
	SIAlembicOPolymesh(const XSI::CRef & in_ref, SIAlembicWriteJob * in_job);
   ~SIAlembicOPolymesh(){};

   ABCStatus Save(double time);
};

class SIAlembicIPolymesh : public SIAlembicIObject, public AlembicIPolymesh
{
public:
	SIAlembicIPolymesh():SIAlembicIObject(){};
   SIAlembicIPolymesh(const XSI::CRef & in_Ref);
   ~SIAlembicIPolymesh(){};

   //void Init(ICENodeContext& in_ctxt);
   void Check();
   void Update(ICENodeContext& in_ctxt);
   void Clean(){};
   void Update(OperatorContext& in_ctxt);
};

#endif