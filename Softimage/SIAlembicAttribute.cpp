#include "SIAlembicAttribute.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void Alembic_ExtraAttributes_Update(CustomProperty& settings)
{
	CString polymesh = settings.GetParameterValue(L"polymeshAttributes").GetAsText();
	CString pointcloud = settings.GetParameterValue(L"pointcloudAttributes").GetAsText();

	Extra_Polymesh_Attributes = (std::string)polymesh.GetAsciiString();
	Extra_PointCloud_Attributes = (std::string)pointcloud.GetAsciiString();
}

bool GetAttributeAtFrame(Primitive& prim,CString name)
{
	ICEAttribute attribute = prim.GetICEAttributeFromName(name);
	// Check Data Integrity
	  switch(attribute.GetDataType())
	  {
		  case siICENodeDataBool:
		  {
			 Application().LogMessage(L"Attribute BOOL :: "+name);
			CICEAttributeDataArrayBool b;
			attribute.GetDataArray(b);
			if(b.GetCount()==0)
			{
				Application().LogMessage(L"[Alembic 1.5] The Attribute "+name+" exists but doesn't seem to be evaluated ---> check it!!!",XSI::siWarningMsg);
				return false;
			}
			else return true;
		  }
		  case siICENodeDataFloat:
		  {
			 Application().LogMessage(L"Attribute FLOAT :: "+name);
			CICEAttributeDataArrayFloat f;
			attribute.GetDataArray(f);
			if(f.GetCount()==0)
			{
				Application().LogMessage(L"[Alembic 1.5] The Attribute "+name+" exists but doesn't seem to be evaluated ---> check it!!!",XSI::siWarningMsg);
				return false;
			}
			else return true;
		  }
		  case XSI::siICENodeDataLong:
		  {
			 Application().LogMessage(L"Attribute LONG :: "+name);
			CICEAttributeDataArrayLong l;

			attribute.GetDataArray(l);
			if(l.GetCount()==0){
				Application().LogMessage(L"[Alembic 1.5] The Attribute "+name+" exists but doesn't seem to be evaluated ---> check it!!!",XSI::siWarningMsg);
				return false;
			}
			else return true;
		  }
		  case XSI::siICENodeDataVector2:
		  {
			 Application().LogMessage(L"Attribute VECTOR2 :: "+name);
			CICEAttributeDataArrayVector2f v2;
			attribute.GetDataArray(v2);
			if(v2.GetCount()==0)return false;
			else return true;
		  }
		  case XSI::siICENodeDataVector3:
		  {
			 Application().LogMessage(L"Attribute VECTOR3 :: "+name);
			CICEAttributeDataArrayVector3f v3;
			attribute.GetDataArray(v3);
			if(v3.GetCount()==0)return false;
			else return true;
		  }
		  case XSI::siICENodeDataVector4:
		  {
			 Application().LogMessage(L"Attribute VECTOR4 :: "+name);
			CICEAttributeDataArrayVector4f v4;
			attribute.GetDataArray(v4);
			if(v4.GetCount()==0)return false;
			else return true;
		  }
		  default:
		  {
			return false;
		  }
	  }
}

