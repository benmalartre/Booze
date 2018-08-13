//#include "AlembicXForm.h"
#include "AlembicPolymesh.h"

BOOZE_NAMESPACE_OPEN_SCOPE

AlembicIPolymesh::AlembicIPolymesh(AbcG::IObject& object) :AlembicIObject(object)
{
	_type = GeometricType_PolyMesh;
}

bool AlembicIPolymesh::Initialize()
{
	AbcG::IPolyMesh mesh(_object);
	_variance = mesh.getSchema().getTopologyVariance();
	GetProperties();
	return true;
}

BOOZE_EXPORT bool ABC_ObjectIsPolymesh(AlembicIObject* obj)
{
	return obj->GetType() == GeometricType_PolyMesh;
};

void AlembicIPolymesh::GetTopoSampleDescription(float frame, ABC_Polymesh_Topo_Sample_Infos* infos)
{
	if (!_object.valid() || _type != GeometricType_PolyMesh)return;

	AbcG::IPolyMesh mesh(_object, Abc::kWrapExisting);
	if (!mesh.valid())return;

	// Get Sample Index
	AbcA::index_t sampleIndex = (AbcA::index_t)int(frame) - 1;

	// Clamp if necessary
	if (infos->_sampleindex < 0)
		infos->_sampleindex = 0;
	else if (infos->_sampleindex >= (AbcA::index_t)mesh.getSchema().getNumSamples())
		infos->_sampleindex = int(mesh.getSchema().getNumSamples()) - 1;

	AbcG::IPolyMeshSchema::Sample sample;
	mesh.getSchema().get(sample, infos->_sampleindex);

	infos->_nbpoints = sample.getPositions()->size();
	infos->_nbfaces = sample.getFaceCounts()->size();
	infos->_nbindices = sample.getFaceIndices()->size();

	uint32_t nbt = 0;
	for (uint32_t i = 0; i<sample.getFaceCounts()->size(); i++){
		nbt += sample.getFaceCounts()->get()[i] - 2;
	}
	infos->_nbsamples = nbt * 3;


	// check for normals
	infos->_hasnormal = false;
	AbcG::IN3fGeomParam meshNormParam = mesh.getSchema().getNormalsParam();
	if (meshNormParam.valid())
	{
		AbcG::N3fArraySamplePtr meshNorm = meshNormParam.getExpandedValue().getVals();

		//io_sample->_nbnorm = meshNorm->size();
		if (meshNorm->size() == infos->_nbindices)
		{
			infos->_hasnormal = true;
		}
	}

	// check for uvs
	infos->_hasuvs = false;
	AbcG::IV2fGeomParam meshUVsParam = mesh.getSchema().getUVsParam();
	if (meshUVsParam.valid())
	{
		AbcG::V2fArraySamplePtr meshUVs = meshUVsParam.getExpandedValue(0).getVals();
		if (meshUVs->size() == infos->_nbindices)
		{
			infos->_hasuvs = true;
		}
	}

	infos->_hascolor = false;
	infos->_hasenvelope = false;

	// check for color
	infos->_hascolor = false;
	AbcG::ICompoundProperty argGeomParamsProp(mesh.getSchema().getArbGeomParams());
	if (argGeomParamsProp.valid()){
		for (uint64_t i = 0; i<(uint64_t)argGeomParamsProp.getNumProperties(); i++){
			AbcG::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);

			if (h.getName() == "Colors"){
				infos->_hascolor = true;
				break;
			}
		}
	}

	// check for envelope
	infos->_hasenvelope = false;
	if (argGeomParamsProp.valid()){
		for (uint64_t i = 0; i<(uint64_t)argGeomParamsProp.getNumProperties(); i++){
			AbcG::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);

			if (h.getName() == "EnvelopeWeights" || h.getName() == "EnvelopeIndices"){
				infos->_hasenvelope = true;
				break;
			}
		}
	}
}

