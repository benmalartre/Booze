#include "PBAlembicAttribute.h"

EXPORT bool ABC_HasProperty(AlembicIObject* obj,const char* p_name){
	for(int i=0;i<obj->GetNumProperties();i++){
		if(obj->GetProperty(i)->GetName() == (std::string)p_name)return true;
	}
	return false;
}

EXPORT bool ABC_GetAttributeSampleDescription(AlembicIProperty* prop,float frame,ABC_Attribute_Sample_Infos* infos)
{
	std::string s(prop->GetName());
	int size = (int)s.size()+1;
	SYS_FastStringCopy((TCHAR*)infos->_name, s.c_str(), size);

	infos->_time = frame;
	infos->_traits = prop->GetDataTraits();
	infos->_type = prop->GetPropertyType();
    infos->_nbitems = prop->GetNbItems((Alembic::Abc::chrono_t)frame);
    return true;

}

EXPORT TCHAR* ABC_GetAttributeSampleName(ABC_Attribute_Sample_Infos* infos)
{
    std::string name = (std::string)infos->_name;
    if(!name.size())
        return NULL;
    int size = (int)name.size()+1;
    
    SYS_FastStringCopy(&alembic_io_string, infos->_name, size);
    
    return &alembic_io_string;
    
}

EXPORT void* ABC_GetAttributeAtIndex(AlembicIProperty* prop,float frame,int index)
{
    if(prop->GetPropertyType() == PropertyType_Array && prop->GetDataTraits() == DataTraits_V3f)
    {
        
        prop->GetSample(frame);
    }
    return NULL;
}

