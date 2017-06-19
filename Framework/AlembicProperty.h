#ifndef _ALEMBIC_PROPERTY_H_
#define _ALEMBIC_PROPERTY_H_

#include "AlembicFoundation.h"
class AlembicOProperty
{
public:
	AlembicOProperty(){};
	AlembicOProperty(Alembic::Abc::OCompoundProperty& iParentProp, const ABCPropertyType type,const ABCDataTraits traits, const ABCGeometryScope scope, const std::string& name);
	~AlembicOProperty(){};
	std::string GetName()const{return _name;};
	ABCPropertyType GetType()const{return _type;};
	ABCDataTraits GetTraits()const{return _traits;};
	Alembic::Abc::OScalarProperty GetScalar(){return _scalar;};
	Alembic::Abc::OArrayProperty GetArray(){return _array;};
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
	AlembicIProperty(){};
	AlembicIProperty(Alembic::Abc::PropertyHeader& header);
	//AlembicIProperty(const ABCPropertyType type,const std::string& name);
	~AlembicIProperty(){};
	void Init(Alembic::Abc::ICompoundProperty& iParentProp);
	bool IsConstant();
	const char*	GetName() const;
	ABCPropertyType	GetPropertyType() const;
	ABCDataTraits GetDataTraits() const;
	Alembic::Abc::int64_t GetNbItems(Alembic::Abc::chrono_t T) const;
	std::string GetInterpretation() const;
	std::string GetMessage() const;
	void SetMessage(const std::string& msg);
	void* GetSample(float T);
	
private:
	Alembic::Abc::IScalarProperty _scalar;
	Alembic::Abc::IArrayProperty _array;
	Alembic::Abc::PropertyHeader _header;
	std::string _name;
	ABCPropertyType _type;
	ABCDataTraits _traits;
	std::string _msg;
	
};

#endif