int AlembicIPolymesh::UpdateTopoSample(ABC_Polymesh_Topo_Sample_Infos* infos, ABC_Polymesh_Topo_Sample* io_sample)
{
	AbcG::IPolyMesh mesh(_object, Abc::kWrapExisting);

	AbcG::IPolyMeshSchema::Sample sample;
	mesh.getSchema().get(sample, infos->_sampleindex);

	AbcG::P3fArraySamplePtr meshPos = sample.getPositions();
	AbcG::Int32ArraySamplePtr meshFaceCount = sample.getFaceCounts();
	AbcG::Int32ArraySamplePtr meshFaceIndices = sample.getFaceIndices();

	std::vector<int> triToSample;

	size_t offset = 0;
	size_t offset1;
	memcpy(io_sample->_positions, meshPos->get(), meshPos->size() * sizeof(Imath::V3f));

	size_t nbv;
	size_t nbt;
	size_t z = 0;
	size_t last;


	// Build TriToSample Array
	for (size_t x = 0; x<meshFaceCount->size(); x++)
	{
		nbv = meshFaceCount->get()[x];
		io_sample->_facecount[x] = nbv;
		nbt = nbv - 2;
		last = z + nbv - 1;
		for (size_t y = 0; y<nbt; y++){
			triToSample.push_back(z + y);
			triToSample.push_back(z + y + 1);
			triToSample.push_back(last);
		}
		z += nbv;
	}

	/*
	for(size_t i=0;i<meshFaceIndices->size();i++)
	{
	io_sample->_faceindices[i]=meshFaceIndices->get()[i];
	}
	*/
	memcpy(io_sample->_faceindices, meshFaceIndices->get(), meshFaceIndices->size() * sizeof(uint32_t));

	AbcG::IN3fGeomParam meshNormParam = mesh.getSchema().getNormalsParam();
	if (infos->_hasnormal == true && meshNormParam.valid())
	{
		AbcG::N3fArraySamplePtr meshNorm = meshNormParam.getExpandedValue().getVals();

		//io_sample->_nbnorm = meshNorm->size();
		if (meshNorm->size() == infos->_nbindices)
		{

			// let's apply it!
			offset = 0;
			int x;
			for (size_t i = 0; i<infos->_nbsamples; i++){
				x = triToSample[i];
				io_sample->_normals[offset++] = meshNorm->get()[x].x;
				io_sample->_normals[offset++] = meshNorm->get()[x].y;
				io_sample->_normals[offset++] = meshNorm->get()[x].z;
			}
		}
	}

	offset = 0;
	AbcG::IV2fGeomParam meshUVsParam = mesh.getSchema().getUVsParam();
	if (infos->_hasuvs == true && meshUVsParam.valid())
	{

		AbcG::V2fArraySamplePtr meshUVs = meshUVsParam.getExpandedValue(0).getVals();
		if (meshUVs->size() == infos->_nbindices)
		{

			// let's apply it!
			offset = 0;
			for (size_t i = 0; i<infos->_nbsamples; i++)
			{
				io_sample->_uvs[offset++] = meshUVs->get()[triToSample[i]].x;
				io_sample->_uvs[offset++] = meshUVs->get()[triToSample[i]].y;
				offset++;
			}

			//for(size_t i=0;i<meshUVs->size();i++)
			//{
			//	io_sample->_uvs[offset++] = meshUVs->get()[i].x;
			//	io_sample->_uvs[offset++] = meshUVs->get()[i].y;
			//	offset++;
			//}
		}

	}

	AbcG::ICompoundProperty argGeomParamsProp(mesh.getSchema().getArbGeomParams());
	if (infos->_hascolor == true && argGeomParamsProp.valid()){
		offset = 0;
		for (size_t i = 0; i<(size_t)argGeomParamsProp.getNumProperties(); i++){
			AbcG::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);

			if (h.getName() == "Colors"){
				AbcG::IC4fArrayProperty colors(argGeomParamsProp, h.getName());
				AbcG::C4fArraySamplePtr propPtr1 = colors.getValue(0);
				size_t nbp = (size_t)propPtr1->size();
				Imath::Color4f color;


				offset = 0;
				for (size_t i = 0; i<infos->_nbsamples; i++)
				{
					io_sample->_colors[offset++] = propPtr1->get()[triToSample[i]].r;
					io_sample->_colors[offset++] = propPtr1->get()[triToSample[i]].g;
					io_sample->_colors[offset++] = propPtr1->get()[triToSample[i]].b;
					io_sample->_colors[offset++] = propPtr1->get()[triToSample[i]].a;
				}
			}
		}
	}

	return meshFaceIndices->size();
}

