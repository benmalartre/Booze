#ifndef _ALEMBIC_POLYMESH_H_
#define _ALEMBIC_POLYMESH_H_

#include "AlembicObject.h"

BOOZE_NAMESPACE_OPEN_SCOPE

struct ABC_Polymesh_Topo_Sample_Infos
{
	int32_t m_numPoints;
	int32_t m_numFaces;
	int32_t m_numIndices;
	int32_t m_numSamples;
	int32_t m_sampleIndex;
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
	int32_t*	m_faceIndices;
	int32_t*	m_faceCount;

};

struct ABC_Polymesh_Sample_Infos
{
	uint32_t m_numPoints;
	bool m_hasVelocity;
	uint32_t m_sampleIndex;
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
	uint32_t m_numDeformers;
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
	virtual	~AlembicIPolymesh(){};
	virtual bool					initialize();
	virtual void					getTopoSampleDescription(float time, ABC_Polymesh_Topo_Sample_Infos* infos);
	virtual int						updateTopoSample(ABC_Polymesh_Topo_Sample_Infos* infos, ABC_Polymesh_Topo_Sample* io_sample);
	virtual void					updatePointPosition(ABC_Polymesh_Sample_Infos* infos, ABC_Polymesh_Sample* io_sample);
	virtual size_t					updateSample(ABC_Polymesh_Topo_Sample_Infos* infos, ABC_Polymesh_Topo_Sample* io_sample);

private:
	AbcG::IXformSchema				m_xform;
	AbcG::IPolyMeshSchema			m_mesh;
	AbcG::ICompoundProperty			m_compound;
	AbcG::IPolyMeshSchema::Sample	m_sample;
	AbcG::MeshTopologyVariance		m_variance;
	int m_numSamples;
};

//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
typedef AbcU::shared_ptr< AbcG::OPolyMesh> ABCOPolymeshPtr;
class AlembicOPolymesh : public AlembicOObject
{
private:
	ABCOPolymeshPtr					m_mesh;
	AbcG::OPolyMeshSchema			m_schema;
	int								m_numSamples;
	ABC_Polymesh_Topo_Sample_Infos  m_infos;
	ABC_Polymesh_Topo_Sample		m_sample;
	std::vector<Imath::V3f>			m_positions;
	std::vector<int32_t>			m_indices;
	std::vector<int32_t>			m_counts;

public:
	AlembicOPolymesh(AlembicOArchive* archive, AlembicOObject* parent, void* customData, const char* name);
	

	virtual void					set(Imath::V3f* positions, int numVertices, int* faceIndices, int* faceCounts, int numFaces);
	virtual void					setPositions(Imath::V3f* positions, int numVertices);
	virtual void					setDescription(int* faceIndices, int* faceCounts, int numFaces);

	virtual ~AlembicOPolymesh(){};
	void							save(AbcA::TimeSamplingPtr time) override;
	AbcG::OObject					get()override{ return *m_mesh; };
	ABCOObjectPtr					getPtr() override { return (ABCOObjectPtr&)m_mesh; };

	void							test(AbcG::OPolyMeshSchema& schema, Imath::V3f& offset){
		/*
		// some apps can arbitrarily name their primary UVs, this function allows
		// you to do that, and must be done before the first time you set UVs
		// on the schema
		mesh.setUVSourceName("test");

		Imath::V2f UVS[3] = {
		Imath::V2f(-1.0f, 0.0f),
		Imath::V2f(0.0f, 1.0f),
		Imath::V2f(1.0f, 0.0f)
		};

		Imath::V3f NORMALS[3] = {
		Imath::V3f(-1.0f, 0.0f, 0.0f),
		Imath::V3f(0.0f, 1.0f, 0.0f),
		Imath::V3f(1.0f, 0.0f, 0.0f)
		};

		// UVs and Normals use GeomParams, which can be written or read
		// as indexed or not, as you'd like.
		AbcG::OV2fGeomParam::Sample uvsamp(AbcG::V2fArraySample((const AbcG::V2f *)UVS,
		3),
		AbcG::kFacevaryingScope);
		// indexed normals
		AbcG::ON3fGeomParam::Sample nsamp(AbcG::N3fArraySample((const AbcG::N3f *)NORMALS,
		3),
		AbcG::kFacevaryingScope);
		*/

		Imath::V3f VERTICES[3] = {
			Imath::V3f(-1.0f, 0.0f, 0.0f) + offset,
			Imath::V3f(0.0f, 1.0f, 0.0f) + offset,
			Imath::V3f(1.0f, 0.0f, 0.0f) + offset
		};

		int32_t INDICES[3] = { 0, 1, 2 };
		int32_t COUNT[1] = { 3 };

		// Set a mesh sample.
		// We're creating the sample inline here,
		// but we could create a static sample and leave it around,
		// only modifying the parts that have changed.
		AbcG::OPolyMeshSchema::Sample sample(
			AbcG::V3fArraySample((const AbcG::V3f *)VERTICES, 3),
			AbcG::Int32ArraySample(INDICES, 3),
			AbcG::Int32ArraySample(COUNT, 1));
		//uvsamp, nsamp);

		// not actually the right data; just making it up
		AbcG::Box3d cbox;
		cbox.extendBy(AbcG::V3f(-1.0, 0.0, 0.0)+offset);
		cbox.extendBy(AbcG::V3f(1.0, 1.0, 0.0)+offset);

		schema.set(sample);
	}
};

BOOZE_NAMESPACE_CLOSE_SCOPE
#endif