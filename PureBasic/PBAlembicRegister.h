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
#include "PureLibrary.h"

static TCHAR alembic_io_string(4096);

#ifndef UNICODE
	typedef std::string String;
#else
	typedef std::wstring String;
#endif

#if defined(_WINDOWS)
	//extern "C"  TCHAR* __cdecl SYS_GetOuputBuffer(int,int);
	#if defined(STATIC_LIB)
		#define EXPORT extern "C" 
	#else
		#define EXPORT extern "C" __declspec(dllexport)
	#endif
    #include "stdint.h"
#else
	//extern "C" char* SYS_GetOuputBuffer(int,int);
	#define EXPORT extern "C"
    #include <stdint.h>
#endif

/*
kernel32.lib
user32.lib
gdi32.lib
winspool.lib
shell32.lib
ole32.lib
oleaut32.lib
uuid.lib
comdlg32.lib
advapi32.lib
libcpmt.lib
shlwapi.lib

*/
#endif
