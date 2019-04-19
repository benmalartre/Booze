#include "Particles.h"
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
AbcG::OObject &iParent,
const ParticleSystem::Parameters &iParams,
size_t iNumFrames,
chrono_t iFps
)
{
	// Make the particle system.
	ParticleSystem parts(iParams);

	// Create the time sampling type.
	AbcG::TimeSampling ts(iFps, 0.0);
	Alembic::Util::uint32_t tsidx = iParent.getArchive().addTimeSampling(ts);

	// Create our object.
	AbcG::OPoints partsOut(iParent, "particleShape1", tsidx);
	std::cout << "Created Simple Particles" << std::endl;

	// Add attributes
	AbcG::OPointsSchema &pSchema = partsOut.getSchema();
	AbcG::MetaData mdata;
	AbcG::SetGeometryScope(mdata, AbcG::kVaryingScope);
	AbcG::OV3fArrayProperty velOut(pSchema, "velocity", mdata, tsidx);
	AbcG::OC3fArrayProperty rgbOut(pSchema, "Cs", tsidx);
	AbcG::OFloatArrayProperty ageOut(pSchema, "age", tsidx);

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

//-*****************************************************************************
void ReadParticles(const std::string &iFileName)
{
	AbcG::IArchive archive(Alembic::AbcCoreOgawa::ReadArchive(),
		iFileName);
	AbcG::IObject topObj(archive, AbcG::kTop);

	AbcG::IPoints points(topObj, "particleShape1");
	AbcG::IPointsSchema& pointsSchema = points.getSchema();

	index_t numSamps = pointsSchema.getNumSamples();
	std::cout << "\n\nReading points back in. Num frames: "
		<< numSamps << std::endl;

	AbcG::IV3fArrayProperty velProp(pointsSchema, "velocity");
	AbcG::IC3fArrayProperty rgbProp(pointsSchema, "Cs");
	AbcG::IFloatArrayProperty ageProp(pointsSchema, "age");

	for (index_t samp = 0; samp < numSamps; ++samp)
	{
		AbcG::IPointsSchema::Sample psamp;
		pointsSchema.get(psamp, samp);

		Box3f bounds;
		bounds.makeEmpty();
		size_t numPoints = psamp.getPositions()->size();
		for (size_t p = 0; p < numPoints; ++p)
		{
			bounds.extendBy((*(psamp.getPositions()))[p]);
		}
		std::cout << "Sample: " << samp << ", numPoints: " << numPoints
			<< ", bounds: " << bounds.min
			<< " to " << bounds.max << std::endl;
	}
}
