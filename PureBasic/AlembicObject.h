#ifndef _ALEMBIC_OBJECT_H_
#define _ALEMBIC_OBJECT_H_

#include "AlembicFoundation.h"
#include "AlembicArchive.h"
#include "AlembicProperty.h"

BOOZE_NAMESPACE_OPEN_SCOPE

class AlembicIArchive;
class AlembicIProperty;

class AlembicIObject
{
public:
	AlembicIObject(Abc::IObject& object):_object(object){};
	virtual bool Initialize();
	virtual const char* GetName();
	virtual const char* GetFullName();
	virtual ABCGeometricType GetType(){return _type;};
	virtual Abc::IObject& Get(){ return _object; };
	virtual void GetProperties();
	virtual bool HasProperty(const char* p_name);
	virtual uint64_t GetNumProperties(){return (uint64_t)_props.size();};
	virtual AlembicIProperty* GetProperty(uint64_t index);

protected:
	Abc::IObject _object;
	ABCGeometricType _type;
	std::vector<AlembicIProperty*> _props;
	void* _sample;
};

class AlembicOArchive;
class AlembicWriteJob;	

class AlembicOObject
{
protected:
	std::string _metadatastr;
	Alembic::Abc::OObject _object;
	Alembic::Abc::MetaData _metadata;
	std::vector<AlembicOProperty*> _props;
	void* _sample;

public:
	void Set(AbcG::OObject& obj);
	AbcG::OObject Get(){ return _object; };
	const char* GetMetaDataStr(){ return _metadatastr.c_str(); };
	Alembic::Abc::MetaData GetMetaData(){ return _metadata; };
	virtual ABCStatus Save(double time){ return Status_OK; };
};

BOOZE_EXPORT AlembicOObject* newOObject(AlembicOArchive* archive, uint64_t index);
BOOZE_EXPORT void deleteOObject(AlembicOObject* object);

BOOZE_EXPORT AlembicIObject* newIObject(AlembicIArchive* archive, uint64_t index);
BOOZE_EXPORT void deleteIObject(AlembicIObject* object);

BOOZE_NAMESPACE_CLOSE_SCOPE
#endif