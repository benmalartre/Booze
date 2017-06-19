#ifndef _ALEMBIC_WRITE_JOB_H_
#define _ALEMBIC_WRITE_JOB_H_

#include "AlembicFoundation.h"

class AlembicWriteJob
{
protected:
	std::string _filename;
    //XSI::CRefArray mSelection;
    std::vector<double> _frames;
    Alembic::Abc::OArchive _archive;
    unsigned int _Ts;
	std::map<std::string,std::string> _options;
public:
	AlembicWriteJob(){};
	/*
      const XSI::CString & in_FileName,
      const XSI::CRefArray & in_Selection,
      const XSI::CDoubleArray & in_Frames);
	  */
	~AlembicWriteJob(){};;

	Alembic::Abc::OArchive GetArchive() { return _archive; }
	const std::vector<double> & GetFrames() { return _frames; }
	const std::string & GetFileName() { return _filename; }
	unsigned int GetAnimatedTs() { return _Ts; }
	void SetOption(const std::string & in_Name, const std::string & in_Value);
	bool HasOption(const std::string & in_Name);
	std::string GetOption(const std::string & in_Name);

	virtual ABCStatus SetObjects();
	virtual ABCStatus SetFrames();
	virtual ABCStatus Process();
};

#endif