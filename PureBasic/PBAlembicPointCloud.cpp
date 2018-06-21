#include "PBAlembicXForm.h"
#include "PBAlembicObject.h"
#include "PBAlembicPointCloud.h"

using namespace std;

EXPORT bool ABC_ObjectIsPointCloud(AlembicIObject* obj)
{
	return AbcG::IPoints::matches(obj->GetIObject()->getMetaData());
	
};

EXPORT char* ABC_TestPointCloud(AlembicIObject* obj,ABC_PointCloud_Sample_Infos* io_sample)
{
	return 0;//ABC_PassStringToPureBasic(std::string("ABC Test Polymesh Succeeded!!"));
}

EXPORT void ABC_GetPointCloudSampleDescription(AlembicIObject* obj,float frame,ABC_PointCloud_Sample_Infos* infos)
{

	if(!obj->GetIObject()->valid()||!ABC_ObjectIsPointCloud(obj))return;
    
    AbcG::IPoints points(*obj->GetIObject(),Abc::kWrapExisting);
	if(!points.valid())return ;
    
	// Get Sample Index
	infos->_sampleindex = (AbcA::index_t)int(frame)-1;

   // Clamp if necessary
   if(infos->_sampleindex >= (AbcA::index_t)points.getSchema().getNumSamples())
      infos->_sampleindex = AbcA::index_t(points.getSchema().getNumSamples()) - 1;
	
	AbcG::IPointsSchema::Sample sample;
	
	points.getSchema().get(sample,infos->_sampleindex);
	
	infos->_nbpoints = sample.getPositions()->size();
	
}

EXPORT int ABC_UpdatePointCloudSample(AlembicIObject* obj,ABC_PointCloud_Sample_Infos* infos,ABC_PointCloud_Sample* io_sample)
{
	AbcG::IPoints points(*obj->GetIObject(),Abc::kWrapExisting);
	AbcG::ICompoundProperty argGeomParamsProp(points.getSchema().getArbGeomParams());
	AbcG::IPointsSchema::Sample sample;
	points.getSchema().get(sample,infos->_sampleindex);

	Abc::P3fArraySamplePtr pos = sample.getPositions();
	memcpy(io_sample->_position,&pos->get()[0],pos->size()*3*sizeof(float));

	if(infos->_hasorientation)
	{
		AbcG::ICompoundProperty argGeomParamsProp(points.getSchema().getArbGeomParams());
		AbcG::IC4fArrayProperty orientation(argGeomParamsProp,"Orientation");
		AbcG::C4fArraySamplePtr quat = orientation.getValue(0);
		
		memcpy(io_sample->_orientation,&quat->get()[0],quat->size()*sizeof(Imath::C4f));
	}
	if(infos->_hasscale)
	{
		
		AbcG::IV3fArrayProperty scale(argGeomParamsProp,"Scale");
		AbcG::V3fArraySamplePtr scl = scale.getValue(0);
		
		memcpy(io_sample->_scale,&scl->get()[0],scl->size()*sizeof(Imath::V3f));
	}
	
    return pos->size();
}
