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

std::string scopeToString(AbcG::GeometryScope input)
{
	switch (input)
	{
	case AbcG::kConstantScope: return "kConstantScope";
	case AbcG::kFacevaryingScope:return "kFacevaryingScope";
	case AbcG::kUniformScope: return "kUniformScope";
	case AbcG::kUnknownScope: return "kUnknownScope";
	case AbcG::kVaryingScope: return "kVaryingScope";
	case AbcG::kVertexScope: return "kVertexScope";
	}
	return "kUnknownScope";
}

using Alembic::AbcCoreAbstract::chrono_t;
using Alembic::AbcCoreAbstract::index_t;

//-*****************************************************************************
AbcU::shared_ptr< Alembic::Abc::OObject >
makeXform(Alembic::Abc::OObject & parent, const char* name)
{
	AbcU::shared_ptr< AbcG::OXform > xfoPtr
		(new AbcG::OXform(parent, name));

	// add a couple of ops
	AbcG::XformOp transop(AbcG::kTranslateOperation, AbcG::kTranslateHint);
	AbcG::XformOp scaleop(AbcG::kScaleOperation, AbcG::kScaleHint);

	AbcG::XformSample samp;
	samp.addOp(transop, AbcG::V3f(1.0, 2.0, 3.0));
	samp.addOp(scaleop, AbcG::V3f(2.0, 4.0, 6.0));

	AbcG::OXformSchema &schema = xfoPtr->getSchema();

	schema.set(samp);

	return xfoPtr;
}

//-*****************************************************************************
AbcU::shared_ptr< Alembic::Abc::OObject >
subdCube(Alembic::Abc::OObject & parent, const char* name)
{
	Alembic::Util::shared_ptr< AbcG::OPolyMesh > meshPtr
		(new AbcG::OPolyMesh(parent, name));

	AbcG::OPolyMeshSchema &schema = meshPtr->getSchema();

	std::vector<AbcG::V3f> verts(8);
	std::vector<Alembic::Util::int32_t> indices(8);

	AbcG::OPolyMeshSchema::Sample sample(verts, indices, indices);

	schema.set(sample);

	return meshPtr;
}

/*
//-*****************************************************************************
void OWrapExisting()
{
	Alembic::Abc::OArchive archive(
		Alembic::AbcCoreOgawa::WriteArchive(),
		"playground_owrap.abc"
		);

	Alembic::Abc::OObject archiveTop = archive.getTop();

	Alembic::Util::shared_ptr< Alembic::Abc::OObject > objAPtr =
		makeXform(archiveTop);

	Alembic::Util::shared_ptr< Alembic::Abc::OObject > objBPtr =
		subdCube(*objAPtr);

	//
	// NOW THE FUN BEGINS
	//
	if(Alembic::AbcGeom::OSubD::matches(objBPtr->getHeader()));
	{
		Alembic::Util::shared_ptr< Alembic::AbcGeom::OSubD > subdObjPtr =
			Alembic::Util::dynamic_pointer_cast< Alembic::AbcGeom::OSubD >
			(objBPtr);
		Alembic::AbcGeom::OSubD subdObj = *subdObjPtr;

		std::cout << "wrapped-existing subd has "
			<< subdObj.getSchema().getNumSamples() << " num samples."
			<< std::endl;


		std::vector<AbcG::V3f> verts(8, AbcG::V3f(2.0, 2.0, 2.0));
		Alembic::AbcGeom::OSubDSchema::Sample sample;
		sample.setPositions(Alembic::Abc::V3fArraySample(&(verts[0]),
			verts.size()));
		subdObj.getSchema().set(sample);
		TESTING_ASSERT(subdObj.getSchema().getNumSamples() == 2);
	}

	TESTING_ASSERT(Alembic::AbcGeom::OXform::matches(objAPtr->getHeader()));
	{
		AbcG::XformOp transop(AbcG::kTranslateOperation, AbcG::kTranslateHint);
		AbcG::XformOp scaleop(AbcG::kScaleOperation, AbcG::kScaleHint);

		AbcG::XformSample samp;
		samp.addOp(transop, AbcG::V3f(4.0, 5.0, 6.0));
		samp.addOp(scaleop, AbcG::V3f(8.0, 10.0, 12.0));

		Alembic::Util::shared_ptr< Alembic::AbcGeom::OXform > xformObjPtr =
			Alembic::Util::dynamic_pointer_cast< Alembic::AbcGeom::OXform >
			(objAPtr);
		Alembic::AbcGeom::OXform xformObj = *xformObjPtr;
		xformObj.getSchema().set(samp);
		TESTING_ASSERT(xformObj.getSchema().getNumSamples() == 2);
	}
}
*/
BOOZE_NAMESPACE_CLOSE_SCOPE