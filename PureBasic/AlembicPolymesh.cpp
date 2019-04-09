//#include "AlembicXForm.h"
#include "AlembicPolymesh.h"
#include "AlembicWriteJob.h"

BOOZE_NAMESPACE_OPEN_SCOPE

AlembicIPolymesh::AlembicIPolymesh(AbcG::IObject& object) :AlembicIObject(object)
{
	m_type = GeometricType_PolyMesh;
}

bool AlembicIPolymesh::Initialize()
{
	AbcG::IPolyMesh mesh(m_object);
	m_variance = mesh.getSchema().getTopologyVariance();
	GetProperties();
	return true;
}

BOOZE_EXPORT bool ABC_ObjectIsPolymesh(AlembicIObject* obj)
{
	return obj->GetType() == GeometricType_PolyMesh;
};

void AlembicIPolymesh::GetTopoSampleDescription(float frame, ABC_Polymesh_Topo_Sample_Infos* infos)
{
	if (!m_object.valid() || m_type != GeometricType_PolyMesh)return;

	AbcG::IPolyMesh mesh(m_object, Abc::kWrapExisting);
	if (!mesh.valid())return;

	// Get Sample Index
	AbcA::index_t sampleIndex = (AbcA::index_t)int(frame) - 1;

	// Clamp if necessary
	if (infos->m_sampleIndex < 0)
		infos->m_sampleIndex = 0;
	else if (infos->m_sampleIndex >= (AbcA::index_t)mesh.getSchema().getNumSamples())
		infos->m_sampleIndex = int(mesh.getSchema().getNumSamples()) - 1;

	AbcG::IPolyMeshSchema::Sample sample;
	mesh.getSchema().get(sample, infos->m_sampleIndex);

	infos->m_numPoints = sample.getPositions()->size();
	infos->m_numFaces = sample.getFaceCounts()->size();
	infos->m_numIndices = sample.getFaceIndices()->size();

	uint32_t nbt = 0;
	for (uint32_t i = 0; i<sample.getFaceCounts()->size(); i++){
		nbt += sample.getFaceCounts()->get()[i] - 2;
	}
	infos->m_numSamples = nbt * 3;


	// check for normals
	infos->m_hasNormal = false;
	AbcG::IN3fGeomParam meshNormParam = mesh.getSchema().getNormalsParam();
	if (meshNormParam.valid())
	{
		AbcG::N3fArraySamplePtr meshNorm = meshNormParam.getExpandedValue().getVals();

		//io_sample->_nbnorm = meshNorm->size();
		if (meshNorm->size() == infos->m_numIndices)
		{
			infos->m_hasNormal = true;
		}
	}

	// check for uvs
	infos->m_hasUvs = false;
	AbcG::IV2fGeomParam meshUVsParam = mesh.getSchema().getUVsParam();
	if (meshUVsParam.valid())
	{
		AbcG::V2fArraySamplePtr meshUVs = meshUVsParam.getExpandedValue(0).getVals();
		if (meshUVs->size() == infos->m_numIndices)
		{
			infos->m_hasUvs = true;
		}
	}

	infos->m_hasColor = false;
	infos->m_hasEnvelope = false;

	// check for color
	infos->m_hasColor = false;
	AbcG::ICompoundProperty argGeomParamsProp(mesh.getSchema().getArbGeomParams());
	if (argGeomParamsProp.valid()){
		for (uint64_t i = 0; i<(uint64_t)argGeomParamsProp.getNumProperties(); i++){
			AbcG::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);

			if (h.getName() == "Colors"){
				infos->m_hasColor = true;
				break;
			}
		}
	}

	// check for envelope
	infos->m_hasEnvelope = false;
	if (argGeomParamsProp.valid()){
		for (uint64_t i = 0; i<(uint64_t)argGeomParamsProp.getNumProperties(); i++){
			AbcG::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);

			if (h.getName() == "EnvelopeWeights" || h.getName() == "EnvelopeIndices"){
				infos->m_hasEnvelope = true;
				break;
			}
		}
	}
}

