#ifndef _BOOZE_MANAGER_H_
#define _BOOZE_MANAGER_H_

#include "AlembicFoundation.h"
#include "AlembicArchive.h"
#include <map>

BOOZE_NAMESPACE_OPEN_SCOPE

class AlembicArchiveManager
{
public:
	AlembicArchiveManager(){};
	virtual AlembicIArchive* OpenArchive(const char* path);
	virtual bool CloseArchive(const char* path);
	virtual uint64_t GetNumOpenArchives();
	virtual bool CloseAllArchives();
private:
	std::map<std::string, AlembicIArchive *> _archives;
};

BOOZE_EXPORT AlembicArchiveManager* newArchiveManager();
BOOZE_EXPORT void deleteArchiveManager(AlembicArchiveManager* manager);

BOOZE_NAMESPACE_CLOSE_SCOPE

#endif