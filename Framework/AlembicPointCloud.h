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