#ifndef _PBALEMBIC_ROOT_H_
#define _PBALEMBIC_ROOT_H_

#include "AlembicObject.h"

BOOZE_NAMESPACE_OPEN_SCOPE

//------------------------------------------------------------------------------------------------
// Alembic Import
//------------------------------------------------------------------------------------------------
class AlembicIRoot : public AlembicIObject
{
public:
	AlembicIRoot(AbcG::IObject& iObj);
	virtual bool initialize();

protected:
	
	Alembic::AbcGeom::IXformSchema			m_schema;
};

//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
typedef AbcU::shared_ptr< AbcG::OObject> ABCORootPtr;
class AlembicORoot : public AlembicOObject
{
public:
	AlembicORoot(AlembicOArchive* archive, void* customData, const char* name);

	~AlembicORoot(){ };
	void									save(AbcA::TimeSamplingPtr time) override;

	AbcG::OObject							get(){ return m_archive->get().getTop(); };
	ABCOObjectPtr							getPtr(){ return NULL; };

protected:

};


BOOZE_NAMESPACE_CLOSE_SCOPE

#endif
