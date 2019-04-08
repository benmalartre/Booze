#include "AlembicProperty.h"

BOOZE_NAMESPACE_OPEN_SCOPE

//------------------------------------------------------
// AlembicIProperty
//------------------------------------------------------
AlembicIProperty::AlembicIProperty(Alembic::Abc::PropertyHeader& header){
	m_name = header.getName();
	m_header = header;
	if (m_header.isScalar())
	{
		m_type = PropertyType_Scalar;
		if(Alembic::Abc::IBoolProperty::matches(m_header))
			m_traits = DataTraits_Bool;
		else if(Alembic::Abc::IUcharProperty::matches(m_header))
			m_traits = DataTraits_UChar;
		else if(Alembic::Abc::ICharProperty::matches(m_header))
			m_traits = DataTraits_Char;
		else if(Alembic::Abc::IUInt16Property::matches(m_header))
			m_traits = DataTraits_UInt16;
		else if(Alembic::Abc::IInt16Property::matches(m_header))
			m_traits = DataTraits_Int16;
		else if(Alembic::Abc::IUInt32Property::matches(m_header))
			m_traits = DataTraits_UInt32;
		else if(Alembic::Abc::IInt32Property::matches(m_header))
			m_traits = DataTraits_Int32;
		else if(Alembic::Abc::IUInt64Property::matches(m_header))
			m_traits = DataTraits_UInt64;
		else if(Alembic::Abc::IInt64Property::matches(m_header))
			m_traits = DataTraits_Int64;
		else if(Alembic::Abc::IHalfProperty::matches(m_header))
			m_traits = DataTraits_Half;
		else if(Alembic::Abc::IFloatProperty::matches(m_header))
			m_traits = DataTraits_Float;
		else if(Alembic::Abc::IDoubleProperty::matches(m_header))
			m_traits = DataTraits_Double;
		else if(Alembic::Abc::IStringProperty::matches(m_header))
			m_traits = DataTraits_String;
		else if(Alembic::Abc::IWstringProperty::matches(m_header))
			m_traits = DataTraits_WString;
		else if(Alembic::Abc::IV2sProperty::matches(m_header))
			m_traits = DataTraits_V2s;
		else if(Alembic::Abc::IV2iProperty::matches(m_header))
			m_traits = DataTraits_V2i;
		else if(Alembic::Abc::IV2fProperty::matches(m_header))
			m_traits = DataTraits_V2f;
		else if(Alembic::Abc::IV2dProperty::matches(m_header))
			m_traits = DataTraits_V2d;

		else if(Alembic::Abc::IV3sProperty::matches(m_header))
			m_traits = DataTraits_V3s;
		else if(Alembic::Abc::IV3iProperty::matches(m_header))
			m_traits = DataTraits_V3i;
		else if(Alembic::Abc::IV3fProperty::matches(m_header))
			m_traits = DataTraits_V3f;
		else if(Alembic::Abc::IV3dProperty::matches(m_header))
			m_traits = DataTraits_V3d;

		else if(Alembic::Abc::IP2sProperty::matches(m_header))
			m_traits = DataTraits_P2s;
		else if(Alembic::Abc::IP2iProperty::matches(m_header))
			m_traits = DataTraits_P2i;
		else if(Alembic::Abc::IP2fProperty::matches(m_header))
			m_traits = DataTraits_P2f;
		else if(Alembic::Abc::IP2dProperty::matches(m_header))
			m_traits = DataTraits_P2d;

		else if(Alembic::Abc::IP3sProperty::matches(m_header))
			m_traits = DataTraits_P3s;
		else if(Alembic::Abc::IP3iProperty::matches(m_header))
			m_traits = DataTraits_P3i;
		else if(Alembic::Abc::IP3fProperty::matches(m_header))
			m_traits = DataTraits_P3f;
		else if(Alembic::Abc::IP3dProperty::matches(m_header))
			m_traits = DataTraits_P3d;

		else if(Alembic::Abc::IBox2sProperty::matches(m_header))
			m_traits = DataTraits_Box2s;
		else if(Alembic::Abc::IBox2iProperty::matches(m_header))
			m_traits = DataTraits_Box2i;
		else if(Alembic::Abc::IBox2fProperty::matches(m_header))
			m_traits = DataTraits_Box2f;
		else if(Alembic::Abc::IBox2dProperty::matches(m_header))
			m_traits = DataTraits_Box2d;

		else if(Alembic::Abc::IBox3sProperty::matches(m_header))
			m_traits = DataTraits_Box3s;
		else if(Alembic::Abc::IBox3iProperty::matches(m_header))
			m_traits = DataTraits_Box3i;
		else if(Alembic::Abc::IBox3fProperty::matches(m_header))
			m_traits = DataTraits_Box3f;
		else if(Alembic::Abc::IBox3dProperty::matches(m_header))
			m_traits = DataTraits_Box3d;

		else if(Alembic::Abc::IM33fProperty::matches(m_header))
			m_traits = DataTraits_M33f;
		else if(Alembic::Abc::IM33dProperty::matches(m_header))
			m_traits = DataTraits_M33d;
		else if(Alembic::Abc::IM44fProperty::matches(m_header))
			m_traits = DataTraits_M44f;
		else if(Alembic::Abc::IM44dProperty::matches(m_header))
			m_traits = DataTraits_M44d;

		else if(Alembic::Abc::IQuatfProperty::matches(m_header))
			m_traits = DataTraits_Quatf;
		else if(Alembic::Abc::IQuatdProperty::matches(m_header))
			m_traits = DataTraits_Quatd;

		else if(Alembic::Abc::IC3hProperty::matches(m_header))
			m_traits = DataTraits_C3h;
		else if(Alembic::Abc::IC3fProperty::matches(m_header))
			m_traits = DataTraits_C3f;
		else if(Alembic::Abc::IC3cProperty::matches(m_header))
			m_traits = DataTraits_C3c;

		else if(Alembic::Abc::IC4hProperty::matches(m_header))
			m_traits = DataTraits_C4h;
		else if(Alembic::Abc::IC4fProperty::matches(m_header))
			m_traits = DataTraits_C4f;
		else if(Alembic::Abc::IC4cProperty::matches(m_header))
			m_traits = DataTraits_C4c;

		else if(Alembic::Abc::IN2fProperty::matches(m_header))
			m_traits = DataTraits_N2f;
		else if(Alembic::Abc::IN2dProperty::matches(m_header))
			m_traits = DataTraits_N2d;

		else if(Alembic::Abc::IN3fProperty::matches(m_header))
			m_traits = DataTraits_N3f;
		else if(Alembic::Abc::IN3dProperty::matches(m_header))
			m_traits = DataTraits_N3d;
		else
			m_traits = DataTraits_Unknown;
	}
    else if (header.isArray())
	{
		m_type = PropertyType_Array;
		if(Alembic::Abc::IBoolArrayProperty::matches(m_header))
			m_traits = DataTraits_Bool;
		else if(Alembic::Abc::IUcharArrayProperty::matches(m_header))
			m_traits = DataTraits_UChar;
		else if(Alembic::Abc::ICharArrayProperty::matches(m_header))
			m_traits = DataTraits_Char;
		else if(Alembic::Abc::IUInt16ArrayProperty::matches(m_header))
			m_traits = DataTraits_UInt16;
		else if(Alembic::Abc::IInt16ArrayProperty::matches(m_header))
			m_traits = DataTraits_Int16;
		else if(Alembic::Abc::IUInt32ArrayProperty::matches(m_header))
			m_traits = DataTraits_UInt32;
		else if(Alembic::Abc::IInt32ArrayProperty::matches(m_header))
			m_traits = DataTraits_Int32;
		else if(Alembic::Abc::IUInt64ArrayProperty::matches(m_header))
			m_traits = DataTraits_UInt64;
		else if(Alembic::Abc::IInt64ArrayProperty::matches(m_header))
			m_traits = DataTraits_Int64;
		else if(Alembic::Abc::IHalfArrayProperty::matches(m_header))
			m_traits = DataTraits_Half;
		else if(Alembic::Abc::IFloatArrayProperty::matches(m_header))
			m_traits = DataTraits_Float;
		else if(Alembic::Abc::IDoubleArrayProperty::matches(m_header))
			m_traits = DataTraits_Double;
		else if(Alembic::Abc::IStringArrayProperty::matches(m_header))
			m_traits = DataTraits_String;
		else if(Alembic::Abc::IWstringArrayProperty::matches(m_header))
			m_traits = DataTraits_WString;
		else if(Alembic::Abc::IV2sArrayProperty::matches(m_header))
			m_traits = DataTraits_V2s;
		else if(Alembic::Abc::IV2iArrayProperty::matches(m_header))
			m_traits = DataTraits_V2i;
		else if(Alembic::Abc::IV2fArrayProperty::matches(m_header))
			m_traits = DataTraits_V2f;
		else if(Alembic::Abc::IV2dArrayProperty::matches(m_header))
			m_traits = DataTraits_V2d;

		else if(Alembic::Abc::IV3sArrayProperty::matches(m_header))
			m_traits = DataTraits_V3s;
		else if(Alembic::Abc::IV3iArrayProperty::matches(m_header))
			m_traits = DataTraits_V3i;
		else if(Alembic::Abc::IV3fArrayProperty::matches(m_header))
			m_traits = DataTraits_V3f;
		else if(Alembic::Abc::IV3dArrayProperty::matches(m_header))
			m_traits = DataTraits_V3d;

		else if(Alembic::Abc::IP2sArrayProperty::matches(m_header))
			m_traits = DataTraits_P2s;
		else if(Alembic::Abc::IP2iArrayProperty::matches(m_header))
			m_traits = DataTraits_P2i;
		else if(Alembic::Abc::IP2fArrayProperty::matches(m_header))
			m_traits = DataTraits_P2f;
		else if(Alembic::Abc::IP2dArrayProperty::matches(m_header))
			m_traits = DataTraits_P2d;

		else if(Alembic::Abc::IP3sArrayProperty::matches(m_header))
			m_traits = DataTraits_P3s;
		else if(Alembic::Abc::IP3iArrayProperty::matches(m_header))
			m_traits = DataTraits_P3i;
		else if(Alembic::Abc::IP3fArrayProperty::matches(m_header))
			m_traits = DataTraits_P3f;
		else if(Alembic::Abc::IP3dArrayProperty::matches(m_header))
			m_traits = DataTraits_P3d;

		else if(Alembic::Abc::IBox2sArrayProperty::matches(m_header))
			m_traits = DataTraits_Box2s;
		else if(Alembic::Abc::IBox2iArrayProperty::matches(m_header))
			m_traits = DataTraits_Box2i;
		else if(Alembic::Abc::IBox2fArrayProperty::matches(m_header))
			m_traits = DataTraits_Box2f;
		else if(Alembic::Abc::IBox2dArrayProperty::matches(m_header))
			m_traits = DataTraits_Box2d;

		else if(Alembic::Abc::IBox3sArrayProperty::matches(m_header))
			m_traits = DataTraits_Box3s;
		else if(Alembic::Abc::IBox3iArrayProperty::matches(m_header))
			m_traits = DataTraits_Box3i;
		else if(Alembic::Abc::IBox3fArrayProperty::matches(m_header))
			m_traits = DataTraits_Box3f;
		else if(Alembic::Abc::IBox3dArrayProperty::matches(m_header))
			m_traits = DataTraits_Box3d;

		else if(Alembic::Abc::IM33fArrayProperty::matches(m_header))
			m_traits = DataTraits_M33f;
		else if(Alembic::Abc::IM33dArrayProperty::matches(m_header))
			m_traits = DataTraits_M33d;
		else if(Alembic::Abc::IM44fArrayProperty::matches(m_header))
			m_traits = DataTraits_M44f;
		else if(Alembic::Abc::IM44dArrayProperty::matches(m_header))
			m_traits = DataTraits_M44d;

		else if(Alembic::Abc::IQuatfArrayProperty::matches(m_header))
			m_traits = DataTraits_Quatf;
		else if(Alembic::Abc::IQuatdArrayProperty::matches(m_header))
			m_traits = DataTraits_Quatd;

		else if(Alembic::Abc::IC3hArrayProperty::matches(m_header))
			m_traits = DataTraits_C3h;
		else if(Alembic::Abc::IC3fArrayProperty::matches(m_header))
			m_traits = DataTraits_C3f;
		else if(Alembic::Abc::IC3cArrayProperty::matches(m_header))
			m_traits = DataTraits_C3c;

		else if(Alembic::Abc::IC4hArrayProperty::matches(m_header))
			m_traits = DataTraits_C4h;
		else if(Alembic::Abc::IC4fArrayProperty::matches(m_header))
			m_traits = DataTraits_C4f;
		else if(Alembic::Abc::IC4cArrayProperty::matches(m_header))
			m_traits = DataTraits_C4c;

		else if(Alembic::Abc::IN2fArrayProperty::matches(m_header))
			m_traits = DataTraits_N2f;
		else if(Alembic::Abc::IN2dArrayProperty::matches(m_header))
			m_traits = DataTraits_N2d;

		else if(Alembic::Abc::IN3fArrayProperty::matches(m_header))
			m_traits = DataTraits_N3f;
		else if(Alembic::Abc::IN3dArrayProperty::matches(m_header))
			m_traits = DataTraits_N3d;
		else
			m_traits = DataTraits_Unknown;
	}
    else
		m_type = PropertyType_Compound;
}

