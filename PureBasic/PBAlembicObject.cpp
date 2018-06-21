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
	
	Abc::IObject* o = (Abc::IObject*)obj;
	
	const Abc::MetaData& md = o->getMetaData();
	
	return (ABCGeometryScope)AbcG::GetGeometryScope( md );
}

// Set Geometry Scope
//----------------------------------------------------------
EXPORT void ABC_SetGeometryScope(AlembicIObject* obj, ABCGeometryScope scope){
	Abc::MetaData md = obj->GetIObject()->getMetaData();
	AbcG::GeometryScope iScope = (AbcG::GeometryScope)scope;
	AbcG::SetGeometryScope( md, iScope );
}

// Init Object
//----------------------------------------------------------
EXPORT void ABC_InitObject(AlembicIObject* obj,ABCGeometricType type){
    std::cout << "ABC_OBJECT INIT CALLED" <<std::endl;
	obj->Init(type);
}
// Get Object From Archive
//----------------------------------------------------------
EXPORT AlembicIObject* ABC_GetObjectFromArchiveByID(AlembicIArchive* archive,uint64_t identifier)
{
	Abc::IObject* obj = archive->GetObjectFromID(identifier);
	if(obj){
		if(AbcG::IPolyMesh::matches(obj->getMetaData())){
			AlembicIPolymesh* polymesh = new AlembicIPolymesh(obj);
			polymesh->Init(GeometricType_PolyMesh);
			return (AlembicIObject*)polymesh;
		}
		else if(AbcG::ISubD::matches(obj->getMetaData())){
			AlembicIPolymesh* polymesh = new AlembicIPolymesh(obj);
			polymesh->Init(GeometricType_PolyMesh);
			return (AlembicIObject*)polymesh;
		}
		else if(AbcG::IPoints::matches(obj->getMetaData())){
			AlembicIPointCloud* points = new AlembicIPointCloud(obj);
			points->Init(GeometricType_Points);
			return (AlembicIObject*)points;
		}
		else if(AbcG::IXform::matches(obj->getMetaData())){
			AlembicIXForm* xform = new AlembicIXForm(obj);
			return (AlembicIObject*)xform;
		}
		else if(AbcG::ICurves::matches(obj->getMetaData())){
			AlembicICurves* curve = new AlembicICurves(obj);
			return (AlembicIObject*)curve;
		}
		else if(AbcG::ICamera::matches(obj->getMetaData())){
			AlembicICamera* camera = new AlembicICamera(obj);
			return (AlembicIObject*)camera;
		}
		else if(AbcG::ILight::matches(obj->getMetaData())){
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
EXPORT AlembicIObject* ABC_GetObjectFromArchiveByName(AlembicIArchive* archive,const char* name)
{
	archive->GetAllObjects();
	for(int i=0;i<archive->GetNumObjects();i++)
	{
		Abc::IObject* obj = archive->GetObjectFromID(i);
		if(obj->getName()==name){
			AlembicIObject* out = new AlembicIObject(obj);
			return out;
		}
	}
	return NULL;
}

EXPORT int ABC_GetNumProperties(AlembicIObject* obj)
{
	return obj->GetNumProperties();
}

EXPORT const char* ABC_GetPropertyName(AlembicIObject* obj,uint64_t id)
{
    
	AlembicIProperty* prop = obj->GetProperty(id);
	std::string name = (std::string)prop->GetName();

	if(!name.size())
		return NULL;

	return name.c_str();
}

EXPORT AlembicIProperty* ABC_GetProperty(AlembicIObject* obj,uint64_t id)
{
	if(id >=obj->GetNumProperties())return NULL;
	return obj->GetProperty(id);
}

EXPORT void ABC_Polymesh_ReadFrame()
{

}

EXPORT const char* ABC_TestString(char* input)
{
	std::string out=std::string(input);
	return out.c_str();
}

EXPORT const char* ABC_GetObjectHeader(AlembicIObject* obj)
{
	if(!obj->GetIObject()->valid())
		return NULL;
	
	AbcA::ObjectHeader header = obj->GetIObject()->getHeader();
	std::string str = header.getFullName();//getMetaData().serialize();
	 
	if(!str.size())
		return NULL;

	return str.c_str();
}

EXPORT const char* ABC_GetObjectMetaData(AlembicIObject* obj)
{	
	if(!obj->GetIObject()->valid())
		return NULL;
	
	std::string metadata = obj->GetIObject()->getMetaData().serialize();
	if(!metadata.size())
		return NULL;

	return metadata.c_str();
}

EXPORT const char* ABC_GetObjectName(AlembicIObject* obj)
{	
	if(!obj->GetIObject()->valid())
		return NULL;
	std::string name = obj->GetIObject()->getName();
	if(!name.size())
		return NULL;

	return name.c_str();
}

EXPORT const char* ABC_GetObjectFullName(AlembicIObject* obj)
{	
	if(!obj->GetIObject()->valid())
		return NULL;
	std::string name = obj->GetIObject()->getFullName();
	
	if(!name.size())
		return NULL;

	return name.c_str();
}

EXPORT const char* ABC_GetInterpretation(AlembicIObject* obj,int ID)
{
	std::string interpretation = obj->_props[ID].GetInterpretation();
	if(!interpretation.size())
		return NULL;

	return interpretation.c_str();
}
