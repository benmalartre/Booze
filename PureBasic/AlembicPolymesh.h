#ifndef _ALEMBIC_POLYMESH_H_
#define _ALEMBIC_POLYMESH_H_

#include "AlembicObject.h"

BOOZE_NAMESPACE_OPEN_SCOPE

struct ABC_Polymesh_Topo_Sample_Infos
{
	uint64_t _nbpoints;
	uint64_t _nbfaces;
	uint64_t _nbindices;
	uint64_t _nbsamples;
	uint64_t _sampleindex;
	bool _hasvelocity;
	bool _hasnormal;
	bool _hascolor;
	bool _hasuvs;
	bool _hastangent;
	bool _hasenvelope;
};

struct ABC_Polymesh_Topo_Sample
{
	float * _positions;
	float *	_velocities;
	float * _normals;
	float * _tangents;
	float * _uvs;
	float * _colors;
	uint32_t*	_faceindices;
	uint32_t*	_facecount;

};

struct ABC_Polymesh_Sample_Infos
{
	uint64_t _nbpoints;
	bool _hasvelocity;
	uint64_t _sampleindex;
};

struct ABC_Polymesh_Sample
{
	float * _positions;
	float *	_velocities;
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
	uint64_t _nbdeformers;
	char *	_indices;
	float * _weights;
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
	AbcG::IPolyMeshSchema _meshschema;
	AbcG::ICompoundProperty _compoundprop;
	AbcG::IPolyMeshSchema::Sample _meshsample;
	AbcG::MeshTopologyVariance _variance;
	int mNumSamples;
};

//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
class AlembicOPolymesh : public AlembicOObject
{
protected:
	Alembic::AbcGeom::OPolyMeshSchema _meshschema;
	Alembic::AbcGeom::OPolyMeshSchema::Sample _meshsample;
	int _numsamples;

public:
	AlembicOPolymesh(AlembicWriteJob * in_Job);
	virtual ABCStatus Save(double time);
};

BOOZE_NAMESPACE_CLOSE_SCOPE
#endif