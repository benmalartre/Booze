#include "AlembicArchive.h"
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
		ParseTree();
		Abc::GetArchiveStartAndEndTime(m_archive, m_startTime, m_endTime);
	}
	return m_valid;
}

const char* AlembicIArchive::GetInfos()
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

void AlembicIArchive::Walk(AbcG::IObject iObj)
{
	std::string path = iObj.getFullName();
	m_identifiers.push_back(std::make_pair(path, iObj));
	for (size_t i = 0; i < iObj.getNumChildren(); i++) {
		Walk(AbcG::IObject(iObj, iObj.getChildHeader(i).getName()));
	};
}

void AlembicIArchive::ParseTree()
{
	// walk object hierarchy and find valid objects
	AbcG::IObject iObj = m_archive.getTop();
	Walk(iObj);
}

void AlembicIArchive::AddObject(AlembicIObject* obj)
{
	m_objects.push_back(obj);
}
uint64_t AlembicIArchive::GetNumObjects()
{
	return m_objects.size();
}

uint64_t AlembicIArchive::GetNumIdentifiers()
{
	return m_identifiers.size();
}

const char* AlembicIArchive::GetIdentifier(uint64_t i)
{
	if (i < m_identifiers.size())return m_identifiers[i].first.c_str();
	return NULL;
}

const AbcG::IObject& AlembicIArchive::GetIObj(uint64_t i)
{
	if (i < m_identifiers.size())return m_identifiers[i].second;
	return AbcG::IObject();
}

// Get Object by Name
//---------------------------------------------------
AlembicIObject* AlembicIArchive::GetObjByName(const char* name)
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
//---------------------------------------------------
uint64_t AlembicIArchive::GetNumTimeSampling()
{
	return m_archive.getNumTimeSamplings();
}

// Get Max Num Samples For Time Sampling
//---------------------------------------------------
Abc::index_t AlembicIArchive::GetMaxNumSamplesForTimeSamplingIndex( uint32_t index)
{
	return m_archive.getMaxNumSamplesForTimeSamplingIndex(index);
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
	m_archive = CreateArchiveWithInfo(
		Alembic::AbcCoreOgawa::WriteArchive(),
		job->GetFileName(),
		"Alembic PureBasic 1.0 Plugin",
		job->GetFileName(),
		Alembic::Abc::ErrorHandler::kThrowPolicy);
	m_valid = m_archive.valid();
	if (m_valid){
		Abc::OObject topArchive = m_archive.getTop();
		m_top = new AlembicOObject(job, NULL);
		m_top->Set(topArchive);
	}
	
}

bool AlembicOArchive::Open(const char* filename)
{
	return true;
}

bool AlembicOArchive::Close()
{
	return true;
}


int AlembicOArchive::GetNumObjects()
{
	return m_objects.size();
}

AlembicOObject* AlembicOArchive::AddObject(AlembicOObject* parent, const char* name, ABCGeometricType type, void* customData){
	switch (type){
	case GeometricType_Camera:
	{
		AlembicOCamera* camera = new AlembicOCamera(GetJob(), customData);
		camera->Set(Alembic::AbcGeom::OCamera(parent->Get(), (string)name));
		m_objects.push_back(camera);
		return camera;
	}
			
	case GeometricType_Curves:
	{
		AlembicOCurves* curves = new AlembicOCurves(GetJob(), customData);
		curves->Set(Alembic::AbcGeom::OCamera(parent->Get(), (string)name));
		m_objects.push_back(curves);
		return curves;
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
		AlembicOPoints* points = new AlembicOPoints(GetJob(), customData);
		points->Set(Alembic::AbcGeom::OPolyMesh(parent->Get(), (string)name));
		m_objects.push_back(points);
		return points;
	}
			
	case GeometricType_PolyMesh:
	{
		AlembicOObject* mesh = new AlembicOPolymesh(GetJob(), customData);
		mesh->Set(Alembic::AbcGeom::OPolyMesh(parent->Get(), (string)name));
		m_objects.push_back(mesh);
		return mesh;
	}
			
	}
	return NULL;
	
	//AlembicOObject* obj = new AlembicOObject(GetJob(), customData);
	//return obj;
}

BOOZE_NAMESPACE_CLOSE_SCOPE