bool CheckAttribute(Primitive prim,CString name)
{
	int state = -1;
	ICEAttribute attribute = prim.GetICEAttributeFromName(name);
	if (!attribute.IsValid())
	{
	  Application().LogMessage(L"Attribute Invalid :: "+name);

	}
	else
	{
		if(attribute.GetElementCount()>0)
		{
			state = 0;
			// only 1-D Attributes for now
			if(attribute.GetStructureType() == XSI::siICENodeStructureSingle)
			{
				switch(attribute.GetDataType())
				{	
					case XSI::siICENodeDataBool:
					{
						CICEAttributeDataArrayBool d;
						attribute.GetDataArray(d);
						if(d.GetCount()>0)state = 1;
						break;
					}
					case XSI::siICENodeDataLong:
					{
						CICEAttributeDataArrayLong d;
						attribute.GetDataArray(d);
						if(d.GetCount()>0)state = 1;
						break;
					}
					case XSI::siICENodeDataFloat:
					{
						CICEAttributeDataArrayBool d;
						attribute.GetDataArray(d);
						if(d.GetCount()>0)state = 1;
						break;
					}
					case XSI::siICENodeDataVector2:
					{
						CICEAttributeDataArrayVector2f d;
						attribute.GetDataArray(d);
						if(d.GetCount()>0)state = 1;
						break;
					}
					case XSI::siICENodeDataVector3:
					{
						CICEAttributeDataArrayVector3f d;
						attribute.GetDataArray(d);
						if(d.GetCount()>0)state = 1;
						break;
					}
					case XSI::siICENodeDataVector4:
					{
						CICEAttributeDataArrayVector4f d;
						attribute.GetDataArray(d);
						if(d.GetCount()>0)state = 1;
						break;
					}
					case XSI::siICENodeDataQuaternion:
					{
						CICEAttributeDataArrayQuaternionf d;
						attribute.GetDataArray(d);
						if(d.GetCount()>0)state = 1;
						break;
					}
					case XSI::siICENodeDataRotation:
					{
						CICEAttributeDataArrayRotationf d;
						attribute.GetDataArray(d);
						if(d.GetCount()>0)state = 1;
						break;
					}
					case XSI::siICENodeDataColor4:
					{
						CICEAttributeDataArrayColor4f d;
						attribute.GetDataArray(d);
						if(d.GetCount()>0)state = 1;
						break;
					}
					case XSI::siICENodeDataMatrix33:
					{
						CICEAttributeDataArrayMatrix3f d;
						attribute.GetDataArray(d);
						if(d.GetCount()>0)state = 1;
						break;
					}
					case XSI::siICENodeDataMatrix44:
					{
						CICEAttributeDataArrayMatrix4f d;
						attribute.GetDataArray(d);
						if(d.GetCount()>0)state = 1;
						break;
					}
					case XSI::siICENodeDataString:
					{
						CICEAttributeDataArrayString d;
						attribute.GetDataArray(d);
						if(d.GetCount()>0)state = 1;
						break;
					}
					default:
						state = 0;

				}
			}
			else
				state = 0;
		}
	}
	switch(state){
		case -1:
			Application().LogMessage(L"[Alembic 1.5] The Attribute "+name+" Doesn't Exist!!!",XSI::siErrorMsg);
			break;
		case 0:
			Application().LogMessage(L"[Alembic 1.5] The Attribute "+name+" exists but doesn't seem to be evaluated ---> check it!!!",XSI::siErrorMsg);
			break;
		case 1:
			Application().LogMessage(L"[Alembic 1.5] The Attribute "+name+" is ready for Alembic Caching!!!",XSI::siInfoMsg);
			break;
	}
	return (state==1);
}
	/*
//SIAlembicOAttribute abc_attr(prop,geom,s[i],metadata);
			if(name == "StaticScale")
			{
				Alembic::Abc::OV3fArrayProperty out(prop, s[i], metadata, job->GetAnimatedTs());
				Application().LogMessage(L"########################### STATIC SCALE ATTRIBUTE CREATED ######################################");
			}
			if(name == "Orientation")
			{
				Alembic::Abc::OQuatfArrayProperty out(prop, s[i], metadata, job->GetAnimatedTs());
				Application().LogMessage(L"########################### ORIENTATION ATTRIBUTE CREATED ######################################");
				CICEAttributeDataArrayRotationf data;
				attr.GetDataArray( data );
				std::vector<Imath::Quatf> arr;
				int nbp = data.GetCount();
				arr.resize(nbp);
				Application().LogMessage(L"------------>>> ===== <<<--------------");
				memcpy(&arr[0],&data[0],sizeof(Imath::Quatf)*nbp);

				out.set(Alembic::Abc::QuatfArraySample(arr));;
				Application().LogMessage(L"------------>>> ===== <<<--------------");
			}
			//CICEAttributeDataGetter::Get(attr,prop,s[i],AlembicOObject::GetMetaData(),time);
			Application().LogMessage(L"Attribute Created :: "+name);
			*/




