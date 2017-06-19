#ifndef _ALEMBIC_CAMERA_H_
#define _ALEMBIC_CAMERA_H_

#include "AlembicObject.h"

class AlembicOCamera : public AlembicOObject
{
protected:
   Alembic::AbcGeom::OXformSchema _xformschema;
   Alembic::AbcGeom::OCameraSchema _cameraschema;
   Alembic::AbcGeom::XformSample _xformsample;
   Alembic::AbcGeom::CameraSample _camerasample;
public:

	AlembicOCamera(){};
	~AlembicOCamera(){};

	virtual ABCStatus Save(double time){return Status_OK;};
};

class AlembicICamera : public AlembicIObject
{
protected:
   Alembic::AbcGeom::IXformSchema _xformxchema;
   Alembic::AbcGeom::ICameraSchema _cameraschema;
   Alembic::AbcGeom::XformSample _xformsample;
   Alembic::AbcGeom::CameraSample _camerasample;
public:

	AlembicICamera(){};
	AlembicICamera(Alembic::Abc::IObject* obj){_abcobject=obj;};
	~AlembicICamera(){};
};

#endif