EXPORT void ABC_GetAttributeSample(AlembicIProperty* prop,ABC_Attribute_Sample_Infos* infos,ABC_Attribute_Sample* io_sample)
{
	switch(prop->GetPropertyType()){
		case PropertyType_Scalar:
			switch(prop->GetDataTraits())
			{
			case DataTraits_Bool:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(bool));
				break;
			case DataTraits_UChar:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Alembic::Abc::uint8_t));
				break;
			case DataTraits_Char:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Alembic::Abc::int8_t));
				break;
			case DataTraits_UInt16:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Alembic::Abc::uint16_t));
				break;
			case DataTraits_Int16:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Alembic::Abc::int16_t));
				break;
			case DataTraits_UInt32:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Alembic::Abc::uint32_t));
				break;
			case DataTraits_Int32:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Alembic::Abc::int32_t));
				break;
			case DataTraits_UInt64:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Alembic::Abc::uint64_t));
				break;
			case DataTraits_Int64:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Alembic::Abc::int64_t));
				break;
			case DataTraits_Half:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Alembic::Abc::float16_t));
				break;
			case DataTraits_Float:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Alembic::Abc::float32_t));
				break;
			case DataTraits_Double:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Alembic::Abc::float64_t));
				break;
			case DataTraits_String:
				//memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Alembic::Abc::float32_t));
				break;
			case DataTraits_WString:
				//memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Alembic::Abc::float64_t));
				break;

			case DataTraits_V2s:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V2s));
				break;
			case DataTraits_V2i:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V2i));
				break;
			case DataTraits_V2f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V2f));
				break;
			case DataTraits_V2d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V2d));
				break;

			case DataTraits_V3s:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V3s));
				break;
			case DataTraits_V3i:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V3i));
				break;
			case DataTraits_V3f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V3f));
				break;
			case DataTraits_V3d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V3d));
				break;

			case DataTraits_P2s:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V2s));
				break;
			case DataTraits_P2i:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V2i));
				break;
			case DataTraits_P2f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V2f));
				break;
			case DataTraits_P2d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V2d));
				break;

			case DataTraits_P3s:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V3s));
				break;
			case DataTraits_P3i:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V3i));
				break;
			case DataTraits_P3f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V3f));
				break;
			case DataTraits_P3d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V3d));
				break;

			case DataTraits_Box2s:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::Box2s));
				break;
			case DataTraits_Box2i:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::Box2i));
				break;
			case DataTraits_Box2f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::Box2f));
				break;
			case DataTraits_Box2d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::Box2d));
				break;

			case DataTraits_Box3s:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::Box3s));
				break;
			case DataTraits_Box3i:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::Box3i));
				break;
			case DataTraits_Box3f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::Box3f));
				break;
			case DataTraits_Box3d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::Box3d));
				break;

			case DataTraits_M33f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::M33f));
				break;
			case DataTraits_M33d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::M33d));
				break;

			case DataTraits_M44f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::M44f));
				break;
			case DataTraits_M44d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::M44d));
				break;

			case DataTraits_Quatf:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::Quatf));
				break;
			case DataTraits_Quatd:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::Quatd));
				break;

			case DataTraits_C3h:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::C3h));
				break;
			case DataTraits_C3f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::C3f));
				break;
			case DataTraits_C3c:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::C3c));
				break;
			
			case DataTraits_C4h:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::C4h));
				break;
			case DataTraits_C4f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::C4f));
				break;
			case DataTraits_C4c:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::C4c));
				break;

			case DataTraits_N2f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V2f));
				break;
			case DataTraits_N2d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V2d));
				break;

			case DataTraits_N3f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V3f));
				break;
			case DataTraits_N3d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::V3d));
				break;
			}
			break;
		case PropertyType_Array:
		{
			Alembic::Abc::uint64_t nb = infos->_nbitems;
			switch(prop->GetDataTraits())
			{
			case DataTraits_Bool:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(bool));
				break;
			case DataTraits_UChar:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Alembic::Abc::uint8_t));
				break;
			case DataTraits_Char:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Alembic::Abc::int8_t));
				break;
			case DataTraits_UInt16:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Alembic::Abc::uint16_t));
				break;
			case DataTraits_Int16:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Alembic::Abc::int16_t));
				break;
			case DataTraits_UInt32:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Alembic::Abc::uint32_t));
				break;
			case DataTraits_Int32:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Alembic::Abc::int32_t));
				break;
			case DataTraits_UInt64:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Alembic::Abc::uint64_t));
				break;
			case DataTraits_Int64:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Alembic::Abc::int64_t));
				break;
			case DataTraits_Half:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Alembic::Abc::float16_t));
				break;
			case DataTraits_Float:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Alembic::Abc::float32_t));
				break;
			case DataTraits_Double:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Alembic::Abc::float64_t));
				break;
			case DataTraits_String:
				//memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Alembic::Abc::float32_t));
				break;
			case DataTraits_WString:
				//memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Alembic::Abc::float64_t));
				break;

			case DataTraits_V2s:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V2s));
				break;
			case DataTraits_V2i:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V2i));
				break;
			case DataTraits_V2f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V2f));
				break;
			case DataTraits_V2d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V2d));
				break;

			case DataTraits_V3s:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V3s));
				break;
			case DataTraits_V3i:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V3i));
				break;
			case DataTraits_V3f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V3f));
				break;
			case DataTraits_V3d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V3d));
				break;

			case DataTraits_P2s:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V2s));
				break;
			case DataTraits_P2i:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V2i));
				break;
			case DataTraits_P2f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V2f));
				break;
			case DataTraits_P2d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V2d));
				break;

			case DataTraits_P3s:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V3s));
				break;
			case DataTraits_P3i:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V3i));
				break;
			case DataTraits_P3f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V3f));
				break;
			case DataTraits_P3d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V3d));
				break;

			case DataTraits_Box2s:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::Box2s));
				break;
			case DataTraits_Box2i:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::Box2i));
				break;
			case DataTraits_Box2f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::Box2f));
				break;
			case DataTraits_Box2d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::Box2d));
				break;

			case DataTraits_Box3s:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),sizeof(Imath::Box3s));
				break;
			case DataTraits_Box3i:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::Box3i));
				break;
			case DataTraits_Box3f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::Box3f));
				break;
			case DataTraits_Box3d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::Box3d));
				break;

			case DataTraits_M33f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::M33f));
				break;
			case DataTraits_M33d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::M33d));
				break;

			case DataTraits_M44f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::M44f));
				break;
			case DataTraits_M44d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::M44d));
				break;

			case DataTraits_Quatf:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::Quatf));
				break;
			case DataTraits_Quatd:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::Quatd));
				break;

			case DataTraits_C3h:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::C3h));
				break;
			case DataTraits_C3f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::C3f));
				break;
			case DataTraits_C3c:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::C3c));
				break;
			
			case DataTraits_C4h:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::C4h));
				break;
			case DataTraits_C4f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::C4f));
				break;
			case DataTraits_C4c:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::C4c));
				break;

			case DataTraits_N2f:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V2f));
				break;
			case DataTraits_N2d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V2d));
				break;

			case DataTraits_N3f:
				memcpy(io_sample->_datas,	prop->GetSample(infos->_time),nb*sizeof(Imath::V3f));
				break;
			case DataTraits_N3d:
				memcpy(io_sample->_datas,prop->GetSample(infos->_time),nb*sizeof(Imath::V3d));
				break;
			}
		}
		break;
	}
}

EXPORT int ABC_GetFloatSize()
{
	return sizeof(float);
}
