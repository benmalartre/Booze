#ifndef _ALEMBIC_CURVE_H_
#define _ALEMBIC_CURVE_H_

#include "AlembicObject.h"

BOOZE_NAMESPACE_OPEN_SCOPE

struct ABC_Curves_Sample_Infos
{
	uint64_t _numpoints;
	uint64_t _numcurves;
	uint64_t _sampleindex;
	bool _hasWidth;
	bool _hasUVs;
	bool _hasNormals;
	bool _hasWeights;
	bool _hasOrders;
	bool _hasKnots;
};

struct ABC_Curves_Sample
{
	float* _positions;
	uint32_t* _numVerticesPerCurve;
	AbcG::CurveType _type;
	AbcG::CurvePeriodicity _periodicity;
	float* width;
	float* uvs;
	float* normals;
	AbcG::BasisType _basis;
	float* _weights;
	char* _orders;
	float* _knots;
};

class AlembicOCurves : public AlembicOObject
{
protected:
   Alembic::AbcGeom::OCurvesSchema _curveschema;
   Alembic::AbcGeom::OCurvesSchema::Sample _curvesample;
   size_t _numsamples;
   size_t _nbp;
	size_t _nbcurves;

public:
	AlembicOCurves( AlembicWriteJob * in_Job);
	virtual ABCStatus Save(double time, ABC_Curves_Sample_Infos* infos, ABC_Curves_Sample* sample);
};


class AlembicICurves : public AlembicIObject
{
public:
	AlembicICurves(AbcG::IObject& iObj);
	virtual bool Initialize();

protected:
	Alembic::AbcGeom::ICurvesSchema _curveschema;
	Alembic::AbcGeom::ICompoundProperty _compoundprop;
	Alembic::AbcGeom::ICurvesSchema::Sample _meshsample;
	size_t mNumSamples;
	size_t _nbp;
	size_t _nbcurves;
};

BOOZE_NAMESPACE_CLOSE_SCOPE

#endif