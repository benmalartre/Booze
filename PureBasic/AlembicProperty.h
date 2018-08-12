#ifndef _ALEMBIC_PROPERTY_H_
#define _ALEMBIC_PROPERTY_H_

#include "AlembicFoundation.h"

BOOZE_NAMESPACE_OPEN_SCOPE

struct ABC_Property_Sample_Infos
{
	uint64_t _nbitems;
	float _time;
	ABCPropertyType _type;
	ABCDataTraits _traits;
};

struct ABC_Property_Sample
{
	void* _datas;
};

class AlembicOProperty
{
public:
	AlembicOProperty(Alembic::Abc::OCompoundProperty& iParentProp, const ABCPropertyType type,const ABCDataTraits traits, const ABCGeometryScope scope, const std::string& name);
	virtual std::string GetName()const{ return _name; };
	virtual ABCPropertyType GetType()const{ return _type; };
	virtual ABCDataTraits GetTraits()const{ return _traits; };
	virtual Alembic::Abc::OScalarProperty GetScalar(){ return _scalar; };
	virtual Alembic::Abc::OArrayProperty GetArray(){ return _array; };
private:
	Alembic::Abc::OScalarProperty _scalar;
	Alembic::Abc::OArrayProperty _array;
	
	std::string _name;
	ABCPropertyType _type;
	ABCDataTraits _traits;

};

class AlembicIProperty
{
public:
	AlembicIProperty(Alembic::Abc::PropertyHeader& header);
	virtual void Init(Alembic::Abc::ICompoundProperty& iParentProp);
	virtual bool IsConstant();
	virtual const char*	GetName() const;
	virtual ABCPropertyType	GetPropertyType() const;
	virtual ABCDataTraits GetDataTraits() const;
	virtual Alembic::Abc::int64_t GetNbItems(Alembic::Abc::chrono_t T) const;
	virtual const char* GetInterpretation() const;
	virtual bool GetSampleDescription(float time, ABC_Property_Sample_Infos* infos);
	virtual void GetSample(float time, ABC_Property_Sample_Infos* infos, ABC_Property_Sample* sample);
	
private:
	Alembic::Abc::IScalarProperty _scalar;
	Alembic::Abc::IArrayProperty _array;
	Alembic::Abc::PropertyHeader _header;
	std::string _name;
	ABCPropertyType _type;
	ABCDataTraits _traits;
	std::string _msg;
};

BOOZE_NAMESPACE_CLOSE_SCOPE

#endif