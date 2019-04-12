#ifndef ALEMBIC_UTIL_H
#define ALEMBIC_UTIL_H

#include <cmath>
#include <vector>
#include <set>
#include <string>

#include "AlembicFoundation.h"

BOOZE_NAMESPACE_OPEN_SCOPE

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

// assuming this is a valid input of /xxx/xxx/xxx/xxx.abc
bool stripFileName(const wchar_t* filePath, wchar_t* fileName);

double getWeightAndIndex(double iFrame,
    Alembic::AbcCoreAbstract::TimeSamplingPtr iTime, size_t numSamps,
    Alembic::AbcCoreAbstract::index_t & oIndex,
    Alembic::AbcCoreAbstract::index_t & oCeilIndex);

bool isColorSet(const Alembic::AbcCoreAbstract::PropertyHeader & iHeader,
    bool iUnmarkedFaceVaryingColors);

template<typename T>
void clamp(T & min, T & max, T & cur)
{
    if (min > max)
        std::swap(min, max);

    cur = std::max(cur, min);
    cur = std::min(cur, max);
}

template<typename T>
T simpleLerp(double alpha, T val1, T val2)
{
    double dv = static_cast<double>( val1 );
    return static_cast<T>( dv + alpha * (static_cast<double>(val2) - dv) );
}

template<typename T>
void vectorLerp(double alpha, std::vector<T> & vec,
    std::vector<T> & vec1, std::vector<T> & vec2)
{
    vec.clear();
    if (vec1.size() != vec2.size())
        return;
    unsigned int size = vec1.size();
    vec.reserve(size);
    for (unsigned int i = 0; i < size; i++)
    {
        T v1 = vec1[i];
        T val = v1 + alpha * (vec2[i] - vec1[i]);
        vec.push_back(val);
    }
}
std::string scopeToString(AbcG::GeometryScope input);

using Alembic::AbcCoreAbstract::chrono_t;
using Alembic::AbcCoreAbstract::index_t;

Alembic::Util::shared_ptr< Alembic::Abc::OObject >
makeXform(Alembic::Abc::OObject & parent, const char* name);


//-*****************************************************************************
Alembic::Util::shared_ptr< Alembic::Abc::OObject >
subdCube(Alembic::Abc::OObject & parent, const char* name);

//-*****************************************************************************
void OWrapExisting();



#define RADIANTODEGREE 57.295779513
#define DEGREETORADIAN 0.01745329252

BOOZE_NAMESPACE_CLOSE_SCOPE

#endif  // ABCIMPORT_UTIL_H_
