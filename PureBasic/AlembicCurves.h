#ifndef _ALEMBIC_CURVE_H_
#define _ALEMBIC_CURVE_H_

#include "AlembicObject.h"

BOOZE_NAMESPACE_OPEN_SCOPE

struct ABC_Curves_Sample_Infos
{
	uint64_t m_numPoints;
	uint64_t m_numCurves;
	uint64_t m_sampleindex;
	bool m_hasWidth;
	bool m_hasUV;
	bool m_hasNormal;
	bool m_hasWeight;
	bool m_hasOrder;
	bool m_hasKnot;
};

struct ABC_Curves_Sample
{
	float* m_position;
	uint32_t* m_numVerticesPerCurve;
	AbcG::CurveType m_type;
	AbcG::CurvePeriodicity m_periodicity;
	float* m_width;
	float* m_uv;
	float* m_normal;
	AbcG::BasisType m_basis;
	float* m_weight;
	char* m_order;
	float* m_knot;
};

//------------------------------------------------------------------------------------------------
// Alembic Import
//------------------------------------------------------------------------------------------------
class AlembicICurves : public AlembicIObject
{
public:
	AlembicICurves(AbcG::IObject& iObj);
	virtual bool Initialize();

protected:
	Alembic::AbcGeom::ICurvesSchema m_curveschema;
	Alembic::AbcGeom::ICompoundProperty m_compoundprop;
	Alembic::AbcGeom::ICurvesSchema::Sample m_meshsample;
	size_t m_numSamples;
	size_t m_numPoints;
	size_t m_numCurves;
};

//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
class AlembicOCurves : public AlembicOObject
{
protected:
	Alembic::AbcGeom::OCurvesSchema m_curvesSchema;
	Alembic::AbcGeom::OCurvesSchema::Sample m_curvesSample;
	size_t m_numsamples;
	size_t m_numPoints;
	size_t m_numCurves;

public:
	AlembicOCurves(AlembicWriteJob* job, void* customData);
	virtual ABCStatus Save(double time, ABC_Curves_Sample_Infos* infos, ABC_Curves_Sample* sample);
};

BOOZE_NAMESPACE_CLOSE_SCOPE

#endif