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


class AlembicOPoints : public AlembicOObject
{
protected:
Alembic::AbcGeom::OPolyMeshSchema _meshschema;
Alembic::AbcGeom::OPolyMeshSchema::Sample _meshsample;
int _numsamples;

public:
	AlembicOPoints( AlembicWriteJob * in_job, void* customData);
	virtual ABCStatus Save(float time);
};


class AlembicIPoints : public AlembicIObject
{
private:
	AbcG::IXformSchema m_xformSchema;
	AbcG::IPolyMeshSchema m_meshSchema;
	AbcG::ICompoundProperty m_compoundProp;
	AbcG::XformSample m_xformSample;
	AbcG::IPolyMeshSchema::Sample m_pointsSample;
	int m_numSamples;
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