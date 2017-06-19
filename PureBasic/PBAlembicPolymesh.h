#ifndef _PBALEMBIC_POLYMESH_H_
#define _PBALEMBIC_POLYMESH_H_

#include "PBAlembicRegister.h"

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

#endif