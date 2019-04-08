#include "AlembicArchive.h"
#include "AlembicXForm.h"
#include "AlembicProperty.h"
#include "AlembicObject.h"
#include "AlembicPolymesh.h"
#include "AlembicPoints.h"
#include "AlembicCurves.h"
#include "AlembicFaceSet.h"
#include "AlembicCamera.h"
#include "AlembicFaceSet.h"
//#include "AlembicLight.h"

BOOZE_NAMESPACE_OPEN_SCOPE

void AlembicOObject::Set(Abc::OObject& obj){
	m_object = obj;
}

BOOZE_EXPORT AlembicIObject* newIObject(AlembicIArchive* archive, uint64_t index)
{
	//AbcG::IObject iObj = archive->GetObj(index);
	// AbcG::IObject(archive->Get(), std::string(archive->GetIdentifier(index)));
	AbcG::IObject iObj = archive->GetIObj(index);

	if (AbcG::IPolyMesh::matches(iObj.getMetaData())){
		AlembicIPolymesh* polymesh = new AlembicIPolymesh(iObj);
		polymesh->Initialize();
		archive->AddObject((AlembicIObject*)polymesh);
		return (AlembicIObject*)polymesh;
	}
	else if (AbcG::IPoints::matches(iObj.getMetaData())){
		AlembicIPoints* points = new AlembicIPoints(iObj);
		points->Initialize();
		archive->AddObject((AlembicIObject*)points);
		return (AlembicIObject*)points;
	}
	else if (AbcG::IXform::matches(iObj.getMetaData())){
		AlembicIXForm* xform = new AlembicIXForm(iObj);
		xform->Initialize();
		archive->AddObject((AlembicIObject*)xform);
		return (AlembicIObject*)xform;
	}
	/*
	else if (AbcG::ISubD::matches(iObj.getMetaData())){
		AlembicISubD* subd = new AlembicISubD(iObj));
		subd->Initialize();
		return (AlembicIObject*)subd;
	}
	*/
	else if (AbcG::ICurves::matches(iObj.getMetaData())){
		AlembicICurves* curves = new AlembicICurves(iObj);
		curves->Initialize();
		archive->AddObject((AlembicIObject*)curves);
		return (AlembicIObject*)curves;
		
	}
	else if (AbcG::IFaceSet::matches(iObj.getMetaData())){
		AlembicIFaceSet* faceset = new AlembicIFaceSet(iObj);
		faceset->Initialize();
		archive->AddObject((AlembicIObject*)faceset);
		return (AlembicIObject*)faceset;
	}
	
	else if (AbcG::ICamera::matches(iObj.getMetaData())){
		AlembicICamera* camera = new AlembicICamera(iObj);
		camera->Initialize();
		archive->AddObject((AlembicIObject*)camera);
		return (AlembicIObject*)camera;
		
	}
	/*
	else if (AbcG::ILight::matches(object.getMetaData())){
		_type = GeometricType_Light;
		
		AlembicILight* light = new AlembicILight(obj);
		return (AlembicIObject*)light;
		
	}
	*/
	return new AlembicIObject(iObj);
}

BOOZE_EXPORT void deleteIObject(AlembicIObject* object)
{
	delete object;
}

bool AlembicIObject::Initialize(){
	m_type = GeometricType_Unknown;
	return false;
}

const char* AlembicIObject::GetName(){
	return m_object.getName().c_str();
}

const char* AlembicIObject::GetFullName(){
	return m_object.getFullName().c_str();
}

bool  AlembicIObject::HasProperty(const char* p_name){
	for (int i = 0; i<this->GetNumProperties(); i++){
		if (this->GetProperty(i)->GetName() == (std::string)p_name)return true;
	}
	return false;
}

