#ifndef _ALEMBIC_POLYMSH_H_
#define _ALEMBIC_POLYMSH_H_

#include "AlembicObject.h"
class AlembicOPolymesh : public AlembicOObject
{
protected:
   Alembic::AbcGeom::OXformSchema _xformschema;
   Alembic::AbcGeom::OPolyMeshSchema _meshschema;
   Alembic::AbcGeom::XformSample _xformsample;
   Alembic::AbcGeom::OPolyMeshSchema::Sample _meshsample;
   int _numsamples;

public:
	AlembicOPolymesh(){};
   AlembicOPolymesh( AlembicWriteJob * in_Job);
   ~AlembicOPolymesh(){};
   virtual ABCStatus Save(double time){return Status_OK;};
};


class AlembicIPolymesh : public AlembicIObject
{
protected:
	Alembic::AbcGeom::IXformSchema _xformschema;
	Alembic::AbcGeom::IPolyMeshSchema _meshschema;
	Alembic::AbcGeom::ICompoundProperty _compoundprop;
	Alembic::AbcGeom::XformSample _xformsample;
	Alembic::AbcGeom::IPolyMeshSchema::Sample _meshsample;
	int mNumSamples;

public:
	AlembicIPolymesh(){};
	AlembicIPolymesh(Alembic::Abc::IObject* obj){_abcobject=obj;};
	~AlembicIPolymesh(){};
	virtual void Clean(){};
	virtual void Update(){};
};

#endif