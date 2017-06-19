#include <string>
#include <iostream>
using namespace std;

#include "AlembicArchiveStorage.h"

int main()
{
	AlembicArchiveManager* manager = ABC_CreateArchiveManager();
	string filename = "D:\\Projects\\Test\\Alembic\\Hierarchy2.abc";

	IAlembicArchive* archive = ABC_OpenArchive(AlembicArchiveManager* manager,filename.c_str());

	cout<< "hello";
	int in;
	cin>>in;
	return 0;
}