int AlembicIPolymesh::UpdateTopoSample(ABC_Polymesh_Topo_Sample_Infos* infos, ABC_Polymesh_Topo_Sample* io_sample)
{
	AbcG::IPolyMesh mesh(m_object, Abc::kWrapExisting);

	AbcG::IPolyMeshSchema::Sample sample;
	mesh.getSchema().get(sample, infos->m_sampleIndex);

	AbcG::P3fArraySamplePtr meshPos = sample.getPositions();
	AbcG::Int32ArraySamplePtr meshFaceCount = sample.getFaceCounts();
	AbcG::Int32ArraySamplePtr meshFaceIndices = sample.getFaceIndices();

	std::vector<int> triToSample;

	size_t offset = 0;
	size_t offset1;
	memcpy(io_sample->m_positions, meshPos->get(), meshPos->size() * sizeof(Imath::V3f));

	size_t nbv;
	size_t nbt;
	size_t z = 0;
	size_t last;


	// Build TriToSample Array
	for (size_t x = 0; x<meshFaceCount->size(); x++)
	{
		nbv = meshFaceCount->get()[x];
		io_sample->m_faceCount[x] = nbv;
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
	memcpy(io_sample->m_faceIndices, meshFaceIndices->get(), meshFaceIndices->size() * sizeof(uint32_t));

	AbcG::IN3fGeomParam meshNormParam = mesh.getSchema().getNormalsParam();
	if (infos->m_hasNormal == true && meshNormParam.valid())
	{
		AbcG::N3fArraySamplePtr meshNorm = meshNormParam.getExpandedValue().getVals();

		//io_sample->_nbnorm = meshNorm->size();
		if (meshNorm->size() == infos->m_numIndices)
		{

			// let's apply it!
			offset = 0;
			int x;
			for (size_t i = 0; i<infos->m_numSamples; i++){
				x = triToSample[i];
				io_sample->m_normals[offset++] = meshNorm->get()[x].x;
				io_sample->m_normals[offset++] = meshNorm->get()[x].y;
				io_sample->m_normals[offset++] = meshNorm->get()[x].z;
			}
		}
	}

	offset = 0;
	AbcG::IV2fGeomParam meshUVsParam = mesh.getSchema().getUVsParam();
	if (infos->m_hasUvs == true && meshUVsParam.valid())
	{

		AbcG::V2fArraySamplePtr meshUVs = meshUVsParam.getExpandedValue(0).getVals();
		if (meshUVs->size() == infos->m_numIndices)
		{

			// let's apply it!
			offset = 0;
			for (size_t i = 0; i<infos->m_numSamples; i++)
			{
				io_sample->m_uvs[offset++] = meshUVs->get()[triToSample[i]].x;
				io_sample->m_uvs[offset++] = meshUVs->get()[triToSample[i]].y;
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
	if (infos->m_hasColor == true && argGeomParamsProp.valid()){
		offset = 0;
		for (size_t i = 0; i<(size_t)argGeomParamsProp.getNumProperties(); i++){
			AbcG::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);

			if (h.getName() == "Colors"){
				AbcG::IC4fArrayProperty colors(argGeomParamsProp, h.getName());
				AbcG::C4fArraySamplePtr propPtr1 = colors.getValue(0);
				size_t nbp = (size_t)propPtr1->size();
				Imath::Color4f color;


				offset = 0;
				for (size_t i = 0; i<infos->m_numSamples; i++)
				{
					io_sample->m_colors[offset++] = propPtr1->get()[triToSample[i]].r;
					io_sample->m_colors[offset++] = propPtr1->get()[triToSample[i]].g;
					io_sample->m_colors[offset++] = propPtr1->get()[triToSample[i]].b;
					io_sample->m_colors[offset++] = propPtr1->get()[triToSample[i]].a;
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
	AbcG::IPolyMesh mesh(m_object, Abc::kWrapExisting);

	AbcG::IPolyMeshSchema::Sample sample;
	mesh.getSchema().get(sample, infos->m_sampleIndex);

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
	memcpy(io_sample->m_positions, &meshPos->get()[0], infos->m_numPoints * 3 * sizeof(float));
	AbcG::IN3fGeomParam meshNormParam = mesh.getSchema().getNormalsParam();
	if (meshNormParam.valid())
	{
		AbcG::N3fArraySamplePtr meshNorm = meshNormParam.getExpandedValue().getVals();

		//io_sample->_nbnorm = meshNorm->size();
		if (meshNorm->size() == infos->m_numIndices)
		{

			// let's apply it!
			offset = 0;
			for (size_t i = 0; i<meshNorm->size(); i++)
			{
				io_sample->m_normals[offset++] = meshNorm->get()[i].x;
				io_sample->m_normals[offset++] = meshNorm->get()[i].y;
				io_sample->m_normals[offset++] = meshNorm->get()[i].z;
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
   if(infos->m_sampleIndex < 0)
      infos->m_sampleIndex = 0;
   else if(infos->m_sampleIndex >= (AbcA::index_t)mesh.getSchema().getNumSamples())
      infos->m_sampleIndex = int(mesh.getSchema().getNumSamples()) - 1;
	
	AbcG::IPolyMeshSchema::Sample sample;
	mesh.getSchema().get(sample,infos->m_sampleIndex);
	
	infos->m_numPoints = sample.getPositions()->size();
	infos->m_numFaces = sample.getFaceCounts()->size();
	infos->m_numIndices = sample.getFaceIndices()->size();
	
	uint32_t nbt = 0;
	for(uint32_t i=0;i<sample.getFaceCounts()->size();i++){
		nbt  += sample.getFaceCounts()->get()[i]-2;
	}
	infos->m_numSamples = nbt*3;
	

	// check for normals
	infos->m_hasNormal = false;
	AbcG::IN3fGeomParam meshNormParam = mesh.getSchema().getNormalsParam();
	if(meshNormParam.valid())
	{
		AbcG::N3fArraySamplePtr meshNorm = meshNormParam.getExpandedValue().getVals();
		
		//io_sample->_nbnorm = meshNorm->size();
		if(meshNorm->size() == infos->m_numIndices)
		{
			infos->m_hasNormal = true;
		}
	}
	
	// check for uvs
	infos->m_hasUvs = false;
	AbcG::IV2fGeomParam meshUVsParam = mesh.getSchema().getUVsParam();
	if(meshUVsParam.valid())
	{
		AbcG::V2fArraySamplePtr meshUVs = meshUVsParam.getExpandedValue(0).getVals();
		if(meshUVs->size() == infos->m_numIndices)
		{
			infos->m_hasUvs = true;
		}
	}

	infos->m_hasColor = false;
	infos->m_hasEnvelope = false;
	
	// check for color
	infos->m_hasColor = false;
	AbcG::ICompoundProperty argGeomParamsProp (mesh.getSchema().getArbGeomParams());
	if(argGeomParamsProp.valid()){
		 for(uint64_t i=0;i<(uint64_t)argGeomParamsProp.getNumProperties();i++){
			 AbcG::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);

			  if(h.getName() == "Colors"){
				infos->m_hasColor = true;
				break;
			  }
		 }
	}

	// check for envelope
	infos->m_hasEnvelope = false;
	if(argGeomParamsProp.valid()){
		 for(uint64_t i=0;i<(uint64_t)argGeomParamsProp.getNumProperties();i++){
			 AbcG::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);

			  if(h.getName() == "EnvelopeWeights" || h.getName() == "EnvelopeIndices"){
				infos->m_hasEnvelope = true;
				break;
			  }
		 }
	}
}

BOOZE_EXPORT int ABC_UpdatePolymeshTopoSample(AlembicIObject* obj, ABC_Polymesh_Topo_Sample_Infos* infos, ABC_Polymesh_Topo_Sample* io_sample)
{
	AbcG::IPolyMesh mesh(obj->Get(),Abc::kWrapExisting);

	AbcG::IPolyMeshSchema::Sample sample;
	mesh.getSchema().get(sample,infos->m_sampleIndex);

	AbcG::P3fArraySamplePtr meshPos = sample.getPositions();
	AbcG::Int32ArraySamplePtr meshFaceCount = sample.getFaceCounts();
	AbcG::Int32ArraySamplePtr meshFaceIndices = sample.getFaceIndices();

	std::vector<int> triToSample;
	
	size_t offset=0;
	size_t offset1;
	memcpy(io_sample->m_positions,meshPos->get(),meshPos->size() * sizeof(Imath::V3f));

   	size_t nbv;
   	size_t nbt;
   	size_t z=0;
	size_t last;


   // Build TriToSample Array
   for(size_t x=0;x<meshFaceCount->size();x++)
   {
		nbv = meshFaceCount->get()[x];
		io_sample->m_faceCount[x]=nbv;
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
   memcpy(io_sample->m_faceIndices,meshFaceIndices->get(),meshFaceIndices->size() * sizeof(uint32_t));

   AbcG::IN3fGeomParam meshNormParam = mesh.getSchema().getNormalsParam();
	if(infos->m_hasNormal==true && meshNormParam.valid())
	{
		AbcG::N3fArraySamplePtr meshNorm = meshNormParam.getExpandedValue().getVals();
		
		//io_sample->_nbnorm = meshNorm->size();
		if(meshNorm->size() == infos->m_numIndices)
		{

			// let's apply it!
			 offset = 0;
			 int x;
			 for(size_t i=0;i<infos->m_numSamples;i++){
				 x = triToSample[i];
				io_sample->m_normals[offset++] = meshNorm->get()[x].x;
				io_sample->m_normals[offset++] = meshNorm->get()[x].y;
				io_sample->m_normals[offset++] = meshNorm->get()[x].z;
			 }
		}
	}
	
	offset = 0;
	AbcG::IV2fGeomParam meshUVsParam = mesh.getSchema().getUVsParam();
	if(infos->m_hasUvs==true && meshUVsParam.valid())
	{
		
		AbcG::V2fArraySamplePtr meshUVs = meshUVsParam.getExpandedValue(0).getVals();
		if(meshUVs->size() == infos->m_numIndices)
		{
			
			// let's apply it!
			 offset = 0;
			for(size_t i=0;i<infos->m_numSamples;i++)
			 {
				io_sample->m_uvs[offset++] = meshUVs->get()[triToSample[i]].x;
				io_sample->m_uvs[offset++] = meshUVs->get()[triToSample[i]].y;
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
	if(infos->m_hasColor==true && argGeomParamsProp.valid()){
		offset = 0;	
		 for(size_t i=0;i<(size_t)argGeomParamsProp.getNumProperties();i++){
			 AbcG::PropertyHeader h = argGeomParamsProp.getPropertyHeader(i);

			  if(h.getName() == "Colors"){
				AbcG::IC4fArrayProperty colors(argGeomParamsProp,h.getName());
				AbcG::C4fArraySamplePtr propPtr1 = colors.getValue(0);
				size_t nbp = (size_t)propPtr1->size();
				Imath::Color4f color;
				
				
				offset = 0;
				for(size_t i=0;i<infos->m_numSamples;i++)
				 {
					io_sample->m_colors[offset++] = propPtr1->get()[triToSample[i]].r;
					io_sample->m_colors[offset++] = propPtr1->get()[triToSample[i]].g;
					io_sample->m_colors[offset++] = propPtr1->get()[triToSample[i]].b;
					io_sample->m_colors[offset++] = propPtr1->get()[triToSample[i]].a;
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
	mesh.getSchema().get(sample,infos->m_sampleIndex);

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
	memcpy(io_sample->m_positions,&meshPos->get()[0],infos->m_numPoints*3*sizeof(float));
   AbcG::IN3fGeomParam meshNormParam = mesh.getSchema().getNormalsParam();
	if(meshNormParam.valid())
	{
		AbcG::N3fArraySamplePtr meshNorm = meshNormParam.getExpandedValue().getVals();
		
		//io_sample->_nbnorm = meshNorm->size();
		if(meshNorm->size() == infos->m_numIndices)
		{

			// let's apply it!
			 offset = 0;
			 for(size_t i=0;i<meshNorm->size();i++)
			 {
				io_sample->m_normals[offset++] = meshNorm->get()[i].x;
				io_sample->m_normals[offset++] = meshNorm->get()[i].y;
				io_sample->m_normals[offset++] = meshNorm->get()[i].z;
			 }
		}
	}

   return 0;
}

//------------------------------------------------------------------------------------------------
// Alembic Export
//------------------------------------------------------------------------------------------------
AlembicOPolymesh::AlembicOPolymesh(AlembicWriteJob* job, AlembicOObject* parent, void* customData, const char* name)
: AlembicOObject(job, parent, customData, GeometricType_PolyMesh){

	std::string xformName = name;
	std::string shapeName = name; 
	shapeName+="Shape";

	Alembic::AbcGeom::OXform xform(parent->Get(), xformName, job->GetAnimatedTs());
	Alembic::AbcGeom::OPolyMesh mesh(xform, shapeName, job->GetAnimatedTs());

	m_xform = xform.getSchema();
	m_mesh = mesh.getSchema();
	m_numSamples = 0;
};

void AlembicOPolymesh::SetPositions(Imath::V3f* positions, int32_t numVertices)
{
	m_sample.setPositions(AbcG::V3fArraySample(positions, numVertices));
}

void AlembicOPolymesh::SetDescription(int32_t* faceIndices, int32_t* faceCounts, int32_t numFaces)
{
	int numIndices = 0;
	for (unsigned i = 0; i<numFaces; i++)numIndices += faceCounts[i];
	m_sample.setFaceIndices(AbcG::Int32ArraySample(faceIndices, numIndices));
	m_sample.setFaceCounts(AbcG::Int32ArraySample(faceCounts, numFaces));
}

/*
ABCStatus AlembicOPolymesh::Set(
	Imath::V3f* positions,
	int numVertices,
	int* faceIndices,
	int* faceCounts,
	int numFaces)
{
	int numIndices = 0;
	for (unsigned i = 0; i<numFaces; i++)numIndices += faceCounts[i];
	m_meshSample.setPositions(AbcG::V3fArraySample(positions, numVertices));
	if (numFaces > 0 && numIndices > 0){
		m_meshSample.setFaceIndices(AbcG::Int32ArraySample(faceIndices, numIndices));
		m_meshSample.setFaceCounts(AbcG::Int32ArraySample(faceCounts, numFaces));
	}

	return Status_OK;
}
*/
ABCStatus AlembicOPolymesh::Save(){
	m_mesh.set(m_sample);

	// not actually the right data; just making it up
	AbcG::Box3d cbox;
	cbox.extendBy(AbcG::V3d(1.0, -1.0, 0.0));
	cbox.extendBy(AbcG::V3d(-1.0, 1.0, 3.0));
	m_mesh.getChildBoundsProperty().set(cbox);
	return Status_OK;
}

BOOZE_EXPORT bool ABC_SavePolymeshSample(
	AlembicOPolymesh* mesh,
	double time,
	Imath::V3f* positions,
	int numVertices,
	int* faceIndices=NULL,
	int* faceCount=NULL,
	int numFaces=0)
{

	/*
	OPolyMeshSchema::Sample mesh_samp(
		V3fArraySample((const V3f *)g_verts, g_numVerts),
		Int32ArraySample(g_indices, g_numIndices),
		Int32ArraySample(g_counts, g_numCounts),
		uvsamp, nsamp);
*/
	return true;
}

/*
// Create a PolyMesh class.
OPolyMesh meshyObj(OObject(archive, kTop), "meshy");
OPolyMeshSchema &mesh = meshyObj.getSchema();

// some apps can arbitrarily name their primary UVs, this function allows
// you to do that, and must be done before the first time you set UVs
// on the schema
mesh.setUVSourceName("test");

// UVs and Normals use GeomParams, which can be written or read
// as indexed or not, as you'd like.
OV2fGeomParam::Sample uvsamp(V2fArraySample((const V2f *)g_uvs,
	g_numUVs),
	kFacevaryingScope);
// indexed normals
ON3fGeomParam::Sample nsamp(N3fArraySample((const N3f *)g_normals,
	g_numNormals),
	kFacevaryingScope);

// Set a mesh sample.
// We're creating the sample inline here,
// but we could create a static sample and leave it around,
// only modifying the parts that have changed.
OPolyMeshSchema::Sample mesh_samp(
	V3fArraySample((const V3f *)g_verts, g_numVerts),
	Int32ArraySample(g_indices, g_numIndices),
	Int32ArraySample(g_counts, g_numCounts),
	uvsamp, nsamp);

// not actually the right data; just making it up
Box3d cbox;
cbox.extendBy(V3d(1.0, -1.0, 0.0));
cbox.extendBy(V3d(-1.0, 1.0, 3.0));

// Set the sample twice
mesh.set(mesh_samp);
mesh.set(mesh_samp);

// do it twice to make sure getChildBoundsProperty works correctly
mesh.getChildBoundsProperty().set(cbox);
mesh.getChildBoundsProperty().set(cbox);

// Alembic objects close themselves automatically when they go out
// of scope. So - we don't have to do anything to finish
// them off!
std::cout << "Writing: " << archive.getName() << std::endl;
*/
/*
	SaveXFormSample(prim.GetParent3DObject().GetKinematics().GetGlobal().GetRef(), _xformschema, _xformsample, time);

	// access the mesh
	PolygonMesh mesh = prim.GetGeometry(time);
	CVector3Array pos = mesh.GetVertices().GetPositionArray();
	CPolygonFaceRefArray faces = mesh.GetPolygons();
	LONG vertCount = pos.GetCount();
	LONG faceCount = faces.GetCount();
	LONG sampleCount = mesh.GetSamples().GetCount();

	Alembic::AbcGeom::OPolyMeshSchema schema = _meshschema;
	Alembic::Abc::OCompoundProperty argGeomParamsProp = _meshschema.getArbGeomParams();

	app.LogMessage(L"Vertices count : " + (CString)vertCount);
	app.LogMessage(L"Faces count : " + (CString)faceCount);
	app.LogMessage(L"Samples count : " + (CString)sampleCount);

	// allocate the points and normals
	std::vector<Alembic::Abc::V3f> posVec(vertCount);
	for (LONG i = 0; i<vertCount; i++)
	{
		posVec[i].x = (float)pos[i].GetX();
		posVec[i].y = (float)pos[i].GetY();
		posVec[i].z = (float)pos[i].GetZ();
	}

	LONG offset = 0;
	std::vector<Alembic::Abc::N3f> normalVec;
	std::vector<Alembic::Abc::V2f> uvVec;
	SIAlembicWriteJob* job = this->GetAlembicJob();

	if (job->GetOption("exportNormals") == "true")
	{
		Application().LogMessage(L"Export Normal Enabled");
		CVector3Array normals = mesh.GetVertices().GetNormalArray();
		if (job->GetOption("vertexNormals") == "true")
		{
			normalVec.resize(vertCount);
			for (LONG i = 0; i<vertCount; i++)
			{
				normalVec[i].x = (float)normals[i].GetX();
				normalVec[i].y = (float)normals[i].GetY();
				normalVec[i].z = (float)normals[i].GetZ();
			}
		}
		else
		{
			CGeometryAccessor accessor = mesh.GetGeometryAccessor(siConstructionModeSecondaryShape);
			CFloatArray shadingNormals;
			accessor.GetNodeNormals(shadingNormals);
			normalVec.resize(shadingNormals.GetCount() / 3);

			// we need to loop the faces, there is nothing we can do
			LONG offset = 0;
			for (LONG i = 0; i<faces.GetCount(); i++)
			{
				PolygonFace face(faces[i]);
				CLongArray samples = face.GetSamples().GetIndexArray();
				for (LONG j = samples.GetCount() - 1; j >= 0; j--)
				{
					LONG sampleIndex = samples[j];
					normalVec[sampleIndex].x = (float)shadingNormals[offset++];
					normalVec[sampleIndex].y = (float)shadingNormals[offset++];
					normalVec[sampleIndex].z = (float)shadingNormals[offset++];
				}
			}
		}
	}

	// allocate for the points and normals
	Alembic::Abc::P3fArraySample posSample(&posVec.front(), posVec.size());

	// if we are the first frame!
	if (_numsamples == 0)
	{
		// we also need to store the face counts as well as face indices
		std::vector<Alembic::Abc::int32_t> faceCountVec(faceCount);
		std::vector<Alembic::Abc::int32_t> faceIndicesVec(sampleCount);
		offset = 0;
		for (LONG i = 0; i<faceCount; i++)
		{
			PolygonFace face(faces[i]);
			CLongArray indices = face.GetVertices().GetIndexArray();
			faceCountVec[i] = indices.GetCount();
			for (LONG j = indices.GetCount() - 1; j >= 0; j--)
				faceIndicesVec[offset++] = indices[j];
		}

		Alembic::Abc::Int32ArraySample faceCountSample(&faceCountVec.front(), faceCountVec.size());
		Alembic::Abc::Int32ArraySample faceIndicesSample(&faceIndicesVec.front(), faceIndicesVec.size());

		_meshsample.setPositions(posSample);
		if (normalVec.size() > 0)
		{
			Alembic::AbcGeom::ON3fGeomParam::Sample normalSample(Alembic::Abc::N3fArraySample(&normalVec.front(), normalVec.size()), Alembic::AbcGeom::kFacevaryingScope);
			_meshsample.setNormals(normalSample);
		}
		_meshsample.setFaceCounts(faceCountSample);
		_meshsample.setFaceIndices(faceIndicesSample);

		// also check if we need to store UV
		CRefArray clusters = mesh.GetClusters();

		if (job->GetOption("exportUVs") == "true")

		{
			CRef uvPropRef;
			for (LONG i = 0; i<clusters.GetCount(); i++)
			{
				Cluster cluster(clusters[i]);
				if (!cluster.GetType().IsEqualNoCase(L"sample"))
					continue;
				CRefArray props(cluster.GetLocalProperties());
				for (LONG k = 0; k<props.GetCount(); k++)
				{
					ClusterProperty prop(props[k]);
					if (prop.GetType().IsEqualNoCase(L"uvspace"))
					{
						uvPropRef = props[k];
						break;
					}
				}
				if (uvPropRef.IsValid())
					break;
			}
			if (uvPropRef.IsValid())
			{
				Application().LogMessage(L"UV Prop Valid : Export It!!!");
				// ok, great, we found UVs, let's set them up
				uvVec.resize(sampleCount);
				CDoubleArray uvValues = ClusterProperty(uvPropRef).GetElements().GetArray();

				LONG offset = 0;
				for (LONG i = 0; i<faces.GetCount(); i++)
				{
					PolygonFace face(faces[i]);
					CLongArray samples = face.GetSamples().GetIndexArray();
					for (LONG j = samples.GetCount() - 1; j >= 0; j--)
					{
						LONG sampleIndex = samples[j];
						uvVec[sampleIndex].x = (float)uvValues[offset++];
						uvVec[sampleIndex].y = (float)uvValues[offset++];
						//offset++;
					}
				}

				Alembic::AbcGeom::OV2fGeomParam::Sample uvSample(Alembic::Abc::V2fArraySample(&uvVec.front(), uvVec.size()), Alembic::AbcGeom::kFacevaryingScope);
				_meshsample.setUVs(uvSample);
			}
		}
		else{
			Application().LogMessage(L"Export UV Disabled...");
		}

		// sweet, now let's have a look at face sets
		std::vector<std::vector<Alembic::Abc::int32_t>> faceSetVecs;
		if (job->GetOption("exportFaceSets") == "true")
		{
			for (LONG i = 0; i<clusters.GetCount(); i++)
			{
				Cluster cluster(clusters[i]);
				if (!cluster.GetType().IsEqualNoCase(L"poly"))
					continue;

				CLongArray elements = cluster.GetElements().GetArray();
				if (elements.GetCount() == 0)
					continue;

				std::string name(cluster.GetName().GetAsciiString());

				faceSetVecs.push_back(std::vector<Alembic::Abc::int32_t>());
				std::vector<Alembic::Abc::int32_t> & faceSetVec = faceSetVecs.back();
				for (LONG j = 0; j<elements.GetCount(); j++)
					faceSetVec.push_back(elements[j]);

				Alembic::AbcGeom::OFaceSet faceSet = _meshschema.createFaceSet(name);
				Alembic::AbcGeom::OFaceSetSchema::Sample faceSetSample(Alembic::Abc::Int32ArraySample(&faceSetVec.front(), faceSetVec.size()));
				faceSet.getSchema().set(faceSetSample);
			}
		}

		std::string export_color = job->GetOption("exportColors");
		if (export_color == "1" || export_color == "2")
		{
			Application().LogMessage(L"Export Colorz!!!");


			Alembic::Abc::OC4fArrayProperty colorProp(argGeomParamsProp, "Colors", this->AlembicOObject::GetMetaData(), job->GetAnimatedTs());

			// Export ICE Color Attribute
			if (export_color == "1"){
				Application().LogMessage(L"Export ICE Color Attribute");
				ICEAttribute color = prim.GetICEAttributeFromName("Color");


				if (color.IsDefined() && color.IsValid()){
					CICEAttributeDataArrayColor4f data;
					color.GetDataArray(data);
					std::vector<Alembic::Abc::C4f> outputColorArray;

					for (ULONG i = 0; i<data.GetCount(); i++)
					{
						Alembic::Abc::C4f c;
						c.r = data[i].GetR();
						c.g = data[i].GetG();
						c.b = data[i].GetB();
						c.a = 1.0;
						outputColorArray.push_back(c);

					}

					colorProp.set(Alembic::Abc::C4fArraySample(outputColorArray));

				}
				else{
					Application().LogMessage(L"No COLOR ICE Attribute on " + prim.GetParent3DObject().GetFullName() + L" -----> Write Colors Skipped...");
				}
			}
			// Export Vertex Color Map(if any...)
			else{
				Application().LogMessage(L"Export Vertex Color Map");
				CGeometryAccessor accessor = mesh.GetGeometryAccessor(siConstructionModeSecondaryShape);
				CRefArray  colors = accessor.GetVertexColors();
				std::vector<Alembic::Abc::C4f> outputColorArray;

				if (colors.GetCount()>0){
					ClusterProperty cp = colors[0];
					CBitArray elemSet;
					CFloatArray colorValues;
					cp.GetValues(colorValues, elemSet);

					std::vector<Alembic::Abc::C4f> outputColorArray;

					outputColorArray.resize(sampleCount);;

					LONG offset = 0;
					for (LONG i = 0; i<faces.GetCount(); i++)
					{
						PolygonFace face(faces[i]);
						CLongArray samples = face.GetSamples().GetIndexArray();
						for (LONG j = samples.GetCount() - 1; j >= 0; j--)
						{
							LONG sampleIndex = samples[j];
							outputColorArray[sampleIndex].r = (float)colorValues[offset++];
							outputColorArray[sampleIndex].g = (float)colorValues[offset++];
							outputColorArray[sampleIndex].b = (float)colorValues[offset++];
							outputColorArray[sampleIndex].a = (float)colorValues[offset++];
						}
					}

					//}

					colorProp.set(Alembic::Abc::C4fArraySample(outputColorArray));
				}

				else{
					Application().LogMessage(L"No Vertex Color Map on " + prim.GetParent3DObject().GetFullName() + L" -----> Write Colors Skipped...");
				}
			}

		}

		if (job->GetOption("exportEnvelope") == "true")
		{
			// find envelope
			CGeometryAccessor accessor = mesh.GetGeometryAccessor();
			if (accessor.GetEnvelopeWeights().GetCount()>0)
			{
				EnvelopeWeight envelope = accessor.GetEnvelopeWeights().GetItem(0);
				CRefArray deformers = envelope.GetDeformers();
				CFloatArray weights;
				envelope.GetValues(weights);

				LONG out;
				Application().GetUIToolkit().MsgBox(L"Weights Size : " + (CString)weights.GetCount(), XSI::siMsgInformation, L"Export Envelope", out);
				Alembic::AbcGeom::OPolyMeshSchema schema = _meshschema;
				Alembic::Abc::OCompoundProperty argGeomParamsProp = _meshschema.getArbGeomParams();
				Alembic::AbcCoreAbstract::MetaData metadata = this->AlembicOObject::GetMetaData();
				Alembic::Abc::OC4cArrayProperty indicesProp(argGeomParamsProp, "EnvelopeIndices", metadata, job->GetAnimatedTs());
				Alembic::Abc::OC4fArrayProperty weightsProp(argGeomParamsProp, "EnvelopeWeights", metadata, job->GetAnimatedTs());
				Alembic::Abc::OUInt32Property nbDeformersProp(argGeomParamsProp, "EnvelopeNbDeformers", metadata, job->GetAnimatedTs());

				std::vector<Alembic::Abc::C4c> indicesArray;
				std::vector<Alembic::Abc::C4f> weightsArray;

				LONG nbp = weights.GetCount() / deformers.GetCount();
				indicesArray.resize(nbp);
				weightsArray.resize(nbp);

				Alembic::Abc::C4c idx;
				Alembic::Abc::C4f wgt;
				CString s_idx;
				CString s_wgt;
				for (LONG i = 0; i<nbp; i++){
					EncodeEnvelope(weights, indicesArray[i], weightsArray[i], i, deformers.GetCount());
					s_idx = L"Vertex " + (CString)i + L" IDs :: (" + (CString)(LONG)indicesArray[i].r + L"," + (CString)(LONG)indicesArray[i].g + L"," + (CString)(LONG)indicesArray[i].b + L"," + (CString)(LONG)indicesArray[i].a + L")";
					s_wgt = L"Weights " + (CString)i + L" Weights :: (" + (CString)weightsArray[i].r + L"," + (CString)weightsArray[i].g + L"," + (CString)weightsArray[i].b + L"," + (CString)weightsArray[i].a + L")";
					Application().LogMessage(s_idx);
					Application().LogMessage(s_wgt);

				}

				indicesProp.set(Alembic::Abc::C4cArraySample(indicesArray));
				weightsProp.set(Alembic::Abc::C4fArraySample(weightsArray));
				nbDeformersProp.set(Alembic::Abc::uint32_t(deformers.GetCount()));
			}
			
			LONG out;
			Application().GetUIToolkit().MsgBox(L"Export Envelope",XSI::siMsgInformation,L"hohoho",out);
			
		}

		_meshschema.set(_meshsample);
	}
	else
	{
		_meshsample.reset();
		_meshsample.setPositions(posSample);
		if (normalVec.size() > 0)
		{
			Alembic::AbcGeom::ON3fGeomParam::Sample normalSample(Alembic::Abc::N3fArraySample(&normalVec.front(), normalVec.size()), Alembic::AbcGeom::kFacevaryingScope);
			_meshsample.setNormals(normalSample);
		}
		_meshschema.set(_meshsample);

	}
	_numsamples++;


	return Status_OK;
}
*/

BOOZE_NAMESPACE_CLOSE_SCOPE