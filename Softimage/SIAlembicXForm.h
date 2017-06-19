#ifndef _SIALEMBIC_XFORM_H_
#define _SIALEMBIC_XFORM_H_

#include "SIAlembicRegister.h"
#include "SIAlembicObject.h"
#include "AlembicXForm.h"

class SIAlembicOXForm : public SIAlembicOObject, public AlembicOXForm
{

public:
	//SIAlembicOPolymesh():SIAlembicOPolymesh(){};
	SIAlembicOXForm(const XSI::CRef & in_ref, SIAlembicWriteJob * in_job);
   ~SIAlembicOXForm(){};

   ABCStatus Save(double time);
};

class SIAlembicIXForm : public  SIAlembicIObject, public AlembicIXForm
{
public:
	SIAlembicIXForm(){};
   SIAlembicIXForm(const XSI::CRef & in_Ref);
   ~SIAlembicIXForm(){};

   void Check();
};

void SaveXFormSample(XSI::CRef kinestateRef, Alembic::AbcGeom::OXformSchema & schema, Alembic::AbcGeom::XformSample & sample, double time);


#endif