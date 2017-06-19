#include "SIAlembicRegister.h"
#include "SIAlembicAttribute.h"
#include "SIAlembicWriteJob.h"

using namespace Alembic;

// Extras Attributes to Save
CString Default_Polymesh_Attributes = L"";
CString Default_PointCloud_Attributes = L"Color,Orientation,Scale,StaticPosition,StaticOrientation,StaticScale";

SICALLBACK Alembic_Export_Init( CRef& in_ctxt )
{
	Context ctxt( in_ctxt );
	Command oCmd;
	oCmd = ctxt.GetSource();
	oCmd.PutDescription(L"");
	oCmd.EnableReturnValue(true);

	ArgumentArray oArgs;
	oArgs = oCmd.GetArguments();
	oArgs.Add(L"filename");								//0
	oArgs.AddWithHandler(L"objects",L"Collection");		//1
	oArgs.Add(L"frameIn");								//2
	oArgs.Add(L"frameOut");								//3
	oArgs.Add(L"frameStep");							//4
	oArgs.Add(L"exportNormals");						//5
	oArgs.Add(L"vertexNormals");						//6
	oArgs.Add(L"exportUVs");							//7
	oArgs.Add(L"exportFaceSets");						//8
	oArgs.Add(L"exportColors");							//9
	oArgs.Add(L"exportEnvelope");						//10
	oArgs.Add(L"polymeshAttributes");					//11
	oArgs.Add(L"pointcloudAttributes");					//12
	return CStatus::OK;
}



SICALLBACK Alembic_Export_Execute( CRef& in_ctxt )
{
	Context ctxt( in_ctxt );
	CValueArray args = ctxt.GetAttribute(L"Arguments");
	Application().LogMessage(L">>>>>>>>>>>>>>> Alembic <<<<<<<<<<<<<<<<<<<");
   // take care of the filename
   CString filename = (CString)args[0].GetAsText();
   Application().LogMessage(L"Export Colors ? ------> "+args[9].GetAsText());
   if(filename.IsEmpty())
   {
      // let's see if we are in interactive mode
      if(Application().IsInteractive())
      {
         CComAPIHandler toolkit;
         toolkit.CreateInstance(L"XSI.UIToolkit");
         CComAPIHandler filebrowser(toolkit.GetProperty(L"FileBrowser"));
         filebrowser.PutProperty(L"InitialDirectory",Application().GetActiveProject().GetPath());
         filebrowser.PutProperty(L"Filter",L"Alembic Files(*.abc)|*.abc||");
         CValue returnVal;
         filebrowser.Call(L"ShowSave",returnVal);
         filename = filebrowser.GetProperty(L"FilePathName").GetAsText();
         if(filename.IsEmpty())
            return CStatus::Abort;
      }
      else
      {
         Application().LogMessage(L"[Alembic 1.1] No filename specified.",siErrorMsg);
         return CStatus::InvalidArgument;
      }
   }
   Application().LogMessage(L"[Alembic 1.1] Filename used: "+filename);

   // check the objects
	CRefArray objects = (CRefArray)args[1];
   if(objects.GetCount() == 0)
   {
      // use the selection
      objects = Application().GetSelection().GetArray();
      if(objects.GetCount() == 0)
      {
         Application().LogMessage(L"[Alembic 1.1] No objects specified.",siErrorMsg);
         return CStatus::InvalidArgument;
      }
   }
   Application().LogMessage(L"[Alembic 1.1] Objects used: "+CValue(objects.GetCount()).GetAsText());

   // check if we have arguments
   if(args[2].GetAsText().IsEmpty())
   {
      // let's setup the property
      CustomProperty settings = Application().GetActiveSceneRoot().AddProperty(L"Alembic_Export_Settings");
		Application().LogMessage(L" Create Custom Settings Property...");
      // inspect it
      CValueArray inspectArgs(5);
      CValue inspectResult;
      inspectArgs[0] = settings.GetFullName();
      inspectArgs[1] = L"";
      inspectArgs[2] = L"Alembic Export Settings";
      inspectArgs[3] = siModal;
      inspectArgs[4] = false;
      Application().ExecuteCommand(L"InspectObj",inspectArgs,inspectResult);
      
      // prepare for deletion
      inspectArgs.Resize(1);
      inspectArgs[0] = settings.GetFullName();
      if((bool)inspectResult)
      {
         Application().ExecuteCommand(L"DeleteObj",inspectArgs,inspectResult);
         return CStatus::Abort;
      }

      // retrieve the options
      args[2] = settings.GetParameterValue(L"frame_in");
      args[3] = settings.GetParameterValue(L"frame_out");
      args[4] = settings.GetParameterValue(L"frame_step");

      LONG normalMode = settings.GetParameterValue(L"normals");
      switch(normalMode)
      {
         case 0:
         {
            args[5] = false;
            args[6] = false;
            args[7] = false;
            break;
         }
         case 1:
         {
            args[5] = true;
            args[6] = true;
            args[7] = false;
            break;
         }
         case 2:
         {
            args[5] = true;
            args[6] = false;
            args[7] = settings.GetParameterValue(L"uvs");
            break;
         }
      }
      args[8] = settings.GetParameterValue(L"facesets");
	  args[9] = settings.GetParameterValue(L"colors");
	  args[10] = settings.GetParameterValue(L"envelope");
	  args[11] = settings.GetParameterValue(L"polymesh_attributes");
	  args[12] = settings.GetParameterValue(L"pointcloud_attributes");
	Application().LogMessage(L"Export Colors ? ------> "+args[9].GetAsText());
      Application().ExecuteCommand(L"DeleteObj",inspectArgs,inspectResult);
   }

   // check the frames
   double frameIn = (double)args[2];
   double frameOut = (double)args[3];
   double frameStep = (double)args[4];
   CDoubleArray frames;
   for(double frame=frameIn; frame<=frameOut; frame+=frameStep)
      frames.Add(frame);
   Application().LogMessage(L"[Alembic 1.1] Frames used: "+CValue(frameIn).GetAsText()+L" to "+CValue(frameOut).GetAsText()+L", step "+CValue(frameStep).GetAsText());

   // create the job
   SIAlembicWriteJob job(filename,objects,frames);

   // check the options 
   job.SetOption("exportNormals",args[5].GetAsText().GetAsciiString());
   job.SetOption("vertexNormals",args[6].GetAsText().GetAsciiString());
   job.SetOption("exportUVs",args[7].GetAsText().GetAsciiString());
   job.SetOption("exportFaceSets",args[8].GetAsText().GetAsciiString());
   job.SetOption("exportColors",args[9].GetAsText().GetAsciiString());
   job.SetOption("exportEnvelope",args[10].GetAsText().GetAsciiString());
   job.SetOption("polymeshAttributes",args[11].GetAsText().GetAsciiString());
   job.SetOption("pointcloudAttributes",args[12].GetAsText().GetAsciiString());
 
   // run the job
   abc_archive_manager.DeleteAllArchives();
	CStatus result = job.Process();
   abc_archive_manager.DeleteAllArchives();
   return result;
}

