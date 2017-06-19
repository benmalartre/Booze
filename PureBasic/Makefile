# Alembic makefile

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

CFLAGS += -DPB_MACOS -std=c++11 -fPIC -arch x86_64 -DNDEBUG -c -I$(BOOST_INCLUDES) -I$(TR1_INCLUDES) -I$(ABC_INCLUDES) -I$(ILM_INCLUDES) -I$(BOOZE_INCLUDES) -I$(Z_INCLUDES) -I$(HDF5_INCLUDES) -I$(PB_INCLUDES) -I$(GLEW_INCLUDES) -I$(GL_INCLUDES) -static
FRAMEWORK_PATH = -L/Users/benmalartre/Documents/RnD/booze/Framework
ALEMBIC_PATH = -L/usr/local/alembic-1.5.8/lib/static
HDF5_PATH = -L/Users/benmalartre/Documents/RnD/hdf5-1.8.7/lib
BOOST_PATH = -L/Users/benmalartre/Documents/RnD/boost_1_49_0/stage/lib
Z_PATH = -L/Users/benmalartre/Documents/RnD/zlib-1.2.8
ILM_PATH = -L/usr/local/lib
PB_PATH = -L/

LIBS += /Users/benmalartre/Documents/RnD/alembic_build/lib/AbcOpenGL/libAlembicAbcOpenGL.a
LIBS += /Users/benmalartre/Documents/RnD/alembic_build/lib/Alembic/AbcGeom/libAlembicAbcGeom.a
LIBS += /Users/benmalartre/Documents/RnD/alembic_build/lib/Alembic/AbcCoreFactory/libAlembicAbcCoreFactory.a
LIBS += /Users/benmalartre/Documents/RnD/alembic_build/lib/Alembic/Abc/libAlembicAbc.a
LIBS += /Users/benmalartre/Documents/RnD/alembic_build/lib/Alembic/AbcCoreHDF5/libAlembicAbcCoreHDF5.a
LIBS += /Users/benmalartre/Documents/RnD/alembic_build/lib/Alembic/AbcCoreOgawa/libAlembicAbcCoreOgawa.a
LIBS += /Users/benmalartre/Documents/RnD/alembic_build/lib/Alembic/AbcCoreAbstract/libAlembicAbcCoreAbstract.a
LIBS += /Users/benmalartre/Documents/RnD/alembic_build/lib/Alembic/Ogawa/libAlembicOgawa.a
LIBS += /Users/benmalartre/Documents/RnD/alembic_build/lib/Alembic/Util/libAlembicUtil.a
LIBS += /Users/benmalartre/Documents/RnD/hdf5-1.8.7/lib/libhdf5.a
LIBS += /Users/benmalartre/Documents/RnD/hdf5-1.8.7/lib/libsz.a
LIBS += /Users/benmalartre/Documents/RnD/hdf5-1.8.7/lib/libz.a
LIBS += /Users/benmalartre/Documents/RnD/hdf5-1.8.7/lib/libhdf5_hl.a
#LIBS += /usr/lib/libm.dylib
#LIBS += -framework GLUT
#LIBS += -framework Cocoa
#LIBS += -framework OpenGL
LIBS += /usr/local/lib/libImath.a
LIBS += /usr/local/lib/libIlmThread.a
LIBS += /usr/local/lib/libIex.a
LIBS += /usr/local/lib/libHalf.a
LIBS += /Users/benmalartre/Documents/RnD/zlib-1.2.8/libz.a
LIBS += /Users/benmalartre/Documents/RnD/booze/Framework/libAbcFramework.a
LIBS += /Users/benmalartre/Documents/RnD/boost_1_49_0/stage/lib/libboost_program_options.a
LIBS += /Users/benmalartre/Documents/RnD/boost_1_49_0/stage/lib/libboost_python.a
LIBS += /Users/benmalartre/Documents/RnD/boost_1_49_0/stage/lib/libboost_thread.a
LIBS += /Applications/PureBasic/compilers/ObjectManager.a
LIBS += /Applications/PureBasic/compilers/StringManager.a

# Rules
#Compile-only with -c option
All: Archive.o Object.o XForm.o Polymesh.o PointCloud.o Attribute.o PBAlembic.so 
Archive.o:
	g++ $(CFLAGS) -c -Wall -g PBAlembicArchive.cpp
Object.o:
	g++ $(CFLAGS) -c -Wall -g PBAlembicObject.cpp PBAlembicObject.h
Polymesh.o:
	g++ $(CFLAGS) -c -Wall -g PBAlembicPolymesh.cpp PBAlembicPolymesh.h
PointCloud.o:
	g++ $(CFLAGS) -c -Wall -g PBAlembicPointCloud.cpp PBAlembicPointCloud.h
XForm.o:
	g++ $(CFLAGS) -c -Wall -g PBAlembicXForm.cpp PBAlembicXForm.h
Attribute.o:
	g++ $(CFLAGS) -c -Wall -g PBAlembicAttribute.cpp PBAlembicAttribute.h

# link in shared library
PBAlembic.so:
	g++ $(LIBS_PATH) -o PBAlembic.so -shared -Wl,-search_paths_first -Wl,-headerpad_max_install_names $(LIBS) -lm PBAlembicArchive.o PBAlembicAttribute.o PBAlembicObject.o PBAlembicXForm.o PBAlembicPointCloud.o PBAlembicPolymesh.o
	
clean:
	rm -rf *.o *.gch *.so