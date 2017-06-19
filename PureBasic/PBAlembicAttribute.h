#ifndef _PBALEMBIC_ATTRIBUTE_H_
#define _PBALEMBIC_ATTRIBUTE_H_

#include "PBAlembicRegister.h"

struct ABC_Attribute_Sample_Infos
{
	char _name[32];
	ABCPropertyType _type;
	ABCDataTraits _traits;
	uint64_t _nbitems;
	float _time;
};

struct ABC_Attribute_Sample
{
	void* _datas;
};

#endif