SICALLBACK Alembic_Export_Settings_Define( CRef& in_ctxt )
{
	Context ctxt( in_ctxt );
	CustomProperty oCustomProperty;
	Parameter oParam;
	oCustomProperty = ctxt.GetSource();

   // get the current frame in an out
   CValueArray cmdArgs(1);
   CValue cmdReturnVal;
   cmdArgs[0] = L"PlayControl.In";
   Application().ExecuteCommand(L"GetValue",cmdArgs,cmdReturnVal);
   oCustomProperty.AddParameter(L"frame_in",CValue::siInt4,siPersistable,L"",L"",cmdReturnVal,-1000000,1000000,1,100,oParam);
   cmdArgs[0] = L"PlayControl.Out";
   Application().ExecuteCommand(L"GetValue",cmdArgs,cmdReturnVal);
   oCustomProperty.AddParameter(L"frame_out",CValue::siInt4,siPersistable,L"",L"",cmdReturnVal,-1000000,1000000,1,100,oParam);
   oCustomProperty.AddParameter(L"frame_step",CValue::siInt4,siPersistable,L"",L"",1,-1000000,1000000,1,5,oParam);

	oCustomProperty.AddParameter(L"normals",CValue::siInt4,siPersistable,L"",L"",2,0,10,0,10,oParam);
   oCustomProperty.AddParameter(L"uvs",CValue::siBool,siPersistable,L"",L"",1,0,1,0,1,oParam);
   oCustomProperty.AddParameter(L"facesets",CValue::siBool,siPersistable,L"",L"",1,0,1,0,1,oParam);

   oCustomProperty.AddParameter(L"colors",CValue::siInt4,siPersistable,L"",L"",0,0,2,0,2,oParam);
   oCustomProperty.AddParameter(L"envelope",CValue::siBool,siPersistable,L"",L"",false,false,true,false,true,oParam);

   oCustomProperty.AddParameter(L"archive_type",CValue::siInt4,siPersistable,L"",L"",0,0,2,0,2,oParam);
   oCustomProperty.AddParameter(L"polymesh_attributes",CValue::siString,siPersistable,L"polymesh_attributes",L"polymesh extras attributes",Default_Polymesh_Attributes,oParam);
   oCustomProperty.AddParameter(L"pointcloud_attributes",CValue::siString,siPersistable,L"pointcloud_attributes",L"pointcloud extras attributes",Default_PointCloud_Attributes,oParam);
   //oCustomProperty.AddParameter(L"pointcloud_attributes",CValue::siString,siPersistable,Default_PointCloud_Attributes,L"",L"",L"",L"",L"",L"",oParam);
	return CStatus::OK;
}

