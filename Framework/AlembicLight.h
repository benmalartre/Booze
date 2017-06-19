#ifndef _ALEMBIC_LIGHT_H_
#define _ALEMBIC_LIGHT_H_

#include "AlembicObject.h"

class AlembicOLight : public AlembicOObject
{
protected:
   Alembic::AbcGeom::OXformSchema _xformschema;
   Alembic::AbcGeom::OLightSchema _lightschema;
   Alembic::AbcGeom::XformSample _xformsample;

public:

	AlembicOLight(){};
	~AlembicOLight(){};

	virtual ABCStatus Save(double time){return Status_OK;};
};

class AlembicILight : public AlembicIObject
{
protected:
   Alembic::AbcGeom::IXformSchema _xformxchema;
   Alembic::AbcGeom::ILightSchema _lightschema;
   Alembic::AbcGeom::XformSample _xformsample;

public:

	AlembicILight(){};
	AlembicILight(Alembic::Abc::IObject* obj){_abcobject=obj;};
	~AlembicILight(){};
};

#endif