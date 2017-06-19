#include "AlembicProperty.h"
using namespace Alembic::AbcGeom;

//------------------------------------------------------
// AlembicIProperty
//------------------------------------------------------
AlembicIProperty::AlembicIProperty(Alembic::Abc::PropertyHeader& header){
	_name = header.getName();
	_header = header;
	if (_header.isScalar())
	{
		_type = PropertyType_Scalar;
		if(Alembic::Abc::IBoolProperty::matches(_header))
			_traits = DataTraits_Bool;
		else if(Alembic::Abc::IUcharProperty::matches(_header))
			_traits = DataTraits_UChar;
		else if(Alembic::Abc::ICharProperty::matches(_header))
			_traits = DataTraits_Char;
		else if(Alembic::Abc::IUInt16Property::matches(_header))
			_traits = DataTraits_UInt16;
		else if(Alembic::Abc::IInt16Property::matches(_header))
			_traits = DataTraits_Int16;
		else if(Alembic::Abc::IUInt32Property::matches(_header))
			_traits = DataTraits_UInt32;
		else if(Alembic::Abc::IInt32Property::matches(_header))
			_traits = DataTraits_Int32;
		else if(Alembic::Abc::IUInt64Property::matches(_header))
			_traits = DataTraits_UInt64;
		else if(Alembic::Abc::IInt64Property::matches(_header))
			_traits = DataTraits_Int64;
		else if(Alembic::Abc::IHalfProperty::matches(_header))
			_traits = DataTraits_Half;
		else if(Alembic::Abc::IFloatProperty::matches(_header))
			_traits = DataTraits_Float;
		else if(Alembic::Abc::IDoubleProperty::matches(_header))
			_traits = DataTraits_Double;
		else if(Alembic::Abc::IStringProperty::matches(_header))
			_traits = DataTraits_String;
		else if(Alembic::Abc::IWstringProperty::matches(_header))
			_traits = DataTraits_WString;
		else if(Alembic::Abc::IV2sProperty::matches(_header))
			_traits = DataTraits_V2s;
		else if(Alembic::Abc::IV2iProperty::matches(_header))
			_traits = DataTraits_V2i;
		else if(Alembic::Abc::IV2fProperty::matches(_header))
			_traits = DataTraits_V2f;
		else if(Alembic::Abc::IV2dProperty::matches(_header))
			_traits = DataTraits_V2d;

		else if(Alembic::Abc::IV3sProperty::matches(_header))
			_traits = DataTraits_V3s;
		else if(Alembic::Abc::IV3iProperty::matches(_header))
			_traits = DataTraits_V3i;
		else if(Alembic::Abc::IV3fProperty::matches(_header))
			_traits = DataTraits_V3f;
		else if(Alembic::Abc::IV3dProperty::matches(_header))
			_traits = DataTraits_V3d;

		else if(Alembic::Abc::IP2sProperty::matches(_header))
			_traits = DataTraits_P2s;
		else if(Alembic::Abc::IP2iProperty::matches(_header))
			_traits = DataTraits_P2i;
		else if(Alembic::Abc::IP2fProperty::matches(_header))
			_traits = DataTraits_P2f;
		else if(Alembic::Abc::IP2dProperty::matches(_header))
			_traits = DataTraits_P2d;

		else if(Alembic::Abc::IP3sProperty::matches(_header))
			_traits = DataTraits_P3s;
		else if(Alembic::Abc::IP3iProperty::matches(_header))
			_traits = DataTraits_P3i;
		else if(Alembic::Abc::IP3fProperty::matches(_header))
			_traits = DataTraits_P3f;
		else if(Alembic::Abc::IP3dProperty::matches(_header))
			_traits = DataTraits_P3d;

		else if(Alembic::Abc::IBox2sProperty::matches(_header))
			_traits = DataTraits_Box2s;
		else if(Alembic::Abc::IBox2iProperty::matches(_header))
			_traits = DataTraits_Box2i;
		else if(Alembic::Abc::IBox2fProperty::matches(_header))
			_traits = DataTraits_Box2f;
		else if(Alembic::Abc::IBox2dProperty::matches(_header))
			_traits = DataTraits_Box2d;

		else if(Alembic::Abc::IBox3sProperty::matches(_header))
			_traits = DataTraits_Box3s;
		else if(Alembic::Abc::IBox3iProperty::matches(_header))
			_traits = DataTraits_Box3i;
		else if(Alembic::Abc::IBox3fProperty::matches(_header))
			_traits = DataTraits_Box3f;
		else if(Alembic::Abc::IBox3dProperty::matches(_header))
			_traits = DataTraits_Box3d;

		else if(Alembic::Abc::IM33fProperty::matches(_header))
			_traits = DataTraits_M33f;
		else if(Alembic::Abc::IM33dProperty::matches(_header))
			_traits = DataTraits_M33d;
		else if(Alembic::Abc::IM44fProperty::matches(_header))
			_traits = DataTraits_M44f;
		else if(Alembic::Abc::IM44dProperty::matches(_header))
			_traits = DataTraits_M44d;

		else if(Alembic::Abc::IQuatfProperty::matches(_header))
			_traits = DataTraits_Quatf;
		else if(Alembic::Abc::IQuatdProperty::matches(_header))
			_traits = DataTraits_Quatd;

		else if(Alembic::Abc::IC3hProperty::matches(_header))
			_traits = DataTraits_C3h;
		else if(Alembic::Abc::IC3fProperty::matches(_header))
			_traits = DataTraits_C3f;
		else if(Alembic::Abc::IC3cProperty::matches(_header))
			_traits = DataTraits_C3c;

		else if(Alembic::Abc::IC4hProperty::matches(_header))
			_traits = DataTraits_C4h;
		else if(Alembic::Abc::IC4fProperty::matches(_header))
			_traits = DataTraits_C4f;
		else if(Alembic::Abc::IC4cProperty::matches(_header))
			_traits = DataTraits_C4c;

		else if(Alembic::Abc::IN2fProperty::matches(_header))
			_traits = DataTraits_N2f;
		else if(Alembic::Abc::IN2dProperty::matches(_header))
			_traits = DataTraits_N2d;

		else if(Alembic::Abc::IN3fProperty::matches(_header))
			_traits = DataTraits_N3f;
		else if(Alembic::Abc::IN3dProperty::matches(_header))
			_traits = DataTraits_N3d;
		else
			_traits = DataTraits_Unknown;
	}
    else if (header.isArray())
	{
		_type = PropertyType_Array;
		if(Alembic::Abc::IBoolArrayProperty::matches(_header))
			_traits = DataTraits_Bool;
		else if(Alembic::Abc::IUcharArrayProperty::matches(_header))
			_traits = DataTraits_UChar;
		else if(Alembic::Abc::ICharArrayProperty::matches(_header))
			_traits = DataTraits_Char;
		else if(Alembic::Abc::IUInt16ArrayProperty::matches(_header))
			_traits = DataTraits_UInt16;
		else if(Alembic::Abc::IInt16ArrayProperty::matches(_header))
			_traits = DataTraits_Int16;
		else if(Alembic::Abc::IUInt32ArrayProperty::matches(_header))
			_traits = DataTraits_UInt32;
		else if(Alembic::Abc::IInt32ArrayProperty::matches(_header))
			_traits = DataTraits_Int32;
		else if(Alembic::Abc::IUInt64ArrayProperty::matches(_header))
			_traits = DataTraits_UInt64;
		else if(Alembic::Abc::IInt64ArrayProperty::matches(_header))
			_traits = DataTraits_Int64;
		else if(Alembic::Abc::IHalfArrayProperty::matches(_header))
			_traits = DataTraits_Half;
		else if(Alembic::Abc::IFloatArrayProperty::matches(_header))
			_traits = DataTraits_Float;
		else if(Alembic::Abc::IDoubleArrayProperty::matches(_header))
			_traits = DataTraits_Double;
		else if(Alembic::Abc::IStringArrayProperty::matches(_header))
			_traits = DataTraits_String;
		else if(Alembic::Abc::IWstringArrayProperty::matches(_header))
			_traits = DataTraits_WString;
		else if(Alembic::Abc::IV2sArrayProperty::matches(_header))
			_traits = DataTraits_V2s;
		else if(Alembic::Abc::IV2iArrayProperty::matches(_header))
			_traits = DataTraits_V2i;
		else if(Alembic::Abc::IV2fArrayProperty::matches(_header))
			_traits = DataTraits_V2f;
		else if(Alembic::Abc::IV2dArrayProperty::matches(_header))
			_traits = DataTraits_V2d;

		else if(Alembic::Abc::IV3sArrayProperty::matches(_header))
			_traits = DataTraits_V3s;
		else if(Alembic::Abc::IV3iArrayProperty::matches(_header))
			_traits = DataTraits_V3i;
		else if(Alembic::Abc::IV3fArrayProperty::matches(_header))
			_traits = DataTraits_V3f;
		else if(Alembic::Abc::IV3dArrayProperty::matches(_header))
			_traits = DataTraits_V3d;

		else if(Alembic::Abc::IP2sArrayProperty::matches(_header))
			_traits = DataTraits_P2s;
		else if(Alembic::Abc::IP2iArrayProperty::matches(_header))
			_traits = DataTraits_P2i;
		else if(Alembic::Abc::IP2fArrayProperty::matches(_header))
			_traits = DataTraits_P2f;
		else if(Alembic::Abc::IP2dArrayProperty::matches(_header))
			_traits = DataTraits_P2d;

		else if(Alembic::Abc::IP3sArrayProperty::matches(_header))
			_traits = DataTraits_P3s;
		else if(Alembic::Abc::IP3iArrayProperty::matches(_header))
			_traits = DataTraits_P3i;
		else if(Alembic::Abc::IP3fArrayProperty::matches(_header))
			_traits = DataTraits_P3f;
		else if(Alembic::Abc::IP3dArrayProperty::matches(_header))
			_traits = DataTraits_P3d;

		else if(Alembic::Abc::IBox2sArrayProperty::matches(_header))
			_traits = DataTraits_Box2s;
		else if(Alembic::Abc::IBox2iArrayProperty::matches(_header))
			_traits = DataTraits_Box2i;
		else if(Alembic::Abc::IBox2fArrayProperty::matches(_header))
			_traits = DataTraits_Box2f;
		else if(Alembic::Abc::IBox2dArrayProperty::matches(_header))
			_traits = DataTraits_Box2d;

		else if(Alembic::Abc::IBox3sArrayProperty::matches(_header))
			_traits = DataTraits_Box3s;
		else if(Alembic::Abc::IBox3iArrayProperty::matches(_header))
			_traits = DataTraits_Box3i;
		else if(Alembic::Abc::IBox3fArrayProperty::matches(_header))
			_traits = DataTraits_Box3f;
		else if(Alembic::Abc::IBox3dArrayProperty::matches(_header))
			_traits = DataTraits_Box3d;

		else if(Alembic::Abc::IM33fArrayProperty::matches(_header))
			_traits = DataTraits_M33f;
		else if(Alembic::Abc::IM33dArrayProperty::matches(_header))
			_traits = DataTraits_M33d;
		else if(Alembic::Abc::IM44fArrayProperty::matches(_header))
			_traits = DataTraits_M44f;
		else if(Alembic::Abc::IM44dArrayProperty::matches(_header))
			_traits = DataTraits_M44d;

		else if(Alembic::Abc::IQuatfArrayProperty::matches(_header))
			_traits = DataTraits_Quatf;
		else if(Alembic::Abc::IQuatdArrayProperty::matches(_header))
			_traits = DataTraits_Quatd;

		else if(Alembic::Abc::IC3hArrayProperty::matches(_header))
			_traits = DataTraits_C3h;
		else if(Alembic::Abc::IC3fArrayProperty::matches(_header))
			_traits = DataTraits_C3f;
		else if(Alembic::Abc::IC3cArrayProperty::matches(_header))
			_traits = DataTraits_C3c;

		else if(Alembic::Abc::IC4hArrayProperty::matches(_header))
			_traits = DataTraits_C4h;
		else if(Alembic::Abc::IC4fArrayProperty::matches(_header))
			_traits = DataTraits_C4f;
		else if(Alembic::Abc::IC4cArrayProperty::matches(_header))
			_traits = DataTraits_C4c;

		else if(Alembic::Abc::IN2fArrayProperty::matches(_header))
			_traits = DataTraits_N2f;
		else if(Alembic::Abc::IN2dArrayProperty::matches(_header))
			_traits = DataTraits_N2d;

		else if(Alembic::Abc::IN3fArrayProperty::matches(_header))
			_traits = DataTraits_N3f;
		else if(Alembic::Abc::IN3dArrayProperty::matches(_header))
			_traits = DataTraits_N3d;
		else
			_traits = DataTraits_Unknown;
	}
    else
		_type = PropertyType_Compound;

	//if(
	/*
	Alembic::AbcCoreAbstract::DataType type = header.getDataType();
	switch(type.getPod()){
		case Alembic::Abc::kBooleanPOD:
			_traits = DataTraits_Bool;
			break;
		case Alembic::Abc::kUint8POD:
			_traits = DataTraits_UInt8;
			break;
		case Alembic::Abc::kInt8POD:
			_traits = DataTraits_Int8;
			break;
		case Alembic::Abc::kUint16POD:
			_traits = DataTraits_UInt16;
			break;
	}
	*/
}

