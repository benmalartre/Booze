#ifndef _ALEMBIC_POINTCLOUD_H_
#define _ALEMBIC_POINTCLOUD_H_

#include "AlembicObject.h"

BOOZE_NAMESPACE_OPEN_SCOPE

struct ABC_Points_Sample_Infos
{
	uint64_t m_numPoints;
	uint64_t m_sampleIndex;
	bool m_hasVelocity;
	bool m_hasSize;
	bool m_hasOrientation;
	bool m_hasScale;
	bool m_hasColor;
};

struct ABC_Points_Sample
{
	uint64_t* m_id;
	float * m_position;
	float *	m_velocity;
	float * m_size;
	float *	m_orientation;
	float * m_scale;
	float * m_color;
};

//------------------------------------------------------------------------------------------------
// Alembic Import
//------------------------------------------------------------------------------------------------
class AlembicIPoints : public AlembicIObject
{
private:
	AbcG::IXformSchema m_xform;
	AbcG::IPolyMeshSchema m_mesh;
	AbcG::ICompoundProperty m_compound;
	AbcG::XformSample m_xformSample;
	AbcG::IPolyMeshSchema::Sample m_sample;
	int m_numSamples;
public:
	AlembicIPoints(AbcG::IObject& iObj);
	virtual bool Initialize();
	virtual void GetSampleDescription(float frame, ABC_Points_Sample_Infos* infos);
	virtual size_t UpdateSample(ABC_Points_Sample_Infos* infos, ABC_Points_Sample* io_sample);
};

//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
class AlembicOPoints : public AlembicOObject
{
protected:
AbcG::OXformSchema m_xform;
AbcG::OPointsSchema m_points;
AbcG::OPointsSchema::Sample m_sample;
int m_numSamples;

public:
	AlembicOPoints( AlembicWriteJob * in_job, AlembicOObject* parent, void* customData, const char* name);
	virtual ABCStatus Save(float time);
};

BOOZE_NAMESPACE_CLOSE_SCOPE
#endif