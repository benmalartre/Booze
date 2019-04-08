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

class AlembicOFaceSet : public AlembicOObject
{
protected:
	Alembic::AbcGeom::OFaceSetSchema m_faceSetSchema;
	Alembic::AbcGeom::OFaceSetSchema::Sample m_faceSetSample;
	size_t m_numSamples;
	size_t m_numFaces;
	size_t m_numFaceVertices;

public:
	AlembicOFaceSet( AlembicWriteJob * in_Job);
	virtual ABCStatus Save(double time, ABC_FaceSet_Sample_Infos* infos, ABC_FaceSet_Sample* sample);
};


class AlembicIFaceSet : public AlembicIObject
{
public:
	AlembicIFaceSet(AbcG::IObject& iObj);
	virtual bool Initialize();

protected:
	Alembic::AbcGeom::IFaceSetSchema m_faceSetSchema;
	Alembic::AbcGeom::IFaceSetSchema::Sample m_faceSetSample;
	size_t m_numSamples;
	size_t m_numFaces;
	size_t m_numFaceVertices;
};

BOOZE_NAMESPACE_CLOSE_SCOPE

#endif