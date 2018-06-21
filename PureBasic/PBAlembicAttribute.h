#ifndef _PBALEMBIC_ATTRIBUTE_H_
#define _PBALEMBIC_ATTRIBUTE_H_

#include "PBAlembicRegister.h"

struct ABC_Attribute_Sample_Infos
{
    uint64_t _nbitems;
    float _time;
	ABCPropertyType _type;
	ABCDataTraits _traits;
    char _name[32];
};

struct ABC_Attribute_Sample
{
	void* _datas;
};

#endif
