#include "PBAlembicRegister.h"

#define PBStringBufferSize 2048

EXPORT const char* ABC_PassStringToPureBasic(std::string& str)
{
   return str.c_str();
}