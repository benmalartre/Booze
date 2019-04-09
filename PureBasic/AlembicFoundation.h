#ifndef _BOOZE_FOUNDATION_H_
#define _BOOZE_FOUNDATION_H_
#ifndef _WIN32
#include <stdint.h>
#else
#include "../stdint.h"
#endif
#include <Alembic/Abc/All.h>
#include <Alembic/AbcCoreAbstract/All.h>
#ifdef ALEMBIC_WITH_HDF5
#include <Alembic/AbcCoreHDF5/All.h>
#endif
#include <Alembic/AbcCoreOgawa/All.h>
#include <Alembic/AbcCoreFactory/All.h>
#include <Alembic/AbcCollection/All.h>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcMaterial/All.h>

#include "AlembicExport.h"

namespace Abc = Alembic::Abc;;
namespace AbcA = Alembic::AbcCoreAbstract;
namespace AbcF = Alembic::AbcCoreFactory;
namespace AbcG = Alembic::AbcGeom;

using namespace std;
extern bool ABC_HDF5_SUPPORT;

enum ABCStatusEnum
{
    Status_OK,
    Status_Fail,
    Status_Error,
    Status_Empty
};

enum ABCGeometricTypeEnum
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

enum ABCPodTypeEnum
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

enum ABCPropertyTypeEnum
{
  	PropertyType_Compound,      //; 0 < A compound property which may contain other properties */
  	PropertyType_Scalar,        //; 1 < A single value property */
  	PropertyType_Array,         //; 2 < A property With an Array of values */
  	PropertyType_Unknown = 127  //  < Unknown To the framework */
};

enum ABCGeometryScopeEnum
{
    GeometryScope_Constant,
    GeometryScope_Uniform,
    GeometryScope_Varying,
    GeometryScope_Vertex,
    GeometryScope_FaceVarying,
    GeometryScope_Unknown = 127
};

enum ABCTopoVarianceEnum
{
    TopoVariance_Constant,
    TopoVariance_Homogenous,
    TopoVariance_Heterogenous
};

enum ABCDataTraitsEnum
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

	DataTraits_Unknown
};

enum ABCArchiveTypeEnum
{
	  ArchiveType_HDF5,			/*! Archive is an HDF5 archive */
	  ArchiveType_Ogawa,		/*! Archive is an Ogawa archive */
	  ArchiveType_Unknown = 127	/*! Don't know what archive type it is */
};

typedef enum ABCStatusEnum ABCStatus;
typedef enum ABCGeometricTypeEnum ABCGeometricType;
typedef enum ABCPodTypeEnum ABCPodType;
typedef enum ABCPropertyTypeEnum ABCPropertyType;
typedef enum ABCGeometryScopeEnum ABCGeometryScope;
typedef enum ABCTopoVarianceEnum ABCTopoVariance;
typedef enum ABCDataTraitsEnum ABCDataTraits;
typedef enum ABCArchiveTypeEnum ABCArchiveType;

#define BOOZE_NAMESPACE_OPEN_SCOPE   namespace BOOZE {
#define BOOZE_NAMESPACE_CLOSE_SCOPE  }  

#endif  // _BOOZE_FOUNDATION_H_
