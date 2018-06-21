#include "PBAlembicXForm.h"
#include "PBAlembicObject.h"
#include "PBAlembicPolymesh.h"

using namespace std;

EXPORT bool ABC_ObjectIsPolymesh(AlembicIObject* obj)
{
	//return Alembic::Abc::IArrayProperty::matches(obj->getMetaData());
	return Alembic::AbcGeom::IPolyMesh::matches(obj->GetIObject()->getMetaData());;
};

EXPORT const char* ABC_TestPolymesh(AlembicIObject* obj,ABC_Polymesh_Topo_Sample_Infos* io_sample)
{
	return 0;//ABC_PassStringToPureBasic(std::string("ABC Test Polymesh Succeeded!!"));
}

EXPORT void ABC_GetEnvelope(AlembicIObject* obj,ABC_Envelope_Sample* io_sample)
{
	if(!obj->GetIObject()->valid()||!ABC_ObjectIsPolymesh(obj))return;

	Alembic::AbcGeom::IPolyMesh mesh(*obj->GetIObject(),Alembic::Abc::kWrapExisting);
	if(!mesh.valid())return ;

	Alembic::AbcGeom::ICompoundProperty argGeomParamsProp(mesh.getSchema().getArbGeomParams());

	long offset;
	if(argGeomParamsProp.valid()){
		offset = 0;	
		 for(long i=0;i<(long)argGeomParamsProp.getNumProperties();i++){
			 Alembic::AbcGeom::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);

			  if(h.getName() == "EnvelopeWeights"){
					Alembic::AbcGeom::IC4fArrayProperty weights(argGeomParamsProp,h.getName());
					Alembic::AbcGeom::C4fArraySamplePtr propPtr1 = weights.getValue(0);
					long nbp = (long)propPtr1->size();
					memcpy(io_sample->_weights,propPtr1->get(),nbp * sizeof(Imath::Color4f));
			  }
			  else if(h.getName() == "EnvelopeIndices"){
				Alembic::AbcGeom::IC4cArrayProperty indices(argGeomParamsProp,h.getName());
				Alembic::AbcGeom::C4cArraySamplePtr propPtr1 = indices.getValue(0);
				long nbp = (long)propPtr1->size();
				memcpy(io_sample->_indices,propPtr1->get(),nbp * sizeof(Imath::Color4c));
				
			  }
			  else if(h.getName() == "EnvelopeNbDeformers"){
				Alembic::AbcGeom::IUInt32Property nbdeformers(argGeomParamsProp,h.getName());
				io_sample->_nbdeformers = nbdeformers.getValue(0);
			  }
		 }
	}
	
}

EXPORT void ABC_GetPolymeshTopoSampleDescription(AlembicIObject* obj,float frame,ABC_Polymesh_Topo_Sample_Infos* infos)
{
	if(!obj->GetIObject()->valid()||!ABC_ObjectIsPolymesh(obj))return;

	Alembic::AbcGeom::IPolyMesh mesh(*obj->GetIObject(),Alembic::Abc::kWrapExisting);
	if(!mesh.valid())return ;

	
	// Get Sample Index
	Alembic::AbcCoreAbstract::index_t sampleIndex = (Alembic::AbcCoreAbstract::index_t)int(frame)-1;

   // Clamp if necessary
   if(infos->_sampleindex < 0)
      infos->_sampleindex = 0;
   else if(infos->_sampleindex >= (Alembic::AbcCoreAbstract::index_t)mesh.getSchema().getNumSamples())
      infos->_sampleindex = int(mesh.getSchema().getNumSamples()) - 1;
	
	Alembic::AbcGeom::IPolyMeshSchema::Sample sample;
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
	Alembic::AbcGeom::IN3fGeomParam meshNormParam = mesh.getSchema().getNormalsParam();
	if(meshNormParam.valid())
	{
		Alembic::AbcGeom::N3fArraySamplePtr meshNorm = meshNormParam.getExpandedValue().getVals();
		
		//io_sample->_nbnorm = meshNorm->size();
		if(meshNorm->size() == infos->_nbindices)
		{
			infos->_hasnormal = true;
		}
	}
	
	// check for uvs
	infos->_hasuvs = false;
	Alembic::AbcGeom::IV2fGeomParam meshUVsParam = mesh.getSchema().getUVsParam();
	if(meshUVsParam.valid())
	{
		Alembic::AbcGeom::V2fArraySamplePtr meshUVs = meshUVsParam.getExpandedValue(0).getVals();
		if(meshUVs->size() == infos->_nbindices)
		{
			infos->_hasuvs = true;
		}
	}

	infos->_hascolor = false;
	infos->_hasenvelope = false;
	
	// check for color
	infos->_hascolor = false;
	Alembic::AbcGeom::ICompoundProperty argGeomParamsProp (mesh.getSchema().getArbGeomParams());
	if(argGeomParamsProp.valid()){
		 for(long i=0;i<(long)argGeomParamsProp.getNumProperties();i++){
			 Alembic::AbcGeom::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);

			  if(h.getName() == "Colors"){
				infos->_hascolor = true;
				break;
			  }
		 }
	}

	// check for envelope
	infos->_hasenvelope = false;
	if(argGeomParamsProp.valid()){
		 for(long i=0;i<(long)argGeomParamsProp.getNumProperties();i++){
			 Alembic::AbcGeom::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);

			  if(h.getName() == "EnvelopeWeights" || h.getName() == "EnvelopeIndices"){
				infos->_hasenvelope = true;
				break;
			  }
		 }
	}
}

