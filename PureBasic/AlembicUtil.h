//-*****************************************************************************
//
// Copyright (c) 2009-2011,
//  Sony Pictures Imageworks, Inc. and
//  Industrial Light & Magic, a division of Lucasfilm Entertainment Company Ltd.
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Sony Pictures Imageworks, nor
// Industrial Light & Magic nor the names of their contributors may be used
// to endorse or promote products derived from this software without specific
// prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//-*****************************************************************************

#ifndef ABCIMPORT_UTIL_H_
#define ABCIMPORT_UTIL_H_

#include <cmath>
#include <vector>
#include <set>
#include <string>

#include "AlembicFoundation.h"

BOOZE_NAMESPACE_OPEN_SCOPE

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

#define RADIANTODEGREE 57.295779513
#define DEGREETORADIAN 0.01745329252

BOOZE_NAMESPACE_CLOSE_SCOPE

#endif  // ABCIMPORT_UTIL_H_
