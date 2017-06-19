#ifndef _PBALEMBIC_POINTCLOUD_H_
#define _PBALEMBIC_POINTCLOUD_H_

#include "PBAlembicRegister.h"

struct ABC_PointCloud_Sample_Infos
{
	uint64_t _nbpoints;
	uint64_t _sampleindex;
	bool _hasvelocity;
	bool _hassize;
	bool _hasorientation;
	bool _hasscale;
	bool _hascolor;
};

struct ABC_PointCloud_Sample
{
	uint64_t* _id;
	float * _position;
	float *	_velocity;
	float * _size;
	float *	_orientation;
	float * _scale;
	float * _color;

};

#endif