void AlembicIPolymesh::UpdatePointPosition(ABC_Polymesh_Sample_Infos* infos, ABC_Polymesh_Sample* io_sample){

}

size_t AlembicIPolymesh::UpdateSample(ABC_Polymesh_Topo_Sample_Infos* infos, ABC_Polymesh_Topo_Sample* io_sample)
{
	AbcG::IPolyMesh mesh(_object, Abc::kWrapExisting);

	AbcG::IPolyMeshSchema::Sample sample;
	mesh.getSchema().get(sample, infos->_sampleindex);

	AbcG::P3fArraySamplePtr meshPos = sample.getPositions();

	int offset = 0;
	int offset1;
	/*
	for(size_t i=0;i<meshPos->size();i++)
	{
	io_sample->_positions[offset++]=meshPos->get()[i].x;
	io_sample->_positions[offset++]=meshPos->get()[i].y;
	io_sample->_positions[offset++]=meshPos->get()[i].z;
	}
	*/
	memcpy(io_sample->_positions, &meshPos->get()[0], infos->_nbpoints * 3 * sizeof(float));
	AbcG::IN3fGeomParam meshNormParam = mesh.getSchema().getNormalsParam();
	if (meshNormParam.valid())
	{
		AbcG::N3fArraySamplePtr meshNorm = meshNormParam.getExpandedValue().getVals();

		//io_sample->_nbnorm = meshNorm->size();
		if (meshNorm->size() == infos->_nbindices)
		{

			// let's apply it!
			offset = 0;
			for (size_t i = 0; i<meshNorm->size(); i++)
			{
				io_sample->_normals[offset++] = meshNorm->get()[i].x;
				io_sample->_normals[offset++] = meshNorm->get()[i].y;
				io_sample->_normals[offset++] = meshNorm->get()[i].z;
			}
		}
	}

	return 0;
}
//****//

BOOZE_EXPORT void ABC_GetPolymeshTopoSampleDescription(AlembicIObject* obj, float frame, ABC_Polymesh_Topo_Sample_Infos* infos)
{
	if(!obj->Get().valid()||!ABC_ObjectIsPolymesh(obj))return;

	AbcG::IPolyMesh mesh(obj->Get(),Abc::kWrapExisting);
	if(!mesh.valid())return ;

	// Get Sample Index
	AbcA::index_t sampleIndex = (AbcA::index_t)int(frame)-1;

   // Clamp if necessary
   if(infos->_sampleindex < 0)
      infos->_sampleindex = 0;
   else if(infos->_sampleindex >= (AbcA::index_t)mesh.getSchema().getNumSamples())
      infos->_sampleindex = int(mesh.getSchema().getNumSamples()) - 1;
	
	AbcG::IPolyMeshSchema::Sample sample;
	mesh.getSchema().get(sample,infos->_sampleindex);
	
	infos->_nbpoints = sample.getPositions()->size();
	infos->_nbfaces = sample.getFaceCounts()->size();
	infos->_nbindices = sample.getFaceIndices()->size();
	
	uint32_t nbt = 0;
	for(uint32_t i=0;i<sample.getFaceCounts()->size();i++){
		nbt  += sample.getFaceCounts()->get()[i]-2;
	}
	infos->_nbsamples = nbt*3;
	

	// check for normals
	infos->_hasnormal = false;
	AbcG::IN3fGeomParam meshNormParam = mesh.getSchema().getNormalsParam();
	if(meshNormParam.valid())
	{
		AbcG::N3fArraySamplePtr meshNorm = meshNormParam.getExpandedValue().getVals();
		
		//io_sample->_nbnorm = meshNorm->size();
		if(meshNorm->size() == infos->_nbindices)
		{
			infos->_hasnormal = true;
		}
	}
	
	// check for uvs
	infos->_hasuvs = false;
	AbcG::IV2fGeomParam meshUVsParam = mesh.getSchema().getUVsParam();
	if(meshUVsParam.valid())
	{
		AbcG::V2fArraySamplePtr meshUVs = meshUVsParam.getExpandedValue(0).getVals();
		if(meshUVs->size() == infos->_nbindices)
		{
			infos->_hasuvs = true;
		}
	}

	infos->_hascolor = false;
	infos->_hasenvelope = false;
	
	// check for color
	infos->_hascolor = false;
	AbcG::ICompoundProperty argGeomParamsProp (mesh.getSchema().getArbGeomParams());
	if(argGeomParamsProp.valid()){
		 for(uint64_t i=0;i<(uint64_t)argGeomParamsProp.getNumProperties();i++){
			 AbcG::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);

			  if(h.getName() == "Colors"){
				infos->_hascolor = true;
				break;
			  }
		 }
	}

	// check for envelope
	infos->_hasenvelope = false;
	if(argGeomParamsProp.valid()){
		 for(uint64_t i=0;i<(uint64_t)argGeomParamsProp.getNumProperties();i++){
			 AbcG::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);

			  if(h.getName() == "EnvelopeWeights" || h.getName() == "EnvelopeIndices"){
				infos->_hasenvelope = true;
				break;
			  }
		 }
	}
}

