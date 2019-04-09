#ifndef _ALEMBIC_PROPERTY_H_
#define _ALEMBIC_PROPERTY_H_

#include "AlembicFoundation.h"

BOOZE_NAMESPACE_OPEN_SCOPE

//------------------------------------------------------------------------------------------------
// Buffer Structures
//------------------------------------------------------------------------------------------------
struct ABC_Property_Sample_Infos
{
	uint64_t m_numItems;
	float m_time;
	ABCPropertyType m_type;
	ABCDataTraits m_traits;
};

struct ABC_Property_Sample
{
	void* m_datas;
};

//------------------------------------------------------------------------------------------------
// Alembic Import
//------------------------------------------------------------------------------------------------
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
	Alembic::Abc::IScalarProperty m_scalar;
	Alembic::Abc::IArrayProperty m_array;
	Alembic::Abc::PropertyHeader m_header;
	std::string m_name;
	ABCPropertyType m_type;
	ABCDataTraits m_traits;
	std::string m_msg;
};

//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
class AlembicOProperty
{
public:
	AlembicOProperty(Alembic::Abc::OCompoundProperty& iParentProp, const ABCPropertyType type,const ABCDataTraits traits, const ABCGeometryScope scope, const std::string& name);
	virtual std::string GetName()const{ return m_name; };
	virtual ABCPropertyType GetType()const{ return m_type; };
	virtual ABCDataTraits GetTraits()const{ return m_traits; };
	virtual Alembic::Abc::OScalarProperty GetScalar(){ return m_scalar; };
	virtual Alembic::Abc::OArrayProperty GetArray(){ return m_array; };
private:
	Alembic::Abc::OScalarProperty m_scalar;
	Alembic::Abc::OArrayProperty m_array;
	
	std::string m_name;
	ABCPropertyType m_type;
	ABCDataTraits m_traits;

};

BOOZE_NAMESPACE_CLOSE_SCOPE

#endif