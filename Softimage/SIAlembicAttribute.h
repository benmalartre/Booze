#include "SIAlembicRegister.h"
#ifndef _SIALEMBIC_ATTRIBUTE_H_
#define _SIALEMBIC_ATTRIBUTE_H_
#include <string>
#include <sstream>
#include <vector>

static std::string Extra_PointCloud_Attributes = "";
static std::string Extra_Polymesh_Attributes = "";

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

bool CheckAttribute(Primitive prim,CString name);

class SIAlembicOAttribute
{
private:
	Alembic::Abc::OCompoundProperty prop;
	std::string name;
	bool isArray;
	Alembic::Abc::OArrayProperty arr;
	Alembic::Abc::OScalarProperty scl;
	Alembic::Abc::MetaData md;

public:
	SIAlembicOAttribute(Alembic::Abc::OCompoundProperty& prop,ICEAttribute& attribute,Alembic::Abc::MetaData metadata,unsigned int Ts);
	//void Get(XSI::ICEAttribute& attr, Alembic::Abc::OCompoundProperty& prop,Alembic::Abc::MetaData& metadata, std::string name,unsigned int Ts);
	void Get(XSI::ICEAttribute& attr);
	CString GetName(){return (CString)name.c_str();};
	size_t GetNumSamples();

};


/*
template < >
class CICEAttributeDataLogger< XSI::CString >
{
    public:
    static void Log( ICEAttribute& attr )
    {
        CICEAttributeDataArrayString data;
        attr.GetDataArray( data );

        Application xsi;
        for( ULONG i=0; i<data.GetCount( ); i++ )
        {
                xsi.LogMessage( data[ i ] );
        }
    }
};
*/
#endif