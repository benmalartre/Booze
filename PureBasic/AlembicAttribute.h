#ifndef ALEMBIC_ATTRIBUTE_H
#define ALEMBIC_ATTRIBUTE_H

#include "AlembicFoundation.h"

// attribute structure
struct BOOZE_ATTRIBUTE{
	char*		m_name;
	int64_t		m_dataType;
	int64_t 	m_dataStructure;
	int64_t		m_dataContext;
	void*		m_datas;
	bool		m_atomic;
	bool		m_isArray;
	bool		m_isConstant;
	bool		m_readOnly;
	bool		m_writable;
	bool		m_dirty;
};

// array structure
struct BOOZE_ARRAY{
	int64_t		m_type;
	int64_t		m_itemSize;
	int64_t		m_itemCount;
	void*		m_datas;
};

// attribute type
#define BOOZE_TYPE_UNDEFINED		0
#define BOOZE_TYPE_NEW				1<<0
#define BOOZE_TYPE_BOOL 			1<<1
#define BOOZE_TYPE_LONG 			1<<2
#define BOOZE_TYPE_INTEGER 			1<<3
#define BOOZE_TYPE_ENUM 			1<<4
#define BOOZE_TYPE_FLOAT 			1<<5
#define BOOZE_TYPE_VECTOR2 			1<<6
#define BOOZE_TYPE_VECTOR3 			1<<7
#define BOOZE_TYPE_VECTOR4			1<<8
#define BOOZE_TYPE_COLOR 			1<<9
#define BOOZE_TYPE_ROTATION 		1<<10
#define BOOZE_TYPE_QUATERNION 		1<<11
#define BOOZE_TYPE_MATRIX3 			1<<12
#define BOOZE_TYPE_MATRIX4 			1<<13
#define BOOZE_TYPE_STRING			1<<14

// attribute structure
#define BOOZE_STRUCT_SINGLE			0
#define BOOZE_STRUCT_ARRAY			1
#define BOOZE_STRUCT_ANY			2

// attribute context
#define BOOZE_CTXT_SINGLETON		0
#define BOOZE_CTXT_COMPONENT0D		1
#define BOOZE_CTXT_COMPONENT1D		2
#define BOOZE_CTXT_COMPONENT2D		3
#define BOOZE_CTXT_COMPONENT0D2D	4
#define BOOZE_CTXT_GENERATOR		5
#define BOOZE_CTXT_ANY				6

// declarations
ABCDataTraitsEnum	BOOZE_ATTR_TYPE(BOOZE_ATTRIBUTE* attr);
ABCPropertyTypeEnum BOOZE_ATTR_STRUCTURE(BOOZE_ATTRIBUTE* attr);

#endif