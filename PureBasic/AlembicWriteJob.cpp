//#include "AlembicMessage.h"
#include "AlembicWriteJob.h"
#include "AlembicObject.h"

BOOZE_NAMESPACE_OPEN_SCOPE

/*
AlembicWriteJob::AlembicWriteJob(const std::string & in_FileName,const CRefArray & in_Selection,const CDoubleArray & in_Frames)
{
   _filename = in_FileName;
   mSelection = in_Selection;

   for(uint64_t i=0;i<in_Frames.GetCount();i++)
      _frames.push_back(in_Frames[i]);
}

AlembicWriteJob::~AlembicWriteJob()
{
}
*/

void AlembicWriteJob::SetOption(const char* in_Name, const char* in_Value)
{
	std::map<std::string,std::string>::iterator it = _options.find(in_Name);
	if(it == _options.end())
		_options.insert(std::pair<std::string,std::string>(in_Name,in_Value));
	else
		it->second = in_Value;
}

bool AlembicWriteJob::HasOption(const char*in_Name)
{
	std::map<std::string,std::string>::iterator it = _options.find(in_Name);
   return it != _options.end();
}

const char* AlembicWriteJob::GetOption(const char* in_Name)
{
	std::map<std::string,std::string>::iterator it = _options.find(in_Name);
   if(it != _options.end())
      return it->second.c_str();
   return std::string("False").c_str();
}

ABCStatus AlembicWriteJob::Process()
{
	/*
	std::string filename = "Test_Alembic";//GetFileName();
	AlembicMessage msg;
	std::string s;
	if(filename.size() == 0){
		s = "[Alembic] FileName is Empty !!! Write Job Aborted...";
		msg.Set(s);
		msg.Log();
		return ABCStatus::Error;
	}

   // check filenames
   if(filename.size() == 0)
   {
      Application().LogMessage(L"[Alembic 1.1] No filename specified.",siErrorMsg);
	  return ABCStatus::Error;
   }

   // check objects
   if(_selection.GetCount() == 0)
   {
      Application().LogMessage(L"[Alembic 1.1] No objects specified.",siErrorMsg);
	  return ABCStatus::Empty;
   }

   // check frames
   if(_frames.size() == 0)
   {
      Application().LogMessage(L"[Alembic 1.1] No frames specified.",siErrorMsg);
	  return ABCStatus::Empty;
   }

   // init archive (use a locally scoped archive)
   CString sceneFileName = L"Exported from: "+Application().GetActiveProject().GetActiveScene().GetParameterValue(L"FileName").GetAsText();
   _archive = CreateArchiveWithInfo(
         Alembic::AbcCoreHDF5::WriteArchive(),
		_filename,
         "Softimage Alembic 1.0 Plugin",
         _filename,
         Alembic::Abc::ErrorHandler::kThrowPolicy);

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
   AbcA::TimeSampling sampling(timePerSample,0.0);
   _Ts = _archive.addTimeSampling(sampling);

   Alembic::Abc::OBox3dProperty boxProp = Alembic::AbcGeom::CreateOArchiveBounds(_archive,_Ts);

   // create object for each
   std::vector<SIAlembicOObjectPtr> objects;
   for(uint64_t i=0;i<_selection.GetCount();i++)
   {
      X3DObject xObj(_selection[i]);
	  Application().LogMessage(L"Type : "+xObj.GetType());
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
   }

   ProgressBar prog;
   prog = Application().GetUIToolkit().GetProgressBar();
   prog.PutMinimum(0);
   prog.PutMaximum((uint64_t)(_frames.size() * objects.size()));
   prog.PutValue(0);
   prog.PutCancelEnabled(true);
   prog.PutVisible(true);

   Application().LogMessage(L"Frames Size : "+(CString)(uint64_t)_frames.size());

   for(unsigned int frame=0; frame < (unsigned int)_frames.size(); frame++)
   {

      for(size_t i=0;i<objects.size();i++)
      {
         prog.PutCaption(L"Frame "+CString(_frames[frame])+L" of "+objects[i]->GetRef().GetAsText());
         CStatus status = objects[i]->Save(_frames[frame]);
         if(status != CStatus::OK)
			 return ABCStatus::OK;
         if(prog.IsCancelPressed())
            break;
         prog.Increment();
      }
      if(prog.IsCancelPressed())
         break;
   }

   prog.PutVisible(false);
   */
    ABCStatus status = Status_OK;
	return status;
}	


ABCStatus AlembicWriteJob::SetFrames()
{
    ABCStatus status = Status_OK;
	return status;
}	

ABCStatus AlembicWriteJob::SetObjects()
{	
    ABCStatus status = Status_OK;
    return status;
}

BOOZE_NAMESPACE_CLOSE_SCOPE