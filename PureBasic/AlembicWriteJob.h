#ifndef _BOOZE_WRITE_JOB_H_
#define _BOOZE_WRITE_JOB_H_

#include "AlembicFoundation.h"

BOOZE_NAMESPACE_OPEN_SCOPE
class AlembicOArchive;
class AlembicWriteJob
{
protected:
	std::string m_filename;
    std::vector<float> m_frames;
    AlembicOArchive* m_archive;
    unsigned int m_Ts;
	std::map<std::string,std::string> m_options;

public:
	AlembicWriteJob(const char* in_FileName, float* in_Frames, int numFrames);
	~AlembicWriteJob();

	virtual AlembicOArchive* GetArchive() { return m_archive; };
	virtual const std::vector<float> & GetFrames() { return m_frames; };
	virtual void SetFileName(const  char* filename) { m_filename = filename; };
	virtual const char* GetFileName() { return m_filename.c_str(); };
	virtual unsigned int GetAnimatedTs() { return m_Ts; };
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
