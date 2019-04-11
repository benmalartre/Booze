#include "AlembicRoot.h"
#include "AlembicWriteJob.h"

BOOZE_NAMESPACE_OPEN_SCOPE


AlembicIRoot::AlembicIRoot(AbcG::IObject& object) :AlembicIObject(object)
{
	m_type = GeometricType_Root;
}

bool AlembicIRoot::initialize()
{
	AbcG::IObject _camera(m_object);
	getProperties();
	return true;
}

//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
AlembicORoot::AlembicORoot(AlembicOArchive* archive, void* customData, const char* name)
: AlembicOObject(archive, NULL, customData, name, GeometricType_Root){
};

void AlembicORoot::save(AbcA::TimeSamplingPtr time, AbcG::OObject& parent)
{
	
}


BOOZE_NAMESPACE_CLOSE_SCOPE