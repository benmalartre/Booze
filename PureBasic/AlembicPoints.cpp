//#include "AlembicXForm.h"
#include "AlembicPoints.h"
#include "AlembicWriteJob.h"

BOOZE_NAMESPACE_OPEN_SCOPE

AlembicIPoints::AlembicIPoints(AbcG::IObject& object) :AlembicIObject(object)
{
	m_type = GeometricType_Points;
}

bool AlembicIPoints::initialize()
{
	AbcG::IPoints points(m_object);
	getProperties();
	return true;
}

BOOZE_EXPORT bool ABC_ObjectIsPointCloud(AlembicIObject* obj)
{
	return AbcG::IPoints::matches(obj->get().getMetaData());
};

void AlembicIPoints::getSampleDescription( float frame, ABC_Points_Sample_Infos* infos)
{

	if(!m_object.valid()||!m_type==GeometricType_Points)return;
    
    AbcG::IPoints points(m_object,Abc::kWrapExisting);
	if(!points.valid())return ;
    
	// Get Sample Index
	infos->m_sampleIndex = (AbcA::index_t)int(frame)-1;

   // Clamp if necessary
   if(infos->m_sampleIndex >= (AbcA::index_t)points.getSchema().getNumSamples())
      infos->m_sampleIndex = AbcA::index_t(points.getSchema().getNumSamples()) - 1;
	
	AbcG::IPointsSchema::Sample sample;
	
	points.getSchema().get(sample,infos->m_sampleIndex);
	
	infos->m_numPoints = sample.getPositions()->size();
	
}

size_t AlembicIPoints::updateSample(ABC_Points_Sample_Infos* infos, ABC_Points_Sample* io_sample)
{
	AbcG::IPoints points(m_object,Abc::kWrapExisting);
	AbcG::ICompoundProperty argGeomParamsProp(points.getSchema().getArbGeomParams());
	AbcG::IPointsSchema::Sample sample;
	points.getSchema().get(sample,infos->m_sampleIndex);

	Abc::P3fArraySamplePtr pos = sample.getPositions();
	memcpy(io_sample->m_position,&pos->get()[0],pos->size()*3*sizeof(float));

	if(infos->m_hasOrientation)
	{
		AbcG::ICompoundProperty argGeomParamsProp(points.getSchema().getArbGeomParams());
		AbcG::IC4fArrayProperty orientation(argGeomParamsProp,"Orientation");
		AbcG::C4fArraySamplePtr quat = orientation.getValue(0);
		
		memcpy(io_sample->m_orientation,&quat->get()[0],quat->size()*sizeof(Imath::C4f));
	}
	if(infos->m_hasScale)
	{
		
		AbcG::IV3fArrayProperty scale(argGeomParamsProp,"Scale");
		AbcG::V3fArraySamplePtr scl = scale.getValue(0);
		
		memcpy(io_sample->m_scale,&scl->get()[0],scl->size()*sizeof(Imath::V3f));
	}
	
    return pos->size();
}

//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
AlembicOPoints::AlembicOPoints(AlembicOArchive* archive, AlembicOObject* parent, void* customData, const char* name)
: AlembicOObject( archive, parent, customData, name, GeometricType_Points){

};

void AlembicOPoints::save(AbcA::TimeSamplingPtr timet)
{

}

BOOZE_NAMESPACE_CLOSE_SCOPE