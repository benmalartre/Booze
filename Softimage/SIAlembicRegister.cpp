//-----------------------------------------------
// Alembic Softimage Plugin
//-----------------------------------------------
#include "AlembicArchiveStorage.h"
#include "SIAlembicRegister.h"
#include "SIAlembicWriteJob.h"

XSI::CStatus RegisterAlembicIPolymeshTopo( XSI::PluginRegistrar& in_reg );
XSI::CStatus RegisterAlembicIPolymesh( XSI::PluginRegistrar& in_reg );
XSI::CStatus RegisterAlembicIXForm( XSI::PluginRegistrar& in_reg );
XSI::CStatus RegisterAlembicIPointCloud( XSI::PluginRegistrar& in_reg );

using namespace Alembic;

char* getPodStr(Alembic::Abc::PlainOldDataType pod)
{
    if(pod == Abc::kBooleanPOD) return "kBooleanPOD";
    if(pod == Abc::kUint8POD) return "kUint8POD";
    if(pod == Abc::kInt8POD) return "kInt8POD";
    if(pod == Abc::kUint16POD) return "kUint16POD";
    if(pod == Abc::kInt16POD) return "kInt16POD";
    if(pod == Abc::kUint32POD) return "kUint32POD";
    if(pod == Abc::kInt32POD) return "kInt32POD";
    if(pod == Abc::kUint64POD) return "kUint64POD";
    if(pod == Abc::kInt64POD) return "kInt64POD";
    if(pod == Abc::kFloat16POD) return "kFloat16POD";
    if(pod == Abc::kFloat32POD) return "kFloat32POD";
    if(pod == Abc::kFloat64POD) return "kFloat64POD";
    if(pod == Abc::kStringPOD) return "kStringPOD";
    if(pod == Abc::kWstringPOD) return "kWstringPOD";
    if(pod == Abc::kNumPlainOldDataTypes) return "kNumPlainOldDataTypes";
    //if(pod == AbcA::kUnknownPOD)  
    return "kUnknownPOD";
}

/*
Abc::ICompoundProperty getProperty(const Abc::ICompoundProperty& props, std::string aproperty)
{
    for(size_t i=0; i<props.getNumProperties(); i++){
       AbcA::PropertyHeader pheader = props.getPropertyHeader(i);
	   
       if(pheader.getName() == aproperty){
          return Abc::ICompoundProperty(props, aproperty); 
       }  
    }
	
    return Abc::ICompoundProperty();
}
*/
SICALLBACK XSILoadPlugin( PluginRegistrar& in_reg )
{
	in_reg.PutAuthor(L"benmalartre");
	in_reg.PutName(L"AlembicPlugin");
	in_reg.PutVersion(1,0);
	in_reg.RegisterCommand(L"Alembic_Export",L"Alembic_Export");
	in_reg.RegisterCommand(L"Alembic_Import",L"Alembic_Import");
	in_reg.RegisterOperator(L"Alembic_XForm");
	in_reg.RegisterOperator(L"Alembic_Camera");
	in_reg.RegisterOperator(L"Alembic_Polymesh_Topo");
	in_reg.RegisterOperator(L"Alembic_Polymesh");
	in_reg.RegisterOperator(L"Alembic_Normals");
	in_reg.RegisterOperator(L"Alembic_UVs");
	in_reg.RegisterMenu(siMenuMainFileExportID,L"Alembic_MenuExport",false,false);
	in_reg.RegisterMenu(siMenuMainFileImportID,L"Alembic_MenuImport",false,false);
	in_reg.RegisterProperty(L"Alembic_Export_Settings");
	in_reg.RegisterProperty(L"Alembic_Import_Settings");

	// ICE Nodes
	//RegisterICEAlembicPolymesh(in_reg);
	RegisterAlembicIXForm(in_reg);
	RegisterAlembicIPointCloud(in_reg);
	RegisterAlembicIPolymeshTopo(in_reg);
	RegisterAlembicIPolymesh(in_reg);

	//abc_archive_manager = new AlembicArchiveManager();

	return CStatus::OK;
}

SICALLBACK XSIUnloadPlugin( const PluginRegistrar& in_reg )
{
	abc_archive_manager.DeleteAllArchives();
	CString strPluginName;
	strPluginName = in_reg.GetName();
	Application().LogMessage(strPluginName + L" has been unloaded.",siVerboseMsg);
	return CStatus::OK;
}

SICALLBACK Alembic_MenuExport_Init( CRef& in_ctxt )
{
	Context ctxt( in_ctxt );
	Menu oMenu;
	oMenu = ctxt.GetSource();
	MenuItem oNewItem;
	oMenu.AddCommandItem(L"Alembic 1.1",L"Alembic_Export",oNewItem);
	return CStatus::OK;
}

SICALLBACK Alembic_MenuImport_Init( CRef& in_ctxt )
{
	Context ctxt( in_ctxt );
	Menu oMenu;
	oMenu = ctxt.GetSource();
	MenuItem oNewItem;
	oMenu.AddCommandItem(L"Alembic 1.1",L"Alembic_Import",oNewItem);
	return CStatus::OK;
}