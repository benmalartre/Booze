#ifndef _ALEMBIC_POLYMESH_H_
#define _ALEMBIC_POLYMESH_H_

#include "AlembicObject.h"

BOOZE_NAMESPACE_OPEN_SCOPE

struct ABC_Polymesh_Topo_Sample_Infos
{
	uint64_t m_numPoints;
	uint64_t m_numFaces;
	uint64_t m_numIndices;
	uint64_t m_numSamples;
	uint64_t m_sampleIndex;
	bool m_hasVelocity;
	bool m_hasNormal;
	bool m_hasColor;
	bool m_hasUvs;
	bool m_hasTangent;
	bool m_hasEnvelope;
};

struct ABC_Polymesh_Topo_Sample
{
	float * m_positions;
	float *	m_velocities;
	float * m_normals;
	float * m_tangents;
	float * m_uvs;
	float * m_colors;
	uint32_t*	m_faceIndices;
	uint32_t*	m_faceCount;

};

struct ABC_Polymesh_Sample_Infos
{
	uint64_t m_numPoints;
	bool m_hasVelocity;
	uint64_t m_sampleIndex;
};

struct ABC_Polymesh_Sample
{
	float * m_positions;
	float *	m_velocities;
};

/*
struct ABC_Envelope_Infos
{
uint32_t* _nbp;
uint32_t* _nbdeformers;
};
*/
struct ABC_Envelope_Sample
{
	uint64_t m_numDeformers;
	char *	m_indices;
	float * m_weights;
};


//------------------------------------------------------------------------------------------------
// Alembic Import
//------------------------------------------------------------------------------------------------
class AlembicIPolymesh : public AlembicIObject
{
public:
	AlembicIPolymesh(AbcG::IObject& iObj);
	bool Initialize();
	virtual void GetTopoSampleDescription(float time, ABC_Polymesh_Topo_Sample_Infos* infos);
	virtual int UpdateTopoSample(ABC_Polymesh_Topo_Sample_Infos* infos, ABC_Polymesh_Topo_Sample* io_sample);
	virtual void UpdatePointPosition(ABC_Polymesh_Sample_Infos* infos, ABC_Polymesh_Sample* io_sample);
	virtual size_t UpdateSample(ABC_Polymesh_Topo_Sample_Infos* infos, ABC_Polymesh_Topo_Sample* io_sample);

private:
	AbcG::IXformSchema m_xform;
	AbcG::IPolyMeshSchema m_mesh;
	AbcG::ICompoundProperty m_compound;
	AbcG::IPolyMeshSchema::Sample m_sample;
	AbcG::MeshTopologyVariance m_variance;
	int m_numSamples;
};

//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
class AlembicOPolymesh : public AlembicOObject
{
protected:
	AbcG::OXformSchema m_xform;
	AbcG::OPolyMeshSchema m_mesh;
	AbcG::OPolyMeshSchema::Sample m_sample;
	int m_numSamples;

public:
	AlembicOPolymesh(AlembicWriteJob* job, AlembicOObject* parent, void* customData, const char* name);
	virtual void AlembicOPolymesh::SetPositions(Imath::V3f* positions, int32_t numVertices);
	virtual void AlembicOPolymesh::SetDescription(int32_t* faceIndices, int32_t* faceCounts, int32_t numFaces);

	ABCStatus	Save();
	void		Set(Abc::OObject& obj);
};

BOOZE_NAMESPACE_CLOSE_SCOPE
#endif