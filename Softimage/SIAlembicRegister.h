#ifndef _SIALEMBIC_REGISTER_H_
#define _SIALEMBIC_REGISTER_H_

//------------------------------------------------
// Softimage SDK Includes
//------------------------------------------------
#include <xsi_application.h>
#include <xsi_context.h>
#include <xsi_pluginregistrar.h>
#include <xsi_status.h>
#include <xsi_argument.h>
#include <xsi_command.h>
#include <xsi_menu.h>
#include <xsi_uitoolkit.h>
#include <xsi_progressbar.h>
#include <xsi_comapihandler.h>
#include <xsi_project.h>
#include <xsi_selection.h>
#include <xsi_model.h>
#include <xsi_camera.h>
#include <xsi_customoperator.h>
#include <xsi_operatorcontext.h>
#include <xsi_kinematics.h>
#include <xsi_kinematicstate.h>
#include <xsi_factory.h>
#include <xsi_primitive.h>
#include <xsi_math.h>
#include <xsi_cluster.h>
#include <xsi_clusterproperty.h>
#include <xsi_primitive.h>
#include <xsi_geometry.h>
#include <xsi_geometryaccessor.h>
#include <xsi_polygonmesh.h>
#include <xsi_nurbscurvelist.h>
#include <xsi_vertex.h>
#include <xsi_triangle.h>
#include <xsi_polygonface.h>
#include <xsi_sample.h>
#include <xsi_ppglayout.h>
#include <xsi_ppgitem.h>
#include <xsi_ppgeventcontext.h>
#include <xsi_ref.h>
#include <xsi_doublearray.h>
#include <xsi_scene.h>
#include <xsi_string.h>
#include <xsi_vertexcolor.h>
#include <xsi_envelopeweight.h>
#include <xsi_status.h>

//-----------------------------------------
// ICE SDK includes
//-----------------------------------------
#include <xsi_icenodecontext.h>
#include <xsi_icenodedef.h>
#include <xsi_vector2f.h>
#include <xsi_vector3f.h>
#include <xsi_vector4f.h>
#include <xsi_matrix3f.h>
#include <xsi_matrix4f.h>
#include <xsi_rotationf.h>
#include <xsi_quaternionf.h>
#include <xsi_color4f.h>
#include <xsi_shape.h>
#include <xsi_icegeometry.h>
#include <xsi_iceportstate.h>
#include <xsi_indexset.h>
#include <xsi_dataarray.h>
#include <xsi_dataarray2D.h>
#include <xsi_iceattribute.h>
#include <xsi_iceattributedataarray.h>
#include <xsi_iceattributedataarray2D.h>
#include <xsi_boolarray.h>


using namespace XSI; 
using namespace MATH; 

// Alembic Includes
#include "AlembicArchiveStorage.h"

//Global Archive Manager
static AlembicArchiveManager abc_archive_manager;

//-------------------------------------------------
// Constants
//-------------------------------------------------
static unsigned char AlembicNodeR = 177;
static unsigned char AlembicNodeG = 193;
static unsigned char AlembicNodeB = 177;

//-------------------------------------------------
// ICE Nodes Common ID ports
//-------------------------------------------------
enum ABCInputPortIDs
{
	ID_IN_Path = 0,
	ID_IN_Identifier = 1,
	ID_IN_Frame = 2,
	ID_G_100 = 100,
	ID_OUT_XForm = 201,
	ID_TYPE_CNS = 400,
	ID_STRUCT_CNS,
	ID_CTXT_CNS,
	ID_UNDEF = ULONG_MAX
};


#endif // _SIALEMBIC_REGISTER_H_