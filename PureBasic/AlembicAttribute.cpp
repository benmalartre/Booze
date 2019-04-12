#include "AlembicAttribute.h"

ABCDataTraitsEnum BOOZE_ATTR_TYPE(BOOZE_ATTRIBUTE* attr){
	if (attr->m_dataType == BOOZE_TYPE_BOOL)return DataTraits_Bool;
	else if (attr->m_dataType == BOOZE_TYPE_LONG)return DataTraits_Int32;
	else if (attr->m_dataType == BOOZE_TYPE_INTEGER)return DataTraits_Int64;
	else if (attr->m_dataType == BOOZE_TYPE_FLOAT)return DataTraits_Float;
	else if (attr->m_dataType == BOOZE_TYPE_VECTOR2)return DataTraits_V2f;
	else if (attr->m_dataType == BOOZE_TYPE_VECTOR3)return DataTraits_V3f;
	else if (attr->m_dataType == BOOZE_TYPE_VECTOR4)return DataTraits_C4f;
	else if (attr->m_dataType == BOOZE_TYPE_COLOR)return DataTraits_C4f;
	else if (attr->m_dataType == BOOZE_TYPE_QUATERNION)return DataTraits_Quatf;
	else if (attr->m_dataType == BOOZE_TYPE_ROTATION)return DataTraits_Quatf;
	else if (attr->m_dataType == BOOZE_TYPE_MATRIX3)return DataTraits_M33f;
	else if (attr->m_dataType == BOOZE_TYPE_MATRIX4)return DataTraits_M44f;
	else if (attr->m_dataType == BOOZE_TYPE_STRING)return DataTraits_String;
}

ABCPropertyTypeEnum BOOZE_ATTR_STRUCTURE(BOOZE_ATTRIBUTE* attr){
	if (attr->m_dataStructure == BOOZE_STRUCT_SINGLE)return PropertyType_Scalar;
	else if (attr->m_dataStructure == BOOZE_STRUCT_ARRAY)return PropertyType_Array;
	else if (attr->m_dataStructure == BOOZE_STRUCT_ANY)return PropertyType_Scalar;
}
