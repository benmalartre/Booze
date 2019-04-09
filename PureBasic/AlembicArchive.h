#ifndef _BOOZE_ARCHIVE_STORAGE_H_
#define _BOOZE_ARCHIVE_STORAGE_H_

#include "AlembicFoundation.h"
#include <map>

BOOZE_NAMESPACE_OPEN_SCOPE

class AlembicOObject;
class AlembicWriteJob;
class AlembicOArchive
{
public:
	AlembicOArchive(AlembicWriteJob* job);
	virtual bool Open(const char* filename);
	virtual bool Close();
	virtual bool IsValid(){ return m_valid; };
	virtual AlembicOObject* AddObject(AlembicOObject* parent, const char* name, ABCGeometricType type, void* ptr);
	virtual Abc::OArchive Get(){ return m_archive; };
	virtual AlembicOObject* GetTop(){ return m_top; };
	virtual AlembicWriteJob* GetJob(){ return m_job; };
	virtual int GetNumObjects();
	virtual AlembicOObject* GetObjectByIndex(unsigned index){ return m_objects[index]; };
	virtual AlembicOObject* GetObjectByName(const char* name);

private:
	
	Abc::OArchive m_archive;
	AlembicOObject* m_top;
	AlembicWriteJob* m_job;
	bool m_valid;
	std::vector<AlembicOObject*> m_objects;
	std::map<string, int> m_map;
};

class AlembicIObject;
class AlembicIArchive
{
public:
	AlembicIArchive() :m_uses(0){};
	virtual bool Open(const char* filename);
	virtual bool Close();
	virtual bool IsValid(){return m_valid;};
	virtual const char* GetInfos();
	virtual void AddObject(AlembicIObject* obj);
	virtual uint64_t GetNumIdentifiers();
	virtual uint64_t GetNumObjects();
	virtual const char* GetIdentifier(uint64_t id);
	virtual const AbcG::IObject& GetIObj(uint64_t i);
	virtual AlembicIObject* GetObj(uint64_t index){return m_objects[index];};
	virtual AlembicIObject* GetObjByName(const char* name);
	virtual Abc::IArchive& Get(){ return m_archive; };
	virtual void IncrementUses(){ m_uses++; };
	virtual void DecrementUses(){ m_uses--; };
	virtual uint32_t NumUses(){ return m_uses; };
	virtual double GetStartTime(){return m_startTime;};
	virtual double GetEndTime(){ return m_endTime; };
	virtual uint64_t GetNumTimeSampling();
	virtual Abc::index_t GetMaxNumSamplesForTimeSamplingIndex(uint32_t index);

private:
	void Walk(AbcG::IObject iObj);
	void ParseTree();
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