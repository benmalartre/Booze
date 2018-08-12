#include "AlembicManager.h"
#include "AlembicArchive.h"

BOOZE_NAMESPACE_OPEN_SCOPE

#ifdef ALEMBIC_WITH_HDF5
extern bool ABC_HDF5_SUPPORT = true;
#else
extern bool ABC_HDF5_SUPPORT = false;
#endif

// Does the lib was compiled with hdf5 support
//------------------------------------------------------------------------
BOOZE_EXPORT bool hasHDF5Support()
{
	return ABC_HDF5_SUPPORT;
}

// Alembic Library Version
//------------------------------------------------------------------------
BOOZE_EXPORT const char* getLibraryVersion()
{
	return AbcA::GetLibraryVersion().c_str();
}

// Alembic Manager Constructor
//------------------------------------------------------------------------
AlembicArchiveManager* newArchiveManager()
{
	return new AlembicArchiveManager();
}

// Alembic Manager Destructor
//------------------------------------------------------------------------
void deleteArchiveManager(AlembicArchiveManager* manager)
{
	delete(manager);
}

//========================================================================
// AlembicArchiveManager Implementation
//========================================================================
// Open Archive
//------------------------------------------------------------------------
AlembicIArchive* AlembicArchiveManager::OpenArchive(const char* path)
{
	std::map<std::string, AlembicIArchive *>::iterator it;
	it = _archives.find(std::string(path));
	if (it != _archives.end())
	{
		AlembicIArchive* archive = it->second;
		archive->IncrementUses();
		return archive;
	}
	AlembicIArchive* archive = new AlembicIArchive();

	_archives[std::string(path)] = archive;
	return archive;
}

// Close Archive
//------------------------------------------------------------------------
bool AlembicArchiveManager::CloseArchive(const char* path)
{
	std::map<std::string, AlembicIArchive *>::iterator it;
	it = _archives.find(std::string(path));
	if (it != _archives.end())
	{
		AlembicIArchive* archive = it->second;
		archive->DecrementUses();
		if (!archive->NumUses())
		{
			archive->Close();
			_archives.erase(it);
			delete archive;
			return true;
		}
	}
	return false;
}

// Get Num Open Archives
//------------------------------------------------------------------------
uint64_t AlembicArchiveManager::GetNumOpenArchives()
{
	return _archives.size();
}

// Close All Archives
//------------------------------------------------------------------------
bool AlembicArchiveManager::CloseAllArchives()
{
	return true;
}

BOOZE_NAMESPACE_CLOSE_SCOPE