#include <AlembicWriteJob.h>
#include <AlembicArchive.h>
#include <AlembicObject.h>
#include <AlembicRoot.h>
#include <AlembicXForm.h>
#include <iostream>

using namespace BOOZE;

void randomTransform(ABCOObjectPtr& obj)
{
	// add a couple of ops
	AbcG::XformOp transop(AbcG::kTranslateOperation, AbcG::kTranslateHint);
	AbcG::XformOp scaleop(AbcG::kScaleOperation, AbcG::kScaleHint);

	AbcG::XformSample sample;
	sample.addOp(transop, AbcG::V3f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX));
	float s = (float)rand() / (float)RAND_MAX * 0.5f + 0.5f;
	sample.addOp(scaleop, AbcG::V3f(s, s, s));

	AbcU::shared_ptr< AbcG::OXform > xfoPtr = AbcU::dynamic_pointer_cast< AbcG::OXform >(obj);
	Alembic::AbcGeom::OXformSchema &schema = xfoPtr->getSchema();

	schema.set(sample);
}


struct ABCCollection
{
	AlembicOObject*					m_root;
	std::vector<AlembicOObject*>		m_objects;
};

// init
void phase1(AlembicWriteJob* job, ABCCollection& collec){
	AlembicOObject* root = job->getArchive()->getRoot();
	AlembicOObject* A = job->getArchive()->addObject(root, "test1", GeometricType_XForm, NULL);
	AlembicOObject* B = job->getArchive()->addObject(A, "test2", GeometricType_XForm, NULL);
	AlembicOObject* C = job->getArchive()->addObject(B, "test3", GeometricType_XForm, NULL);
	/*
	AlembicOObject* root = job->getArchive()->getRoot();
	std::cout << "created root" << std::endl;
	AlembicOXForm* A = new AlembicOXForm(job->getArchive(), root, NULL, "xfo1");
	std::cout << "created xfo1" << std::endl;
	
	AlembicOObject* B = new AlembicOXForm(job->getArchive(), A, NULL, "xfo2");
	std::cout << "created xfo2" << std::endl;
	AlembicOObject* C = new AlembicOXForm(job->getArchive(), B, NULL, "xfo3");
	std::cout << "created xfo3" << std::endl;
	*/
	collec.m_objects.push_back(A);
	collec.m_objects.push_back(B);
	collec.m_objects.push_back(C);

}

// write frames
void phase2(AlembicWriteJob* job, ABCCollection& collec){
	for (int32_t i = 0; i < collec.m_objects.size(); i++){
		std::cout << "random deform" << std::endl;
		randomTransform(collec.m_objects[i]->getPtr());
	}
}

// clean u p
void phase3(AlembicWriteJob* job, ABCCollection& collec){
	for (int32_t i = 0; i < collec.m_objects.size(); i++){
		delete collec.m_objects[i];
		std::cout << "clean (should save)" << std::endl;
	}
	collec.m_objects.clear();
}

int main(){

	float frames[10] = { 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f };
	std::string filename = "E:/Projects/RnD/Noodle/abc/Export.abc";
	AlembicWriteJob* job = new AlembicWriteJob(filename.c_str(), &frames[0], 10);
	job->setFramerate(6);
	AlembicOArchive* archive = job->getArchive();
	ABCCollection collec;
	collec.m_root = job->getArchive()->getRoot();
	std::cout << "ARCHIVE NUM OBJECTS : " << archive->getNumObjects() << std::endl;
	//phase0(job);
	phase1(job, collec);
	for (unsigned i = 0; i < 12; i++){
		
		phase2(job, collec);
	}
	phase3(job, collec);
	std::cout << "READY TO DEBUG MOTHER FUCKER" << std::endl;

	delete job;
}