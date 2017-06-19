#ifndef _SIALEMBIC_CURVE_H_
#define _SIALEMBIC_CURVE_H_

#include "AlembicFoundation.h"
#include "AlembicCurve.h"
#include "SIAlembicRegister.h"
#include "SIAlembicObject.h"

class SIAlembicOCurves : public SIAlembicOObject, public AlembicOCurves
{

public:
	//SIAlembicOPolymesh():SIAlembicOPolymesh(){};
	SIAlembicOCurves(const XSI::CRef & in_ref, SIAlembicWriteJob * in_job);
   ~SIAlembicOCurves(){};

   ABCStatus Save(double time);
};

class SIAlembicICurves : public SIAlembicIObject, public AlembicICurves
{
public:
	SIAlembicICurves():SIAlembicIObject(){};
	SIAlembicICurves(const XSI::CRef & in_Ref);
   ~SIAlembicICurves(){};

   	//int GetNbPoints(){return _nbp;};
	void Check();
	//Alembic::AbcGeom::IPointsSchema::Sample GetSample(){return _cloudsample;};

   //CStatus Update(int frame);
   void Clean(){};
   CStatus Update(int frame);

};

#endif