BOOZE_EXPORT int ABC_UpdatePolymeshTopoSample(AlembicIObject* obj, ABC_Polymesh_Topo_Sample_Infos* infos, ABC_Polymesh_Topo_Sample* io_sample)
{
	AbcG::IPolyMesh mesh(obj->Get(),Abc::kWrapExisting);

	AbcG::IPolyMeshSchema::Sample sample;
	mesh.getSchema().get(sample,infos->_sampleindex);

	AbcG::P3fArraySamplePtr meshPos = sample.getPositions();
	AbcG::Int32ArraySamplePtr meshFaceCount = sample.getFaceCounts();
	AbcG::Int32ArraySamplePtr meshFaceIndices = sample.getFaceIndices();

	std::vector<int> triToSample;
	
	size_t offset=0;
	size_t offset1;
	memcpy(io_sample->_positions,meshPos->get(),meshPos->size() * sizeof(Imath::V3f));

   	size_t nbv;
   	size_t nbt;
   	size_t z=0;
	size_t last;


   // Build TriToSample Array
   for(size_t x=0;x<meshFaceCount->size();x++)
   {
		nbv = meshFaceCount->get()[x];
		io_sample->_facecount[x]=nbv;
		nbt = nbv-2;
		last = z+nbv-1;
		for(size_t y=0;y<nbt;y++){
			triToSample.push_back(z+y);
			triToSample.push_back(z+y+1);
			triToSample.push_back(last);
		}
		z+=nbv;
   }

   /*
   for(size_t i=0;i<meshFaceIndices->size();i++)
   {
		io_sample->_faceindices[i]=meshFaceIndices->get()[i];
   }
	*/
   memcpy(io_sample->_faceindices,meshFaceIndices->get(),meshFaceIndices->size() * sizeof(uint32_t));

   AbcG::IN3fGeomParam meshNormParam = mesh.getSchema().getNormalsParam();
	if(infos->_hasnormal==true && meshNormParam.valid())
	{
		AbcG::N3fArraySamplePtr meshNorm = meshNormParam.getExpandedValue().getVals();
		
		//io_sample->_nbnorm = meshNorm->size();
		if(meshNorm->size() == infos->_nbindices)
		{

			// let's apply it!
			 offset = 0;
			 int x;
			 for(size_t i=0;i<infos->_nbsamples;i++){
				 x = triToSample[i];
				io_sample->_normals[offset++] = meshNorm->get()[x].x;
				io_sample->_normals[offset++] = meshNorm->get()[x].y;
				io_sample->_normals[offset++] = meshNorm->get()[x].z;
			 }
		}
	}
	
	offset = 0;
	AbcG::IV2fGeomParam meshUVsParam = mesh.getSchema().getUVsParam();
	if(infos->_hasuvs==true && meshUVsParam.valid())
	{
		
		AbcG::V2fArraySamplePtr meshUVs = meshUVsParam.getExpandedValue(0).getVals();
		if(meshUVs->size() == infos->_nbindices)
		{
			
			// let's apply it!
			 offset = 0;
			for(size_t i=0;i<infos->_nbsamples;i++)
			 {
				io_sample->_uvs[offset++] = meshUVs->get()[triToSample[i]].x;
				io_sample->_uvs[offset++] = meshUVs->get()[triToSample[i]].y;
				offset++;
			}
			 
			//for(size_t i=0;i<meshUVs->size();i++)
			//{
			//	io_sample->_uvs[offset++] = meshUVs->get()[i].x;
			//	io_sample->_uvs[offset++] = meshUVs->get()[i].y;
			//	offset++;
			//}
		}
		
	}

	AbcG::ICompoundProperty argGeomParamsProp(mesh.getSchema().getArbGeomParams());
	if(infos->_hascolor==true && argGeomParamsProp.valid()){
		offset = 0;	
		 for(size_t i=0;i<(size_t)argGeomParamsProp.getNumProperties();i++){
			 AbcG::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);

			  if(h.getName() == "Colors"){
				AbcG::IC4fArrayProperty colors(argGeomParamsProp,h.getName());
				AbcG::C4fArraySamplePtr propPtr1 = colors.getValue(0);
				size_t nbp = (size_t)propPtr1->size();
				Imath::Color4f color;
				
				
				offset = 0;
				for(size_t i=0;i<infos->_nbsamples;i++)
				 {
					io_sample->_colors[offset++] = propPtr1->get()[triToSample[i]].r;
					io_sample->_colors[offset++] = propPtr1->get()[triToSample[i]].g;
					io_sample->_colors[offset++] = propPtr1->get()[triToSample[i]].b;
					io_sample->_colors[offset++] = propPtr1->get()[triToSample[i]].a;
				}
			  }
		 }
	}

   return meshFaceIndices->size();
}

