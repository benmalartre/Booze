#include "AlembicArchive.h"
#include "AlembicRoot.h"
#include "AlembicObject.h"
#include "AlembicXForm.h"
#include "AlembicCamera.h"
#include "AlembicPolymesh.h"
#include "AlembicPoints.h"
#include "AlembicCurves.h"
#include "AlembicWriteJob.h"
#include <map>
#include <functional>



BOOZE_NAMESPACE_OPEN_SCOPE

//=================================================================================================
// IArchive
//=================================================================================================
// Constructor
//-------------------------------------------------------------------------------------------------
AlembicIArchive* newIArchive()
{
	AlembicIArchive* archive = new AlembicIArchive();
	return archive;
}

// Destructor
//-------------------------------------------------------------------------------------------------
void deleteIArchive(AlembicIArchive* archive)
{
	delete archive;
}

// Open Archive
//-------------------------------------------------------------------------------------------------
bool AlembicIArchive::open(const char* filename)
{
	close();

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
	m_archive = factory.getArchive(std::string(filename), coreType);
	m_valid = m_archive.valid();
	if (coreType == AbcF::IFactory::kHDF5)
	{
		m_format = "hdf5";
#ifdef ALEMBIC_WITH_HDF5
		m_valid = true;
#endif
	}
	else if (coreType == AbcF::IFactory::kOgawa)
	{
		m_format = "ogawa";
		m_valid = true;
	}
	else m_format = "unknown";

	m_uses = 1;
	m_filename = std::string(filename);
	if (m_valid)
	{
		parseTree();
		Abc::GetArchiveStartAndEndTime(m_archive, m_startTime, m_endTime);
	}
	return m_valid;
}

// Get Infos
//-------------------------------------------------------------------------------------------------
const char* AlembicIArchive::getInfos()
{

	std::string infos;

	if (m_archive)
	{
		infos = Alembic::AbcCoreAbstract::GetLibraryVersion() + "|";
		std::string appName;
		std::string libraryVersionString;
		Alembic::Util::uint32_t libraryVersion;
		std::string whenWritten;
		std::string userDescription;
		GetArchiveInfo(m_archive,
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
			infos += m_archive.getName();
			infos += "|(file doesn't have any ArchiveInfo)|";
		}
	}
	else infos = "invalid archive...";
	return infos.c_str();
}

void AlembicIArchive::walk(AbcG::IObject iObj)
{
	std::string path = iObj.getFullName();
	m_identifiers.push_back(std::make_pair(path, iObj));
	for (size_t i = 0; i < iObj.getNumChildren(); i++) {
		walk(AbcG::IObject(iObj, iObj.getChildHeader(i).getName()));
	};
}

void AlembicIArchive::parseTree()
{
	// walk object hierarchy and find valid objects
	AbcG::IObject iObj = m_archive.getTop();
	walk(iObj);
}

void AlembicIArchive::addObject(AlembicIObject* obj)
{
	m_objects.push_back(obj);
}
uint64_t AlembicIArchive::getNumObjects()
{
	return m_objects.size();
}

uint64_t AlembicIArchive::getNumIdentifiers()
{
	return m_identifiers.size();
}

const char* AlembicIArchive::getIdentifier(uint64_t i)
{
	if (i < m_identifiers.size())return m_identifiers[i].first.c_str();
	return NULL;
}

const AbcG::IObject& AlembicIArchive::getIObj(uint64_t i)
{
	if (i < m_identifiers.size())return m_identifiers[i].second;
	return AbcG::IObject();
}

// Get Object by Name
//-------------------------------------------------------------------------------------------------
AlembicIObject* AlembicIArchive::getObjByName(const char* name)
{
	size_t j = 0;
	for (size_t i = 0; i < m_identifiers.size(); i++)
	{
		if (m_identifiers[i].first == std::string(name))return m_objects[j];
		if (m_identifiers[i].second.valid())j++;
	}
	
	return NULL;
}

