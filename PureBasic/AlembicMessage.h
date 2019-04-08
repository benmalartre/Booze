#ifndef _ALEMBIC_MESSAGE_H_
#define _ALEMBIC_MESSAGE_H_

#include "AlembicFoundation.h"


class AlembicMessage
{
protected:
	std::string m_msg;
	
public:
	AlembicMessage(){};
	~AlembicMessage(){};

	void Set(std::string& msg);
	virtual void Log();
};

#endif