#ifndef _ALEMBIC_OBJECT_H_
#define _ALEMBIC_OBJECT_H_

#include "AlembicFoundation.h"
#include "AlembicArchive.h"
#include "AlembicProperty.h"
#include "AlembicTimeSampling.h"

BOOZE_NAMESPACE_OPEN_SCOPE

// ===============================================================================================
//	ALEMBIC IMPORT  :  AlembicIObject
// ===============================================================================================
class AlembicIArchive;
class AlembicIProperty;

class AlembicIObject
{
public:
	AlembicIObject(Abc::IObject& object):m_object(object){};
	virtual bool					initialize();
	virtual const char*				getName();
	virtual const char*				getFullName();
	virtual ABCGeometricType		getType(){return m_type;};
	virtual Abc::IObject&			get(){ return m_object; };
	virtual void					getProperties();
	virtual bool					hasProperty(const char* p_name);
	virtual uint64_t				getNumProperties(){return (uint64_t)m_props.size();};
	virtual AlembicIProperty*		getProperty(uint64_t index);

protected:
	Abc::IObject					m_object;
	ABCGeometricType				m_type;
	std::vector<AlembicIProperty*>	m_props;
	void*							m_sample;
};

// ===============================================================================================
//	ALEMBIC EXPORT  :  AlembicOObject
// ===============================================================================================
typedef AbcU::shared_ptr< AbcG::OObject> ABCOObjectPtr;
class AlembicOArchive;
class AlembicWriteJob;
class AlembicOObject
{
public:
	AlembicOObject(AlembicOArchive* archive, AlembicOObject* parent, void* data, const char* name, ABCGeometricType type) :
		m_archive(archive),
		m_data(data), 
		m_type(type),
		m_parent(parent),
		m_name(name){
	};

	virtual ~AlembicOObject(){};
	virtual Alembic::Abc::MetaData&	getMetaData(){ return m_metadata; };
	virtual void*					getCustomData(){ return m_data; };
	virtual	AbcG::OObject			get() = 0;
	virtual	ABCOObjectPtr			getPtr() = 0;
	virtual AlembicOArchive*		getArchive(){ return m_archive; };
	virtual AlembicTimeSampling*	getTimeSampling();
	virtual AlembicWriteJob*		getJob(){ return m_archive->getJob(); };
	virtual void					save(AbcA::TimeSamplingPtr time)=0;
	virtual void					addChild(AlembicOObject* obj){m_children.push_back(obj);};

protected:
	AlembicOArchive*				m_archive;
	AlembicOObject*					m_parent;
	ABCGeometricType				m_type;
	Abc::MetaData					m_metadata;
	std::vector<AlembicOProperty*>	m_props;
	std::vector<AlembicOObject*>	m_children;
	void*							m_sample;
	void*							m_data;
	std::string						m_name;
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