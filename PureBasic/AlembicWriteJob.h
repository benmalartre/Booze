#ifndef _BOOZE_WRITE_JOB_H_
#define _BOOZE_WRITE_JOB_H_

#include "AlembicFoundation.h"
#define DEFAULT_FRAMERATE 24

BOOZE_NAMESPACE_OPEN_SCOPE
class AlembicOArchive;
class AlembicOObject;

class AlembicWriteJob
{
protected:
	std::string									m_filename;
    std::vector<float>							m_frames;
    AlembicOArchive*							m_archive;
	std::map<std::string,std::string>			m_options;
	float										m_timePerSample;
	AlembicOObject*								m_current;
	AbcA::TimeSamplingPtr						m_timeSampling;


public:
	AlembicWriteJob(const char* fileName, float* frames, int numFrames);
	~AlembicWriteJob();

	virtual AlembicOArchive*				getArchive() { return m_archive; };
	virtual const std::vector<float> &		getFrames() { return m_frames; };
	virtual void							setFileName(const  char* filename) { m_filename = filename; };
	virtual const char*						getFileName() { return m_filename.c_str(); };
	virtual void							setFramerate(float framerate);
	virtual void							setOption(const char* in_Name, const char* in_Value);
	virtual bool							hasOption(const char* in_Name);
	virtual const char*						getOption(const char* in_Name);
	virtual void							save(float time);
	virtual ABCStatus						setObjects();
	virtual ABCStatus						setFrames();
	virtual ABCStatus						process();

	inline float							startFrame(){ return m_frames.front(); };
	inline float							endFrame(){ return m_frames.back(); };

	int32_t									timeToIndex( float time );
};

BOOZE_EXPORT AlembicWriteJob* newWriteJob(const char* filename, float* frames, int numFrames=0);
BOOZE_EXPORT void deleteWriteJob(AlembicWriteJob* archive);

BOOZE_NAMESPACE_CLOSE_SCOPE

#endif //_BOOZE_ARCHIVE_STORAGE_H_
