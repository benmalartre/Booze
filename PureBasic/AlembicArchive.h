#ifndef _BOOZE_ARCHIVE_STORAGE_H_
#define _BOOZE_ARCHIVE_STORAGE_H_

#include "AlembicFoundation.h"
#include <map>

BOOZE_NAMESPACE_OPEN_SCOPE

//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
class AlembicOObject;
class AlembicWriteJob;
class AlembicOArchive
{
public:
	AlembicOArchive(AlembicWriteJob* job);
	~AlembicOArchive();
	virtual bool							open(const char* filename);
	virtual bool							close();
	virtual bool							isValid(){ return m_valid; };
	virtual AlembicOObject*					addObject(AlembicOObject* parent, char* name, ABCGeometricType type, void* ptr);
	virtual Abc::OArchive&					get(){ return m_archive; };
	virtual AlembicOObject*					getRoot(){ return m_root; };
	virtual AlembicWriteJob*				getJob(){ return m_job; };
	virtual int								getNumObjects();
	virtual AlembicOObject*					getObjectByIndex(unsigned index){ return m_objects[index]; };
	virtual AlembicOObject*					getObjectByName(const char* name);

private:
	
	Abc::OArchive							m_archive;
	AlembicOObject*							m_root;
	AlembicWriteJob*						m_job;
	bool									m_valid;
	std::vector<AlembicOObject*>			m_objects;
	std::map<string, int>					m_map;
};

//------------------------------------------------------------------------------------------------
// Alembic Import
//------------------------------------------------------------------------------------------------
class AlembicIObject;
class AlembicIArchive
{
public:
	AlembicIArchive() :m_uses(0){};
	virtual bool							open(const char* filename);
	virtual bool							close();
	virtual bool							isValid(){return m_valid;};
	virtual const char*						getInfos();
	virtual void							addObject(AlembicIObject* obj);
	virtual uint64_t						getNumIdentifiers();
	virtual uint64_t						getNumObjects();
	virtual const char*						getIdentifier(uint64_t id);
	virtual const AbcG::IObject&			getIObj(uint64_t i);
	virtual AlembicIObject*					getObj(uint64_t index){return m_objects[index];};
	virtual AlembicIObject*					getObjByName(const char* name);
	virtual Abc::IArchive&					get(){ return m_archive; };
	virtual void							incrementUses(){ m_uses++; };
	virtual void							decrementUses(){ m_uses--; };
	virtual uint32_t						numUses(){ return m_uses; };
	virtual double							getStartTime(){return m_startTime;};
	virtual double							getEndTime(){ return m_endTime; };
	virtual uint64_t						getNumTimeSampling();
	virtual Abc::index_t					getMaxNumSamplesForTimeSamplingIndex(uint32_t index);

private:
	void walk(AbcG::IObject iObj);
	void parseTree();
	Abc::IArchive m_archive;
	double m_startTime;
	double m_endTime;
	bool m_valid;
	uint32_t m_uses;
	std::string m_format;
	std::string m_filename;
	std::vector<std::pair<std::string, AbcG::IObject>> m_identifiers;
	std::vector<AlembicIObject*> m_objects;
};

BOOZE_EXPORT AlembicIArchive* newIArchive();
BOOZE_EXPORT void deleteIArchive(AlembicIArchive* archive);
BOOZE_EXPORT AlembicOArchive* newOArchive();
BOOZE_EXPORT void deleteOArchive(AlembicOArchive* archive);

BOOZE_NAMESPACE_CLOSE_SCOPE

#endif //_BOOZE_ARCHIVE_STORAGE_H_
