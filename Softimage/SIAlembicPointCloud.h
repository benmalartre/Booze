#ifndef _SIALEMBIC_POINTCLOUD_H_
#define _SIALEMBIC_POINTCLOUD_H_

#include "AlembicFoundation.h"
#include "SIAlembicObject.h"
#include "SIAlembicAttribute.h"
#include "AlembicPointCloud.h"
#include "SIAlembicRegister.h"
#include "SIAlembicAttribute.h"

class SIAlembicOPointCloud : public SIAlembicOObject, public AlembicOPointCloud
{

public:
	//SIAlembicOPolymesh():SIAlembicOPolymesh(){};
	SIAlembicOPointCloud(const XSI::CRef & in_ref, SIAlembicWriteJob * in_job);
   ~SIAlembicOPointCloud();

   ABCStatus Save(double time);
private:
	std::vector<SIAlembicOAttribute*> attributes;
};

class SIAlembicIPointCloud : public SIAlembicIObject, public AlembicIPointCloud
{
public:
	SIAlembicIPointCloud():SIAlembicIObject(){};
   SIAlembicIPointCloud(const XSI::CRef & in_Ref);
   ~SIAlembicIPointCloud(){};

   	int GetNbPoints(){return _nbp;};
	void Check();
	//Alembic::AbcGeom::IPointsSchema::Sample GetSample(){return _cloudsample;};

   //CStatus Update(int frame);
   void Clean(){};
   CStatus Update(int frame);

};

#endif