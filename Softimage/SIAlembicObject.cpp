#include "SIAlembicObject.h"

SIAlembicOObject::SIAlembicOObject(const XSI::CRef & in_ref,SIAlembicWriteJob * in_job)
{
   _ref = in_ref;
   _job = in_job;
}


SIAlembicIObject::SIAlembicIObject(const XSI::CRef & in_ref)
{
	_ref = in_ref;
	_init = false;
}

bool SIAlembicIObject::IsDirty( ICENodeContext& in_ctxt  )
{
	CICEPortState PathState( in_ctxt, ID_IN_Path );
	_pathdirty = PathState.IsDirty( CICEPortState::siDataDirtyState);
	PathState.ClearState();

	CICEPortState IdentifierState( in_ctxt, ID_IN_Identifier );
	_identifierdirty = IdentifierState.IsDirty( CICEPortState::siDataDirtyState );
	IdentifierState.ClearState();

	/*
	CICEPortState FrameState( in_ctxt, ID_IN_Frame );
	bool dirty = FrameState.IsDirty( CICEPortState::siTimeDirtyState );
	FrameState.ClearState();
	*/
	return (_pathdirty || _identifierdirty );
}

void SIAlembicIObject::Clean()
{
	_init = false;
	SetIObject(NULL);
}

void SIAlembicIObject::Init(ICENodeContext& in_ctxt)
{
	//Get Parameters Value
	CDataArrayString pathData(in_ctxt, ID_IN_Path);
	CDataArrayString identifierData(in_ctxt, ID_IN_Identifier);

	// Check Alembic File valid
	CString path;
	pathData.GetData(0,path);
	CString identifier;
	identifierData.GetData(0,identifier);
	_init = false;

	AlembicIArchive* archive = abc_archive_manager.GetArchiveFromID(path.GetAsciiString());

	if(archive)
	{
		archive->GetAllObjects();

		std::vector<Alembic::Abc::IObject*>::iterator it = archive->_objects.begin();
		for(;it<archive->_objects.end();it++)
		{
			Application().LogMessage((*it)->getFullName().c_str());
			//if((*it)->getFullName() == identifier)
				
			/* && (*it)->getMetaData().matches(metadata))*/
		}
		
		Application().LogMessage(L"Identifier : "+(CString)identifier.GetAsciiString());
		
		Alembic::Abc::IObject* obj = archive->GetObjectFromID(identifier.GetAsciiString());
		if(obj)
		{
			if(Alembic::AbcGeom::IXform::matches(obj->getMetaData()))
			{
				Application().LogMessage(L"There is a Meta Data Match...");
			}
			Application().LogMessage(L"MetaData : "+(CString)this->GetMetaDataString().c_str());
			//if(metadata.matches(obj->getMetaData()))
			//{
				SetIObject(obj);
				_init = true;
				std::string fn = obj->getFullName();
				Application().LogMessage(L"Init Object : "+(CString)fn.c_str());
			/*}
			else
				Application().LogMessage(L"Can't matche MetaDaTa!!!");
				*/
		}
		else
		{
			Application().LogMessage(L"Failed Loading Alembic Object...");
		}
	}
	else{
		Application().LogMessage(L"Failed Loading Alembic Archive : "+path);
	}
}

void SIAlembicIObject::Init(ICENodeContext& in_ctxt, const Alembic::Abc::MetaData& metadata)
{
	//Get Parameters Value
	CDataArrayString pathData(in_ctxt, ID_IN_Path);
	CDataArrayString identifierData(in_ctxt, ID_IN_Identifier);

	// Check Alembic File valid
	CString path;
	pathData.GetData(0,path);
	CString identifier;
	identifierData.GetData(0,identifier);
	_init = false;

	AlembicIArchive* archive = abc_archive_manager.GetArchiveFromID(path.GetAsciiString());

	if(archive)
	{
		Application().LogMessage(L"Identifier : "+(CString)identifier.GetAsciiString());
		
		Alembic::Abc::IObject* obj = archive->GetObjectFromID(identifier.GetAsciiString());
		if(obj)
		{
			if(Alembic::AbcGeom::IXform::matches(obj->getMetaData()))
			{
				Application().LogMessage(L"There is a Meta Data Match...");
			}
			Application().LogMessage(L"MetaData : "+(CString)this->GetMetaDataString().c_str());
			//if(metadata.matches(obj->getMetaData()))
			//{
				SetIObject(obj);
				_init = true;
				std::string fn = obj->getFullName();
				Application().LogMessage(L"Init Object : "+(CString)fn.c_str());
			/*}
			else
				Application().LogMessage(L"Can't matche MetaDaTa!!!");
				*/
		}
		else
		{
			Application().LogMessage(L"Failed Loading Alembic Object...");
		}
	}
}

SIAlembicIObject::~SIAlembicIObject()
{
}

