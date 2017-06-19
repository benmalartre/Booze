#include "PBAlembicRegister.h"

EXPORT TCHAR* ABC_PassStringToPureBasic(std::string& str)
{
   int size = (int)str.size()+1;
   SYS_FastStringCopy(&alembic_io_string, str.c_str(), size);
   return &alembic_io_string;
}