BOOZE_EXPORT void ABC_UpdatePointPosition(AlembicIObject* obj, ABC_Polymesh_Topo_Sample_Infos* infos, ABC_Polymesh_Topo_Sample* io_sample){

}

BOOZE_EXPORT int ABC_UpdatePolymeshSample(AlembicIObject* obj, ABC_Polymesh_Topo_Sample_Infos* infos, ABC_Polymesh_Topo_Sample* io_sample)
{
	AbcG::IPolyMesh mesh(obj->Get(),Abc::kWrapExisting);

	AbcG::IPolyMeshSchema::Sample sample;
	mesh.getSchema().get(sample,infos->_sampleindex);

	AbcG::P3fArraySamplePtr meshPos = sample.getPositions();

	int offset=0;
	int offset1;
	/*
   for(size_t i=0;i<meshPos->size();i++)
   {
		io_sample->_positions[offset++]=meshPos->get()[i].x;
		io_sample->_positions[offset++]=meshPos->get()[i].y;
		io_sample->_positions[offset++]=meshPos->get()[i].z;	
   }
	*/
	memcpy(io_sample->_positions,&meshPos->get()[0],infos->_nbpoints*3*sizeof(float));
   AbcG::IN3fGeomParam meshNormParam = mesh.getSchema().getNormalsParam();
	if(meshNormParam.valid())
	{
		AbcG::N3fArraySamplePtr meshNorm = meshNormParam.getExpandedValue().getVals();
		
		//io_sample->_nbnorm = meshNorm->size();
		if(meshNorm->size() == infos->_nbindices)
		{

			// let's apply it!
			 offset = 0;
			 for(size_t i=0;i<meshNorm->size();i++)
			 {
				io_sample->_normals[offset++] = meshNorm->get()[i].x;
				io_sample->_normals[offset++] = meshNorm->get()[i].y;
				io_sample->_normals[offset++] = meshNorm->get()[i].z;
			 }
		}
	}

   return 0;
}

BOOZE_NAMESPACE_CLOSE_SCOPE