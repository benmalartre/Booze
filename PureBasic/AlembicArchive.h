#ifndef _BOOZE_ARCHIVE_STORAGE_H_
#define _BOOZE_ARCHIVE_STORAGE_H_

#include "AlembicFoundation.h"

BOOZE_NAMESPACE_OPEN_SCOPE

class AlembicOObject;
class AlembicOArchive
{
public:
	virtual bool Open(const char* filename);
	virtual bool Close();
	virtual bool IsValid();
private:
	std::vector<AlembicOObject*> _objects;
	Abc::OArchive _archive;
};

class AlembicIObject;
class AlembicIArchive
{
public:
	AlembicIArchive() :_uses(0){};
	virtual bool Open(const char* filename);
	virtual bool Close();
	virtual bool IsValid(){return _valid;};
	virtual const char* GetInfos();
	virtual void AddObject(AlembicIObject* obj);
	virtual uint64_t GetNumIdentifiers();
	virtual uint64_t GetNumObjects();
	virtual const char* GetIdentifier(uint64_t id);
	virtual AbcG::IObject& GetIObj(uint64_t i);
	virtual AlembicIObject* GetObj(uint64_t index){return _objects[index];};
	virtual AlembicIObject* GetObjByName(const char* name);
	virtual Abc::IArchive& Get(){ return _archive; };
	virtual void IncrementUses(){ _uses++; };
	virtual void DecrementUses(){ _uses--; };
	virtual uint32_t NumUses(){ return _uses; };
	virtual double GetStartTime(){return _startTime;};
	virtual double GetEndTime(){ return _endTime; };
	virtual uint64_t GetNumTimeSampling();
	virtual Abc::index_t GetMaxNumSamplesForTimeSamplingIndex(uint32_t index);

private:
	void Walk(AbcG::IObject iObj);
	void ParseTree();
	Abc::IArchive _archive;
	double _startTime;
	double _endTime;
	bool _valid;
	uint32_t _uses;
	std::string _format;
	std::string _filename;
	std::vector<std::pair<std::string, AbcG::IObject>> _identifiers;
	std::vector<AlembicIObject*> _objects;
};

BOOZE_EXPORT AlembicIArchive* newIArchive();
BOOZE_EXPORT void deleteIArchive(AlembicIArchive* archive);
BOOZE_EXPORT AlembicOArchive* newOArchive();
BOOZE_EXPORT void deleteOArchive(AlembicOArchive* archive);

BOOZE_NAMESPACE_CLOSE_SCOPE

#endif //_BOOZE_ARCHIVE_STORAGE_H_