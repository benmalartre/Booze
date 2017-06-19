#ifndef _ALEMBIC_XFORM_H_
#define _ALEMBIC_XFORM_H_

#include "AlembicObject.h"
class AlembicOXForm: public AlembicOObject
{
protected:
	Alembic::AbcGeom::OXformSchema _xformschema;
   Alembic::AbcGeom::XformSample _xformsample;
   int _numsamples;

public:
	AlembicOXForm(){};
   AlembicOXForm( AlembicWriteJob * in_Job);
   ~AlembicOXForm(){};
   ABCStatus Save(double time){return Status_OK;};
};


class AlembicIXForm: public AlembicIObject
{
protected:
	Alembic::AbcGeom::IXform _xform;
	Alembic::AbcGeom::IXformSchema _xformschema;
	Alembic::AbcGeom::XformSample _xformsample;
	int mNumSamples;

public:
	AlembicIXForm(){};
	AlembicIXForm(Alembic::Abc::IObject* obj){_abcobject = obj;};
	~AlembicIXForm(){};

};

#endif