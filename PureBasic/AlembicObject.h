#ifndef _ALEMBIC_OBJECT_H_
#define _ALEMBIC_OBJECT_H_

#include "AlembicFoundation.h"
#include "AlembicArchive.h"
#include "AlembicProperty.h"

BOOZE_NAMESPACE_OPEN_SCOPE

// ===============================================================================================
//	AlembicIObject
// ===============================================================================================
class AlembicIArchive;
class AlembicIProperty;

class AlembicIObject
{
public:
	AlembicIObject(Abc::IObject& object):m_object(object){};
	virtual bool				Initialize();
	virtual const char*			GetName();
	virtual const char*			GetFullName();
	virtual ABCGeometricType	GetType(){return m_type;};
	virtual Abc::IObject&		Get(){ return m_object; };
	virtual void				GetProperties();
	virtual bool				HasProperty(const char* p_name);
	virtual uint64_t			GetNumProperties(){return (uint64_t)m_props.size();};
	virtual AlembicIProperty*	GetProperty(uint64_t index);

protected:
	Abc::IObject					m_object;
	ABCGeometricType				m_type;
	std::vector<AlembicIProperty*>	m_props;
	void*							m_sample;
};

// ===============================================================================================
//	AlembicIObject
// ===============================================================================================
class AlembicOArchive;
class AlembicWriteJob;
class AlembicOObject
{
public:
	AlembicOObject(AlembicWriteJob* job, AlembicOObject* parent, void* customData, ABCGeometricType type) :
		m_job(job),
		m_customData(customData), 
		m_type(type),
		m_parent(parent){};

	virtual AbcG::OObject			Get(){ return m_object; };
	virtual Alembic::Abc::MetaData	GetMetaData(){ return m_metadata; };
	virtual void*					GetCustomData(){ return m_customData; };
	virtual AlembicWriteJob*		GetWriteJob(){ return m_job; };
	virtual ABCStatus				Save(double time){ return Status_OK; };
protected:
	AlembicOObject*					m_parent;
	ABCGeometricType				m_type;
	Alembic::Abc::OObject			m_object;
	Alembic::Abc::MetaData			m_metadata;
	std::vector<AlembicOProperty*>	m_props;
	void*							m_sample;
	AlembicWriteJob*				m_job;
	void*							m_customData;
};

// ===============================================================================================
//	EXPORTS
// ===============================================================================================
BOOZE_EXPORT AlembicOObject*	newOObject(AlembicOArchive* archive, uint64_t index);
BOOZE_EXPORT void				deleteOObject(AlembicOObject* object);

BOOZE_EXPORT AlembicIObject*	newIObject(AlembicIArchive* archive, uint64_t index);
BOOZE_EXPORT void				deleteIObject(AlembicIObject* object);

BOOZE_NAMESPACE_CLOSE_SCOPE
#endif