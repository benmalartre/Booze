#include "AlembicTimeSampling.h"


BOOZE_NAMESPACE_OPEN_SCOPE

AlembicTimeSampling::AlembicTimeSampling(TimeSamplingType type, float timePerCycle, float startTime)
{
	AbcA::TimeSamplingPtr smp(new AbcA::TimeSampling(timePerCycle, startTime));
	m_sampling = smp;

	/*/
	switch (type){
		case TimeSamplingType::TIME_SAMPLING_UNIFORM:
			m_sampling = new AbcA::TimeSampling(timePerCycle, startTime);
			break;
		case TimeSamplingType::TIME_SAMPLING_CYCLIC:
			m_sampling = new AbcA::TimeSampling(samplingType, frames
			break;
	}
	*/
}

size_t AlembicTimeSampling::numStoredTimes(){
	return m_sampling->getNumStoredTimes();
}

double* AlembicTimeSampling::getStoredTimes(){
	return (double*)&m_sampling->getStoredTimes()[0];
}

//========================================================
// AlembicTimeSampling
//========================================================
// Constructor
//--------------------------------------------------------
AlembicTimeSampling* newTimeSampling(float frameRate, float startFrame)
{
	AlembicTimeSampling* ts = new AlembicTimeSampling(TIME_SAMPLING_UNIFORM, 1.0f / frameRate, startFrame);

	return ts;
}

// Destructor
//--------------------------------------------------------
void deleteTimeSampling(AlembicTimeSampling* ts)
{
	delete ts;
}

BOOZE_NAMESPACE_CLOSE_SCOPE