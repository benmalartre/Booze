#include "AlembicUtil.h"

#include <algorithm>

BOOZE_NAMESPACE_OPEN_SCOPE

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

BOOZE_NAMESPACE_CLOSE_SCOPE