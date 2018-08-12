#ifndef _ALEMBIC_FACESET_H_
#define _ALEMBIC_FACESET_H_

#include "AlembicObject.h"

BOOZE_NAMESPACE_OPEN_SCOPE

struct ABC_FaceSet_Sample_Infos
{
	uint64_t _nbfaces;
	uint64_t _nbfacesvertices;
	uint64_t _sampleindex;
};

struct ABC_FaceSet_Sample
{
	float * _positions;
};

class AlembicOFaceSet : public AlembicOObject
{
protected:
	Alembic::AbcGeom::OFaceSetSchema __facesetchema;
	Alembic::AbcGeom::OFaceSetSchema::Sample _facesetample;
	size_t _numsamples;
	size_t _nbf;
	size_t _nbfv;

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
	Alembic::AbcGeom::IFaceSetSchema _facesetchema;
	Alembic::AbcGeom::IFaceSetSchema::Sample _facesetsample;
	size_t mNumSamples;
	size_t _nbf;
	size_t _nbfv;
};

BOOZE_NAMESPACE_CLOSE_SCOPE

#endif