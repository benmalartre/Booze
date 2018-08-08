#include "AlembicArchiveStorage.h"
#include <map>

// Archive Manager
//--------------------------------------------------------
AlembicIArchive* AlembicArchiveManager::GetArchiveFromID(std::string path)
{
   std::map<string,AlembicIArchive *>::iterator it;
   it = _archives.find(path);
   if(it == _archives.end())
   {
		// check if the file exists
		FILE * file = fopen(path.c_str(),"rb");
		if(file != NULL)
		{
			fclose(file);
			AlembicIArchive* archive = new AlembicIArchive(path);
			if (archive->IsValid())
			{
				// add archive to global map
				AddArchive(archive);
				// initialize objects list
				archive->GetAllObjects();
			}
			return archive;
		}
		return NULL;
   }

   return it->second;
}

std::string AlembicArchiveManager::AddArchive(AlembicIArchive* archive)
{
	string name = archive->GetName();
	_archives.insert(std::pair<string,AlembicIArchive *>(name, archive));
	return name;
}

bool AlembicArchiveManager::RemoveArchive(AlembicIArchive* archive)
{
    map<string,AlembicIArchive *>::iterator it;
    for(it = _archives.begin(); it != _archives.end(); it++) 
	{
        if(it->second == archive)
        {
			_archives.erase(it);
			delete(archive);
			return true;
        }
    }
	return false;
}

void AlembicArchiveManager::DeleteArchive(std::string path)
{
    map<string,AlembicIArchive *>::iterator it;
    it = _archives.find(path);
    if(it == _archives.end())
        return;
    
	AlembicIArchive* archive = it->second;
    _archives.erase(it);
	delete(archive);
    
}

void AlembicArchiveManager::DeleteAllArchives()
{
  map<string,AlembicIArchive *>::iterator it;
  for(it = _archives.begin(); it != _archives.end(); it++)
   delete(it->second);
  _archives.clear();
}

uint64_t AlembicArchiveManager::GetNumOpenArchives()
{
	return (uint64_t)_archives.size();
}


	
// IArchive
//----------------------------------------------------------
AlembicIArchive::AlembicIArchive(std::string path)
{
	Open(path);
}

AlembicIArchive::~AlembicIArchive()
{
	/*
	vector<IObject>::iterator it = _objects.begin();
	for(;it<_objects.end();it++)delete (*it);
	*/
	_objects.clear();
	//delete _archive;

}

Alembic::Abc::IArchive* AlembicIArchive::GetArchive()
{
	return &_archive;
}

void AlembicIArchive::Open(std::string path)
{
	Close();

	//_archive = new IArchive( Alembic::AbcCoreHDF5::ReadArchive(), path);
	AbcF::IFactory factory;
	AbcF::IFactory::CoreType coreType;
	//factory.setOgawaNumStreams(ogawaStreams);
	//factory.setPolicy(Abc::ErrorHandler::kThrowPolicy);
	_valid = false;
	_archive = factory.getArchive(path, coreType);
	if (coreType == AbcF::IFactory::kHDF5)
	{
		_format = "hdf5";
#ifdef ALEMBIC_WITH_HDF5
		_valid = true;
#endif
	}
	else if (coreType == AbcF::IFactory::kOgawa)
	{
		_format = "ogawa";
		_valid = true;
	}
	else _format = "unknown";
	if (_valid) GetStartEndTimes();

}

void AlembicIArchive::GetStartEndTimes()
{
	if (_archive.valid())
     {
		Abc::GetArchiveStartAndEndTime(_archive, _startTime, _endTime);
     }
}

void AlembicIArchive::Close()
{
	if(_archive!=NULL)
	{
		/*
		vector<IObject>::iterator it = _objects.begin();
		for(;it<_objects.end();it++)delete (*it);
		*/
		_objects.clear();
		//delete _archive;
		//_archive = NULL;
	}
}


void AlembicIArchive::GetAllObjects()
{
	Alembic::AbcGeom::IObject* top = new Alembic::AbcGeom::IObject(_archive.getTop());
	_objects.push_back(top);

	// recurse finding all objects
	GetObjectChildren(top);
}

void AlembicIArchive::GetObjectChildren(Alembic::AbcGeom::IObject* obj)
{
	for(size_t i=0;i<obj->getNumChildren();i++)
	{
		if(obj->getChild(i).valid())
		{
			Alembic::AbcGeom::IObject* child = new Alembic::AbcGeom::IObject(obj->getChild(i));
			_objects.push_back(child);
			if(child->getNumChildren()>0)
				GetObjectChildren(child);
		}
	}
}

string AlembicIArchive::GetName()
{
	return _archive.getName();
}

uint64_t AlembicIArchive::GetNumObjects()
{
	return (uint64_t)_objects.size();
}

uint64_t AlembicIArchive::GetNumTimeSamplings()
{
	return (uint64_t)_archive.getNumTimeSamplings();
}

Alembic::Abc::IObject* AlembicIArchive::GetObjectFromID(std::string identifier)
{
	if(_archive == NULL)
		return NULL;

	std::vector<Alembic::Abc::IObject*>::iterator it = _objects.begin();
	for(;it<_objects.end();it++)
	{
		if((*it)->getFullName() == identifier)
			return (*it);
		/* && (*it)->getMetaData().matches(metadata))*/
	}
	return NULL;
}

Alembic::Abc::IObject* AlembicIArchive::GetObjectFromID(std::string identifier, std::string& debug)
{
	if(_archive == NULL)
		return NULL;

	std::vector<Alembic::Abc::IObject*>::iterator it = _objects.begin();
	for(;it<_objects.end();it++)
	{
		debug += (*it)->getFullName();
		debug += ",";
		std::string fName = (*it)->getFullName();
		if( fName == identifier)
		{
			return (*it);
		}
	}
	return NULL;
}

Alembic::Abc::IObject* AlembicIArchive::GetObjectFromID(uint64_t id)
{
	if(id>=_objects.size())
		return NULL;
	return _objects[id];
}
