#ifndef _PBALEMBIC_OBJECT_H_
#define _PBALEMBIC_OBJECT_H_

#include "AlembicFoundation.h"
#include "AlembicArchiveStorage.h"
#include "AlembicUtil.h"
#include "PBAlembicRegister.h"

class AlembicWriteJob;
/*
class PBAlembicOObject : public AlembicOObject
{
private:
   AlembicWriteJob * _job;

public:
   PBAlembicOObject(const void* PB_Object, AlembicWriteJob * in_Job);
   ~PBAlembicOObject();

   const void* GetRef();

   ABCStatus Save(double time){};
};


class PBAlembicIObject : public AlembicIObject
{
public:
	PBAlembicIObject(){};
	PBAlembicIObject(Alembic::Abc::IObject* obj){this->_abcobject = obj;};
   ~PBAlembicIObject();

   const void* GetRef();
};
*/
#include "AlembicWriteJob.h"

#endif