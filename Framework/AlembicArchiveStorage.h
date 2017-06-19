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
	long GetNumObjects();
	long GetNumTimeSamplings();
	Alembic::Abc::IObject* GetObjectFromID(std::string identifier);
	Alembic::Abc::IObject* GetObjectFromID(std::string identifier,std::string& debug);
	Alembic::Abc::IObject* GetObjectFromID(long id);
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
	//std::map<std::string,Alembic::Abc::IArchive *> _archives;
	std::map<std::string,AlembicIArchive *> _archives;
public:
	AlembicIArchive* GetArchiveFromID(std::string path);
	std::string AddArchive(AlembicIArchive * archive);
    void RemoveArchive(AlembicIArchive* archive);
	void DeleteArchive(std::string path);
	void DeleteAllArchives();
	long GetNumOpenArchives();
};

#endif //_BOOZE_ARCHIVE_STORAGE_H_