void AlembicIProperty::Init(Alembic::Abc::ICompoundProperty& iParentProp)
{
	if(m_type == PropertyType_Scalar)
	{
		switch(m_traits)
		{
			case DataTraits_Bool:
			{
				m_scalar = Alembic::AbcGeom::IBoolProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_UChar:
			{
				m_scalar = Alembic::AbcGeom::IUcharProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_Char:
			{
				m_scalar = Alembic::AbcGeom::ICharProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_UInt16:
			{
				m_scalar = Alembic::AbcGeom::IUInt16Property(iParentProp,m_name,0);
				break;
			}
			case DataTraits_Int16:
			{
				m_scalar = Alembic::AbcGeom::IInt16Property(iParentProp,m_name,0);
				break;
			}
			case DataTraits_UInt32:
			{
				m_scalar = Alembic::AbcGeom::IUInt32Property(iParentProp,m_name,0);
				break;
			}
			case DataTraits_Int32:
			{
				m_scalar = Alembic::AbcGeom::IInt32Property(iParentProp,m_name,0);
				break;
			}
			case DataTraits_UInt64:
			{
				m_scalar = Alembic::AbcGeom::IUInt64Property(iParentProp,m_name,0);
				break;
			}
			case DataTraits_Int64:
			{
				m_scalar = Alembic::AbcGeom::IInt64Property(iParentProp,m_name,0);
				break;
			}
			case DataTraits_Half:
			{
				m_scalar = Alembic::AbcGeom::IHalfProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_Float:
			{
				m_scalar = Alembic::AbcGeom::IFloatProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_Double:
			{
				m_scalar = Alembic::AbcGeom::IDoubleProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_V2s:
			{
				m_scalar = Alembic::AbcGeom::IV2sProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_V2i:
			{
				m_scalar = Alembic::AbcGeom::IV2iProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_V2f:
			{
				m_scalar = Alembic::AbcGeom::IV2fProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_V2d:
			{
				m_scalar = Alembic::AbcGeom::IV2dProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_V3s:
			{
				m_scalar = Alembic::AbcGeom::IV3sProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_V3i:
			{
				m_scalar = Alembic::AbcGeom::IV3iProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_V3f:
			{
				m_scalar = Alembic::AbcGeom::IV3fProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_V3d:
			{
				m_scalar = Alembic::AbcGeom::IV3dProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_P3s:
			{
				m_scalar = Alembic::AbcGeom::IP3sProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_P3i:
			{
				m_scalar = Alembic::AbcGeom::IP3iProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_P3f:
			{
				m_scalar = Alembic::AbcGeom::IP3fProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_P3d:
			{
				m_scalar = Alembic::AbcGeom::IP3dProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_Box2s:
			{
				m_scalar = Alembic::AbcGeom::IBox2sProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_Box2i:
			{
				m_scalar = Alembic::AbcGeom::IBox2iProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_Box2f:
			{
				m_scalar = Alembic::AbcGeom::IBox2fProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_Box2d:
			{
				m_scalar = Alembic::AbcGeom::IBox2dProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_Box3s:
			{
				m_scalar = Alembic::AbcGeom::IBox3sProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_Box3i:
			{
				m_scalar = Alembic::AbcGeom::IBox3iProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_Box3f:
			{
				m_scalar = Alembic::AbcGeom::IBox3fProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_Box3d:
			{
				m_scalar = Alembic::AbcGeom::IBox3dProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_M33f:
			{
				m_scalar = Alembic::AbcGeom::IM33fProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_M33d:
			{
				m_scalar = Alembic::AbcGeom::IM33dProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_M44f:
			{
				m_scalar = Alembic::AbcGeom::IM44fProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_M44d:
			{
				m_scalar = Alembic::AbcGeom::IM44dProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_Quatf:
			{
				m_scalar = Alembic::AbcGeom::IQuatfProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_Quatd:
			{
				m_scalar = Alembic::AbcGeom::IQuatdProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_C3h:
			{
				m_scalar = Alembic::AbcGeom::IC3hProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_C3f:
			{
				m_scalar = Alembic::AbcGeom::IC3fProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_C3c:
			{
				m_scalar = Alembic::AbcGeom::IC3cProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_C4h:
			{
				m_scalar = Alembic::AbcGeom::IC4hProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_C4f:
			{
				m_scalar = Alembic::AbcGeom::IC4fProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_C4c:
			{
				m_scalar = Alembic::AbcGeom::IC4cProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_N2f:
			{
				m_scalar = Alembic::AbcGeom::IN2fProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_N2d:
			{
				m_scalar = Alembic::AbcGeom::IN2dProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_N3f:
			{
				m_scalar = Alembic::AbcGeom::IN3fProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_N3d:
			{
				m_scalar = Alembic::AbcGeom::IN3dProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_String:
			{
				m_scalar = Alembic::AbcGeom::IStringProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_WString:
			{
				m_scalar = Alembic::AbcGeom::IWstringProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_P2d:
			{
				m_scalar = Alembic::AbcGeom::IP2dProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_P2f:
			{
				m_scalar = Alembic::AbcGeom::IP2fProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_P2i:
			{
				m_scalar = Alembic::AbcGeom::IP2iProperty(iParentProp,m_name,0);
				break;
			}
			case DataTraits_P2s:
			{
				m_scalar = Alembic::AbcGeom::IP2sProperty(iParentProp,m_name,0);
				break;
			}
			default:
			{
				break;
			}
		}
	}
	else if(m_type == PropertyType_Array)
	{
		switch(m_traits)
		{
			case DataTraits_Bool:
			{
				m_array = Alembic::Abc::IBoolArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_UChar:
			{
				m_array = Alembic::Abc::IUcharArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_Char:
			{
				m_array = Alembic::Abc::ICharArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_UInt16:
			{
				m_array = Alembic::Abc::IUInt16ArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_Int16:
			{
				m_array = Alembic::Abc::IInt16ArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_UInt32:
			{
				m_array = Alembic::Abc::IUInt32ArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_Int32:
			{
				m_array = Alembic::Abc::IInt32ArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_UInt64:
			{
				m_array = Alembic::Abc::IUInt64ArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_Int64:
			{
				m_array = Alembic::Abc::IInt64ArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_Half:
			{
				m_array = Alembic::Abc::IHalfArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_Float:
			{
				m_array = Alembic::Abc::IFloatArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_Double:
			{
				m_array = Alembic::Abc::IDoubleArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_V2s:
			{
				m_array = Alembic::Abc::IV2sArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_V2i:
			{
				m_array = Alembic::Abc::IV2iArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_V2f:
			{
				m_array = Alembic::Abc::IV2fArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_V2d:
			{
				m_array = Alembic::Abc::IV2dArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_P2s:
			{
				m_array = Alembic::Abc::IP2sArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_P2i:
			{
				m_array = Alembic::Abc::IP2iArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_P2f:
			{
				m_array = Alembic::Abc::IP2fArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_P2d:
			{
				m_array = Alembic::Abc::IP2dArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_V3s:
			{
				m_array = Alembic::Abc::IV3sArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_V3i:
			{
				m_array = Alembic::Abc::IV3iArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_V3f:
			{
				m_array = Alembic::Abc::IV3fArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_V3d:
			{
				m_array = Alembic::Abc::IV3dArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_P3s:
			{
				m_array = Alembic::Abc::IP3sArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_P3i:
			{
				m_array = Alembic::Abc::IP3iArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_P3f:
			{
				m_array = Alembic::Abc::IP3fArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_P3d:
			{
				m_array = Alembic::Abc::IP3dArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_Box2s:
			{
				m_array = Alembic::Abc::IBox2sArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_Box2i:
			{
				m_array = Alembic::Abc::IBox2iArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_Box2f:
			{
				m_array = Alembic::Abc::IBox2fArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_Box2d:
			{
				m_array = Alembic::Abc::IBox2dArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_Box3s:
			{
				m_array = Alembic::Abc::IBox3sArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_Box3i:
			{
				m_array = Alembic::Abc::IBox3iArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_Box3f:
			{
				m_array = Alembic::Abc::IBox3fArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_Box3d:
			{
				m_array = Alembic::Abc::IBox3dArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_M33f:
			{
				m_array = Alembic::Abc::IM33fArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_M33d:
			{
				m_array = Alembic::Abc::IM33dArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_M44f:
			{
				m_array = Alembic::Abc::IM44fArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_M44d:
			{
				m_array = Alembic::Abc::IM44dArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_Quatf:
			{
				m_array = Alembic::Abc::IQuatfArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_Quatd:
			{
				m_array = Alembic::Abc::IQuatdArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_C3h:
			{
				m_array = Alembic::Abc::IC3hArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_C3f:
			{
				m_array = Alembic::Abc::IC3fArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_C3c:
			{
				m_array = Alembic::Abc::IC3cArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_C4h:
			{
				m_array = Alembic::Abc::IC4hArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_C4f:
			{
				m_array = Alembic::Abc::IC4fArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_C4c:
			{
				m_array = Alembic::Abc::IC4cArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_N2f:
			{
				m_array = Alembic::Abc::IN2fArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_N2d:
			{
				m_array = Alembic::Abc::IN2dArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_N3f:
			{
				m_array = Alembic::Abc::IN3fArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_N3d:
			{
				m_array = Alembic::Abc::IN3dArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_String:
			{
				m_array = Alembic::Abc::IStringArrayProperty(iParentProp,m_name);
				break;
			}
			case DataTraits_WString:
			{
				m_array = Alembic::Abc::IWstringArrayProperty(iParentProp,m_name);
				break;
			}
			default:
			{
				break;
			}
		}
	}
}

bool AlembicIProperty::GetSampleDescription(float time, ABC_Property_Sample_Infos* infos)
{
	infos->m_time = time;
	infos->m_traits = GetDataTraits();
	infos->m_type = GetPropertyType();
	infos->m_numItems = GetNbItems((Abc::chrono_t)time);
	return true;

}

void AlembicIProperty::GetSample(float time, ABC_Property_Sample_Infos* infos, ABC_Property_Sample* sample)
{	
	Alembic::AbcCoreAbstract::chrono_t t(time);
	Alembic::Abc::ISampleSelector selector(t,Alembic::Abc::ISampleSelector::kNearIndex);
	switch(m_type){
		case PropertyType_Scalar:
		{
			void* out;
			m_scalar.get(out,selector);
			memcpy(sample->m_datas, out, m_scalar.getDataType().getNumBytes());
			break;
		}
		case PropertyType_Array:
		{
			Alembic::AbcCoreAbstract::ArraySamplePtr out;
			m_array.get(out,selector);
			memcpy(sample->m_datas, (void*)out->getData(), infos->m_numItems * m_array.getDataType().getNumBytes());
			break;
		}
		case PropertyType_Compound:
		{
			//TO BE IMPLEMENTED
			break;
		}
		default:
		{
			//TO BE IMPLEMENTED
			break;
		}
	}
}

bool AlembicIProperty::IsConstant(){
	return false;
}

const char*	AlembicIProperty::GetName()const{
	return this->m_name.c_str();
}


ABCPropertyType	AlembicIProperty::GetPropertyType()const{
	return this->m_type;
}

ABCDataTraits AlembicIProperty::GetDataTraits()const{
	return this->m_traits;
}

Alembic::Abc::int64_t AlembicIProperty::GetNbItems(Alembic::Abc::chrono_t T)const{
	Alembic::Abc::ISampleSelector selector(T,Alembic::Abc::ISampleSelector::kNearIndex);

	switch(m_type){
		case PropertyType_Scalar:
		{
			return 1;
		}
		case PropertyType_Array:
		{
			if(!m_array)return -1;
			if(!m_array.valid())return 0;

            Alembic::AbcCoreAbstract::ArraySamplePtr sample;
			m_array.get(sample,selector);
            
			return sample->size();
		}
		case PropertyType_Compound:
		{
			//TO BE IMPLEMENTED
			return 0;
		}
		default:
		{
			//TO BE IMPLEMENTED
			return 0;
		}
	}
}

const char* AlembicIProperty::GetInterpretation() const{
	std::string out = m_header.getMetaData().get("interpretation");
	if(out=="")out = "simple";
	return out.c_str();
}

//------------------------------------------------------
// AlembicOProperty
//------------------------------------------------------
AlembicOProperty::AlembicOProperty(Alembic::Abc::OCompoundProperty& iParentProp, const ABCPropertyType type,const ABCDataTraits traits, const ABCGeometryScope scope, const std::string& name){
	m_name = name;
	m_type = type;
	m_traits = traits;
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

	switch(m_type){
		case PropertyType_Scalar:
		{
			switch(m_traits){
				case DataTraits_Bool:
				{
					m_scalar = Alembic::AbcGeom::OBoolProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_UChar:
				{
					m_scalar = Alembic::AbcGeom::OUcharProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Char:
				{
					m_scalar = Alembic::AbcGeom::OCharProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_UInt16:
				{
					m_scalar = Alembic::AbcGeom::OUInt16Property(iParentProp,name,0);
					break;
				}
				case DataTraits_Int16:
				{
					m_scalar = Alembic::AbcGeom::OInt16Property(iParentProp,name,0);
					break;
				}
				case DataTraits_UInt32:
				{
					m_scalar = Alembic::AbcGeom::OUInt32Property(iParentProp,name,0);
					break;
				}
				case DataTraits_Int32:
				{
					m_scalar = Alembic::AbcGeom::OInt32Property(iParentProp,name,0);
					break;
				}
				case DataTraits_UInt64:
				{
					m_scalar = Alembic::AbcGeom::OUInt64Property(iParentProp,name,0);
					break;
				}
				case DataTraits_Int64:
				{
					m_scalar = Alembic::AbcGeom::OInt64Property(iParentProp,name,0);
					break;
				}
				case DataTraits_Half:
				{
					m_scalar = Alembic::AbcGeom::OHalfProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Float:
				{
					m_scalar = Alembic::AbcGeom::OFloatProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Double:
				{
					m_scalar = Alembic::AbcGeom::ODoubleProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_V2s:
				{
					m_scalar = Alembic::AbcGeom::OV2sProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_V2i:
				{
					m_scalar = Alembic::AbcGeom::OV2iProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_V2f:
				{
					m_scalar = Alembic::AbcGeom::OV2fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_V2d:
				{
					m_scalar = Alembic::AbcGeom::OV2dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_P2s:
				{
					m_scalar = Alembic::AbcGeom::OP2sProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_P2i:
				{
					m_scalar = Alembic::AbcGeom::OP2iProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_P2f:
				{
					m_scalar = Alembic::AbcGeom::OP2fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_P2d:
				{
					m_scalar = Alembic::AbcGeom::OP2dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_V3s:
				{
					m_scalar = Alembic::AbcGeom::OV3sProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_V3i:
				{
					m_scalar = Alembic::AbcGeom::OV3iProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_V3f:
				{
					m_scalar = Alembic::AbcGeom::OV3fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_V3d:
				{
					m_scalar = Alembic::AbcGeom::OV3dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_P3s:
				{
					m_scalar = Alembic::AbcGeom::OP3sProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_P3i:
				{
					m_scalar = Alembic::AbcGeom::OP3iProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_P3f:
				{
					m_scalar = Alembic::AbcGeom::OP3fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_P3d:
				{
					m_scalar = Alembic::AbcGeom::OP3dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Box2s:
				{
					m_scalar = Alembic::AbcGeom::OBox2sProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Box2i:
				{
					m_scalar = Alembic::AbcGeom::OBox2iProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Box2f:
				{
					m_scalar = Alembic::AbcGeom::OBox2fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Box2d:
				{
					m_scalar = Alembic::AbcGeom::OBox2dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Box3s:
				{
					m_scalar = Alembic::AbcGeom::OBox3sProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Box3i:
				{
					m_scalar = Alembic::AbcGeom::OBox3iProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Box3f:
				{
					m_scalar = Alembic::AbcGeom::OBox3fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Box3d:
				{
					m_scalar = Alembic::AbcGeom::OBox3dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_M33f:
				{
					m_scalar = Alembic::AbcGeom::OM33fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_M33d:
				{
					m_scalar = Alembic::AbcGeom::OM33dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_M44f:
				{
					m_scalar = Alembic::AbcGeom::OM44fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_M44d:
				{
					m_scalar = Alembic::AbcGeom::OM44dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Quatf:
				{
					m_scalar = Alembic::AbcGeom::OQuatfProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_Quatd:
				{
					m_scalar = Alembic::AbcGeom::OQuatdProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_C3h:
				{
					m_scalar = Alembic::AbcGeom::OC3hProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_C3f:
				{
					m_scalar = Alembic::AbcGeom::OC3fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_C3c:
				{
					m_scalar = Alembic::AbcGeom::OC3cProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_C4h:
				{
					m_scalar = Alembic::AbcGeom::OC4hProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_C4f:
				{
					m_scalar = Alembic::AbcGeom::OC4fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_C4c:
				{
					m_scalar = Alembic::AbcGeom::OC4cProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_N2f:
				{
					m_scalar = Alembic::AbcGeom::ON2fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_N2d:
				{
					m_scalar = Alembic::AbcGeom::ON2dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_N3f:
				{
					m_scalar = Alembic::AbcGeom::ON3fProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_N3d:
				{
					m_scalar = Alembic::AbcGeom::ON3dProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_String:
				{
					m_scalar = Alembic::AbcGeom::OStringProperty(iParentProp,name,0);
					break;
				}
				case DataTraits_WString:
				{
					m_scalar = Alembic::AbcGeom::OWstringProperty(iParentProp,name,0);
					break;
				}
				default:
				{
					break;
				}
			}
			
			break;
		}
		case PropertyType_Array:
		{
			switch(m_traits){
				case DataTraits_Bool:
				{
					Alembic::AbcGeom::OBoolGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_UChar:
				{
					Alembic::AbcGeom::OUcharGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_Char:
				{
					Alembic::AbcGeom::OCharGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_UInt16:
				{
					Alembic::AbcGeom::OUInt16GeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_Int16:
				{
					Alembic::AbcGeom::OInt16GeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_UInt32:
				{
					Alembic::AbcGeom::OUInt32GeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_Int32:
				{
					Alembic::AbcGeom::OInt32GeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_UInt64:
				{
					Alembic::AbcGeom::OUInt64GeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_Int64:
				{
					Alembic::AbcGeom::OInt64GeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_Half:
				{
					Alembic::AbcGeom::OHalfGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_Float:
				{
					Alembic::AbcGeom::OFloatGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_Double:
				{
					Alembic::AbcGeom::ODoubleGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_V2s:
				{
					Alembic::AbcGeom::OV2sGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_V2i:
				{
					Alembic::AbcGeom::OV2iGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_V2f:
				{
					Alembic::AbcGeom::OV2fGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_V2d:
				{
					Alembic::AbcGeom::OV2dGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_P2s:
				{
					Alembic::AbcGeom::OP2sGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_P2i:
				{
					Alembic::AbcGeom::OP2iGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_P2f:
				{
					Alembic::AbcGeom::OP2fGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_P2d:
				{
					Alembic::AbcGeom::OP2dGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_V3s:
				{
					Alembic::AbcGeom::OV3sGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_V3i:
				{
					Alembic::AbcGeom::OV3iGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_V3f:
				{
					Alembic::AbcGeom::OV3fGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_V3d:
				{
					Alembic::AbcGeom::OV3dGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_P3s:
				{
					Alembic::AbcGeom::OP3sGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_P3i:
				{
					Alembic::AbcGeom::OP3iGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_P3f:
				{
					Alembic::AbcGeom::OP3fGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_P3d:
				{
					Alembic::AbcGeom::OP3dGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_Box2s:
				{
					Alembic::AbcGeom::OBox2sGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_Box2i:
				{
					Alembic::AbcGeom::OBox2iGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_Box2f:
				{
					Alembic::AbcGeom::OBox2fGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_Box2d:
				{
					Alembic::AbcGeom::OBox2dGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_Box3s:
				{
					Alembic::AbcGeom::OBox3sGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_Box3i:
				{
					Alembic::AbcGeom::OBox3iGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_Box3f:
				{
					Alembic::AbcGeom::OBox3fGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_Box3d:
				{
					Alembic::AbcGeom::OBox3dGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_M33f:
				{
					Alembic::AbcGeom::OM33fGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_M33d:
				{
					Alembic::AbcGeom::OM33dGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_M44f:
				{
					Alembic::AbcGeom::OM44fGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_M44d:
				{
					Alembic::AbcGeom::OM44dGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_Quatf:
				{
					Alembic::AbcGeom::OQuatfGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_Quatd:
				{
					Alembic::AbcGeom::OQuatdGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_C3h:
				{
					Alembic::AbcGeom::OC3hGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_C3f:
				{
					Alembic::AbcGeom::OC3fGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_C3c:
				{
					Alembic::AbcGeom::OC3cGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_C4h:
				{
					Alembic::AbcGeom::OC4hGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_C4f:
				{
					Alembic::AbcGeom::OC4fGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_C4c:
				{
					Alembic::AbcGeom::OC4cGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_N2f:
				{
					Alembic::AbcGeom::ON2fGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_N2d:
				{
					Alembic::AbcGeom::ON2dGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_N3f:
				{
					Alembic::AbcGeom::ON3fGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_N3d:
				{
					Alembic::AbcGeom::ON3dGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_String:
				{
					Alembic::AbcGeom::OStringGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				case DataTraits_WString:
				{
					Alembic::AbcGeom::OWstringGeomParam p(iParentProp,name, false, iScope, 1,0);
					m_array = p.getValueProperty();
					break;
				}
				default:
				{
					break;
				}
			}
		}
		case PropertyType_Compound:
		{
			// TO BE IMPLEMENTED
			break;
		}
		default:
		{
			break;
		}
	}
}

BOOZE_NAMESPACE_CLOSE_SCOPE