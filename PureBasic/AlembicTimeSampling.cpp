#include "AlembicTimeSampling.h"


BOOZE_NAMESPACE_OPEN_SCOPE

AlembicTimeSampling::AlembicTimeSampling(TimeSamplingType type, float timePerCycle, float startTime)
{
	m_sampling = AbcA::TimeSamplingPtr(new AbcA::TimeSampling(timePerCycle, startTime));

	/*
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

int32_t AlembicTimeSampling::numStoredTimes(){
	return m_sampling->getNumStoredTimes();
}

double* AlembicTimeSampling::getStoredTimes(){
	return (double*)&m_sampling->getStoredTimes()[0];
}

BOOZE_NAMESPACE_CLOSE_SCOPE