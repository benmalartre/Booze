#include "AlembicFoundation.h"
#include <ImathRandom.h>
using namespace Imath;

using AbcA::chrono_t;
using AbcA::index_t;

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


void RunAndWriteParticles
(
AbcG::OObject &iParent,
const ParticleSystem::Parameters &iParams,
size_t iNumFrames,
chrono_t iFps
);

void ReadParticles(const std::string &iFileName);