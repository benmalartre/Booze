//-*****************************************************************************
//
// Copyright (c) 2009-2012,
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

#include "AlembicUtil.h"

#include <algorithm>

bool stripFileName(const wchar_t* filePath, wchar_t* fileName)
{
	/*
    std::string str(filePath.asChar());
    size_t found;
    found = str.find_last_of("/\\");
    str = str.substr(found+1);

    // str is now in the form of xxx.abc
    found = str.find_first_of(".");
    str = str.substr(0, found);
    fileName = MString(str.c_str());
	*/
    return true;
}

double getWeightAndIndex(double iFrame,
    Alembic::AbcCoreAbstract::TimeSamplingPtr iTime, size_t numSamps,
    Alembic::AbcCoreAbstract::index_t & oIndex,
    Alembic::AbcCoreAbstract::index_t & oCeilIndex)
{
    if (numSamps == 0)
        numSamps = 1;

    std::pair<Alembic::AbcCoreAbstract::index_t, double> floorIndex =
        iTime->getFloorIndex(iFrame, numSamps);

    oIndex = floorIndex.first;
    oCeilIndex = oIndex;

    if (fabs(iFrame - floorIndex.second) < 0.0001)
        return 0.0;

    std::pair<Alembic::AbcCoreAbstract::index_t, double> ceilIndex =
        iTime->getCeilIndex(iFrame, numSamps);

    if (oIndex == ceilIndex.first)
        return 0.0;

    oCeilIndex = ceilIndex.first;

    double alpha = (iFrame - floorIndex.second) /
        (ceilIndex.second - floorIndex.second);

    // we so closely match the ceiling so we'll just use it
    if (fabs(1.0 - alpha) < 0.0001)
    {
        oIndex = oCeilIndex;
        return 0.0;
    }

    return alpha;
}

bool isColorSet(const Alembic::AbcCoreAbstract::PropertyHeader & iHeader,
    bool iUnmarkedFaceVaryingColors)
{
    return (Alembic::AbcGeom::IC3fGeomParam::matches(iHeader) ||
            Alembic::AbcGeom::IC4fGeomParam::matches(iHeader)) && 
            Alembic::AbcGeom::GetGeometryScope(iHeader.getMetaData()) ==
                Alembic::AbcGeom::kFacevaryingScope &&
            (iUnmarkedFaceVaryingColors ||
            iHeader.getMetaData().get("ColorAtVertices") != "");
}