SIAlembicOAttribute::SIAlembicOAttribute(Alembic::Abc::OCompoundProperty& prop,ICEAttribute& attr,Alembic::Abc::MetaData metadata,unsigned int Ts)
{
	md = metadata;
	name = (std::string)attr.GetName().GetAsciiString();

	//Alembic::Abc::MetaData metadata = prop.getParent().getMetaData();

	Application().LogMessage(L"--------------------> create attribute : "+(CString)name.c_str()+L" <-------------------------");
	//attribute.GetName().GetAsciiString();
	//SIAlembicOAttribute abc_attr(prop,geom,s[i],metadata);
	// singleton ---> scalarProp
	if(attr.GetContextType() == XSI::siICENodeStructureSingle && attr.GetContextType() == XSI::siICENodeContextSingleton)
	{
		Application().LogMessage(L"Singleton Attribute :: "+(CString)name.c_str());
		isArray = false;
		md.set("isArray","0");
		switch(attr.GetDataType())
		{
			case XSI::siICENodeDataBool:
			{
				Alembic::Abc::OBoolProperty out(prop, name,metadata, Ts);
				scl = out;
				Application().LogMessage(L"BOOLEAN SCALAR ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataLong:
			{
				Alembic::Abc::OUInt32Property out(prop, name,metadata, Ts);
				scl = out;
				Application().LogMessage(L"LONG SCALAR ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataFloat:
			{
				Alembic::Abc::OFloatProperty out(prop, name,metadata, Ts);
				scl = out;
				Application().LogMessage(L"FLOAT SCALAR ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataVector2:
			{
				Alembic::Abc::OV2fProperty out(prop, name,metadata, Ts);
				scl = out;
				Application().LogMessage(L"VECTOR2 SCALAR ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataVector3:
			{
				Alembic::Abc::OV3fProperty out(prop, name,metadata, Ts);
				scl = out;
				Application().LogMessage(L"VECTOR3 SCALAR ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataVector4:
			{
				Alembic::Abc::OC4fProperty out(prop, name,metadata, Ts);
				scl = out;
				Application().LogMessage(L"VECTOR4 SCALAR ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataColor4:
			{
				Alembic::Abc::OC4fProperty out(prop, name,metadata, Ts);
				scl = out;
				Application().LogMessage(L"Color4 SCALAR ATTRIBUTE CREATED "+(CString)name.c_str());
				break;	
			}
			case XSI::siICENodeDataQuaternion:
			{
				Alembic::Abc::OQuatfProperty out(prop, name,metadata, Ts);
				scl = out;
				Application().LogMessage(L"QUATERNION SCALAR ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataRotation:
			{
				Alembic::Abc::OQuatfProperty out(prop, name,metadata, Ts);
				scl = out;
				Application().LogMessage(L"ROTATION SCALAR ATTRIBUTE CREATED "+(CString)name.c_str());
				
			}
			case XSI::siICENodeDataMatrix33:
			{
				Alembic::Abc::OM33fProperty out(prop, name,metadata, Ts);
				scl = out;
				Application().LogMessage(L"MATRIX33 SCALAR ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataMatrix44:
			{
				Alembic::Abc::OM44fProperty out(prop, name,metadata, Ts);
				scl = out;
				Application().LogMessage(L"MATRIX44 SCALAR ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataString:
			{
				Alembic::Abc::OStringProperty out(prop, name,metadata, Ts);
				scl = out;
				Application().LogMessage(L"STRING SCALAR ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
		}
	}
	else
	{
		isArray = true;
		md.set("isArray","1");
		switch(attr.GetDataType())
		{
			case XSI::siICENodeDataBool:
			{
				Alembic::Abc::OBoolArrayProperty out(prop, name,metadata, Ts);
				arr = out;
				Application().LogMessage(L"BOOLEAN ARRAY ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataLong:
			{
				Alembic::Abc::OUInt32ArrayProperty out(prop, name,metadata, Ts);
				arr = out;
				Application().LogMessage(L"LONG ARRAY ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataFloat:
			{
				Alembic::Abc::OFloatArrayProperty out(prop, name,metadata, Ts);
				arr = out;
				Application().LogMessage(L"FLOAT ARRAY ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataVector2:
			{
				Alembic::Abc::OV2fArrayProperty out(prop, name,metadata, Ts);
				arr = out;
				Application().LogMessage(L"VECTOR2 ARRAY ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataVector3:
			{
				Alembic::Abc::OV3fArrayProperty out(prop, name,metadata, Ts);
				arr = out;
				Application().LogMessage(L"VECTOR3 ARRAY ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataVector4:
			{
				Alembic::Abc::OC4fArrayProperty out(prop, name,metadata, Ts);
				arr = out;
				Application().LogMessage(L"VECTOR4 ARRAY ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataColor4:
			{
				Alembic::Abc::OC4fArrayProperty out(prop, name,metadata, Ts);
				arr = out;
				Application().LogMessage(L"COLOR4 ARRAY ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataQuaternion:
			{
				Alembic::Abc::OQuatfArrayProperty out(prop, name, Ts);
				arr = out;
				Application().LogMessage(L"VECTOR4 ARRAY ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataRotation:
			{
				Alembic::Abc::OQuatfArrayProperty out(prop, name, Ts);
				arr = out;
				Application().LogMessage(L"ROTATION ARRAY ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataMatrix33:
			{
				Alembic::Abc::OM33fArrayProperty out(prop, name,metadata, Ts);
				arr = out;
				Application().LogMessage(L"MATRIX33 ARRAY ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataMatrix44:
			{
				Alembic::Abc::OM44fArrayProperty out(prop, name,metadata, Ts);
				arr = out;
				Application().LogMessage(L"MATRIX44 ARRAY ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
			case XSI::siICENodeDataString:
			{
				Alembic::Abc::OStringArrayProperty out(prop, name,metadata, Ts);
				arr = out;
				Application().LogMessage(L"STRING ARRAY ATTRIBUTE CREATED "+(CString)name.c_str());
				break;
			}
		}
	}
}	

void SIAlembicOAttribute::Get(XSI::ICEAttribute& attr)
{

	if(isArray)
	{
		switch(attr.GetDataType())
		{
			case XSI::siICENodeDataBool:
			{
				CICEAttributeDataArrayBool data;
				attr.GetDataArray( data );
				std::vector<Alembic::Abc::bool_t> v;
				int nbp = data.GetCount();
				v.resize(nbp);
				for(LONG i=0;i<nbp;i++){
					v[i] = data[i];
				}
				arr.set(Alembic::Abc::BoolArraySample(v));;
				break;
			}
			
			case XSI::siICENodeDataLong:
			{
				CICEAttributeDataArrayLong data;
				attr.GetDataArray( data );
				std::vector<Alembic::Abc::uint32_t> v;
				int nbp = data.GetCount();
				v.resize(nbp);
				for(LONG i=0;i<nbp;i++){
					v[i] = data[i];
				}
				arr.set(Alembic::Abc::UInt32ArraySample(v));;
				break;
			}

			case XSI::siICENodeDataFloat:
			{
				CICEAttributeDataArrayFloat data;
				attr.GetDataArray( data );
				std::vector<Alembic::Abc::float32_t> v;
				int nbp = data.GetCount();
				v.resize(nbp);
				for(LONG i=0;i<nbp;i++){
					v[i] = data[i];
				}
				arr.set(Alembic::Abc::FloatArraySample(v));;
				break;
			}
		
			case XSI::siICENodeDataVector2:
			{
				CICEAttributeDataArrayVector2f data;
				attr.GetDataArray( data );
				std::vector<Imath::V2f> v;
				int nbp = data.GetCount();
				v.resize(nbp);
				for(LONG i=0;i<nbp;i++){
					v[i].x = data[i].GetX();
					v[i].y = data[i].GetY();
				}
				arr.set(Alembic::Abc::V2fArraySample(v));;
				break;
			}

			case XSI::siICENodeDataVector3:
			{
				
				CICEAttributeDataArrayVector3f data;
				attr.GetDataArray( data );

				std::vector<Imath::V3f> v;
				int nbp = data.GetCount();
				v.resize(nbp);
				
				for(int i=0;i<nbp;i++){
					v[i].x = data[i].GetX();
					v[i].y = data[i].GetY();
					v[i].z = data[i].GetZ();
				}
				arr.set(Alembic::Abc::V3fArraySample(v));;
				break;
			}

			case XSI::siICENodeDataVector4:
			{
				CICEAttributeDataArrayVector4f data;
				attr.GetDataArray( data );
				std::vector<Imath::C4f> v;
				int nbp = data.GetCount();
				v.resize(nbp);
				for(LONG i=0;i<nbp;i++){
					v[i].r = data[i].GetX();
					v[i].g = data[i].GetY();
					v[i].b = data[i].GetZ();
					v[i].a = data[i].GetW();
				}
				arr.set(Alembic::Abc::C4fArraySample(v));;
				break;
			}

			case XSI::siICENodeDataQuaternion:
			{
				CICEAttributeDataArrayQuaternionf data;
				attr.GetDataArray( data );
				std::vector<Imath::Quatf> v;
				int nbp = data.GetCount();
				v.resize(nbp);
				memcpy(&v[0],&data[0],sizeof(Imath::Quatf)*nbp);
				arr.set(Alembic::Abc::QuatfArraySample(v));;
				break;
			}

			case XSI::siICENodeDataRotation:
			{
				CICEAttributeDataArrayRotationf data;
				attr.GetDataArray( data );
				std::vector<Imath::Quatf> v;
				int nbp = data.GetCount();
				v.resize(nbp);
				CQuaternionf quat;
				for(LONG i=0;i<nbp;i++){
					CQuaternionf quat = data[i].GetQuaternion();
					v[i].r = quat.GetX();
					v[i].v.x = quat.GetY();
					v[i].v.y = quat.GetZ();
					v[i].v.z = quat.GetW();
				}

				arr.set(Alembic::Abc::QuatfArraySample(v));;
				break;
			}
		
			case XSI::siICENodeDataColor4:
			{
				CICEAttributeDataArrayColor4f data;
				attr.GetDataArray( data );
				std::vector<Imath::C4f> v;
				int nbp = data.GetCount();
				v.resize(nbp);
				memcpy(&v[0],&data[0],sizeof(Imath::C4f)*nbp);
				arr.set(Alembic::Abc::C4fArraySample(v));;
				break;
			}

			case XSI::siICENodeDataMatrix33:
			{
				CICEAttributeDataArrayMatrix3f data;
				attr.GetDataArray( data );
				std::vector<Imath::M33f> v;
				int nbp = data.GetCount();
				v.resize(nbp);
				memcpy(&v[0],&data[0],sizeof(Imath::M33f)*nbp);

				arr.set(Alembic::Abc::M33fArraySample(v));;
				break;
			}

			case XSI::siICENodeDataMatrix44:
			{
				CICEAttributeDataArrayMatrix4f data;
				attr.GetDataArray( data );
				std::vector<Imath::M44f> v;
				int nbp = data.GetCount();
				v.resize(nbp);
				memcpy(&v[0],&data[0],sizeof(Imath::M44f)*nbp);
				arr.set(Alembic::Abc::M44fArraySample(v));;
				break;
			}

			case XSI::siICENodeDataString:
			{
				CICEAttributeDataArrayString data;
				attr.GetDataArray( data );
				std::vector<std::string> v;
				int nbp = data.GetCount();
				v.resize(nbp);
				memcpy(&v[0],&data[0],sizeof(std::string)*nbp);
				arr.set(Alembic::Abc::StringArraySample(v));;
				break;
			}

		}
	}
	else
	{
		Application().LogMessage(L" Singleton Attribute...");
	}
	
		
}

/*
	Alembic::Abc::OC4cArrayProperty indicesProp(argGeomParamsProp, "EnvelopeIndices", metadata, job->GetAnimatedTs());
	std::vector<Alembic::Abc::C4c> indicesArray;
	indicesArray.resize(nbp);
	Alembic::Abc::C4c idx;
	for(LONG i=0;i<nbp;i++){
		EncodeEnvelope(weights,indicesArray[i], weightsArray[i],i,deformers.GetCount());
	}
	indicesProp.set(Alembic::Abc::C4cArraySample(indicesArray));
*/

size_t SIAlembicOAttribute::GetNumSamples()
{
	if(isArray && arr.valid())return arr.getNumSamples();
	else if(!isArray && scl.valid()) return scl.getNumSamples();
	else return 0;
}