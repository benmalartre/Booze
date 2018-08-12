#ifndef _BOOZE_WRITE_JOB_H_
#define _BOOZE_WRITE_JOB_H_

#include "AlembicFoundation.h"

BOOZE_NAMESPACE_OPEN_SCOPE

class AlembicWriteJob
{
protected:
	std::string _filename;
    std::vector<double> _frames;
    Alembic::Abc::OArchive _archive;
    unsigned int _Ts;
	std::map<std::string,std::string> _options;

public:
	AlembicWriteJob(){};

	virtual Alembic::Abc::OArchive GetArchive() { return _archive; }
	virtual const std::vector<double> & GetFrames() { return _frames; }
	virtual const char* GetFileName() { return _filename.c_str(); }
	virtual unsigned int GetAnimatedTs() { return _Ts; }
	virtual void SetOption(const char* in_Name, const char* in_Value);
	virtual bool HasOption(const char* in_Name);
	virtual const char* GetOption(const char* in_Name);

	virtual ABCStatus SetObjects();
	virtual ABCStatus SetFrames();
	virtual ABCStatus Process();
};

BOOZE_NAMESPACE_CLOSE_SCOPE
#endif