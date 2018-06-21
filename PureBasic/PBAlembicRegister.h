#ifndef _PBALEMBICREGISTER_H_
#define _PBALEMBICREGISTER_H_

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
#include <iostream>
#include <exception>
#include <string>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "AlembicArchiveStorage.h"

#if defined(_WINDOWS)
	#if defined(STATIC_LIB)
		#define EXPORT extern "C" 
	#else
		#define EXPORT extern "C" __declspec(dllexport)
	#endif
    #include "stdint.h"
#else
	#define EXPORT extern "C"
    #include <stdint.h>
#endif
#endif

namespace Abc  = ::Alembic::Abc;;
namespace AbcA = ::Alembic::AbcCoreAbstract;
namespace AbcF = ::Alembic::AbcCoreFactory;
namespace AbcG = ::Alembic::AbcGeom;