EXPORT int ABC_UpdatePolymeshTopoSample(AlembicIObject* obj,ABC_Polymesh_Topo_Sample_Infos* infos,ABC_Polymesh_Topo_Sample* io_sample)
{
	Alembic::AbcGeom::IPolyMesh mesh(*obj->GetIObject(),Alembic::Abc::kWrapExisting);

	Alembic::AbcGeom::IPolyMeshSchema::Sample sample;
	mesh.getSchema().get(sample,infos->_sampleindex);

	Alembic::AbcGeom::P3fArraySamplePtr meshPos = sample.getPositions();
	Alembic::AbcGeom::Int32ArraySamplePtr meshFaceCount = sample.getFaceCounts();
	Alembic::AbcGeom::Int32ArraySamplePtr meshFaceIndices = sample.getFaceIndices();


	std::vector<int> triToSample;
	
	int offset=0;
	int offset1;
	memcpy(io_sample->_positions,meshPos->get(),meshPos->size() * sizeof(Imath::V3f));

   int nbv;
   int nbt;
   int z=0;
	int last;


   // Build TriToSample Array
   for(int x=0;x<meshFaceCount->size();x++)
   {
		nbv = meshFaceCount->get()[x];
		io_sample->_facecount[x]=nbv;
		nbt = nbv-2;
		last = z+nbv-1;
		for(int y=0;y<nbt;y++){
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

   Alembic::AbcGeom::IN3fGeomParam meshNormParam = mesh.getSchema().getNormalsParam();
	if(infos->_hasnormal==true && meshNormParam.valid())
	{
		Alembic::AbcGeom::N3fArraySamplePtr meshNorm = meshNormParam.getExpandedValue().getVals();
		
		//io_sample->_nbnorm = meshNorm->size();
		if(meshNorm->size() == infos->_nbindices)
		{

			// let's apply it!
			 offset = 0;
			 int x;
			 for(int i=0;i<infos->_nbsamples;i++){
				 x = triToSample[i];
				io_sample->_normals[offset++] = meshNorm->get()[x].x;
				io_sample->_normals[offset++] = meshNorm->get()[x].y;
				io_sample->_normals[offset++] = meshNorm->get()[x].z;
			 }
		}
	}
	
	offset = 0;
	Alembic::AbcGeom::IV2fGeomParam meshUVsParam = mesh.getSchema().getUVsParam();
	if(infos->_hasuvs==true && meshUVsParam.valid())
	{
		
		Alembic::AbcGeom::V2fArraySamplePtr meshUVs = meshUVsParam.getExpandedValue(0).getVals();
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

	Alembic::AbcGeom::ICompoundProperty argGeomParamsProp(mesh.getSchema().getArbGeomParams());
	if(infos->_hascolor==true && argGeomParamsProp.valid()){
		offset = 0;	
		 for(long i=0;i<(long)argGeomParamsProp.getNumProperties();i++){
			 Alembic::AbcGeom::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);

			  if(h.getName() == "Colors"){
				Alembic::AbcGeom::IC4fArrayProperty colors(argGeomParamsProp,h.getName());
				Alembic::AbcGeom::C4fArraySamplePtr propPtr1 = colors.getValue(0);
				long nbp = (long)propPtr1->size();
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

EXPORT void ABC_UpdatePointPosition(AlembicIObject* obj,ABC_Polymesh_Topo_Sample_Infos* infos,ABC_Polymesh_Topo_Sample* io_sample){

}

EXPORT int ABC_UpdatePolymeshSample(AlembicIObject* obj,ABC_Polymesh_Topo_Sample_Infos* infos,ABC_Polymesh_Topo_Sample* io_sample)
{
	Alembic::AbcGeom::IPolyMesh mesh(*obj->GetIObject(),Alembic::Abc::kWrapExisting);

	Alembic::AbcGeom::IPolyMeshSchema::Sample sample;
	mesh.getSchema().get(sample,infos->_sampleindex);

	Alembic::AbcGeom::P3fArraySamplePtr meshPos = sample.getPositions();

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
   Alembic::AbcGeom::IN3fGeomParam meshNormParam = mesh.getSchema().getNormalsParam();
	if(meshNormParam.valid())
	{
		Alembic::AbcGeom::N3fArraySamplePtr meshNorm = meshNormParam.getExpandedValue().getVals();
		
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