// Get Num Time Sampling
//-------------------------------------------------------------------------------------------------
uint64_t AlembicIArchive::getNumTimeSampling()
{
	return m_archive.getNumTimeSamplings();
}

// Get Max Num Samples For Time Sampling
//-------------------------------------------------------------------------------------------------
Abc::index_t AlembicIArchive::getMaxNumSamplesForTimeSamplingIndex( uint32_t index)
{
	return m_archive.getMaxNumSamplesForTimeSamplingIndex(index);
}

/*
// Get Start Frame
//-------------------------------------------------------------------------------------------------
float AlembicIArchive::GetStartTime( uint64_t fps)
{
	return _startTime*(float)fps;
}

// Get End Frame
//-------------------------------------------------------------------------------------------------
float AlembicIArchive::GetEndTime(uint64_t fps)
{
	return _endTime*(float)fps;
}
*/
// Close ARchive
//-------------------------------------------------------------------------------------------------
bool AlembicIArchive::close()
{
	return false;
}

//=================================================================================================
// OArchive
//=================================================================================================
// Constructor
//-------------------------------------------------------------------------------------------------
AlembicOArchive* newOArchive(AlembicWriteJob* job)
{
	AlembicOArchive* archive = new AlembicOArchive(job);
	return archive;
}

// Destructor
//--------------------------------------------------------
void deleteOArchive(AlembicOArchive* archive)
{
	delete archive;
}

AlembicOArchive::AlembicOArchive(AlembicWriteJob* job)
{
	m_job = job;
	
	// create a new archive for writing
	m_archive = CreateArchiveWithInfo(
		Alembic::AbcCoreOgawa::WriteArchive(),
		job->getFileName(),
		"Alembic PureBasic 1.0 Plugin",
		job->getFileName(),
		Alembic::Abc::ErrorHandler::kThrowPolicy
		);

	m_valid = m_archive.valid();
	m_root = new AlembicORoot(this, NULL, "root");
	m_objects.push_back(m_root);
}

AlembicOArchive::~AlembicOArchive()
{
	
	close();
}

bool AlembicOArchive::open(const char* filename)
{
	return true;
}

bool AlembicOArchive::close()
{
	for (int32_t i = m_objects.size() - 1; i >= 0; i--){
		delete m_objects[i];
	}
	return true;
}

int AlembicOArchive::getNumObjects()
{
	return m_objects.size();
}

AlembicOObject* AlembicOArchive::getObjectByName(const char* name)
{
	std::map<string, int>::iterator it;
	it = m_map.find(name);
	if (it != m_map.end())return m_objects[it->second];
	return NULL;
}

#define ARCHIVE_ADD_OBJECT(TYPE, NAME) \
	TYPE* NAME = new TYPE(this, parent, customData, name); \
if (parent)parent->addChild(NAME); \
	m_objects.push_back(NAME); \
	return NAME;\

AlembicOObject* AlembicOArchive::addObject(AlembicOObject* parent, char* name, ABCGeometricType type, void* customData){
	
	switch (type){
		case GeometricType_Camera:
		{
									 ARCHIVE_ADD_OBJECT(AlembicOCamera, camera);
		}
			
		case GeometricType_Curves:
		{
									 ARCHIVE_ADD_OBJECT(AlembicOCurves, curves);
		}
			
		case GeometricType_FaceSet:
		{
			break;
		}

		case GeometricType_Light:
		{
			break;
		}
			
		case GeometricType_Points:
		{
									 ARCHIVE_ADD_OBJECT(AlembicOPoints, points);
		}
			
		case GeometricType_PolyMesh:
		{
									   ARCHIVE_ADD_OBJECT(AlembicOPolymesh, mesh);
		}

		case GeometricType_XForm:
		{
									ARCHIVE_ADD_OBJECT(AlembicOXForm, xform);
		}
			
	}
	
	return NULL;
	
}

BOOZE_NAMESPACE_CLOSE_SCOPE