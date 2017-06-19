#include "AlembicMessage.h"

//namespace AbcA = ::Alembic::AbcCoreAbstract::ALEMBIC_VERSION_NS;

//using namespace AbcA;


void AlembicMessage::Set(std::string& msg)
{
	this->_msg = msg;
}

void AlembicMessage::Log()
{
}

/*
// Alembic Output
void AlembicOObject::SetOObject(Alembic::Abc::OObject* obj)
{
	_abcobject = obj;
	_metadatastr = _abcobject->getMetaData().serialize();
}


#define EXPORT extern "C"

EXPORT float ThisIsAFuckinTest()
{
	return 7.666;
}

*/

