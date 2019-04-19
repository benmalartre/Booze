//#include "AlembicXForm.h"
#include "AlembicFoundation.h"
#include "AlembicPoints.h"
#include "AlembicWriteJob.h"
#include <ImathRandom.h>
using AbcA::chrono_t;
using AbcA::index_t;

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


using namespace Imath;

//-*****************************************************************************
//-*****************************************************************************
//-*****************************************************************************
// PARTICLE SYSTEM
//-*****************************************************************************
//-*****************************************************************************
//-*****************************************************************************
class ParticleSystem
{
public:
	struct Parameters
	{
		V3f gravity;
		float elasticity;
		float lifespan;
		float emitRate;
		float emitRadius;

		float emitVelocitySpread;
		V3f emitVelocity;

		float emitColorSpread;
		C3f emitColor;
	};

	ParticleSystem(const Parameters &params);

	void advectExisting(chrono_t dt);

	void destroyOld(chrono_t dt);

	void emitNew(chrono_t dt);

	void timeStep(chrono_t dt)
	{
		advectExisting(dt);
		destroyOld(dt);
		emitNew(dt);
		m_currentTime += dt;
	}

	size_t numParticles() const { return m_id.size(); }
	const std::vector<Alembic::Util::uint64_t> &idVec() const { return m_id; }
	const std::vector<V3f> &positionVec() const { return m_position; }
	const std::vector<C3f> &colorVec() const { return m_color; }
	const std::vector<V3f> &velocityVec() const { return m_velocity; }
	const std::vector<Alembic::Util::float32_t> &ageVec() const
	{
		return m_age;
	}

protected:
	Parameters m_params;

	std::vector<Alembic::Util::uint64_t> m_id;
	std::vector<V3f> m_position;
	std::vector<C3f> m_color;
	std::vector<V3f> m_velocity;
	std::vector<Alembic::Util::float32_t> m_age;

	Alembic::Util::uint64_t m_nextId;
	double m_emitDither;
	chrono_t m_currentTime;
};

//-*****************************************************************************
ParticleSystem::ParticleSystem(const Parameters &params)
: m_params(params)
, m_nextId(1)
, m_emitDither(0.0)
, m_currentTime(0.0)
{
	// Nothing
}

//-*****************************************************************************
void ParticleSystem::advectExisting(chrono_t dt)
{
	size_t numParticles = m_id.size();
	for (size_t part = 0; part < numParticles; ++part)
	{
		V3f &velF = m_velocity[part];
		V3f velD(velF.x, velF.y, velF.z);
		velD += m_params.gravity * dt;

		V3f &pos = m_position[part];
		pos += velD * dt;

		if (pos.y < 0.0)
		{
			pos.y = -pos.y;
			if (velD.y < 0.0)
			{
				velD.y = -velD.y;
				velD *= (double)m_params.elasticity;
			}
		}

		velF.setValue(velD);

		m_age[part] += (float)dt;
	}
}

//-*****************************************************************************
void ParticleSystem::destroyOld(chrono_t dt)
{
	// Delete everybody whose age is greater than lifespan.
	// We delete by simply swapping out with the last position.
	size_t numParticles = m_id.size();
	size_t lastParticle = numParticles - 1;
	for (size_t part = 0; part < numParticles; ++part)
	{
		if (m_age[part] >= m_params.lifespan)
		{
			std::swap(m_id[part], m_id[lastParticle]);
			std::swap(m_position[part], m_position[lastParticle]);
			std::swap(m_color[part], m_color[lastParticle]);
			std::swap(m_velocity[part], m_velocity[lastParticle]);
			std::swap(m_age[part], m_age[lastParticle]);
			--numParticles;
			--lastParticle;
		}
	}

	m_id.resize(numParticles);
	m_position.resize(numParticles);
	m_color.resize(numParticles);
	m_velocity.resize(numParticles);
	m_age.resize(numParticles);
}

//-*****************************************************************************
void ParticleSystem::emitNew(chrono_t dt)
{
	Imath::Rand48 rand48;

	double numNewPartsD = m_emitDither + (dt * m_params.emitRate);
	size_t numNewParts = (size_t)floor(numNewPartsD);
	m_emitDither = numNewPartsD - (double)numNewParts;

	for (size_t newPart = 0; newPart < numNewParts; ++newPart)
	{
		Alembic::Util::uint64_t newId = m_nextId;
		++m_nextId;

		rand48.init((Alembic::Util::uint64_t)newId);

		// Position
		V3f emitPos = Imath::gaussSphereRand<V3f>(rand48)
			* (double)m_params.emitRadius;

		// Velocity
		V3f emitVel = Imath::gaussSphereRand<V3f>(rand48)
			* m_params.emitVelocitySpread;
		emitVel += m_params.emitVelocity;

		// Age.
		float emitAge = dt * rand48.nextf();

		// Color.
		C3f emitColor = Imath::gaussSphereRand<C3f>(rand48)
			* m_params.emitColorSpread;
		emitColor += m_params.emitColor;
		emitColor.x = std::max(emitColor.x, 0.0f);
		emitColor.y = std::max(emitColor.y, 0.0f);
		emitColor.z = std::max(emitColor.z, 0.0f);

		// Do it.
		m_id.push_back(newId);
		m_position.push_back(emitPos);
		m_velocity.push_back(emitVel);
		m_color.push_back(emitColor);
		m_age.push_back(emitAge);
	}
}

