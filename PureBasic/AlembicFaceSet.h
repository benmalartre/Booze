#ifndef _ALEMBIC_FACESET_H_
#define _ALEMBIC_FACESET_H_

#include "AlembicObject.h"

BOOZE_NAMESPACE_OPEN_SCOPE

struct ABC_FaceSet_Sample_Infos
{
	uint64_t m_numFaces;
	uint64_t m_numFaceVertices;
	uint64_t m_sampleIndex;
};

struct ABC_FaceSet_Sample
{
	float * m_position;
};

//------------------------------------------------------------------------------------------------
// Alembic Import
//------------------------------------------------------------------------------------------------
class AlembicIFaceSet : public AlembicIObject
{
public:
	AlembicIFaceSet(AbcG::IObject& iObj);
	virtual bool								initialize();

protected:
	Alembic::AbcGeom::IFaceSetSchema			m_faceSetSchema;
	Alembic::AbcGeom::IFaceSetSchema::Sample	m_faceSetSample;
	size_t										m_numSamples;
	size_t										m_numFaces;
	size_t										m_numFaceVertices;
};

//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
class AlembicOFaceSet : public AlembicOObject
{
protected:
	Alembic::AbcGeom::OFaceSet					m_faceset;
	Alembic::AbcGeom::OFaceSetSchema			m_schema;
	Alembic::AbcGeom::OFaceSetSchema::Sample	m_sample;
	size_t										m_numSamples;
	size_t										m_numFaces;
	size_t										m_numFaceVertices;

public:
	AlembicOFaceSet( AlembicWriteJob * in_Job);
	virtual ABCStatus							save(float time);
};

BOOZE_NAMESPACE_CLOSE_SCOPE

#endif