SICALLBACK Alembic_Export_Settings_DefineLayout( CRef& in_ctxt )
{
	Context ctxt( in_ctxt );
	PPGLayout oLayout;
	PPGItem oItem;
	oLayout = ctxt.GetSource();
	oLayout.Clear();

	oLayout.AddGroup(L"Archive");
	CValueArray archiveItems(2);
	archiveItems[0] = L"HDF5";
	archiveItems[1] = 0l;
	archiveItems[2] = L"Ogawa";
	archiveItems[3] = 1l;
	oLayout.AddEnumControl(L"archive_type",archiveItems,L"Type");
	oLayout.EndGroup();

   oLayout.AddGroup(L"Animation");
   oLayout.AddItem(L"frame_in",L"In");
   oLayout.AddItem(L"frame_out",L"Out");
   oLayout.AddItem(L"frame_step",L"Step");
   oLayout.EndGroup();

   CValueArray normalItems(6);
   oLayout.AddGroup(L"Geometry");
   normalItems[0] = L"No Normals";
   normalItems[1] = 0l;
   normalItems[2] = L"Per Vertex Normals";
   normalItems[3] = 1l;
   normalItems[4] = L"Per Face Normals";
   normalItems[5] = 2l;
   oLayout.AddEnumControl(L"normals",normalItems,L"Normals");
   oLayout.AddItem(L"uvs",L"UVs");
   oLayout.AddItem(L"facesets",L"Clusters");
   oLayout.EndGroup();

   CValueArray colorItems(6);
   oLayout.AddGroup(L"Geometry");
   colorItems[0] = L"No Colors";
   colorItems[1] = 0l;
   colorItems[2] = L"Color Attribute";
   colorItems[3] = 1l;
   colorItems[4] = L"Vertex Color Map";
   colorItems[5] = 2l;
   oLayout.AddEnumControl(L"colors",colorItems,L"Colors");
   oLayout.EndGroup();

   oLayout.AddGroup(L"Envelope");
   oLayout.AddItem(L"envelope",L"Envelope");
   oLayout.EndGroup();

   oLayout.AddGroup(L"Extra Attributes");
   oLayout.AddItem(L"polymesh_attributes",L"Polymesh");
   oLayout.AddItem(L"pointcloud_attributes",L"PointCloud");
   oLayout.EndGroup();

	return CStatus::OK;
}

XSIPLUGINCALLBACK CStatus Alembic_Export_Settings_PPGEvent( const CRef& in_ctxt )
{
	PPGEventContext ctxt( in_ctxt ) ;
	if ( ctxt.GetEventID() == PPGEventContext::siParameterChange )
	{
		Parameter param = ctxt.GetSource() ;	
		CString paramName = param.GetScriptName() ; 

      // depending on what we changed
      if(paramName.IsEqualNoCase(L"normals"))
      {
         Property(param.GetParent()).GetParameter(L"uvs").PutCapabilityFlag(siReadOnly,(LONG)param.GetValue() != 2l);
      }
	}
   else if (ctxt.GetEventID() == PPGEventContext::siButtonClicked)
   {
      CString buttonName = ctxt.GetAttribute(L"Button");
   }

	return CStatus::OK ;
}