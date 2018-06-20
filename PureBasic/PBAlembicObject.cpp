#include "PBAlembicObject.h"
#include "AlembicPolymesh.h"
#include "AlembicPointCloud.h"
#include "AlembicXForm.h"
#include "AlembicCurve.h"
#include "AlembicCamera.h"
#include "AlembicLight.h"

// Get Geometry Scope
//----------------------------------------------------------
EXPORT ABCGeometryScope ABC_GetGeometryScope(AlembicIObject* obj){
	
	Alembic::Abc::IObject* o = (Alembic::Abc::IObject*)obj;
	
	const Alembic::Abc::MetaData& md = o->getMetaData();
	
	return (ABCGeometryScope)Alembic::AbcGeom::GetGeometryScope( md );
}

// Set Geometry Scope
//----------------------------------------------------------
EXPORT void ABC_SetGeometryScope(AlembicIObject* obj, ABCGeometryScope scope){
	Alembic::Abc::MetaData md = obj->GetIObject()->getMetaData();
	Alembic::AbcGeom::GeometryScope iScope = (Alembic::AbcGeom::GeometryScope)scope;
	Alembic::AbcGeom::SetGeometryScope( md, iScope );
}

// Init Object
//----------------------------------------------------------
EXPORT void ABC_InitObject(AlembicIObject* obj,ABCGeometricType type){
    std::cout << "ABC_OBJECT INIT CALLED" <<std::endl;
	obj->Init(type);
}
// Get Object From Archive
//----------------------------------------------------------
EXPORT AlembicIObject* ABC_GetObjectFromArchiveByID(AlembicIArchive* archive,long identifier)
{
	Alembic::Abc::IObject* obj = archive->GetObjectFromID(identifier);
	if(obj){
		if(Alembic::AbcGeom::IPolyMesh::matches(obj->getMetaData())){
			AlembicIPolymesh* polymesh = new AlembicIPolymesh(obj);
			polymesh->Init(GeometricType_PolyMesh);
			return (AlembicIObject*)polymesh;
		}
		else if(Alembic::AbcGeom::IPoints::matches(obj->getMetaData())){
			AlembicIPointCloud* points = new AlembicIPointCloud(obj);
			points->Init(GeometricType_Points);
			return (AlembicIObject*)points;
		}
		else if(Alembic::AbcGeom::IXform::matches(obj->getMetaData())){
			AlembicIXForm* xform = new AlembicIXForm(obj);
			return (AlembicIObject*)xform;
		}
		else if(Alembic::AbcGeom::ICurves::matches(obj->getMetaData())){
			AlembicICurves* curve = new AlembicICurves(obj);
			return (AlembicIObject*)curve;
		}
		else if(Alembic::AbcGeom::ICamera::matches(obj->getMetaData())){
			AlembicICamera* camera = new AlembicICamera(obj);
			return (AlembicIObject*)camera;
		}
		else if(Alembic::AbcGeom::ILight::matches(obj->getMetaData())){
			AlembicILight* light = new AlembicILight(obj);
			return (AlembicIObject*)light;
		}
		else
			return new AlembicIObject(obj);
	}
	return NULL;
}

// Get Object From Archive by Name
//----------------------------------------------------------
EXPORT AlembicIObject* ABC_GetObjectFromArchiveByName(AlembicIArchive* archive,TCHAR* name)
{
	archive->GetAllObjects();
	for(int i=0;i<archive->GetNumObjects();i++)
	{
		Alembic::Abc::IObject* obj = archive->GetObjectFromID(i);
		if(obj->getName()==name){
			AlembicIObject* out = new AlembicIObject(obj);
			return out;
		}
	}
	return NULL;
}

EXPORT bool ABC_ObjectIsProperty(AlembicIObject* obj)
{
	return false;//Alembic::AbcGeom::ICompoundProperty::matches(obj->GetIObject()->getMetaData());;
	
};

EXPORT int ABC_GetNumProperties(AlembicIObject* obj)
{
	return obj->GetNumProperties();
}

EXPORT TCHAR* ABC_GetPropertyName(AlembicIObject* obj,int ID)
{
    
	AlembicIProperty* prop = obj->GetProperty(ID);
	std::string name = (std::string)prop->GetName();

	if(!name.size())
		return NULL;
	int size = (int)name.size()+1;
	
	SYS_FastStringCopy(&alembic_io_string, name.c_str(), size);
	
	return &alembic_io_string;
}

EXPORT AlembicIProperty* ABC_GetProperty(AlembicIObject* obj,int ID)
{
	if(ID<0 || ID >=obj->GetNumProperties())return NULL;
	return obj->GetProperty(ID);
}

EXPORT void ABC_Polymesh_ReadFrame()
{

}

EXPORT TCHAR* ABC_TestString(TCHAR* input)
{
	std::string *out=new std::string(input);
	return (char*)out->c_str();
}

EXPORT TCHAR* ABC_GetObjectHeader(AlembicIObject* obj)
{
	if(!obj->GetIObject()->valid())
		return NULL;
	
	Alembic::AbcCoreAbstract::ObjectHeader header = obj->GetIObject()->getHeader();
	std::string str = header.getFullName();//getMetaData().serialize();
	 
	if(!str.size())
		return NULL;

	int size = (int)str.size()+1;
	SYS_FastStringCopy(&alembic_io_string, str.c_str(), size);
	
	return &alembic_io_string;
}

EXPORT TCHAR* ABC_GetObjectMetaData(AlembicIObject* obj)
{	
	if(!obj->GetIObject()->valid())
		return NULL;
	
	std::string metadata = obj->GetIObject()->getMetaData().serialize();
	if(!metadata.size())
		return NULL;

	int size = (int)metadata.size()+1;

	SYS_FastStringCopy(&alembic_io_string, metadata.c_str(), size);
	
	return &alembic_io_string;
}

EXPORT TCHAR* ABC_GetObjectName(AlembicIObject* obj)
{	
	if(!obj->GetIObject()->valid())
		return NULL;
	std::string name = obj->GetIObject()->getName();
	if(!name.size())
		return NULL;
	int size = (int)name.size()+1;

	SYS_FastStringCopy(&alembic_io_string, name.c_str(), size);
	
	return &alembic_io_string;
}

EXPORT TCHAR* ABC_GetObjectFullName(AlembicIObject* obj)
{	
	if(!obj->GetIObject()->valid())
		return NULL;
	std::string name = obj->GetIObject()->getFullName();
	
	if(!name.size())
		return NULL;
	int size = (int)name.size()+1;
	
	SYS_FastStringCopy(&alembic_io_string, name.c_str(), size);
	
	return &alembic_io_string;
}

EXPORT TCHAR* ABC_GetInterpretation(AlembicIObject* obj,int ID)
{
	std::string interpretation = obj->_props[ID].GetInterpretation();
	if(!interpretation.size())
		return NULL;
	int size = (int)interpretation.size()+1;
	
	SYS_FastStringCopy(&alembic_io_string, interpretation.c_str(), size);
	
	return &alembic_io_string;
	//m_Interp = in_PropHeader.getMetaData().get( "interpretation" );
}
