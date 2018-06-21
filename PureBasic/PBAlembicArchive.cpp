#include "PBAlembicRegister.h"
// Create Archive Manager
//---------------------------------------------------
EXPORT AlembicArchiveManager* ABC_CreateArchiveManager()
{
	AlembicArchiveManager* m = new AlembicArchiveManager();
	return m;
}

// Delete Archive Manager
//---------------------------------------------------
EXPORT void  ABC_DeleteArchiveManager(AlembicArchiveManager* manager)
{
	if(manager!=NULL){
		manager->DeleteAllArchives();
		delete manager;
	}
}

// Get Num Objects In Archive
//---------------------------------------------------
EXPORT uint64_t ABC_GetNumOpenArchives(AlembicArchiveManager* manager)
{
	return manager->GetNumOpenArchives();
}

// Get Infos
//---------------------------------------------------
EXPORT const char* ABC_GetInfosFromArchive(AlembicIArchive* archive)
{
	std::string infos;

	if(archive)
	{
		infos = Alembic::AbcCoreAbstract::GetLibraryVersion ()+"|";
		std::string appName;
        std::string libraryVersionString;
        Alembic::Util::uint32_t libraryVersion;
        std::string whenWritten;
        std::string userDescription;
        GetArchiveInfo (*archive->GetArchive(),
                        appName,
                        libraryVersionString,
                        libraryVersion,
                        whenWritten,
                        userDescription);

		if (appName != "")
        {
            infos += "  file written by: " + appName+"|";
            infos += "  using Alembic : " + libraryVersionString+"|";
            infos += "  written on : " + whenWritten +"|";
            infos += "  user description : " + userDescription+"|";
        }
        else
        {
            infos += archive->GetName();
            infos += "|(file doesn't have any ArchiveInfo)|";
        }
	}

    return infos.c_str();
}

// Open Archive
//---------------------------------------------------
EXPORT AlembicIArchive* ABC_OpenArchive(AlembicArchiveManager* manager,const char* filename)
{
	std::string path(filename);
	AlembicIArchive* archive = manager->GetArchiveFromID(path);
	return archive;
}

// Close Archive
//---------------------------------------------------
EXPORT bool ABC_CloseArchive(AlembicArchiveManager* manager,AlembicIArchive* archive)
{
    return manager->RemoveArchive(archive);
}

// Get Num Objects In Archive
//---------------------------------------------------
EXPORT uint64_t ABC_GetNumObjectsInArchive(AlembicIArchive* archive)
{
	return archive->GetNumObjects();
}

// Get Num Time Sampling
//---------------------------------------------------
EXPORT uint64_t ABC_GetNumTimeSamplingInArchive(AlembicIArchive* archive)
{
	return archive->GetNumTimeSamplings();
}

// Get Time Sampling at Index
//---------------------------------------------------
EXPORT Abc::TimeSamplingPtr ABC_GetTimeSampling(AlembicIArchive* archive,uint32_t index)
{
    return archive->GetArchive()->getTimeSampling(index);
}

// Get Max Num Samples For Time Sampling
//---------------------------------------------------
EXPORT Abc::index_t ABC_GetMaxNumSamplesForTimeSamplingIndex(AlembicIArchive* archive,uint32_t index)
{
    return archive->GetArchive()->getMaxNumSamplesForTimeSamplingIndex(index);
}

// Get Start Frame
//---------------------------------------------------
EXPORT Abc::chrono_t ABC_GetStartFrame(AlembicIArchive* archive)
{
    Abc::IArchive* a = archive->GetArchive();
    
    AbcA::TimeSamplingPtr sampler(a->getTimeSampling(1));
    return sampler->getSampleTime(0);
    
}

// Get End Frame
//---------------------------------------------------
EXPORT Abc::chrono_t ABC_GetEndFrame(AlembicIArchive* archive)
{
    AbcA::TimeSamplingPtr sampler = archive->GetArchive()->getTimeSampling(1);
    AbcA::index_t t = archive->GetArchive()->getMaxNumSamplesForTimeSamplingIndex(1);
    return sampler->getSampleTime(t);
}
