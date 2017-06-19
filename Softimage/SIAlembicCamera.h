#ifndef _SIALEMBIC_CAMERA_H_
#define _SIALEMBIC_CAMERA_H_

#include "AlembicFoundation.h"
#include "AlembicCamera.h"
#include "SIAlembicRegister.h"
#include "SIAlembicObject.h"


class SIAlembicOCamera : public SIAlembicOObject, AlembicOCamera 
{
public:
	//SIOAlembicCamera():SIOAlembicObject(){};
   SIAlembicOCamera(const XSI::CRef & in_ref, SIAlembicWriteJob * in_job);
   ~SIAlembicOCamera(){};

   ABCStatus Save(double time);
};

class SIAlembicICamera : public SIAlembicIObject, AlembicICamera
{
public:
	SIAlembicICamera(){};
	SIAlembicICamera(const XSI::CRef & in_Ref){};
   ~SIAlembicICamera(){};

   void Check();
};

#endif