void AlembicIProperty::Init(Alembic::Abc::ICompoundProperty& iParentProp)
{
	if(_type == PropertyType_Scalar)
	{
		switch(_traits)
		{
			case DataTraits_Bool:
			{
				_scalar = Alembic::AbcGeom::IBoolProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_UChar:
			{
				_scalar = Alembic::AbcGeom::IUcharProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_Char:
			{
				_scalar = Alembic::AbcGeom::ICharProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_UInt16:
			{
				_scalar = Alembic::AbcGeom::IUInt16Property(iParentProp,_name,0);
				break;
			}
			case DataTraits_Int16:
			{
				_scalar = Alembic::AbcGeom::IInt16Property(iParentProp,_name,0);
				break;
			}
			case DataTraits_UInt32:
			{
				_scalar = Alembic::AbcGeom::IUInt32Property(iParentProp,_name,0);
				break;
			}
			case DataTraits_Int32:
			{
				_scalar = Alembic::AbcGeom::IInt32Property(iParentProp,_name,0);
				break;
			}
			case DataTraits_UInt64:
			{
				_scalar = Alembic::AbcGeom::IUInt64Property(iParentProp,_name,0);
				break;
			}
			case DataTraits_Int64:
			{
				_scalar = Alembic::AbcGeom::IInt64Property(iParentProp,_name,0);
				break;
			}
			case DataTraits_Half:
			{
				_scalar = Alembic::AbcGeom::IHalfProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_Float:
			{
				_scalar = Alembic::AbcGeom::IFloatProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_Double:
			{
				_scalar = Alembic::AbcGeom::IDoubleProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_V2s:
			{
				_scalar = Alembic::AbcGeom::IV2sProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_V2i:
			{
				_scalar = Alembic::AbcGeom::IV2iProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_V2f:
			{
				_scalar = Alembic::AbcGeom::IV2fProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_V2d:
			{
				_scalar = Alembic::AbcGeom::IV2dProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_V3s:
			{
				_scalar = Alembic::AbcGeom::IV3sProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_V3i:
			{
				_scalar = Alembic::AbcGeom::IV3iProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_V3f:
			{
				_scalar = Alembic::AbcGeom::IV3fProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_V3d:
			{
				_scalar = Alembic::AbcGeom::IV3dProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_P3s:
			{
				_scalar = Alembic::AbcGeom::IP3sProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_P3i:
			{
				_scalar = Alembic::AbcGeom::IP3iProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_P3f:
			{
				_scalar = Alembic::AbcGeom::IP3fProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_P3d:
			{
				_scalar = Alembic::AbcGeom::IP3dProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_Box2s:
			{
				_scalar = Alembic::AbcGeom::IBox2sProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_Box2i:
			{
				_scalar = Alembic::AbcGeom::IBox2iProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_Box2f:
			{
				_scalar = Alembic::AbcGeom::IBox2fProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_Box2d:
			{
				_scalar = Alembic::AbcGeom::IBox2dProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_Box3s:
			{
				_scalar = Alembic::AbcGeom::IBox3sProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_Box3i:
			{
				_scalar = Alembic::AbcGeom::IBox3iProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_Box3f:
			{
				_scalar = Alembic::AbcGeom::IBox3fProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_Box3d:
			{
				_scalar = Alembic::AbcGeom::IBox3dProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_M33f:
			{
				_scalar = Alembic::AbcGeom::IM33fProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_M33d:
			{
				_scalar = Alembic::AbcGeom::IM33dProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_M44f:
			{
				_scalar = Alembic::AbcGeom::IM44fProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_M44d:
			{
				_scalar = Alembic::AbcGeom::IM44dProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_Quatf:
			{
				_scalar = Alembic::AbcGeom::IQuatfProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_Quatd:
			{
				_scalar = Alembic::AbcGeom::IQuatdProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_C3h:
			{
				_scalar = Alembic::AbcGeom::IC3hProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_C3f:
			{
				_scalar = Alembic::AbcGeom::IC3fProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_C3c:
			{
				_scalar = Alembic::AbcGeom::IC3cProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_C4h:
			{
				_scalar = Alembic::AbcGeom::IC4hProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_C4f:
			{
				_scalar = Alembic::AbcGeom::IC4fProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_C4c:
			{
				_scalar = Alembic::AbcGeom::IC4cProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_N2f:
			{
				_scalar = Alembic::AbcGeom::IN2fProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_N2d:
			{
				_scalar = Alembic::AbcGeom::IN2dProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_N3f:
			{
				_scalar = Alembic::AbcGeom::IN3fProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_N3d:
			{
				_scalar = Alembic::AbcGeom::IN3dProperty(iParentProp,_name,0);
				break;
			}
			/*
			case DataTraits_V4f:
			{
				_scalar = Alembic::AbcGeom::IC4fProperty(iParentProp,_name,0);
				break;
			}
			case DataTraits_Rotf:
			{
				_scalar = Alembic::AbcGeom::IQuatfProperty(iParentProp,_name,0);
				break;
			}
			*/
		}
	}
	else if(_type == PropertyType_Array)
	{
		switch(_traits)
		{
			case DataTraits_Bool:
			{
				_array = Alembic::Abc::IBoolArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_UChar:
			{
				_array = Alembic::Abc::IUcharArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_Char:
			{
				_array = Alembic::Abc::ICharArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_UInt16:
			{
				_array = Alembic::Abc::IUInt16ArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_Int16:
			{
				_array = Alembic::Abc::IInt16ArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_UInt32:
			{
				_array = Alembic::Abc::IUInt32ArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_Int32:
			{
				_array = Alembic::Abc::IInt32ArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_UInt64:
			{
				_array = Alembic::Abc::IUInt64ArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_Int64:
			{
				_array = Alembic::Abc::IInt64ArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_Half:
			{
				_array = Alembic::Abc::IHalfArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_Float:
			{
				_array = Alembic::Abc::IFloatArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_Double:
			{
				_array = Alembic::Abc::IDoubleArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_V2s:
			{
				_array = Alembic::Abc::IV2sArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_V2i:
			{
				_array = Alembic::Abc::IV2iArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_V2f:
			{
				_array = Alembic::Abc::IV2fArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_V2d:
			{
				_array = Alembic::Abc::IV2dArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_V3s:
			{
				_array = Alembic::Abc::IV3sArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_V3i:
			{
				_array = Alembic::Abc::IV3iArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_V3f:
			{
				_array = Alembic::Abc::IV3fArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_V3d:
			{
				_array = Alembic::Abc::IV3dArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_P3s:
			{
				_array = Alembic::Abc::IP3sArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_P3i:
			{
				_array = Alembic::Abc::IP3iArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_P3f:
			{
				_array = Alembic::Abc::IP3fArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_P3d:
			{
				_array = Alembic::Abc::IP3dArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_Box2s:
			{
				_array = Alembic::Abc::IBox2sArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_Box2i:
			{
				_array = Alembic::Abc::IBox2iArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_Box2f:
			{
				_array = Alembic::Abc::IBox2fArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_Box2d:
			{
				_array = Alembic::Abc::IBox2dArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_Box3s:
			{
				_array = Alembic::Abc::IBox3sArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_Box3i:
			{
				_array = Alembic::Abc::IBox3iArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_Box3f:
			{
				_array = Alembic::Abc::IBox3fArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_Box3d:
			{
				_array = Alembic::Abc::IBox3dArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_M33f:
			{
				_array = Alembic::Abc::IM33fArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_M33d:
			{
				_array = Alembic::Abc::IM33dArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_M44f:
			{
				_array = Alembic::Abc::IM44fArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_M44d:
			{
				_array = Alembic::Abc::IM44dArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_Quatf:
			{
				_array = Alembic::Abc::IQuatfArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_Quatd:
			{
				_array = Alembic::Abc::IQuatdArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_C3h:
			{
				_array = Alembic::Abc::IC3hArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_C3f:
			{
				_array = Alembic::Abc::IC3fArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_C3c:
			{
				_array = Alembic::Abc::IC3cArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_C4h:
			{
				_array = Alembic::Abc::IC4hArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_C4f:
			{
				_array = Alembic::Abc::IC4fArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_C4c:
			{
				_array = Alembic::Abc::IC4cArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_N2f:
			{
				_array = Alembic::Abc::IN2fArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_N2d:
			{
				_array = Alembic::Abc::IN2dArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_N3f:
			{
				_array = Alembic::Abc::IN3fArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_N3d:
			{
				_array = Alembic::Abc::IN3dArrayProperty(iParentProp,_name);
				break;
			}
			/*
			case DataTraits_V4f:
			{
				_array = Alembic::Abc::IC4fArrayProperty(iParentProp,_name);
				break;
			}
			case DataTraits_Rotf:
			{
				_array = Alembic::Abc::IQuatfArrayProperty(iParentProp,_name);
				break;
			}
			*/
		}
	}
}

void* AlembicIProperty::GetSample(float T){
	
	Alembic::AbcCoreAbstract::chrono_t t(T);
	Alembic::Abc::ISampleSelector selector(t,Alembic::Abc::ISampleSelector::kNearIndex);
	switch(_type){
		case PropertyType_Scalar:
		{
			void* out;
			_scalar.get(out,selector);
			return out;
		}
		case PropertyType_Array:
		{
			Alembic::AbcCoreAbstract::ArraySamplePtr sample;
			_array.get(sample,selector);
			return (void*)sample->getData();
			break;
		}
	}
	return NULL;
}

bool AlembicIProperty::IsConstant(){
	return false;
}

const char*	AlembicIProperty::GetName()const{
	return this->_name.c_str();
}


ABCPropertyType	AlembicIProperty::GetPropertyType()const{
	return this->_type;
}

ABCDataTraits AlembicIProperty::GetDataTraits()const{
	return this->_traits;
}

void AlembicIProperty::SetMessage(const std::string& msg)
{
	_msg = msg;
}

Alembic::Abc::int64_t AlembicIProperty::GetNbItems(Alembic::Abc::chrono_t T)const{
	Alembic::Abc::ISampleSelector selector(T,Alembic::Abc::ISampleSelector::kNearIndex);

	switch(_type){
		case PropertyType_Scalar:
		{
			return 1;
		}
		case PropertyType_Array:
		{
			if(!_array)return -1;
			if(!_array.valid())return 0;

            Alembic::AbcCoreAbstract::ArraySamplePtr sample;
			_array.get(sample,selector);
            
			return sample->size();
		}
	}
}

std::string AlembicIProperty::GetMessage() const{
	return _msg;
}

std::string AlembicIProperty::GetInterpretation() const{
	std::string out = _header.getMetaData().get("interpretation");
	if(out=="")out = "simple";
	return out;
}

//------------------------------------------------------
// AlembicOProperty
//------------------------------------------------------
AlembicOProperty::AlembicOProperty(Alembic::Abc::OCompoundProperty& iParentProp, const ABCPropertyType type,const ABCDataTraits traits, const ABCGeometryScope scope, const std::string& name){
	_name = name;
	_type = type;
	_traits = traits;
	//_scope = scope;
	Alembic::AbcGeom::GeometryScope iScope;
	switch(scope){
		case GeometryScope_Constant:
			iScope = Alembic::AbcGeom::kConstantScope;
			break;
		case GeometryScope_Uniform:
			iScope = Alembic::AbcGeom::kUniformScope;
			break;
		case GeometryScope_Varying:
			iScope = Alembic::AbcGeom::kVaryingScope;
			break;
		case GeometryScope_Vertex:
			iScope = Alembic::AbcGeom::kVertexScope;
			break;
		case GeometryScope_FaceVarying:
			iScope = Alembic::AbcGeom::kFacevaryingScope;
			break;
		default:
			iScope = Alembic::AbcGeom::kUnknownScope;
			break;
	}

	switch(_type){
		case PropertyType_Scalar:
		{
			switch(_traits){
				case DataTraits_Bool:
				{
					_scalar = Alembic::AbcGeom::OBoolProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_UChar:
				{
					_scalar = Alembic::AbcGeom::OUcharProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Char:
				{
					_scalar = Alembic::AbcGeom::OCharProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_UInt16:
				{
					_scalar = Alembic::AbcGeom::OUInt16Property(iParentProp,name,0);
					break;
				}
				case DataTraits_Int16:
				{
					_scalar = Alembic::AbcGeom::OInt16Property(iParentProp,name,0);
					break;
				}
				case DataTraits_UInt32:
				{
					_scalar = Alembic::AbcGeom::OUInt32Property(iParentProp,name,0);
					break;
				}
				case DataTraits_Int32:
				{
					_scalar = Alembic::AbcGeom::OInt32Property(iParentProp,name,0);
					break;
				}
				case DataTraits_UInt64:
				{
					_scalar = Alembic::AbcGeom::OUInt64Property(iParentProp,name,0);
					break;
				}
				case DataTraits_Int64:
				{
					_scalar = Alembic::AbcGeom::OInt64Property(iParentProp,name,0);
					break;
				}
				case DataTraits_Half:
				{
					_scalar = Alembic::AbcGeom::OHalfProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Float:
				{
					_scalar = Alembic::AbcGeom::OFloatProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Double:
				{
					_scalar = Alembic::AbcGeom::ODoubleProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_V2s:
				{
					_scalar = Alembic::AbcGeom::OV2sProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_V2i:
				{
					_scalar = Alembic::AbcGeom::OV2iProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_V2f:
				{
					_scalar = Alembic::AbcGeom::OV2fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_V2d:
				{
					_scalar = Alembic::AbcGeom::OV2dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_V3s:
				{
					_scalar = Alembic::AbcGeom::OV3sProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_V3i:
				{
					_scalar = Alembic::AbcGeom::OV3iProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_V3f:
				{
					_scalar = Alembic::AbcGeom::OV3fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_V3d:
				{
					_scalar = Alembic::AbcGeom::OV3dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_P3s:
				{
					_scalar = Alembic::AbcGeom::OP3sProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_P3i:
				{
					_scalar = Alembic::AbcGeom::OP3iProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_P3f:
				{
					_scalar = Alembic::AbcGeom::OP3fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_P3d:
				{
					_scalar = Alembic::AbcGeom::OP3dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Box2s:
				{
					_scalar = Alembic::AbcGeom::OBox2sProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Box2i:
				{
					_scalar = Alembic::AbcGeom::OBox2iProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Box2f:
				{
					_scalar = Alembic::AbcGeom::OBox2fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Box2d:
				{
					_scalar = Alembic::AbcGeom::OBox2dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Box3s:
				{
					_scalar = Alembic::AbcGeom::OBox3sProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Box3i:
				{
					_scalar = Alembic::AbcGeom::OBox3iProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Box3f:
				{
					_scalar = Alembic::AbcGeom::OBox3fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Box3d:
				{
					_scalar = Alembic::AbcGeom::OBox3dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_M33f:
				{
					_scalar = Alembic::AbcGeom::OM33fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_M33d:
				{
					_scalar = Alembic::AbcGeom::OM33dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_M44f:
				{
					_scalar = Alembic::AbcGeom::OM44fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_M44d:
				{
					_scalar = Alembic::AbcGeom::OM44dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Quatf:
				{
					_scalar = Alembic::AbcGeom::OQuatfProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Quatd:
				{
					_scalar = Alembic::AbcGeom::OQuatdProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_C3h:
				{
					_scalar = Alembic::AbcGeom::OC3hProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_C3f:
				{
					_scalar = Alembic::AbcGeom::OC3fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_C3c:
				{
					_scalar = Alembic::AbcGeom::OC3cProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_C4h:
				{
					_scalar = Alembic::AbcGeom::OC4hProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_C4f:
				{
					_scalar = Alembic::AbcGeom::OC4fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_C4c:
				{
					_scalar = Alembic::AbcGeom::OC4cProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_N2f:
				{
					_scalar = Alembic::AbcGeom::ON2fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_N2d:
				{
					_scalar = Alembic::AbcGeom::ON2dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_N3f:
				{
					_scalar = Alembic::AbcGeom::ON3fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_N3d:
				{
					_scalar = Alembic::AbcGeom::ON3dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_String:
				{
					_scalar = Alembic::AbcGeom::OStringProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_WString:
				{
					_scalar = Alembic::AbcGeom::OWstringProperty(iParentProp,name,0);
					break;
				}
				
				case DataTraits_V4f:
				{
					_scalar = Alembic::AbcGeom::OC4fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Rotf:
				{
					_scalar = Alembic::AbcGeom::OQuatfProperty(iParentProp,name,0);
					break;
				}
			}
			
			break;
		}
		case PropertyType_Array:
		{
			switch(_traits){
				case DataTraits_Bool:
				{
					Alembic::AbcGeom::OBoolGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_UChar:
				{
					Alembic::AbcGeom::OUcharGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_Char:
				{
					Alembic::AbcGeom::OCharGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_UInt16:
				{
					Alembic::AbcGeom::OUInt16GeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_Int16:
				{
					Alembic::AbcGeom::OInt16GeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_UInt32:
				{
					Alembic::AbcGeom::OUInt32GeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_Int32:
				{
					Alembic::AbcGeom::OInt32GeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_UInt64:
				{
					Alembic::AbcGeom::OUInt64GeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_Int64:
				{
					Alembic::AbcGeom::OInt64GeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_Half:
				{
					Alembic::AbcGeom::OHalfGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_Float:
				{
					Alembic::AbcGeom::OFloatGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_Double:
				{
					Alembic::AbcGeom::ODoubleGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_V2s:
				{
					Alembic::AbcGeom::OV2sGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_V2i:
				{
					Alembic::AbcGeom::OV2iGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_V2f:
				{
					Alembic::AbcGeom::OV2fGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_V2d:
				{
					Alembic::AbcGeom::OV2dGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_V3s:
				{
					Alembic::AbcGeom::OV3sGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_V3i:
				{
					Alembic::AbcGeom::OV3iGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_V3f:
				{
					Alembic::AbcGeom::OV3fGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_V3d:
				{
					Alembic::AbcGeom::OV3dGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_P3s:
				{
					Alembic::AbcGeom::OP3sGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_P3i:
				{
					Alembic::AbcGeom::OP3iGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_P3f:
				{
					Alembic::AbcGeom::OP3fGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_P3d:
				{
					Alembic::AbcGeom::OP3dGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_Box2s:
				{
					Alembic::AbcGeom::OBox2sGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_Box2i:
				{
					Alembic::AbcGeom::OBox2iGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_Box2f:
				{
					Alembic::AbcGeom::OBox2fGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_Box2d:
				{
					Alembic::AbcGeom::OBox2dGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_Box3s:
				{
					Alembic::AbcGeom::OBox3sGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_Box3i:
				{
					Alembic::AbcGeom::OBox3iGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_Box3f:
				{
					Alembic::AbcGeom::OBox3fGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_Box3d:
				{
					Alembic::AbcGeom::OBox3dGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_M33f:
				{
					Alembic::AbcGeom::OM33fGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_M33d:
				{
					Alembic::AbcGeom::OM33dGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_M44f:
				{
					Alembic::AbcGeom::OM44fGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_M44d:
				{
					Alembic::AbcGeom::OM44dGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_Quatf:
				{
					Alembic::AbcGeom::OQuatfGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_Quatd:
				{
					Alembic::AbcGeom::OQuatdGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_C3h:
				{
					Alembic::AbcGeom::OC3hGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_C3f:
				{
					Alembic::AbcGeom::OC3fGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_C3c:
				{
					Alembic::AbcGeom::OC3cGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_C4h:
				{
					Alembic::AbcGeom::OC4hGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_C4f:
				{
					Alembic::AbcGeom::OC4fGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_C4c:
				{
					Alembic::AbcGeom::OC4cGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_N2f:
				{
					Alembic::AbcGeom::ON2fGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_N2d:
				{
					Alembic::AbcGeom::ON2dGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_N3f:
				{
					Alembic::AbcGeom::ON3fGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_N3d:
				{
					Alembic::AbcGeom::ON3dGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_String:
				{
					Alembic::AbcGeom::OStringGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_WString:
				{
					Alembic::AbcGeom::OWstringGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_V4f:
				{
					Alembic::AbcGeom::OC4fGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
				case DataTraits_Rotf:
				{
					Alembic::AbcGeom::OQuatfGeomParam p(iParentProp,name, false, iScope, 1,0);
					_array = p.getValueProperty();
					break;
				}
			}
			break;
		}
	}
}