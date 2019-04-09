#ifndef _PBALEMBIC_CAMERA_H_
#define _PBALEMBIC_CAMERA_H_

#include "AlembicObject.h"

BOOZE_NAMESPACE_OPEN_SCOPE

struct ABC_Camera_Sample_Infos
{

};

struct ABC_Camera_Sample
{

};

//------------------------------------------------------------------------------------------------
// Alembic Import
//------------------------------------------------------------------------------------------------
class AlembicICamera : public AlembicIObject
{
public:
	AlembicICamera(AbcG::IObject& iObj);
	virtual bool Initialize();

protected:
	Alembic::AbcGeom::ICameraSchema m_camera;
};

//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
class AlembicOCamera : public AlembicOObject
{
protected:
	Alembic::AbcGeom::OCameraSchema m_camera;
	//Alembic::AbcGeom::OCameraSchema::Sample _sample;

public:
	AlembicOCamera(AlembicWriteJob * in_Job, AlembicOObject* parent, void* customData, const char* name);
	virtual ABCStatus Save(double time, ABC_Camera_Sample_Infos* infos, ABC_Camera_Sample* sample);
};


BOOZE_NAMESPACE_CLOSE_SCOPE

#endif
