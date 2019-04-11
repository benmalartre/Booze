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
	virtual bool initialize();

protected:
	Alembic::AbcGeom::ICameraSchema			m_camera;
};

//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
typedef AbcU::shared_ptr< AbcG::OCamera> ABCOCameraPtr;
class AlembicOCamera : public AlembicOObject
{
protected:
	ABCOCameraPtr					m_camera;
	Alembic::AbcGeom::OCameraSchema m_schema;

public:
	AlembicOCamera(AlembicOArchive* archive, AlembicOObject* parent, void* customData, const char* name);
	
	~AlembicOCamera(){ if (m_camera)delete m_camera.get(); };
	void							save(AbcA::TimeSamplingPtr time) override;
	AbcG::OObject					get(){ return *m_camera; };
	ABCOObjectPtr					getPtr() override { return m_camera; };
};


BOOZE_NAMESPACE_CLOSE_SCOPE

#endif
