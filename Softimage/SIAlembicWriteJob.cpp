#include "SIAlembicWriteJob.h"
#include "SIAlembicObject.h"
#include "SIAlembicXform.h"
#include "SIAlembicCamera.h"
#include "SIAlembicPolymesh.h"
#include "SIAlembicPointCloud.h"
//#include "SIAlembicPolyMsh.h"

using namespace XSI;
using namespace MATH;

SIAlembicWriteJob::SIAlembicWriteJob(const XSI::CString & in_filename,const XSI::CRefArray & in_selection,const XSI::CDoubleArray & in_frames)
{
   _filename = in_filename.GetAsciiString();
   _selection = in_selection;

   for(LONG i=0;i<in_frames.GetCount();i++)
      _frames.push_back(in_frames[i]);
}

SIAlembicWriteJob::~SIAlembicWriteJob()
{
}

ABCStatus SIAlembicWriteJob::Process()
{
	Application().LogMessage(L"PROCESS");
	std::string filename = GetFileName();
   // check filenames
   if(filename.size() == 0)
   {
      Application().LogMessage(L"[Alembic 1.5] No filename specified.",siErrorMsg);
	  return Status_Error;
   }

   // check objects
   if(_selection.GetCount() == 0)
   {
      Application().LogMessage(L"[Alembic 1.5] No objects specified.",siErrorMsg);
	  return Status_Empty;
   }

   // check frames
   if(_frames.size() == 0)
   {
      Application().LogMessage(L"[Alembic 1.5] No frames specified.",siErrorMsg);
	  return Status_Empty;
   }

   // init archive (use a locally scoped archive)
   CString sceneFileName = L"Exported from: "+Application().GetActiveProject().GetActiveScene().GetParameterValue(L"FileName").GetAsText();
Application().LogMessage(sceneFileName);
   _archive = CreateArchiveWithInfo(
         Alembic::AbcCoreHDF5::WriteArchive(),
		_filename,
         "Softimage Alembic 1.5 Plugin",
         _filename,
         Alembic::Abc::ErrorHandler::kThrowPolicy);
Application().LogMessage(L"ARCHIVE CREATED");
   // get the frame rate
   double frameRate = 25.0;
   CValue returnVal;
   CValueArray args(1);
   args[0] = L"PlayControl.Rate";
   Application().ExecuteCommand(L"GetValue",args,returnVal);
   frameRate = returnVal;
   if(frameRate == 0.0)
      frameRate = 25.0;
   double timePerSample = 1.0 / frameRate;

   // create the sampling
   Alembic::Abc::TimeSampling sampling(timePerSample,0.0);
   _Ts = _archive.addTimeSampling(sampling);

   Alembic::Abc::OBox3dProperty boxProp = Alembic::AbcGeom::CreateOArchiveBounds(_archive,_Ts);

   // create object for each
   std::vector<SIAlembicOObjectPtr> objects;
   Application().LogMessage(L"Selected Objects Count : "+_selection.GetCount());
   for(LONG i=0;i<_selection.GetCount();i++)
   {
	   
      X3DObject xObj(_selection[i]);
	  Application().LogMessage(L"################## Type : "+xObj.GetType());
      if(xObj.GetType().IsEqualNoCase(L"camera"))
      {
		  Application().LogMessage(L"------------------- CAMERA --------------------------");
         SIAlembicOObjectPtr ptr;
         ptr.reset(new SIAlembicOCamera(xObj.GetActivePrimitive().GetRef(),this));
         objects.push_back(ptr);
		 
      }
      else if(xObj.GetType().IsEqualNoCase(L"polymsh"))
      {
		  Application().LogMessage(L"------------------- POLYMESH --------------------------");
         SIAlembicOObjectPtr ptr;
		 CRef ref = xObj.GetActivePrimitive().GetRef();
         ptr.reset(new SIAlembicOPolymesh(ref,this));
         objects.push_back(ptr);
		 
      }
	  if(xObj.GetType().IsEqualNoCase(L"pointcloud"))
      {
		  Application().LogMessage(L"------------------- POINT CLOUD --------------------------");
         SIAlembicOObjectPtr ptr;
		 CRef ref = xObj.GetActivePrimitive().GetRef();
         ptr.reset(new SIAlembicOPointCloud(ref,this));
         objects.push_back(ptr);
      }
	  if(xObj.GetType().IsEqualNoCase(L"nurbscurvelist"))
      {
		  Application().LogMessage(L"------------------- POINT CLOUD --------------------------");
         SIAlembicOObjectPtr ptr;
		 CRef ref = xObj.GetActivePrimitive().GetRef();
         ptr.reset(new SIAlembicOPointCloud(ref,this));
         objects.push_back(ptr);
		 
      }
   }

   ProgressBar prog;
   prog = Application().GetUIToolkit().GetProgressBar();
   prog.PutMinimum(0);
   prog.PutMaximum((LONG)(_frames.size() * objects.size()));
   prog.PutValue(0);
   prog.PutCancelEnabled(true);
   prog.PutVisible(true);

   Application().LogMessage(L"Frames Size : "+(CString)(ULONG)_frames.size());
   Application().LogMessage(L"Objects Size : "+(CString)(ULONG)objects.size());

   for(unsigned int frame=0; frame < (unsigned int)_frames.size(); frame++)
   {

      for(size_t i=0;i<objects.size();i++)
      {
         prog.PutCaption(L"Frame "+CString(_frames[frame])+L" of "+objects[i]->GetRef().GetAsText());
         CStatus status = objects[i]->Save(_frames[frame]);
         if(status != CStatus::OK)
			 return Status_OK;
         if(prog.IsCancelPressed())
            break;
         prog.Increment();
      }
      if(prog.IsCancelPressed())
         break;
   }

   prog.PutVisible(false);

   return Status_OK;
}
