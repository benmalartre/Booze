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
	virtual bool					initialize();

protected:
	AbcG::IXformSchema				m_xform;
	AbcG::ICurvesSchema				m_curves;
	AbcG::ICompoundProperty			m_compound;
	AbcG::ICurvesSchema::Sample		m_sample;
	size_t							m_numSamples;
	size_t							m_numPoints;
	size_t							m_numCurves;
};

//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
typedef AbcU::shared_ptr< AbcG::OCurves> ABCOCurvesPtr;
class AlembicOCurves : public AlembicOObject
{
protected:
	ABCOCurvesPtr					m_curves;
	AbcG::OCurvesSchema				m_schema;
	AbcG::OCurvesSchema::Sample		m_sample;
	size_t							m_numsamples;
	size_t							m_numPoints;
	size_t							m_numCurves;

public:
	AlembicOCurves(AlembicOArchive* archive, AlembicOObject* parent, void* customData, const char* name);
	virtual void					save(AbcA::TimeSamplingPtr time, AbcG::OObject& parent) override;
};

BOOZE_NAMESPACE_CLOSE_SCOPE

#endif