//#include "AlembicXForm.h"
#include "AlembicPoints.h"

BOOZE_NAMESPACE_OPEN_SCOPE

AlembicIPoints::AlembicIPoints(AbcG::IObject& object) :AlembicIObject(object)
{
	_type = GeometricType_Points;
}

bool AlembicIPoints::Initialize()
{
	AbcG::IPoints points(_object);
	GetProperties();
	return true;
}

BOOZE_EXPORT bool ABC_ObjectIsPointCloud(AlembicIObject* obj)
{
	return AbcG::IPoints::matches(obj->Get().getMetaData());
};

void AlembicIPoints::GetSampleDescription( float frame, ABC_Points_Sample_Infos* infos)
{

	if(!_object.valid()||!_type==GeometricType_Points)return;
    
    AbcG::IPoints points(_object,Abc::kWrapExisting);
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

size_t AlembicIPoints::UpdateSample(ABC_Points_Sample_Infos* infos, ABC_Points_Sample* io_sample)
{
	AbcG::IPoints points(_object,Abc::kWrapExisting);
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

BOOZE_NAMESPACE_CLOSE_SCOPE