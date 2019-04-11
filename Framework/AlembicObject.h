#ifndef _ALEMBIC_OBJECT_H_
#define _ALEMBIC_OBJECT_H_

#include "AlembicFoundation.h"
#include "AlembicProperty.h"

class AlembicWriteJob;
class AlembicOObject
{
protected:
	std::string _metadatastr;
	Alembic::Abc::OObject* _abcobject;
	Alembic::Abc::MetaData _metadata;
	
public:
	std::vector<AlembicOProperty> _props;
	AlembicOObject(AlembicWriteJob* job, void* customData, const char* name, ABCGeometricType type);
	~AlembicOObject(){};

	void SetOObject(Alembic::Abc::OObject* obj);
	Alembic::Abc::OObject* GetOObject(){return _abcobject;};
	std::string GetMetaDataStr(){return _metadatastr;};
	Alembic::Abc::MetaData GetMetaData(){return _metadata;};
	virtual ABCStatus Save(float time, AbcG::OObject& parent){return Status_OK;};
};

class AlembicIObject
{
protected:
	
	std::string _metadatastr;
	Alembic::Abc::MetaData _metadata;
	bool _init;
	bool _pathdirty;
	bool _identifierdirty;
	ABCGeometricType _type;
	Alembic::Abc::IObject* _abcobject;

public:
	std::vector<AlembicIProperty> _props;
	AlembicIObject(){};
	AlembicIObject(Alembic::Abc::IObject* obj);
	~AlembicIObject(){};

	void Init(ABCGeometricType type);
	std::string GetMetaDataString(){return _metadatastr;};
	void SetIObject(Alembic::Abc::IObject* obj);
	Alembic::Abc::IObject* GetIObject(){return _abcobject;};
	bool Initialized(){return _init;};
	void GetProperties();
	uint64_t GetNumProperties();
	AlembicIProperty* GetProperty(uint64_t id);
	
	//virtual bool HasProperty(std::string);
	//virtual void Init(){};
};
typedef std::shared_ptr < AlembicIObject > AlembicIObjectPtr;
typedef std::shared_ptr < AlembicOObject> AlembicOObjectPtr;

#include "AlembicWriteJob.h"

#endif