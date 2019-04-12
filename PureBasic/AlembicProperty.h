#ifndef _ALEMBIC_PROPERTY_H_
#define _ALEMBIC_PROPERTY_H_

#include "AlembicFoundation.h"
#include "AlembicAttribute.h"

BOOZE_NAMESPACE_OPEN_SCOPE

//------------------------------------------------------------------------------------------------
// Buffer Structures
//------------------------------------------------------------------------------------------------
struct ABC_Property_Sample_Infos
{
	uint64_t				m_numItems;
	float					m_time;
	ABCPropertyType			m_type;
	ABCDataTraits			m_traits;
};

struct ABC_Property_Sample
{
	void*					m_datas;
};

//------------------------------------------------------------------------------------------------
// Alembic Import
//------------------------------------------------------------------------------------------------
class AlembicIProperty
{
public:
	AlembicIProperty(Alembic::Abc::PropertyHeader& header);
	virtual void					init(Alembic::Abc::ICompoundProperty& iParentProp);
	virtual bool					isConstant();
	virtual const char*				getName() const;
	virtual ABCPropertyType			getPropertyType() const;
	virtual ABCDataTraits			getDataTraits() const;
	virtual Alembic::Abc::int64_t	getNbItems(Alembic::Abc::chrono_t T) const;
	virtual const char*				getInterpretation() const;
	virtual bool					getSampleDescription(float time, ABC_Property_Sample_Infos* infos);
	virtual void					getSample(float time, ABC_Property_Sample_Infos* infos, ABC_Property_Sample* sample);

private:
	Alembic::Abc::IScalarProperty	m_scalar;
	Alembic::Abc::IArrayProperty	m_array;
	Alembic::Abc::PropertyHeader	m_header;
	std::string						m_name;
	ABCPropertyType					m_type;
	ABCDataTraits					m_traits;
	std::string						m_msg;
};

//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
class AlembicOProperty
{
public:
	AlembicOProperty(AbcG::OCompoundProperty& prop, BOOZE_ATTRIBUTE* attribute, unsigned int Ts);
	AlembicOProperty(AbcG::OCompoundProperty& iParentProp, const ABCPropertyType type,const ABCDataTraits traits, const ABCGeometryScope scope, const std::string& name);
	virtual std::string						getName()const{ return m_name; };
	virtual ABCPropertyType					getType()const{ return m_type; };
	virtual ABCDataTraits					getTraits()const{ return m_traits; };
	virtual Alembic::Abc::OScalarProperty	getScalar(){ return m_scalar; };
	virtual Alembic::Abc::OArrayProperty	getArray(){ return m_array; };
	virtual void							save(BOOZE_ATTRIBUTE* datas);

private:
	Alembic::Abc::OScalarProperty			m_scalar;
	Alembic::Abc::OArrayProperty			m_array;
	
	std::string								m_name;
	ABCPropertyType							m_type;
	ABCDataTraits							m_traits;
	bool									m_isArray;

};

BOOZE_NAMESPACE_CLOSE_SCOPE

#endif