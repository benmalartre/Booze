#ifndef _BOOZE_WRITE_JOB_H_
#define _BOOZE_WRITE_JOB_H_

#include "AlembicFoundation.h"

BOOZE_NAMESPACE_OPEN_SCOPE
class AlembicOArchive;
class AlembicWriteJob
{
protected:
	std::string _filename;
    std::vector<float> _frames;
    AlembicOArchive* _archive;
    unsigned int _Ts;
	std::map<std::string,std::string> _options;

public:
	AlembicWriteJob();
	AlembicWriteJob(const char* in_FileName, float* in_Frames, int numFrames);
	~AlembicWriteJob();

	virtual AlembicOArchive* GetArchive() { return _archive; };
	virtual const std::vector<float> & GetFrames() { return _frames; };
	virtual void SetFileName(const  char* filename) { _filename = filename; };
	virtual const char* GetFileName() { return _filename.c_str(); };
	virtual unsigned int GetAnimatedTs() { return _Ts; };
	virtual void SetOption(const char* in_Name, const char* in_Value);
	virtual bool HasOption(const char* in_Name);
	virtual const char* GetOption(const char* in_Name);

	virtual ABCStatus SetObjects();
	virtual ABCStatus SetFrames();
	virtual ABCStatus Process();
};

BOOZE_EXPORT AlembicWriteJob* newWriteJob(const char* filename, float* frames, int numFrames=0);
BOOZE_EXPORT void deleteWriteJob(AlembicWriteJob* archive);

BOOZE_NAMESPACE_CLOSE_SCOPE

#endif //_BOOZE_ARCHIVE_STORAGE_H_