//-*****************************************************************************
//-*****************************************************************************
//-*****************************************************************************
// PARTICLES WRITER
//-*****************************************************************************
//-*****************************************************************************
//-*****************************************************************************
void RunAndWriteParticles
(
ABCOPointsPtr &iPoints,
const ParticleSystem::Parameters &iParams,
size_t iNumFrames,
chrono_t iFps, 
AbcA::TimeSamplingPtr iTs
)
{
	// Make the particle system.
	ParticleSystem parts(iParams);

	// Add attributes
	AbcG::OPointsSchema &pSchema = iPoints->getSchema();
	AbcG::MetaData mdata;
	AbcG::SetGeometryScope(mdata, AbcG::kVaryingScope);
	AbcG::OV3fArrayProperty velOut(pSchema, "velocity", mdata, iTs);
	AbcG::OC3fArrayProperty rgbOut(pSchema, "Cs", iTs);
	AbcG::OFloatArrayProperty ageOut(pSchema, "age", iTs);

	// Get seconds per frame.
	chrono_t iSpf = 1.0 / iFps;

	// CJH: Until we fix zero-array-property bug, loop a few frames.
	for (int preRoll = 0; preRoll < 100; ++preRoll)
	{
		parts.timeStep(iSpf);
		if (parts.numParticles() > 0)
		{
			break;
		}
	}

	ABCA_ASSERT(parts.numParticles() > 0,
		"Degenerate particle system");

	// Loop over the frames.
	for (index_t sampIndex = 0;
		sampIndex < (index_t)iNumFrames; ++sampIndex)
	{
		// First, write the sample.
		AbcG::OPointsSchema::Sample psamp(
			AbcG::V3fArraySample(parts.positionVec()),
			AbcG::UInt64ArraySample(parts.idVec()));
		pSchema.set(psamp);
		velOut.set(AbcG::V3fArraySample(parts.velocityVec()));
		rgbOut.set(AbcG::C3fArraySample(parts.colorVec()));
		ageOut.set(AbcG::FloatArraySample(parts.ageVec()));

		// Now time step.
		parts.timeStep(iSpf);

		// Print!
		std::cout << "Wrote " << parts.numParticles()
			<< " particles to frame: " << sampIndex << std::endl;
	}

	// End it.
	std::cout << "Finished Sim, About to finish writing" << std::endl;
}



//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
AlembicOPoints::AlembicOPoints(AlembicOArchive* archive, AlembicOObject* parent, void* customData, const char* name)
: AlembicOObject( archive, parent, customData, name, GeometricType_Points){
	ABCOPointsPtr pnts(new AbcG::OPoints(parent->get(), name, getJob()->getTimeSampling()->getPtr()));
	m_points = pnts;

};

void AlembicOPoints::set(AbcG::V3f* positions, int32_t* ids, int32_t numPoints)
{

	// positions
	m_positions.resize(numPoints);
	memcpy(&m_positions[0][0], &positions[0][0], numPoints*sizeof(AbcG::V3f));

	// indices
	m_indices.resize(numPoints);
	memcpy(&m_indices[0], &ids[0], numPoints*sizeof(int32_t));

	// sample
	m_sample.m_position = &m_positions[0][0];
	m_sample.m_id		= &m_indices[0];

	// infos
	m_infos.m_numPoints = numPoints;
	m_infos.m_hasVelocity = false;
	m_infos.m_hasColor = false;
	m_infos.m_hasSize = false;
	m_infos.m_hasOrientation = false;
	m_infos.m_hasScale = false;

}

void AlembicOPoints::setPositions(Imath::V3f* positions, int32_t numPoints)
{
	if (numPoints != m_positions.size())m_positions.resize(numPoints);
	memcpy(&m_positions[0][0], &positions[0][0], numPoints*sizeof(Imath::V3f));

	m_sample.m_position = &m_positions[0][0];
	m_infos.m_numPoints = numPoints;

}

void AlembicOPoints::save(AbcA::TimeSamplingPtr time)
{
	/*
	ABCOPointsPtr pntPtr = AbcU::dynamic_pointer_cast< AbcG::OPoints >(getPtr());	
	ParticleSystem::Parameters params;
	params.gravity = V3f(0.0, -9.81, 0.0);
	params.elasticity = 0.75f;
	params.lifespan = 4.5f;
	params.emitRate = 600.0f;
	params.emitRadius = 0.5f;
	params.emitVelocitySpread = 1.5f;
	params.emitVelocity = V3f(1.0, 10.0, 2.35);
	params.emitColorSpread = 0.25f;
	params.emitColor = C3f(0.85f, 0.9f, 0.1f);

	RunAndWriteParticles(pntPtr, params, 120, 24, time);
	*/
	ABCOPointsPtr pntPtr = AbcU::dynamic_pointer_cast< AbcG::OPoints >(getPtr());
	AbcG::OPointsSchema& schema = pntPtr->getSchema();
	
	AbcG::OPointsSchema::Sample sample(
		AbcG::P3fArraySample((Imath::V3f*)m_sample.m_position, m_infos.m_numPoints),
		AbcG::UInt64ArraySample(m_sample.m_id, m_infos.m_numPoints));
	schema.set(sample);
	
	// not actually the right data; just making it up
	AbcG::Box3d cbox;
	cbox.extendBy(AbcG::V3d(1.0, -1.0, 0.0));
	cbox.extendBy(AbcG::V3d(-1.0, 1.0, 3.0));
	
	for (int i = 0; i < m_children.size(); ++i)m_children[i]->save(time);

}

BOOZE_NAMESPACE_CLOSE_SCOPE