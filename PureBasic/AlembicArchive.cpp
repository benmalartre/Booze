#include "AlembicArchive.h"
#include "AlembicObject.h"
#include <map>
#include <functional>

BOOZE_NAMESPACE_OPEN_SCOPE

//========================================================
// IArchive
//========================================================
// Constructor
//--------------------------------------------------------
AlembicIArchive* newIArchive()
{
	AlembicIArchive* archive = new AlembicIArchive();
	return archive;
}

// Destructor
//--------------------------------------------------------
void deleteIArchive(AlembicIArchive* archive)
{
	delete archive;
}

bool AlembicIArchive::Open(const char* filename)
{
	Close();

	// check if the file exists
	FILE * file = fopen(filename, "rb");
	if (file == NULL)
	{
		return false;
		
	}
	fclose(file);

	// open the iarchive
	AbcF::IFactory factory;
	factory.setPolicy(Abc::ErrorHandler::kQuietNoopPolicy);
	AbcF::IFactory::CoreType coreType;
	_archive = factory.getArchive(std::string(filename), coreType);
	_valid = _archive.valid();
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

	_uses = 1;
	_filename = std::string(filename);
	if (_valid)
	{
		ParseTree();
		Abc::GetArchiveStartAndEndTime(_archive, _startTime, _endTime);
	}
	return _valid;
}

const char* AlembicIArchive::GetInfos()
{

	std::string infos;

	if (_archive)
	{
		infos = Alembic::AbcCoreAbstract::GetLibraryVersion() + "|";
		std::string appName;
		std::string libraryVersionString;
		Alembic::Util::uint32_t libraryVersion;
		std::string whenWritten;
		std::string userDescription;
		GetArchiveInfo(_archive,
			appName,
			libraryVersionString,
			libraryVersion,
			whenWritten,
			userDescription);

		if (appName != "")
		{
			infos += "  file written by: " + appName + "|";
			infos += "  using Alembic : " + libraryVersionString + "|";
			infos += "  written on : " + whenWritten + "|";
			infos += "  user description : " + userDescription + "|";
		}
		else
		{
			infos += _archive.getName();
			infos += "|(file doesn't have any ArchiveInfo)|";
		}
	}

}

void AlembicIArchive::Walk(AbcG::IObject iObj)
{
	std::string path = iObj.getFullName();
	_identifiers.push_back(std::make_pair(path, iObj));
	for (size_t i = 0; i < iObj.getNumChildren(); i++) {
		Walk(AbcG::IObject(iObj, iObj.getChildHeader(i).getName()));
	};
}

void AlembicIArchive::ParseTree()
{
	// walk object hierarchy and find valid objects
	AbcG::IObject iObj = _archive.getTop();
	Walk(iObj);
	_numObjects = _identifiers.size();
}

void AlembicIArchive::AddObject(AlembicIObject* obj)
{
	_objects.push_back(obj);
}
uint64_t AlembicIArchive::GetNumObjects()
{
	return _numObjects;
}

const char* AlembicIArchive::GetIdentifier(uint64_t i)
{
	if (i < _identifiers.size())return _identifiers[i].first.c_str();
	return NULL;
}

AbcG::IObject& AlembicIArchive::GetIObj(uint64_t i)
{
	if (i < _identifiers.size())return _identifiers[i].second;
	return AbcG::IObject();
}

// Get Object by Name
//---------------------------------------------------
AlembicIObject* AlembicIArchive::GetObjByName(const char* name)
{
	for (size_t i=0; i < _identifiers.size();i++)
	if (_identifiers[i].first == std::string(name))return _objects[i];

	return NULL;
}

// Get Num Time Sampling
//---------------------------------------------------
uint64_t AlembicIArchive::GetNumTimeSampling()
{
	return _archive.getNumTimeSamplings();
}

// Get Max Num Samples For Time Sampling
//---------------------------------------------------
Abc::index_t AlembicIArchive::GetMaxNumSamplesForTimeSamplingIndex( uint32_t index)
{
	return _archive.getMaxNumSamplesForTimeSamplingIndex(index);
}

/*
// Get Start Frame
//---------------------------------------------------
float AlembicIArchive::GetStartTime( uint64_t fps)
{
	return _startTime*(float)fps;
}

// Get End Frame
//---------------------------------------------------
float AlembicIArchive::GetEndTime(uint64_t fps)
{
	return _endTime*(float)fps;
}
*/
// Close ARchive
//---------------------------------------------------
bool AlembicIArchive::Close()
{
	return false;
}

//========================================================
// OArchive
//========================================================
// Constructor
//--------------------------------------------------------
AlembicOArchive* newOArchive()
{
	AlembicOArchive* archive = new AlembicOArchive();
	return archive;
}

// Destructor
//--------------------------------------------------------
void deleteOArchive(AlembicOArchive* archive)
{
	delete archive;
}

bool AlembicOArchive::Open(const char* filename)
{
	return true;
}

bool AlembicOArchive::Close()
{
	return true;
}

bool AlembicOArchive::IsValid()
{
	return true;
}

BOOZE_NAMESPACE_CLOSE_SCOPE