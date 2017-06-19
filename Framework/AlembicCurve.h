#ifndef _ALEMBIC_CURVE_H_
#define _ALEMBIC_CURVE_H_

#include "AlembicObject.h"
class AlembicOCurves : public AlembicOObject
{
protected:
   Alembic::AbcGeom::OXformSchema _xformschema;
   Alembic::AbcGeom::OCurvesSchema _curveschema;
   Alembic::AbcGeom::XformSample _xformsample;
   Alembic::AbcGeom::OCurvesSchema::Sample _curvesample;
   size_t _numsamples;
   size_t _nbp;
	size_t _nbcurves;

public:
	AlembicOCurves(){};
   AlembicOCurves( AlembicWriteJob * in_Job);
   ~AlembicOCurves(){};
   virtual ABCStatus Save(double time){return Status_OK;};
};


class AlembicICurves : public AlembicIObject
{
protected:
	Alembic::AbcGeom::IXformSchema _xformschema;
	Alembic::AbcGeom::ICurvesSchema _curveschema;
	Alembic::AbcGeom::ICompoundProperty _compoundprop;
	Alembic::AbcGeom::XformSample _xformsample;
	Alembic::AbcGeom::ICurvesSchema::Sample _meshsample;
	size_t mNumSamples;
	size_t _nbp;
	size_t _nbcurves;
public:
	AlembicICurves(){};
	AlembicICurves(Alembic::Abc::IObject* obj){_abcobject=obj;};
	~AlembicICurves(){};
	virtual void Clean(){};
	virtual void Update(){};
};

#endif