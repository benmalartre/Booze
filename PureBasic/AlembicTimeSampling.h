#pragma once
#include "AlembicFoundation.h"
#include <Alembic/AbcCoreAbstract/TimeSampling.h>
#include <Alembic/AbcCoreAbstract/TimeSamplingType.h>

BOOZE_NAMESPACE_OPEN_SCOPE

enum TimeSamplingTypeEnum{
	TIME_SAMPLING_UNIFORM,
	TIME_SAMPLING_CYCLIC,
	TIME_SAMPLING_ACYCLIC
};
typedef TimeSamplingTypeEnum TimeSamplingType;

//------------------------------------------------------------------------------------------------
// Alembic Time Sampling Interface
//------------------------------------------------------------------------------------------------
class AlembicTimeSampling 
{
public:
	AlembicTimeSampling(TimeSamplingType type, float timePerCycle, float startTime=0.0);
	virtual AbcA::TimeSampling				get(){ return* m_sampling; };
	virtual AbcA::TimeSamplingPtr			getPtr(){ return m_sampling; };
	virtual int32_t							numStoredTimes();
	virtual double*							getStoredTimes();


protected:
	AbcA::TimeSamplingPtr					m_sampling;
};



BOOZE_NAMESPACE_CLOSE_SCOPE
