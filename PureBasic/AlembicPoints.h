#ifndef _ALEMBIC_POINTCLOUD_H_
#define _ALEMBIC_POINTCLOUD_H_

#include "AlembicObject.h"

BOOZE_NAMESPACE_OPEN_SCOPE

struct ABC_Points_Sample_Infos
{
	uint64_t _nbpoints;
	uint64_t _sampleindex;
	bool _hasvelocity;
	bool _hassize;
	bool _hasorientation;
	bool _hasscale;
	bool _hascolor;
};

struct ABC_Points_Sample
{
	uint64_t* _id;
	float * _position;
	float *	_velocity;
	float * _size;
	float *	_orientation;
	float * _scale;
	float * _color;

};


class AlembicOPoints : public AlembicOObject
{
protected:
Alembic::AbcGeom::OPolyMeshSchema _meshschema;
Alembic::AbcGeom::OPolyMeshSchema::Sample _meshsample;
int _numsamples;

public:
	AlembicOPoints( AlembicWriteJob * in_Job);
	virtual ABCStatus Save(double time);
};


class AlembicIPoints : public AlembicIObject
{
private:
	AbcG::IXformSchema _xformschema;
	AbcG::IPolyMeshSchema _meshschema;
	AbcG::ICompoundProperty _compoundprop;
	AbcG::XformSample _xformsample;
	AbcG::IPolyMeshSchema::Sample _pointssample;
	int mNumSamples;
public:
	AlembicIPoints(AbcG::IObject& iObj);
	virtual bool Initialize();
	virtual void GetSampleDescription(float frame, ABC_Points_Sample_Infos* infos);
	virtual size_t UpdateSample(ABC_Points_Sample_Infos* infos, ABC_Points_Sample* io_sample);
};

BOOZE_NAMESPACE_CLOSE_SCOPE
#endif

/*
#ifndef _ALEMBIC_POINTCLOUD_H_
#define _ALEMBIC_POINTCLOUD_H_

#include "AlembicObject.h"
class AlembicOPointCloud : public AlembicOObject
{
protected:
	Alembic::AbcGeom::OXform	_xform;
   Alembic::AbcGeom::OXformSchema _xformschema;
   Alembic::AbcGeom::OPointsSchema _pointsschema;
   Alembic::AbcGeom::XformSample _xformsample;
   Alembic::AbcGeom::OPointsSchema::Sample _pointssample;
   int _numsamples;

public:
	AlembicOPointCloud(){};
   AlembicOPointCloud( AlembicWriteJob * in_Job);
   ~AlembicOPointCloud(){};
   virtual ABCStatus Save(double time){return Status_OK;};
};


class AlembicIPointCloud : public AlembicIObject
{
protected:
	Alembic::AbcGeom::OXform	_xform;
	Alembic::AbcGeom::IXformSchema _xformschema;
   Alembic::AbcGeom::IPointsSchema _pointsschema;
   Alembic::AbcGeom::XformSample _xformsample;
   Alembic::AbcGeom::IPointsSchema::Sample _pointssample;
   int _numsamples;
   int _nbp;


public:
	AlembicIPointCloud(){};
	AlembicIPointCloud(Alembic::Abc::IObject* obj){_abcobject = obj;};
	~AlembicIPointCloud(){};
	virtual void Clean(){};
	virtual void Update(){};
};

#endif
*/