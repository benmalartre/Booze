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

//=================================================================================================
// Alembic Export
//=================================================================================================
// Constructor
//-------------------------------------------------------------------------------------------------
BOOZE_EXPORT AlembicIObject* newIObject(AlembicIArchive* archive, uint64_t index)
{
	//AbcG::IObject iObj = archive->GetObj(index);
	// AbcG::IObject(archive->Get(), std::string(archive->GetIdentifier(index)));
	AbcG::IObject iObj = archive->getIObj(index);

	if (AbcG::IPolyMesh::matches(iObj.getMetaData())){
		AlembicIPolymesh* polymesh = new AlembicIPolymesh(iObj);
		polymesh->initialize();
		archive->addObject((AlembicIObject*)polymesh);
		return (AlembicIObject*)polymesh;
	}
	else if (AbcG::IPoints::matches(iObj.getMetaData())){
		AlembicIPoints* points = new AlembicIPoints(iObj);
		points->initialize();
		archive->addObject((AlembicIObject*)points);
		return (AlembicIObject*)points;
	}
	else if (AbcG::IXform::matches(iObj.getMetaData())){
		AlembicIXForm* xform = new AlembicIXForm(iObj);
		xform->initialize();
		archive->addObject((AlembicIObject*)xform);
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
		curves->initialize();
		archive->addObject((AlembicIObject*)curves);
		return (AlembicIObject*)curves;
		
	}
	else if (AbcG::IFaceSet::matches(iObj.getMetaData())){
		AlembicIFaceSet* faceset = new AlembicIFaceSet(iObj);
		faceset->initialize();
		archive->addObject((AlembicIObject*)faceset);
		return (AlembicIObject*)faceset;
	}
	
	else if (AbcG::ICamera::matches(iObj.getMetaData())){
		AlembicICamera* camera = new AlembicICamera(iObj);
		camera->initialize();
		archive->addObject((AlembicIObject*)camera);
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

// Destructor
//-------------------------------------------------------------------------------------------------
BOOZE_EXPORT void deleteIObject(AlembicIObject* object)
{
	delete object;
}

bool AlembicIObject::initialize(){
	m_type = GeometricType_Root;
	return false;
}

const char* AlembicIObject::getName(){
	return m_object.getName().c_str();
}

const char* AlembicIObject::getFullName(){
	return m_object.getFullName().c_str();
}

// Properties
//-------------------------------------------------------------------------------------------------
bool  AlembicIObject::hasProperty(const char* p_name){
	for (int i = 0; i<this->getNumProperties(); i++){
		if (this->getProperty(i)->getName() == (std::string)p_name)return true;
	}
	return false;
}

void AlembicIObject::getProperties()
{
	AbcG::ICompoundProperty p;
	bool hasArbGeomParams = false;
	switch (m_type)
	{
		case GeometricType_PolyMesh:
		{
			Alembic::AbcGeom::IPolyMesh mesh(get(), Alembic::Abc::kWrapExisting);
			if (!mesh.valid())return;
			p = Alembic::AbcGeom::ICompoundProperty(mesh.getSchema().getArbGeomParams());
			if (p.valid())hasArbGeomParams = true;
			break;
		}
		case GeometricType_Points:
		{
			Alembic::AbcGeom::IPoints points(get(), Alembic::Abc::kWrapExisting);
			if (!points.valid())return;
			p = Alembic::AbcGeom::ICompoundProperty(points.getSchema().getArbGeomParams());
			if (p.valid())hasArbGeomParams = true;
			break;
		}
		case GeometricType_Curves:
		{
			Alembic::AbcGeom::ICurves curves(get(), Alembic::Abc::kWrapExisting);
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
			prop->init(p);
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

AlembicIProperty* AlembicIObject::getProperty(uint64_t index)
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


void AlembicOObject::save(AbcA::TimeSamplingPtr time, AbcG::OObject& parent){
	for (int i = 0; i < m_children.size(); ++i)
	{
		m_children[i]->save(time, parent);
	}
}


BOOZE_NAMESPACE_CLOSE_SCOPE




