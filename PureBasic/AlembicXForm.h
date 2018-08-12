#ifndef _ALEMBIC_XFORM_H_
#define _ALEMBIC_XFORM_H_

#include "AlembicObject.h"

BOOZE_NAMESPACE_OPEN_SCOPE

struct XForm_Sample
{
	float pos[3];
	float ori[4];
	float scl[3];
};


/*
class AlembicOXForm : public AlembicOObject
{
protected:
	Alembic::AbcGeom::OXformSchema _xformschema;
	Alembic::AbcGeom::XformSample _xformsample;
	int _numsamples;

public:
	AlembicOXForm(){};
	AlembicOXForm(AlembicWriteJob * in_Job);
	~AlembicOXForm(){};
	ABCStatus Save(double time){ return Status_OK; };
};
*/

class AlembicIXForm : public AlembicIObject
{
protected:
	Alembic::AbcGeom::IXform _xform;
	Alembic::AbcGeom::IXformSchema _xformschema;
	Alembic::AbcGeom::XformSample _xformsample;
	int mNumSamples;

public:
	AlembicIXForm(Alembic::Abc::IObject& obj);
	bool Initialize();
};
BOOZE_NAMESPACE_CLOSE_SCOPE
#endif