// Get Properties
void AlembicIObject::GetProperties()
{
	AbcG::ICompoundProperty p;
	bool hasArbGeomParams = false;
	switch (m_type)
	{
		case GeometricType_PolyMesh:
		{
			Alembic::AbcGeom::IPolyMesh mesh(Get(), Alembic::Abc::kWrapExisting);
			if (!mesh.valid())return;
			p = Alembic::AbcGeom::ICompoundProperty(mesh.getSchema().getArbGeomParams());
			if (p.valid())hasArbGeomParams = true;
			break;
		}
		case GeometricType_Points:
		{
			Alembic::AbcGeom::IPoints points(Get(), Alembic::Abc::kWrapExisting);
			if (!points.valid())return;
			p = Alembic::AbcGeom::ICompoundProperty(points.getSchema().getArbGeomParams());
			if (p.valid())hasArbGeomParams = true;
			break;
		}
		case GeometricType_Curves:
		{
			Alembic::AbcGeom::ICurves curves(Get(), Alembic::Abc::kWrapExisting);
			if (!curves.valid())return;
			p = Alembic::AbcGeom::ICompoundProperty(curves.getSchema().getArbGeomParams());
			if (p.valid())hasArbGeomParams = true;
			break;
		}
		default:
			return;
	}

	if (hasArbGeomParams)
	{
		for (size_t i = 0; i < p.getNumProperties(); i++) {
			Alembic::Abc::PropertyHeader header = p.getPropertyHeader(i);
			AlembicIProperty* prop = new AlembicIProperty(header);
			prop->Init(p);
			m_props.push_back(prop);
		}
	}
	/*
	else
	{
		Abc::ICompoundProperty props = Get().getProperties();
		for (size_t i = 0; i < props.getNumProperties(); i++)
		{
			Abc::PropertyHeader header = props.getPropertyHeader(i);
			AlembicIProperty* prop = new AlembicIProperty(header);
			prop->Init(p);
			_props.push_back(prop);
		}
	}
	*/
}

AlembicIProperty* AlembicIObject::GetProperty(uint64_t index)
{
	if (index<m_props.size())
		return m_props[index];
	return NULL;
}

/*
AlembicIProperty* AlembicIObject::GetProperty(uint64_t id)
{
	
	if (id<_props.size())
		return &_props[id];
	
	return NULL;
}
*/

/*
AlembicIObject::AlembicIObject(Alembic::Abc::IObject* obj){
	_abcobject = obj;
    _props.resize(0);
}

void AlembicIObject::Init(ABCGeometricType type){
	_type = type;
	GetProperties();
}

void AlembicIObject::SetIObject(Alembic::Abc::IObject* obj)
{
	_abcobject = obj;
	if(obj!=NULL)
		_metadatastr = _abcobject->getMetaData().serialize();
	else
		_metadatastr = "";
}

// Alembic Output
void AlembicOObject::SetOObject(Alembic::Abc::OObject* obj)
{
	_abcobject = obj;
	_metadatastr = _abcobject->getMetaData().serialize();
}


// Get Properties
void AlembicIObject::GetProperties()
{
	Alembic::AbcGeom::ICompoundProperty p;
	bool hasArbGeomParams = false;
	switch(_type){
		case GeometricType_PolyMesh:
		{
			Alembic::AbcGeom::IPolyMesh mesh(*GetIObject(),Alembic::Abc::kWrapExisting);
			if(!mesh.valid())return;
			p = Alembic::AbcGeom::ICompoundProperty(mesh.getSchema().getArbGeomParams());
			if(p.valid())hasArbGeomParams = true;
			break;
		}
		case GeometricType_Points:
		{
			Alembic::AbcGeom::IPoints points(*GetIObject(),Alembic::Abc::kWrapExisting);
			if(!points.valid())return;
			p = Alembic::AbcGeom::ICompoundProperty(points.getSchema().getArbGeomParams());
			if(p.valid())hasArbGeomParams = true;
			break;
		}
		case GeometricType_Curves:
		{
			Alembic::AbcGeom::ICurves curves(*GetIObject(),Alembic::Abc::kWrapExisting);
			if(!curves.valid())return;
			p = Alembic::AbcGeom::ICompoundProperty(curves.getSchema().getArbGeomParams());
			if(p.valid())hasArbGeomParams = true;
			break;
		}
		default:
			return;

	}
	
	
	Abc::ICompoundProperty props = GetIObject()->getProperties();
	for ( size_t i = 0 ; i < props.getNumProperties() ; i++ ) {
		Abc::PropertyHeader header = props.getPropertyHeader( i );
		AlembicIProperty prop(header);
		prop.Init(p);
		_props.push_back(prop);
	}
	
	if(hasArbGeomParams)
	{
		for ( size_t i = 0 ; i < p.getNumProperties() ; i++ ) {
			Alembic::Abc::PropertyHeader header = p.getPropertyHeader( i );
			AlembicIProperty prop(header);
			prop.Init(p);
			_props.push_back(prop);
		}
	}
    

}

uint64_t AlembicIObject::GetNumProperties()
{
	return (uint64_t)_props.size();
}

AlembicIProperty* AlembicIObject::GetProperty(uint64_t id)
{
    if(id<_props.size())
		return &_props[id];
    return NULL;
}
*/

BOOZE_NAMESPACE_CLOSE_SCOPE




