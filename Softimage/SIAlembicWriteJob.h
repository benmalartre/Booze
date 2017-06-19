#ifndef _SIALEMBIC_WRITE_JOB_H_
#define _SIALEMBIC_WRITE_JOB_H_

#include "AlembicWriteJob.h"
#include "SIAlembicRegister.h"

class SIAlembicWriteJob : public AlembicWriteJob
{
private:
	XSI::CRefArray _selection;
public:
	SIAlembicWriteJob(const XSI::CString & in_FileName,const XSI::CRefArray & in_Selection,const XSI::CDoubleArray & in_Frames);
	~SIAlembicWriteJob();

	ABCStatus Process();
};

#endif