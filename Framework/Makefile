# Includes
BOOST_INCLUDES = /Users/benmalartre/Documents/RnD/boost_1_49_0
TR1_INCLUDES = /Users/benmalartre/Documents/RnD/boost_1_49_0/boost/tr1
ILM_INCLUDES = /usr/local/include/OpenEXR
HDF5_INCLUDES = /Users/benmalartre/Documents/RnD/hdf5-1.8.7/include
ABC_INCLUDES = /Users/benmalartre/Documents/RnD/alembic-1_05_08/lib
BOOZE_INCLUDES = /Users/benmalartre/Documents/RnD/booze/Framework
Z_INCLUDES = /Users/benmalartre/Documents/RnD/zlib-1.2.8
PB_INCLUDES = /Applications/PureBasic/sdk/c/PureLibraries
GLEW_INCLUDES = /Users/benmalartre/Documents/RnD/glew/include
GL_INCLUDES = /Applications/Unity/PlaybackEngines/WebGLSupport/BuildTools/Emscripten/system/include

CFLAGS +=  -DPLATFORM_DARWIN -std=c++11 -fPIC -arch x86_64 -DNDEBUG -c -I$(GL_INCLUDES) -I$(GLEW_INCLUDES) -I$(BOOST_INCLUDES) -I$(TR1_INCLUDES) -I$(ABC_INCLUDES) -I$(ILM_INCLUDES) -I$(BOOZE_INCLUDES) -I$(Z_INCLUDES) -I$(HDF5_INCLUDES) -I$(PB_INCLUDES) -static

FRAMEWORK_PATH = -L/Users/benmalartre/Documents/RnD/booze/Framework
ALEMBIC_PATH = -L/usr/local/alembic-1.5.8/lib/static
HDF5_PATH = -L/Users/benmalartre/Documents/RnD/hdf5-1.8.7/lib
BOOST_PATH = -L/Users/benmalartre/Documents/RnD/boost_1_49_0/stage/lib
Z_PATH = -L/Users/benmalartre/Documents/RnD/zlib-1.2.8
ILM_PATH = -L/usr/local/lib

#-isysroot 
#/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk 
#-Wl,-search_paths_first -Wl,-headerpad_max_install_names  
#CMakeFiles/SimpleAbcViewer.dir/Transport.cpp.o CMakeFiles/SimpleAbcViewer.dir/Viewer.cpp.o CMakeFiles/SimpleAbcViewer.dir/main.cpp.o  -o SimpleAbcViewer  
LIBS = /Users/benmalartre/Documents/RnD/alembic_build/lib/AbcOpenGL/libAlembicAbcOpenGL.a 
LIBS += /Users/benmalartre/Documents/RnD/alembic_build/lib/Alembic/AbcGeom/libAlembicAbcGeom.a 
LIBS += /Users/benmalartre/Documents/RnD/alembic_build/lib/Alembic/AbcCoreFactory/libAlembicAbcCoreFactory.a 
LIBS += /Users/benmalartre/Documents/RnD/alembic_build/lib/Alembic/Abc/libAlembicAbc.a 
LIBS += /Users/benmalartre/Documents/RnD/alembic_build/lib/Alembic/AbcCoreHDF5/libAlembicAbcCoreHDF5.a 
LIBS += /Users/benmalartre/Documents/RnD/alembic_build/lib/Alembic/AbcCoreOgawa/libAlembicAbcCoreOgawa.a 
LIBS += /Users/benmalartre/Documents/RnD/alembic_build/lib/Alembic/AbcCoreAbstract/libAlembicAbcCoreAbstract.a 
LIBS += /Users/benmalartre/Documents/RnD/alembic_build/lib/Alembic/AbcCollection/libAlembicAbcCollection.a 
LIBS += /Users/benmalartre/Documents/RnD/alembic_build/lib/Alembic/Ogawa/libAlembicOgawa.a 
LIBS += /Users/benmalartre/Documents/RnD/alembic_build/lib/Alembic/Util/libAlembicUtil.a 
LIBS += /Users/benmalartre/Documents/RnD/hdf5-1.8.7/lib/libhdf5.a 
LIBS += /Users/benmalartre/Documents/RnD/hdf5-1.8.7/lib/libhdf5_hl.a 
#LIBS += /Users/benmalartre/Documents/RnD/hdf5-1.8.7/lib/libsz.a 
LIBS += /Users/benmalartre/Documents/RnD/hdf5-1.8.7/lib/libz.a 
#LIBS += /usr/lib/libm.dylib 
#LIBS += -framework GLUT 
#LIBS += -framework Cocoa 
#LIBS += -framework OpenGL 
LIBS += /usr/local/lib/libImath.a 
LIBS += /usr/local/lib/libIlmThread.a 
LIBS += /usr/local/lib/libIex.a 
LIBS += /usr/local/lib/libHalf.a 
LIBS += -lm 

LIBS_PATH = $(FRAMEWORK_PATH) $(ALEMBIC_PATH) $(HDF5_PATH) $(BOOST_PATH) $(Z_PATH) $(ILM_PATH)
#LIBS =  -lboost_program_options -lboost_thread -lboost_python -lAlembicAbc -lAlembicAbcCollection -lAlembicAbcCoreAbstract -lAlembicAbcCoreFactory -lAlembicAbcCoreHDF5 -lAlembicAbcCoreOgawa -lAlembicAbcGeom -lAlembicUtil -lpthread -lz -lhdf5 -lhdf5_hl -lImath -lIlmThread -lIex -lHalf

LDFLAGS += -static $(LIBS)

# Rules

all: AbcUtil.o AbcArchiveStorage.o AbcMessage.o AbcObject.o AbcProperty.o AbcWriteJob.o libAbcFramework.a
	
AbcUtil.o: AlembicUtil.cpp AlembicUtil.h
	g++ $(CFLAGS) -c -Wall -g AlembicUtil.cpp
	
AbcArchiveStorage.o: AlembicArchiveStorage.cpp AlembicArchiveStorage.h
	g++ $(CFLAGS) -c -Wall -g AlembicArchiveStorage.cpp

AbcMessage.o: AlembicMessage.cpp AlembicMessage.h
	g++ $(CFLAGS) -c -Wall -g AlembicMessage.cpp
	
AbcObject.o: AlembicObject.cpp AlembicObject.h
	g++ $(CFLAGS) -c -Wall -g AlembicObject.cpp

AbcProperty.o: AlembicProperty.cpp AlembicProperty.h
	g++ $(CFLAGS) -c -Wall -g AlembicProperty.cpp
	
AbcWriteJob.o: AlembicWriteJob.cpp AlembicWriteJob.h
	g++ $(CFLAGS) -c -Wall -g AlembicWriteJob.cpp

libAbcFramework.a:  AlembicUtil.o AlembicArchiveStorage.o AlembicMessage.o AlembicObject.o  AlembicProperty.o AlembicWriteJob.o
	        ar rcs $@ $^ 

clean:
	rm -rf *.o *.gah