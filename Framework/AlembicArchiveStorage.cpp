#include "AlembicArchiveStorage.h"
#include <map>
#include <boost/algorithm/string.hpp>

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
			AddArchive(archive);

			// initialize objects list
			archive->GetAllObjects();
			return archive;
		}
		return NULL;
   }

   return it->second;
}

std::string AlembicArchiveManager::AddArchive(AlembicIArchive* archive)
{
   _archives.insert(std::pair<string,AlembicIArchive *>(archive->GetName(),archive));
   return archive->GetName();
}


void AlembicArchiveManager::RemoveArchive(AlembicIArchive* archive)
{
    map<string,AlembicIArchive *>::iterator it;
    for(it = _archives.begin(); it != _archives.end(); it++) {
        // iterator->first = key
        if(it->second = archive)
        {
            _archives.erase(it);
            delete(it->second);
        }
    }
    _archives.erase(it);
}

void AlembicArchiveManager::DeleteArchive(std::string path)
{
    map<string,AlembicIArchive *>::iterator it;
    it = _archives.find(path);
    if(it == _archives.end())
        return;
    
    _archives.erase(it);
    delete(it->second);
}

void AlembicArchiveManager::DeleteAllArchives()
{
  map<string,AlembicIArchive *>::iterator it;
  for(it = _archives.begin(); it != _archives.end(); it++)
   delete(it->second);
  _archives.clear();
}

long AlembicArchiveManager::GetNumOpenArchives()
{
	return (long)_archives.size();
}

// IArchive
//----------------------------------------------------------
AlembicIArchive::AlembicIArchive(std::string path)
{
	//_archive = new IArchive( Alembic::AbcCoreHDF5::ReadArchive(), path);
     Alembic::AbcCoreFactory::IFactory factory;
     Alembic::AbcCoreFactory::IFactory::CoreType coreType;
     //factory.setOgawaNumStreams(ogawaStreams);
     //double time_start = getTimeSec();
     
    _archive = factory.getArchive(path, coreType);

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

     //_archive = new IArchive( Alembic::AbcCoreHDF5::ReadArchive(true), path);
     Alembic::AbcCoreFactory::IFactory factory;
     Alembic::AbcCoreFactory::IFactory::CoreType coreType;
     //factory.setOgawaNumStreams(ogawaStreams);
     //double time_start = getTimeSec();
     
     _archive = factory.getArchive(path, coreType);
     
     if (_archive.valid())
     {
     printf("héhéhé");
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
	Alembic::Abc::IObject* top = new Alembic::Abc::IObject(_archive.getTop());
	_objects.push_back(top);

	// recurse finding all objects
	GetObjectChildren(top);
}

void AlembicIArchive::GetObjectChildren(Alembic::Abc::IObject* obj)
{
	for(size_t i=0;i<obj->getNumChildren();i++)
	{
		Alembic::Abc::IObject* child = new Alembic::Abc::IObject(obj->getChild(i));
		_objects.push_back(child);
		if(child->getNumChildren()>0)
			GetObjectChildren(child);
	}
}

string AlembicIArchive::GetName()
{
	return _archive.getName();
}

long AlembicIArchive::GetNumObjects()
{
	return (int)_objects.size();
}

long AlembicIArchive::GetNumTimeSamplings()
{
	return _archive.getNumTimeSamplings();
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

Alembic::Abc::IObject* AlembicIArchive::GetObjectFromID(long id)
{
	if(id<0 || id>_objects.size())
		return NULL;
	return _objects[id];
}
