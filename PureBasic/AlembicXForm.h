#ifndef _ALEMBIC_XFORM_H_
#define _ALEMBIC_XFORM_H_

#include "AlembicObject.h"

BOOZE_NAMESPACE_OPEN_SCOPE

struct XForm_Sample
{
	float pos[3];
	float ori[4];
	float scl[3];
};

//------------------------------------------------------------------------------------------------
// Alembic Import
//------------------------------------------------------------------------------------------------
class AlembicIXForm : public AlembicIObject
{
protected:
	Alembic::AbcGeom::IXform			m_xform;
	Alembic::AbcGeom::IXformSchema		m_xformschema;
	Alembic::AbcGeom::XformSample		m_xformsample;
	int									m_numSamples;

public:
	AlembicIXForm(Alembic::Abc::IObject& obj);
	bool								initialize();
};

//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
typedef AbcU::shared_ptr< AbcG::OXform> ABCOXformPtr;

class AlembicOXForm : public AlembicOObject
{
protected:
	
	Alembic::AbcGeom::OXformSchema		m_schema;
	Alembic::AbcGeom::XformSample		m_sample;
	int									m_numSamples;
	Imath::M44f							m_m;
	ABCOXformPtr						m_xform;

public:
	AlembicOXForm(AlembicOArchive* archive, AlembicOObject* parent, void* customData, const char* name);
	~AlembicOXForm(){};
	
	virtual	void						setTransform(Imath::M44f& xform){ m_m = xform; };
	virtual const Imath::M44f&			getTransform(){ return m_m; };
	virtual void						save(AbcA::TimeSamplingPtr time, AbcG::OObject& parent) override;	
};


BOOZE_NAMESPACE_CLOSE_SCOPE
#endif