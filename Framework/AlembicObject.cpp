#include "AlembicObject.h"

//namespace AbcA = ::Alembic::AbcCoreAbstract::ALEMBIC_VERSION_NS;

//using namespace AbcA;

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
	switch(_type){
		case GeometricType_PolyMesh:
			{
			Alembic::AbcGeom::IPolyMesh mesh(*GetIObject(),Alembic::Abc::kWrapExisting);
			if(!mesh.valid())return;
			p = Alembic::AbcGeom::ICompoundProperty(mesh.getSchema().getArbGeomParams());
			break;
			}
		case GeometricType_Points:
			{
			Alembic::AbcGeom::IPoints points(*GetIObject(),Alembic::Abc::kWrapExisting);
			if(!points.valid())return;
			p = Alembic::AbcGeom::ICompoundProperty(points.getSchema().getArbGeomParams());
			break;
			}
		case GeometricType_Curves:
			{
			Alembic::AbcGeom::ICurves curves(*GetIObject(),Alembic::Abc::kWrapExisting);
			if(!curves.valid())return;
			p = Alembic::AbcGeom::ICompoundProperty(curves.getSchema().getArbGeomParams());
			break;
			}
		default:
			return;

	}
	
	/*
    for ( size_t i = 0 ; i < p.getNumProperties() ; i++ ) {
		Alembic::Abc::PropertyHeader header = p.getPropertyHeader( i );
		AlembicIProperty prop(header);
		prop.Init(p);
		_props.push_back(prop);
    }
	*/
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





