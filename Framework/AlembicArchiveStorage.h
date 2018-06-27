#ifndef _BOOZE_ARCHIVE_STORAGE_H_
#define _BOOZE_ARCHIVE_STORAGE_H_

#include <Alembic/AbcCoreAbstract/All.h>
#include <Alembic/AbcCoreFactory/All.h>
#include <Alembic/AbcCoreHDF5/All.h>

#include "AlembicFoundation.h"
#include "AlembicObject.h"
#include <map>


using namespace std;

class AlembicIArchive
{
private:
	double _startTime;
	double _endTime;
public:
	std::vector<Alembic::Abc::IObject*> _objects;
	Alembic::Abc::IArchive _archive;

//public:
	AlembicIArchive(std::string path);
	~AlembicIArchive();
	void Open(std::string path);
	void Close();
	string GetName();
	Alembic::Abc::IArchive* GetArchive();
	void GetAllObjects();
	void GetObjectChildren(Alembic::Abc::IObject* obj);
	uint64_t GetNumObjects();
	uint64_t GetNumTimeSamplings();
	Alembic::Abc::IObject* GetObjectFromID(std::string identifier);
	Alembic::Abc::IObject* GetObjectFromID(std::string identifier,std::string& debug);
	Alembic::Abc::IObject* GetObjectFromID(uint64_t id);
	void getStartEndTimes();
	double getStartTime(){return _startTime;};
	double getEndTime(){return _endTime;};
};

class AlembicOArchive
{
private:
	std::vector<Alembic::Abc::OObject*> _objects;
	Alembic::Abc::OArchive _archive;

public:
	void Open(std::string path);
	//void GetObject(std::string path, std::string identifier);
};

class AlembicArchiveManager
{
public:
	std::map<std::string,AlembicIArchive *> _archives;
public:
	AlembicIArchive* GetArchiveFromID(std::string path);
	std::string AddArchive(AlembicIArchive * archive);
    bool RemoveArchive(AlembicIArchive* archive);
	void DeleteArchive(std::string path);
	void DeleteAllArchives();
	uint64_t GetNumOpenArchives();
};

#endif //_BOOZE_ARCHIVE_STORAGE_H_