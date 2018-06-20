#ifndef _BOOZE_FOUNDATION_H_
#define _BOOZE_FOUNDATION_H_

#include <Alembic/Abc/All.h>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreFactory/All.h>
#include <Alembic/Util/All.h>

#include <ImathMath.h>
#include <ImathVec.h>
#include <ImathMatrix.h>
#include <ImathBox.h>
#include <ImathQuat.h>
#include <ImathColor.h>
#include <ImathFun.h>
#include <ImathBoxAlgo.h>

#include <iostream>
#include <algorithm>
#include <utility>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <iostream>
#include <exception>
#include <string>
#include <limits>
#include <memory>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include <stdexcept>
#include <float.h>

enum ABCStatus_
{
    Status_OK,
    Status_Fail,
    Status_Error,
    Status_Empty
};

enum ABCGeometricType_
{
	GeometricType_Unknown,
    GeometricType_XForm,
	GeometricType_Points,
	GeometricType_Curves,
    GeometricType_PolyMesh,
    GeometricType_SubD,
	GeometricType_FaceSet,
	GeometricType_NuPatch,
	GeometricType_Camera,
	GeometricType_Light
};

enum ABCPodType_
{
    PodType_Boolean,
    PodType_UInt8,
    PodType_Int8,
    PodType_UInt16,
    PodType_Int16,
    PodType_UInt32,
    PodType_Int32,
    PodType_UInt64,
    PodType_Int64,
    PodType_Float16,
    PodType_Float32,
    PodType_Float64,

    PodType_String,
    PodType_WString,
    PodType_NumPods,
    PodType_Unknown
};

enum ABCPropertyType_
{
  	PropertyType_Compound,      //; 0 < A compound property which may contain other properties */
  	PropertyType_Scalar,        //; 1 < A single value property */
  	PropertyType_Array,         //; 2 < A property With an Array of values */
  	PropertyType_Unknown = 127  //  < Unknown To the framework */
};

enum ABCGeometryScope_
{
    GeometryScope_Constant,
    GeometryScope_Uniform,
    GeometryScope_Varying,
    GeometryScope_Vertex,
    GeometryScope_FaceVarying,
    GeometryScope_Unknown = 127
};

enum ABCTopoVariance_
{
    TopoVariance_Constant,
    TopoVariance_Homogenous,
    TopoVariance_Heterogenous
};

enum ABCDataTraits_
{
	DataTraits_Bool = 0,
	DataTraits_UChar,
	DataTraits_Char,

	DataTraits_UInt16,
	DataTraits_Int16,

	DataTraits_UInt32,
	DataTraits_Int32,

	DataTraits_UInt64,
	DataTraits_Int64,

	DataTraits_Half,
	DataTraits_Float,
	DataTraits_Double,

	DataTraits_String,
	DataTraits_WString,

	DataTraits_V2s,
	DataTraits_V2i,
	DataTraits_V2f,
	DataTraits_V2d,

	DataTraits_V3s,
	DataTraits_V3i,
	DataTraits_V3f,
	DataTraits_V3d,

	DataTraits_P2s,
	DataTraits_P2i,
	DataTraits_P2f,
	DataTraits_P2d,

	DataTraits_P3s,
	DataTraits_P3i,
	DataTraits_P3f,
	DataTraits_P3d,

	DataTraits_Box2s,
	DataTraits_Box2i,
	DataTraits_Box2f,
	DataTraits_Box2d,

	DataTraits_Box3s,
	DataTraits_Box3i,
	DataTraits_Box3f,
	DataTraits_Box3d,

	DataTraits_M33f,
	DataTraits_M33d,
	DataTraits_M44f,
	DataTraits_M44d,

	DataTraits_Quatf,
	DataTraits_Quatd,

	DataTraits_C3h,
	DataTraits_C3f,
	DataTraits_C3c,

	DataTraits_C4h,
	DataTraits_C4f,
	DataTraits_C4c,

	DataTraits_N2f,
	DataTraits_N2d,

	DataTraits_N3f,
	DataTraits_N3d,

	DataTraits_V4f,

	DataTraits_Rotf,

	DataTraits_NumDataTypes,
	DataTraits_Unknown
};

enum ABCArchiveType_
{
	  ArchiveType_HDF5,			/*! Archive is an HDF5 archive */
	  ArchiveType_Ogawa,		/*! Archive is an Ogawa archive */
	  ArchiveType_Any = 127		/*! Don't know what archive type it is */
};

typedef enum ABCStatus_ ABCStatus;
typedef enum ABCGeometricType_ ABCGeometricType;
typedef enum ABCPodType_ ABCPodType;
typedef enum ABCPropertyType_ ABCPropertyType;
typedef enum ABCGeometryScope_ ABCGeometryScope;
typedef enum ABCTopoVariance_ ABCTopoVariance;
typedef enum ABCDataTraits_ ABCDataTraits;
typedef enum ABCArchiveType_ ABCArchiveType;

#endif  // _BOOZE_FOUNDATION_H_
