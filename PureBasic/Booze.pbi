﻿; ============================================================================
;  Alembic Module
; ............................................................................
;  ILM/Sony Cache File Format Import Functions
; ============================================================================
;  2013/06/05 | Ben Malartre
;  - creation
; 
; ============================================================================
; XIncludeFile "../core/Math.pbi"
; XIncludeFile "../core/Time.pbi"
; XIncludeFile "../core/Array.pbi"
; XIncludeFile "../objects/Model.pbi"
; XIncludeFile "../objects/Polymesh.pbi"
; XIncludeFile "../objects/InstanceCloud.pbi"


; ============================================================================
;  Alembic Module Declaration
; ============================================================================
DeclareModule Alembic
  Global.b ALEMBIC_WITH_HDF5
  Global.s ALEMBIC_VERSION
  Macro ABCGeometricType : l : EndMacro
  Enumeration
    #ABC_OBJECT_UNKNOWN = 0
    #ABC_OBJECT_XFORM
    #ABC_OBJECT_POINTCLOUD
    #ABC_OBJECT_CURVE
    #ABC_OBJECT_POLYMESH
    #ABC_OBJECT_SUBD
    #ABC_OBJECT_FACESET
    #ABC_OBJECT_NUPATCH
    #ABC_OBJECT_CAMERA
    #ABC_OBJECT_LIGHT
  EndEnumeration
  
  Macro ABCPODType : l : EndMacro
  Enumeration
    #ABC_PodType_Boolean
    #ABC_PodType_UInt8
    #ABC_PodType_Int8
    #ABC_PodType_UInt16
    #ABC_PodType_Int16
    #ABC_PodType_UInt32
    #ABC_PodType_Int32
    #ABC_PodType_UInt64
    #ABC_PodType_Int64
    #ABC_PodType_Float16
    #ABC_PodType_Float32
    #ABC_PodType_Float64

    #ABC_PodType_String
    #ABC_PodType_WString
    #ABC_PodType_NumPods
    #ABC_PodType_Unknown
  EndEnumeration
  
  Macro ABCPropertyType : l : EndMacro
  Enumeration
  	#ABC_PropertyType_Compound        ; 0 < A compound property which may contain other properties */
  	#ABC_PropertyType_Scalar          ; 1 < A single value property */
  	#ABC_PropertyType_Array           ; 2 < A property With an Array of values */
  	#ABC_PropertyType_Unknown = 127   ;   < Unknown To the framework */
	EndEnumeration

  Macro ABCGeometryScope : l : EndMacro
  Enumeration
    #ABC_kConstantScope
    #ABC_kUniformScope
    #ABC_kVaryingScope
    #ABC_kVertexScope
    #ABC_kFacevaryingScope
    #ABC_kUnknownScope = 127
  EndEnumeration
  
  Macro ABCTopoVariance : l : EndMacro
  Enumeration
    #ABC_TopoVariance_Constant
    #ABC_TopoVariance_Homogenous
    #ABC_TopoVariance_Heterogenous
  EndEnumeration
  
  Macro ABCDataTraits : l :EndMacro
  Enumeration

  	#ABC_DataTraits_Bool = 0
  	#ABC_DataTraits_UChar
  	#ABC_DataTraits_Char
  	

  	#ABC_DataTraits_UInt16
  	#ABC_DataTraits_Int16
  	
  	#ABC_DataTraits_UInt32
  	#ABC_DataTraits_Int32
  	
  	#ABC_DataTraits_UInt64
  	#ABC_DataTraits_Int64
  	
  	#ABC_DataTraits_Half
  	#ABC_DataTraits_Float
  	#ABC_DataTraits_Double
  	
  	#ABC_DataTraits_String
  	#ABC_DataTraits_WString
  
  	#ABC_DataTraits_V2s
  	#ABC_DataTraits_V2i
  	#ABC_DataTraits_V2f
  	#ABC_DataTraits_V2d
  
  	#ABC_DataTraits_V3s
  	#ABC_DataTraits_V3i
  	#ABC_DataTraits_V3f
  	#ABC_DataTraits_V3d
  
  	#ABC_DataTraits_P2s
  	#ABC_DataTraits_P2i
  	#ABC_DataTraits_P2f
  	#ABC_DataTraits_P2d
  
  	#ABC_DataTraits_P3s
  	#ABC_DataTraits_P3i
  	#ABC_DataTraits_P3f
  	#ABC_DataTraits_P3d
  
  	#ABC_DataTraits_Box2s
  	#ABC_DataTraits_Box2i
  	#ABC_DataTraits_Box2f
  	#ABC_DataTraits_Box2d
  
  	#ABC_DataTraits_Box3s
  	#ABC_DataTraits_Box3i
  	#ABC_DataTraits_Box3f
  	#ABC_DataTraits_Box3d
  
  	#ABC_DataTraits_M33f
  	#ABC_DataTraits_M33d
  	#ABC_DataTraits_M44f
  	#ABC_DataTraits_M44d
  
  	#ABC_DataTraits_Quatf
  	#ABC_DataTraits_Quatd
  
  	#ABC_DataTraits_C3h
  	#ABC_DataTraits_C3f
  	#ABC_DataTraits_C3c
  
  	#ABC_DataTraits_C4h
  	#ABC_DataTraits_C4f
  	#ABC_DataTraits_C4c
  
  	#ABC_DataTraits_N2f
  	#ABC_DataTraits_N2d
  
  	#ABC_DataTraits_N3f
  	#ABC_DataTraits_N3d
  
    #ABC_DataTraits_V4f
  
  	#ABC_DataTraits_Rotf
  
  	#ABC_DataTraits_NumDataTypes
  	#ABC_DataTraits_Unknown

  EndEnumeration
  
  Macro ABCArchiveType : l :EndMacro
  Enumeration
	  #ABC_ArchiveType_HDF5			;/*! Archive is an HDF5 archive */
	  #ABC_ArchiveType_Ogawa		;/*! Archive is an Ogawa archive */
	  #ABC_ArchiveType_Any = 127;/*! Don't know what archive type it is */
	EndEnumeration
	

  ;----------------------------------------
  ;Object Infos
  ;----------------------------------------
  Structure ABC_Object_Infos
    *name
    type.i
    *obj
  EndStructure
  
  ;----------------------------------------
  ;XForm Sample
  ;----------------------------------------
  Structure ABC_XForm_Sample
    pos.f[3]
    ori.f[4]
    scl.f[3]
  EndStructure
  
  ;----------------------------------------
  ;Polymesh Topology Sample
  ;-----------------------------------------
  Structure ABC_Polymesh_Topo_Sample_Infos
    nbpoints.i
    nbfacecount.i
    nbindices.i
    nbsamples.i
    sampleindex.i
    
    hasvelocity.b
  	hasnormal.b
  	hascolor.b
  	hasuvs.b
  	hastangent.b
  	hasenvelope.b
  EndStructure
  
  Structure ABC_Polymesh_TopoSimple_Sample
    *positions
  	*faceindices
  	*facecount  
  EndStructure
  
  Structure ABC_Polymesh_Topo_Sample
    *positions
    *velocities 
    *normals
    *tangents
  	*uvs
  	*colors
  	*faceindices
  	*facecount  
  EndStructure
  
  Structure ABC_Envelope_Sample
    nbdeformers.i
    *indices
    *weights 
  EndStructure
  
  ;----------------------------------------
  ;Polymesh Sample
  ;-----------------------------------------
  Structure ABC_Polymesh_Sample_Infos
    nbpoints.i
    hasvelocity.b
  	sampleindex.i
  EndStructure

  Structure ABC_Polymesh_Sample
    *positions
    *velocities 
  EndStructure
  
  ;----------------------------------------
  ;PointCloud Sample
  ;-----------------------------------------
  Structure ABC_PointCloud_Sample_Infos
    nbpoints.i
    sampleindex.i
    b_velocity.b
	  b_size.b
	  b_orientation.b
  	b_scale.b
  	b_color.b
  EndStructure
  
  Structure ABC_PointCloud_Sample
    *id
    *position
    *velocity
  	*size
  	*orientation
  	*scale
  	*color
  EndStructure
  
  ;----------------------------------------
  ;Skeleton Sample
  ;-----------------------------------------
  Structure ABC_Skeleton_Sample_Infos
  	nbpoints.i
  	sampleindex.i
  EndStructure


  Structure ABC_Skeleton_Sample
  	*id
  	*position
  	*orientation
  	*scale
  	*staticposition
  	*staticorientation
  	*staticscale
  	*color
  EndStructure

  
  ;----------------------------------------
  ;Attribute Sample
  ;-----------------------------------------
  Structure ABC_Attribute_Sample_Infos
    nbitems.i
  	time.f
    type.ABCPropertyType
  	traits.ABCDataTraits
    name.c[32]
  EndStructure

  Structure ABC_Attribute_Sample
  	*datas
  EndStructure
  
  
  ;-----------------------------------------
  ; Property Opaque Structure
  ;-----------------------------------------
  Structure ABC_Property
  EndStructure
  
  ;-----------------------------------------
  ; TimeSampling Opaque Structure
  ;-----------------------------------------
  Structure ABC_TimeSampling
  EndStructure
  
  ;-----------------------------------------
  ; CPP Interfaces Objects
  ;-----------------------------------------
  ; IArchiveManager
  Interface IArchiveManager
    OpenArchive.l(filename.p-utf8)
	  CloseArchive.b(filename.p-utf8)
	  GetNumOpenArchives.i()
	  CloseAllArchives.b()
  EndInterface
  
  ; IArchive
  Interface IArchive
    Open.b(fp.p-utf8)
    Close.b()
    IsValid.b()
    GetInfos()
    GetNumObjects.i()
    GetIdentifier.l(index.i)
    GetObject.l(index.i)
    Get.l()
    IncrementUses.l()
    DecrementUses.l()
    NumUses.l()
  EndInterface 

  ; IObject
  Interface IObject
    GetName.l()
    GetFullName.l()
    GetType.l()
    GetNumProperties.i()
    Get.l()
    Initialize.b()
  	GetProperties.l()
  	GetProperty.l(index.i)
  EndInterface
  
  ; IPolymesh
  Interface IPolymesh Extends IObject
    GetTopoSampleDescription.l(time.f, *infos.ABC_Polymesh_Topo_Sample_Infos)
    UpdateTopoSample.l(*infos.ABC_Polymesh_Topo_Sample_Infos, *sample.ABC_Polymesh_Topo_Sample)
    GetSampleDescription.l(time.f, *infos.ABC_Polymesh_Topo_Sample_Infos)
    UpdateSample.l(*infos.ABC_Polymesh_Topo_Sample_Infos, *sample.ABC_Polymesh_Topo_Sample)
  EndInterface
  
  ; IPoints
  Interface IPoints Extends IObject
    GetSampleDescription.l(time.f, *infos.ABC_Polymesh_Topo_Sample_Infos)
    UpdateSample.l(*infos.ABC_Polymesh_Topo_Sample_Infos, *sample.ABC_Polymesh_Topo_Sample)
  EndInterface

  ; IProperty
  Interface IProperty
  	Init.l(*parentProperty)
  	IsConstant.b()
  	GetName.l()
  	GetPropertyType.l()
  	GetDataTraits.l()
  	GetNbItems.i(time.f)
  	GetInterpretation()
  	GetMessage()
  	SetMessage(msg.p-utf8)
  	GetSample(time.f)
  EndInterface
  
  ; Import C Library
  ;-------------------------------------------------------
  If FileSize("../../libs")=-2
    CompilerSelect #PB_Compiler_OS
      CompilerCase  #PB_OS_Windows
        ; Global alembic_lib = OpenLibrary(#PB_Any, "..\..\libs\x64\windows\Alembic.dll")
        Global alembic_lib = OpenLibrary(#PB_Any, "E:\Projects\RnD\Noodle\libs\x64\windows\Alembic.dll")
      CompilerCase #PB_OS_MacOS
        Global alembic_lib = OpenLibrary(#PB_Any, "../../libs/x64/macosx/PBAlembic.so")
      CompilerCase #PB_OS_Linux
         Global alembic_lib = OpenLibrary(#PB_Any, "../../libs/x64/linux/PBAlembic.so")
    CompilerEndSelect
  Else
    CompilerSelect #PB_Compiler_OS
      CompilerCase  #PB_OS_Windows
        ;         Global alembic_lib = OpenLibrary(#PB_Any, "libs\x64\windows\Alembic.dll")
        Global alembic_lib = OpenLibrary(#PB_Any, "E:\Projects\RnD\Noodle\libs\x64\windows\Alembic.dll")
      CompilerCase #PB_OS_MacOS
        Global alembic_lib = OpenLibrary(#PB_Any, "libs/x64/macosx/PBAlembic.so")
      CompilerCase #PB_OS_Linux
        Global alembic_lib = OpenLibrary(#PB_Any, "libs/x64/linux/PBAlembic.so")
    CompilerEndSelect
  EndIf
  
  ; prototypes
  PrototypeC.b  ABCHASHDF5SUPPORT()
  PrototypeC    ABCVERSION()
  
  PrototypeC    ABCNEWARCHIVEMANAGER()
  PrototypeC    ABCDELETEARCHIVEMANAGER(manager.IArchiveManager)
  
  PrototypeC    ABCNEWIARCHIVE()
  PrototypeC    ABCDELETEIARCHIVE(arc.IArchive)
  PrototypeC    ABCNEWIOBJECT(arch.IArchive, index.i)
  PrototypeC    ABCDELETEIOBJECT(obj.IObject)
  
  PrototypeC    ABCNEWOARCHIVE()
  PrototypeC    ABCDELETEOARCHIVE(arc.IArchive)
  PrototypeC    ABCNEWOOBJECT(arch.IArchive, index.i)
  PrototypeC    ABCDELETEOOBJECT(obj.IObject)
  
  
  ; import functions
  If alembic_lib  
    Global getLibraryVersion.ABCVERSION = GetFunction(alembic_lib, "getLibraryVersion")
    Global hasHDF5Support.ABCHASHDF5SUPPORT = GetFunction(alembic_lib, "hasHDF5Support")
    
    Global newIArchiveManager.ABCNEWARCHIVEMANAGER = GetFunction(alembic_lib, "newArchiveManager")
    Global deleteIArchiveManager.ABCDELETEARCHIVEMANAGER = GetFunction(alembic_lib, "deleteArchiveManager")
    
    Global newIArchive.ABCNEWIARCHIVE = GetFunction(alembic_lib, "newIArchive")
    Global deleteIArchive.ABCDELETEIARCHIVE = GetFunction(alembic_lib, "deleteIArchive")
    Global newIObject.ABCNEWIOBJECT = GetFunction(alembic_lib, "newIObject")
    Global deleteIObject.ABCDELETEIOBJECT = GetFunction(alembic_lib, "deleteIObject")
    
    Global newOArchive.ABCNEWOARCHIVE = GetFunction(alembic_lib, "newOArchive")
    Global deleteOArchive.ABCDELETEOARCHIVE = GetFunction(alembic_lib, "deleteOArchive")
    Global newOObject.ABCNEWOOBJECT = GetFunction(alembic_lib, "newOObject")
    Global deleteOObject.ABCDELETEOOBJECT = GetFunction(alembic_lib, "deleteOObject")
  EndIf

  Declare Init()
  Declare Terminate()
  Declare OpenIArchive(filename.s)
  Declare CloseIArchive(archive.IArchive)
  Declare AddIObject(archive.IArchive, index.i)
  Declare RemoveIObject(object.IObject)
  
  Global abc_manager.IArchiveManager
EndDeclareModule

; ============================================================================
;  AlembicIObject Module Declaration
; ============================================================================
DeclareModule AlembicIObject
  Structure AlembicIObject_t
    iObj.Alembic::IObject
    *infos      ; Sample Infos
    *sample     ; Sample
    ;*obj.Object3D::Object3D_t
    *parent.AlembicIObject_t
    ;List *attributes.Attribute::Attribute_t()
  EndStructure
  
  Declare New(*archive.Alembic::IArchive,index.i)
  Declare Delete(*Me.AlembicIObject_t)
  Declare Get(*Me.AlembicIObject_t)
  Declare Init(*Me.AlembicIObject_t,*parent.AlembicIObject_t=#Null)
  Declare Get3DObject(*Me.AlembicIObject_t)
  Declare GetType(*Me.AlembicIObject_t)
  Declare CreateSample(*Me.AlembicIObject_t)
  Declare UpdateSample(*Me.AlembicIObject_t,frame.f)
  Declare DeleteSample(*Me.AlembicIObject_t)
;   Declare GetXFormSampleAtFrame(*Me.AlembicObject_t,frame.f)
;   Declare GetPolymeshSampleAtFrame(*Me.AlembicObject_t,frame.f)
;   Declare Getproperties(*Me.AlembicObject_t)
;   Declare UpdateProperties(*Me.AlembicObject_t,frame.f)
;   Declare ApplyProperty(*Me.AlembicObject_t,name.s)
; ;   Declare ApplyProperty2(*Me.AlembicObject_t,name.s,*arr.CArray::CArrayT)
;   Declare LogProperties(*Me.AlembicObject_t)
EndDeclareModule

; ============================================================================
;  Alembic Module Implementation
; ============================================================================
Module Alembic
  Procedure Init()
    ALEMBIC_WITH_HDF5 = hasHDF5Support()
    ALEMBIC_VERSION = PeekS(getLibraryVersion(), -1, #PB_Ascii)
    Debug "=================================================================="
    Debug "   Alembic Version :" +ALEMBIC_VERSION
    Debug "   With Hdf5 : "+Str(ALEMBIC_WITH_HDF5)
    Debug "=================================================================="
    abc_manager = newIArchiveManager()
  EndProcedure
  
  Procedure Terminate()
    abc_manager\CloseAllArchives()
    deleteIArchiveManager(abc_manager)
  EndProcedure
  
  Procedure OpenIArchive(filename.s)
    Protected manager.IArchiveManager = abc_manager
    Protected archive.IArchive = manager\OpenArchive(filename)
    Debug "FILENAME : "+filename
    Debug "Num Objects : "+Str(archive\GetNumObjects())
    Debug "Num Uses Archive :"+Str( archive\NumUses())
    If Not archive\NumUses()
      archive\Open(filename)
      Define identifier.s
      Define numObjects = archive\GetNumObjects()
      For i=0 To numObjects-1
        identifier = PeekS(archive\GetIdentifier(i), -1, #PB_UTF8)
        If identifier <> "/"
          Define iObject.IObject = Alembic::AddIObject(archive, i)
        EndIf
      Next
    EndIf
    
    ProcedureReturn archive
  EndProcedure

  Procedure CloseIArchive(archive.IArchive)
    deleteIArchive(archive)  
  EndProcedure


  Procedure AddIObject(archive.IArchive, index.i)
    Protected object.IObject = newIObject(archive, index)
    Debug "-----------------------------------------------------------------------------------------------------------------------"
    Debug Chr(9)+"NAME : "+PeekS(object\GetName(), -1, #PB_UTF8)
    Debug Chr(9)+"FULLNAME : "+PeekS(object\GetFullName(), -1, #PB_UTF8)
    Debug Chr(9)+"NUM PROPERTIES : "+Str(object\GetNumProperties())
    
    Select object\GetType()
      Case #ABC_OBJECT_XFORM
        Debug Chr(9)+"OBJECT TYPE : XFORM"
      Case #ABC_OBJECT_POLYMESH
        Protected polymesh.IPolymesh = object
        Protected infos.ABC_Polymesh_Topo_Sample_Infos
        polymesh\GetTopoSampleDescription(0, @infos)
        Debug Chr(9)+"OBJECT TYPE : POLYMESH"
        Debug Chr(9)+Chr(9)+"HAS COLORS : "+Str(infos\hascolor)
        Debug Chr(9)+Chr(9)+"HAS ENVELOPE : "+Str(infos\hasenvelope)
        Debug Chr(9)+Chr(9)+"HAS NORMAL : "+Str(infos\hasnormal)
        Debug Chr(9)+Chr(9)+"HAS UVS : "+Str(infos\hasuvs)
        Debug Chr(9)+Chr(9)+"HAS TANGENTS : "+Str(infos\hastangent)
        Debug Chr(9)+Chr(9)+"NUM POINTS: "+Str(infos\nbpoints)
        Debug Chr(9)+Chr(9)+"NUM INDICES: "+Str(infos\nbindices)
        Debug Chr(9)+Chr(9)+"NUM SAMPLES: "+Str(infos\nbsamples)
        Debug Chr(9)+Chr(9)+"NUM FACECOUNT: "+Str(infos\nbfacecount)
      Case #ABC_OBJECT_POINTCLOUD
        Debug Chr(9)+"OBJECT TYPE : POINT CLOUD"
      Case #ABC_OBJECT_CURVE
        Debug Chr(9)+"OBJECT TYPE : CURVE"
      Case #ABC_OBJECT_FACESET
        Debug Chr(9)+"OBJECT TYPE : FACESET"
      Case #ABC_OBJECT_UNKNOWN
        Debug Chr(9)+"OBJECT TYPE : UNKNOWN"
    EndSelect
    
    Debug Chr(9)+" NUM PROPERTIES : "+Str(object\GetNumProperties())
    For i=0 To object\GetNumProperties()-1
      Define prop.IProperty = object\GetProperty(i)
      Debug Chr(9)+Chr(9)+PeekS(prop\GetName(), -1, #PB_UTF8)
    Next
    
    ProcedureReturn object
  EndProcedure
  
  Procedure RemoveIObject(object.IObject)
    deleteIObject(object)  
  EndProcedure
EndModule


; ============================================================================
;  Alembic Object Module Implementation
; ============================================================================
Module AlembicIObject
;   UseModule Math
  ;----------------------------------------
  ; Debug XForm Sample
  ;----------------------------------------
  Procedure DebugXFormSample(*sample.Alembic::ABC_XForm_Sample)
    Debug "Position : ("+StrF(*sample\pos[0])+","+StrF(*sample\pos[1])+","+StrF(*sample\pos[2])+")"  
    Debug "Rotation : ("+StrF(*sample\ori[0])+","+StrF(*sample\ori[1])+","+StrF(*sample\ori[2])+","+StrF(*sample\ori[3])+")" 
    Debug "Scale : ("+StrF(*sample\scl[0])+","+StrF(*sample\scl[1])+","+StrF(*sample\scl[2])+")"
  EndProcedure
  
  ; Debug Polymesh Sample
  ;----------------------------------------
  Procedure DebugPolymeshTopoSample(*sample.Alembic::ABC_Polymesh_Topo_Sample_Infos)
  Debug "Nb Points in Polymesh : "+Str(*sample\nbpoints)
  EndProcedure
  
  
  ;---------------------------------------------------------
  ; Get
  ;---------------------------------------------------------
  Procedure Get(*o.AlembicIObject_t)
    ProcedureReturn *o\iObj
  EndProcedure
  
  ;---------------------------------------------------------
  ; Get 3DObject
  ;---------------------------------------------------------
  Procedure Get3DObject(*o.AlembicIObject_t)
;     ProcedureReturn *o\obj
  EndProcedure
  
  ;---------------------------------------------------------
  ; Get Type
  ;---------------------------------------------------------
  Procedure GetType(*o.AlembicIObject_t)
    ProcedureReturn *o\iObj\GetType()
  EndProcedure
  
  ;---------------------------------------------------------
  ; Create Sample
  ;---------------------------------------------------------
  Procedure CreateSample(*o.AlembicIObject_t)
  
    Select *o\iObj\GetType()
      Case Alembic::#ABC_OBJECT_XFORM
        *o\sample = AllocateMemory(SizeOf(Alembic::ABC_XForm_Sample))
        InitializeStructure(*o\sample,Alembic::ABC_XForm_Sample)
;         *o\initialized = #False
      Case Alembic::#ABC_OBJECT_POLYMESH
        *o\sample = AllocateMemory(SizeOf(Alembic::ABC_Polymesh_Topo_Sample))
        InitializeStructure(*o\sample,Alembic::ABC_Polymesh_Topo_Sample)
        *o\infos = AllocateMemory(SizeOf(Alembic::ABC_Polymesh_Topo_Sample_Infos))
        Protected polymesh.Alembic::IPolymesh = *o\iObj
        polymesh\GetTopoSampleDescription(0,*o\infos)

;         *o\initialized = #False
      Case Alembic::#ABC_OBJECT_POINTCLOUD
        *o\sample = AllocateMemory(SizeOf(Alembic::ABC_PointCloud_Sample))
        InitializeStructure(*o\sample,Alembic::ABC_PointCloud_Sample)
        *o\infos = AllocateMemory(SizeOf(Alembic::ABC_PointCloud_Sample_Infos))
        Protected points.Alembic::IPoints = *o\iObj
        points\GetSampleDescription(0,*o\infos)
;         Alembic::ABC_GetPointCloudSampleDescription(*o\iObj,1,*o\infos)        
        
        Protected *infos.Alembic::ABC_PointCloud_Sample_Infos = *o\infos
;         *o\initialized = #False
    EndSelect
  EndProcedure
  
  
  ;---------------------------------------------------------------
  ; Get XForm Sample At Frame
  ;---------------------------------------------------------------
  Procedure GetXFormSampleAtFrame(*abc.AlembicIObject_t,frame.f)
    Protected *sample.Alembic::ABC_XForm_Sample = *abc\sample
;     If *abc\obj <>#Null
; 
;       Alembic::ABC_GetXFormSample(*abc\ptr,frame,*sample)
;       Protected *t.Transform::Transform_t = *abc\obj\localT
;       Transform::SetTranslationFromXYZValues(*t,*sample\pos [0],*sample\pos[1],*sample\pos[2])
;       Transform::SetScaleFromXYZValues(*t,*sample\scl[0],*sample\scl[1],*sample\scl[2])
;       Protected *q.q4f32 = *t\t\rot
;       Quaternion::SetFromAxisAngleValues(*q,*sample\ori[0],*sample\ori[1],*sample\ori[2],*sample\ori[3] * #F32_DEG2RAD)
;       Transform::SetRotationFromQuaternion(*t,*q)
;       Transform::UpdateMatrixFromSRT(*t)
;       Object3D::SetLocalTransform(*abc\obj,*t)
;       Protected *parent.Object3D::Object3D_t
;       Object3D::UpdateTransform(*abc\obj,*parent\globalT)
; 
;     Else
;       Alembic::ABC_GetXFormSample(*abc\ptr,frame,*sample)
;     EndIf
    
  EndProcedure
  
  ;----------------------------------------
  ; Get PointCloud Sample At Frame
  ;----------------------------------------
  ;{
  Procedure GetPointCloudSampleAtFrame(*o.AlembicIObject_t,frame.f)

    Protected *cloud_sample.Alembic::ABC_PointCloud_Sample = *o\sample
    Protected *cloud.InstanceCloud::InstanceCloud_t = *o\obj
    Protected *cloud_infos.Alembic::ABC_PointCloud_Sample_Infos = *o\infos
    
    Protected *cloud_geom.Geometry::PointCloudGeometry_t = *cloud\geom
    
    Alembic::ABC_GetPointCloudSampleDescription(*o\ptr,frame,*cloud_infos)
    
    CArray::SetCount(*cloud_geom\a_positions,*cloud_infos\nbpoints)
    CArray::SetCount(*cloud_geom\a_velocities,*cloud_infos\nbpoints)
    CArray::SetCount(*cloud_geom\a_color,*cloud_infos\nbpoints)
    CArray::SetCount(*cloud_geom\a_indices,*cloud_infos\nbpoints)
    
    
    *cloud_sample\position = CArray::GetPtr(*cloud_geom\a_positions,0)
    *cloud_sample\velocity = CArray::GetPtr(*cloud_geom\a_velocities,0)
    *cloud_sample\color = CArray::GetPtr(*cloud_geom\a_color,0)
    *cloud_sample\id = CArray::GetPtr(*cloud_geom\a_indices,0)
    
    update.i =  Alembic::ABC_UpdatePointCloudSample(*o\ptr,*cloud_infos,*cloud_sample)
    MessageRequester("Velocities Size",Str(update)+" ---> "+Str(CArray::GetCount(*cloud_geom\a_velocities)))
    UpdateProperties(*o,frame/30)
    ApplyProperty(*o,"Scale")
    ApplyProperty(*o,"Orientation")
    ApplyProperty(*o,"Color")

;     If *geom\nbpoints <> *infos\nbpoints Or *geom\nbsamples <> *infos\nbindices Or *geom\nbpolygons <> *infos\nbfacecount
;       *o\initialized = #False
;     EndIf
;     
;     Protected update.i
;     
;     If Not *o\initialized 
;       ;we need to create the topology
;       *geom\nbpoints = *infos\nbpoints
;       *geom\nbsamples = *infos\nbsamples
;       *geom\nbindices = *infos\nbindices
;       *geom\nbpolygons = *infos\nbfacecount
;       *geom\nbtriangles = *infos\nbsamples / 3
;       ; Resize Mesh Datas
;       CArray::SetCount(*geom\a_positions,*infos\nbpoints)
;       CArray::SetCount(*geom\a_pointnormals,*infos\nbpoints)
;       CArray::SetCount(*geom\a_velocities,*infos\nbpoints)
;       CArray::SetCount(*geom\a_normals,*infos\nbsamples)
;       CArray::SetCount(*geom\a_tangents,*infos\nbsamples)
;       CArray::SetCount(*geom\a_uvws,*infos\nbsamples)
;       CArray::SetCount(*geom\a_colors,*infos\nbsamples)
;       CArray::SetCount(*geom\a_faceindices,*infos\nbindices)
;       CArray::SetCount(*geom\a_facecount,*infos\nbfacecount)
;       CArray::SetCount(*geom\a_triangleindices,*geom\nbsamples)
; 
;       
; ;       MessageRequester("Alembic Object : "+*o\name,"Has Color : "+Str(*infos\hascolor)+Chr(10)+
; ;                                  "Has UVs : "+Str(*infos\hasuvs)+Chr(10)+
; ;                                  "Has Normal : "+Str(*infos\hasnormal)+Chr(10)+
; ;                                  "Has Velocity : "+Str(*infos\hasvelocity)+Chr(10)+
; ;                                  "Nb Face Count : "+Str(*infos\nbfacecount)+Chr(10)+
; ;                                  "Nb Face Indices : "+Str(*infos\nbindices)+Chr(10)+
; ;                                  "Nb Points : "+Str(*infos\nbpoints)+Chr(10)+
; ;                                  "Nb Samples : "+Str(*infos\nbsamples)+Chr(10)+
; ;                                  "Nb Face Count : "+Str(*infos\nbfacecount)+Chr(10)+
; ;                                  "Sample Index : "+Str(*infos\sampleindex))
;      
;      
;       ; Bind to ABC_Sample
;       *mesh_sample\positions = *geom\a_positions\data
;       *mesh_sample\velocities = *geom\a_velocities\data 
;       *mesh_sample\normals = *geom\a_normals\data
;       *mesh_sample\uvs = *geom\a_uvws\data
;       *mesh_sample\colors = *geom\a_colors\data
;       *mesh_sample\faceindices = *geom\a_faceindices\data
;       *mesh_sample\facecount = *geom\a_facecount\data
; 
;       *infos\sampleindex = frame
;       update.i =  Alembic::ABC_UpdatePolymeshTopoSample(*o\ptr,*infos,*mesh_sample)
; 
;        PolymeshGeometry::RecomputeTriangle(*geom)
;        
;        
;        If Not *infos\hascolor : PolymeshGeometry::SetColors(*geom) : EndIf
;        If Not *infos\hasnormal : PolymeshGeometry::RecomputeNormals(*geom): EndIf
;        If Not *infos\hasuvs : PolymeshGeometry::GetUVWSFromPosition(*geom) : EndIf
;        If Not *infos\hastangent : PolymeshGeometry::RecomputeTangents(*geom):EndIf
;       
;        PolymeshGeometry::GetTopology(*geom)
;        
;        
;        If *infos\hasenvelope
;          Protected envelope.Alembic::ABC_Envelope_Sample
;          Protected *weights.CArray::CArrayC4F32 = CArray::newCArrayC4F32()
;          Protected *indices.CArray::CArrayC4U8 = CArray::newCArrayC4U8()
;          CArray::SetCount(*weights,*infos\nbpoints)
;          CArray::SetCount(*indices,*infos\nbpoints)
;          envelope\weights = CArray::GetPtr(*weights,0)
;          envelope\indices = CArray::GetPtr(*indices,0)
;          ;envelope\nbdeformers
;          Alembic::ABC_GetEnvelope(*o\ptr,@envelope)
;          
;          Protected i
;          Protected *ids.c4u8
;          Protected *weight.c4f32
;          For i=0 To *infos\nbpoints-1
;            *ids = CArray::GetValue(*indices,i)
;            *weight = CArray::GetValue(*weights,i)
;            Debug "------------- Vertex "+Str(i)
;            Debug "IDs : "+Str(*ids\r)+","+Str(*ids\g)+","+Str(*ids\b)+","+Str(*ids\a)
;            Debug "Weights : "+StrF(*weight\r)+","+StrF(*weight\g)+","+StrF(*weight\b)+","+StrF(*weight\a)
;            
;            
;          Next
;          
;          PolymeshGeometry::EnvelopeColors(*geom,*weights,*indices,envelope\nbdeformers)
;        EndIf
;        
;        
;       *o\initialized = #True
;   
;     Else
;       Debug "Update Polymesh Sample"
;       *infos\sampleindex = frame
;       update.i =  Alembic::ABC_UpdatePolymeshSample(*o\ptr,*infos,*mesh_sample)
;     EndIf
;     
    *cloud\dirty = Object3D::#DIRTY_STATE_DEFORM
    
  EndProcedure
  ;}
  
  
  ;----------------------------------------
  ; Get Polymesh Sample At Frame
  ;----------------------------------------
  ;{
  Procedure GetPolymeshSampleAtFrame(*o.AlembicObject_t,frame.f)
  
    Protected *mesh_sample.Alembic::ABC_Polymesh_Topo_Sample = *o\sample
    Protected *mesh.Polymesh::Polymesh_t = *o\obj
    Protected *geom.Geometry::PolymeshGeometry_t = *mesh\geom
    Protected *infos.Alembic::ABC_Polymesh_Topo_Sample_Infos = *o\infos

    Alembic::ABC_GetPolymeshTopoSampleDescription(*o\ptr,frame,*infos)
    
    If *infos\hasenvelope
      MessageRequester("[ALEMBIC]","Envelope Detected!!!")
    EndIf
    
    If *geom\nbpoints <> *infos\nbpoints Or *geom\nbpolygons <> *infos\nbfacecount
      Debug ">>>>>>>>>>>>>>>>>>>>>>>>>> RESET INITIALIZATION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
      *o\initialized = #False
    EndIf
    
    Protected update.i
    
    If Not *o\initialized 
      ;we need to create the topology
      *geom\nbpoints = *infos\nbpoints
      *geom\nbsamples = *infos\nbsamples
      *geom\nbindices = *infos\nbindices
      *geom\nbpolygons = *infos\nbfacecount
      *geom\nbtriangles = *infos\nbsamples / 3
      ; Resize Mesh Datas
      CArray::SetCount(*geom\a_positions,*infos\nbpoints)
      CArray::SetCount(*geom\a_pointnormals,*infos\nbpoints)
      CArray::SetCount(*geom\a_velocities,*infos\nbpoints)
      CArray::SetCount(*geom\a_normals,*infos\nbsamples)
      CArray::SetCount(*geom\a_tangents,*infos\nbsamples)
      CArray::SetCount(*geom\a_uvws,*infos\nbsamples)
      CArray::SetCount(*geom\a_colors,*infos\nbsamples)
      CArray::SetCount(*geom\a_faceindices,*infos\nbindices)
      CArray::SetCount(*geom\a_facecount,*infos\nbfacecount)
      CArray::SetCount(*geom\a_triangleindices,*geom\nbsamples)
     
     
      ; Bind to ABC_Sample
      *mesh_sample\positions = *geom\a_positions\data
      *mesh_sample\velocities = *geom\a_velocities\data 
      *mesh_sample\normals = *geom\a_normals\data
      *mesh_sample\tangents = *geom\a_tangents\data
      *mesh_sample\uvs = *geom\a_uvws\data
      *mesh_sample\colors = *geom\a_colors\data
      *mesh_sample\faceindices = *geom\a_faceindices\data
      *mesh_sample\facecount = *geom\a_facecount\data

      Alembic::ABC_UpdatePolymeshTopoSample(*o\ptr,*infos,*mesh_sample)

       PolymeshGeometry::RecomputeTriangles(*geom)
       
       
       If Not *infos\hascolor : PolymeshGeometry::SetColors(*geom) : EndIf
       If Not *infos\hasnormal : PolymeshGeometry::RecomputeNormals(*geom,1.0): EndIf
       If Not *infos\hasuvs : PolymeshGeometry::GetUVWSFromPosition(*geom) : EndIf
       If Not *infos\hastangent : PolymeshGeometry::RecomputeTangents(*geom):EndIf
      
       PolymeshGeometry::GetTopology(*geom)
       
       
       If *infos\hasenvelope
         Protected envelope.Alembic::ABC_Envelope_Sample
         Protected *weights.CArray::CArrayC4F32 = CArray::newCArrayC4F32()
         Protected *indices.CArray::CArrayC4U8 = CArray::newCArrayC4U8()
         CArray::SetCount(*weights,*infos\nbpoints)
         CArray::SetCount(*indices,*infos\nbpoints)
         envelope\weights = CArray::GetPtr(*weights,0)
         envelope\indices = CArray::GetPtr(*indices,0)
         ;envelope\nbdeformers
         Alembic::ABC_GetEnvelope(*o\ptr,@envelope)
         
         Protected i
         Protected *ids.c4u8
         Protected *weight.c4f32
         For i=0 To *infos\nbpoints-1
           *ids = CArray::GetValue(*indices,i)
           *weight = CArray::GetValue(*weights,i)
           Debug "------------- Vertex "+Str(i)
           Debug "IDs : "+Str(*ids\r)+","+Str(*ids\g)+","+Str(*ids\b)+","+Str(*ids\a)
           Debug "Weights : "+StrF(*weight\r)+","+StrF(*weight\g)+","+StrF(*weight\b)+","+StrF(*weight\a)
           
           
         Next
         
         PolymeshGeometry::EnvelopeColors(*geom,*weights,*indices,envelope\nbdeformers)
       EndIf
       
       
      *o\initialized = #True
  
    Else
      *infos\sampleindex = frame
      update.i =  Alembic::ABC_UpdatePolymeshSample(*o\ptr,*infos,*mesh_sample)
    EndIf
    
    *mesh\dirty = Object3D::#DIRTY_STATE_DEFORM
    
  EndProcedure
  ;}
  
  ;---------------------------------------------------------
  ; Update Sample
  ;---------------------------------------------------------
  Procedure UpdateSample(*o.AlembicObject_t,frame.f)
    
    Select *o\type
      Case Alembic::#ABC_OBJECT_XFORM
        GetXFormSampleAtFrame(*o,frame)
        
      Case Alembic::#ABC_OBJECT_POLYMESH
        GetPolymeshSampleAtFrame(*o,frame)
        
      Case Alembic::#ABC_OBJECT_POINTCLOUD
        GetPointCloudSampleAtFrame(*o,frame)
        
        
    EndSelect
  EndProcedure
  
  ;---------------------------------------------------------
  ; Delete Sample
  ;---------------------------------------------------------
  Procedure DeleteSample(*Me.AlembicObject_t)
  
  EndProcedure
  
  ;---------------------------------------------------------
  ; Log Properties
  ;---------------------------------------------------------
  Procedure LogProperties(*Me.AlembicObject_t)
    Debug "################# LOG PROPERTIES #################################"
    Protected title.s
    Select *Me\type
      Case Alembic::#ABC_OBJECT_XFORM
        title = "[XFORM]"
      Case Alembic::#ABC_OBJECT_POLYMESH
        title = "[POLYMESH]"
      Case Alembic::#ABC_OBJECT_POINTCLOUD
        title = "[POINTCLOUD]"
      Case Alembic::#ABC_OBJECT_CURVE
        title = "[CURVES]"
    EndSelect
   EndProcedure
   
  ;---------------------------------------------------------
  ; Get Property Sample
  ;---------------------------------------------------------
  Procedure GetPropertySample(*Me.AlembicObject_t,ID.i)
     SelectElement(*Me\props(),ID)
;      Protected traits.Alembic::ABCDataTraits = Alembic::ABC_GetA
  EndProcedure
  
  ;---------------------------------------------------------
  ; Get Properties
  ;---------------------------------------------------------
  Procedure CreateAttributeFromProperty(*Me.AlembicObject_t,*infos.Alembic::ABC_Attribute_Sample_Infos)
    Protected *obj.Object3D::Object3D_t = *Me\obj
    Protected *data.CArray::CArrayT
    Protected *attribute.Attribute::Attribute_t
    Protected context.i = Attribute::#ATTR_CTXT_SINGLETON
    Protected struct.i = Attribute::#ATTR_STRUCT_SINGLE
    If *infos\type = Alembic::#ABC_PropertyType_Array
      struct = Attribute::#ATTR_STRUCT_ARRAY
    EndIf
    
    Protected name.s = PeekS(Alembic::ABC_GetAttributeSampleName(*infos),-1, #PB_Ascii)
    Select *infos\traits
      Case Alembic::#ABC_DataTraits_Bool
        *data = CArray::newCArrayBool()
        *attribute = Attribute::New(name,Attribute::#ATTR_TYPE_BOOL,struct,context,*data,#False,Bool(struct=Attribute::#ATTR_STRUCT_SINGLE))
      Case Alembic::#ABC_DataTraits_Int32
        *data = CArray::newCArrayLong()
        *attribute = Attribute::New(name,Attribute::#ATTR_TYPE_LONG,struct,context,*data,#False,Bool(struct=Attribute::#ATTR_STRUCT_SINGLE))
      Case Alembic::#ABC_DataTraits_Int64
        *data = CArray::newCArrayInt()
        *attribute = Attribute::New(name,Attribute::#ATTR_TYPE_INTEGER,struct,context,*data,#False,Bool(struct=Attribute::#ATTR_STRUCT_SINGLE))
      Case Alembic::#ABC_DataTraits_Float
        *data = CArray::newCArrayFloat()
        *attribute = Attribute::New(name,Attribute::#ATTR_TYPE_FLOAT,struct,context, *data,#False,Bool(struct=Attribute::#ATTR_STRUCT_SINGLE))
      Case Alembic::#ABC_DataTraits_V2f
        *data = CArray::newCArrayV2F32()
        *attribute = Attribute::New(name,Attribute::#ATTR_TYPE_VECTOR2,struct,context, *data,#False,Bool(struct=Attribute::#ATTR_STRUCT_SINGLE))
      Case Alembic::#ABC_DataTraits_V3f
        *data = CArray::newCArrayV3F32()
        *attribute = Attribute::New(name,Attribute::#ATTR_TYPE_VECTOR3,struct,context, *data,#False,Bool(struct=Attribute::#ATTR_STRUCT_SINGLE))
      Case Alembic::#ABC_DataTraits_V4f
        *data = CArray::newCArrayC4F32()
        *attribute = Attribute::New(name,Attribute::#ATTR_TYPE_VECTOR4,struct,context, *data,#False,Bool(struct=Attribute::#ATTR_STRUCT_SINGLE))
      Case Alembic::#ABC_DataTraits_C4f
        *data = CArray::newCArrayC4F32()
        *attribute = Attribute::New(name,Attribute::#ATTR_TYPE_COLOR,struct,context, *data,#False,Bool(struct=Attribute::#ATTR_STRUCT_SINGLE))
      Case Alembic::#ABC_DataTraits_Quatf
        *data = CArray::newCArrayQ4F32()
        *attribute = Attribute::New(name,Attribute::#ATTR_TYPE_QUATERNION,struct,context, *data,#False,Bool(struct=Attribute::#ATTR_STRUCT_SINGLE))
      Case Alembic::#ABC_DataTraits_M33f
        *data = CArray::newCArrayM3F32()
        *attribute = Attribute::New(name,Attribute::#ATTR_TYPE_MATRIX3,struct,context, *data,#False,Bool(struct=Attribute::#ATTR_STRUCT_SINGLE))
      Case Alembic::#ABC_DataTraits_M44f
        *data = CArray::newCArrayM4F32()
        *attribute = Attribute::New(name,Attribute::#ATTR_TYPE_MATRIX4,struct,context, *data,#False,Bool(struct=Attribute::#ATTR_STRUCT_SINGLE))
        
      Default
        MessageRequester("[Alembic]","Create Attribute From Property Failed!"+Chr(10)+name+" Traits Unsupported!!")
    EndSelect
    
    If *attribute
      Debug "Add Atribute ; "+*Me\obj\name+" >>> "+*attribute\name
      Object3D::AddAttribute(*Me\obj,*attribute)
    EndIf
    
    
    ProcedureReturn *attribute
  
   EndProcedure
   
   
  ;---------------------------------------------------------
  ; Get Properties
  ;---------------------------------------------------------
   Procedure GetProperties(*Me.AlembicObject_t)
     Protected i
     ClearList(*Me\props())

    For i=0 To Alembic::ABC_GetNumProperties(*Me\ptr)-1
     AddElement(*Me\props())
     *Me\props() = Alembic::ABC_GetProperty(*Me\ptr,i)
    Next
    
    Protected t.s
    Protected *infos.Alembic::ABC_Attribute_Sample_Infos = AllocateMemory(SizeOf(Alembic::ABC_Attribute_Sample_Infos))
    InitializeStructure(*infos,Alembic::ABC_Attribute_Sample_Infos)
    
    Protected ID = 0
    t + "Nb Properties : "+Str(ListSize(*Me\props()))+Chr(10)
    Protected *sample.Alembic::ABC_Attribute_Sample = AllocateMemory(SizeOf(Alembic::ABC_Attribute_Sample))
    
    Protected *attr.Attribute::Attribute_t
    Protected x
    
    ForEach *Me\props()   
      Alembic::ABC_GetAttributeSampleDescription(*Me\props(),0,*infos)
      Protected n.s = PeekS(Alembic::ABC_GetAttributeSampleName(*infos),-1,#PB_UTF8)
     t+n+Chr(10)+" : "+Str(*infos\nbitems)+" items "+Chr(10)
     Debug t
     *attr = CreateAttributeFromProperty(*Me,*infos)
     AddElement(*Me\attributes())
     *Me\attributes() = *attr
     
     Debug "Nb Items : "+Str(*infos\nbitems)
     CArray::SetCount(*attr\data,*infos\nbitems)
     *sample\datas = CArray::GetPtr(*attr\data,0)
     Debug "Prop : "+Str(*Me\props())
     Debug "Infos : "+Str(*infos)
     Debug "Sample : "+Str(*sample)
     Debug Alembic::ABC_GetAttributeSample
     Alembic::ABC_GetAttributeSample(*Me\props(),*infos,*sample)
    
     
;      Select *infos\traits
;        Case Alembic::#ABC_DataTraits_V3f
;          Debug ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Property V3F32 "+n
;    
;           For x=0 To *infos\nbitems-1
;             Vector3::Echo(CArray::GetValue(*attr\data,x),n)
;           Next
;         Case Alembic::#ABC_DataTraits_Bool
;           Debug ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Property BOOL"+n
; 
;           For x=0 To *infos\nbitems-1
;             Debug CArray::GetValueB(*attr\data,x)
;           Next
;        Case Alembic::#ABC_DataTraits_Quatf
;           Debug ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Property QUATERNION"+n
; 
;           For x=0 To *infos\nbitems-1
;            Quaternion::Echo(CArray::GetValue(*attr\data,x),n)
;          Next
;        Case Alembic::#ABC_DataTraits_C4f
;           Debug ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Property COLOR"+n
; 
;           For x=0 To *infos\nbitems-1
;            Color::Echo(CArray::GetValue(*attr\data,x),n)
;           Next
; ;         
;      EndSelect
;      
     ID+1
     
   Next
     
   ClearStructure(*infos,Alembic::ABC_Attribute_Sample_Infos)
   FreeMemory(*infos)
   FreeMemory(*sample)

;        MessageRequester("ALEMBIC PROPERTIES",t)
 EndProcedure
 
 Procedure UpdateProperty(*Me.AlembicObject_t,frame.f,name.s)
    Protected t.s
    Protected infos.Alembic::ABC_Attribute_Sample_Infos
    
    Protected ID = 0
    Protected io_sample.Alembic::ABC_Attribute_Sample
    
    Protected *attr.Attribute::Attribute_t
    Protected x=0
    
    Debug "Frame "+Str(frame)
    infos\time = frame
    
    ForEach *Me\props() 
      
      Alembic::ABC_GetAttributeSampleDescription(*Me\props(),frame,@infos)
      Protected n.s = PeekS(Alembic::ABC_GetAttributeSampleName(*infos),-1,#PB_UTF8)
      If n = name 
        
       SelectElement(*Me\attributes(),x)
       *attr = *Me\attributes()
       If CArray::GetCount(*attr\data)<>infos\nbitems
         CArray::SetCount(*attr\data,infos\nbitems)
       EndIf
       
       io_sample\datas = CArray::GetPtr(*attr\data,0)   
       Alembic::ABC_GetAttributeSample(*Me\props(),@infos,@io_sample)
       Break
     EndIf
     x+1
   Next
 EndProcedure
 
 
 Procedure UpdateProperties(*Me.AlembicObject_t,frame.f)
    Protected t.s
    Protected infos.Alembic::ABC_Attribute_Sample_Infos
    
    Protected ID = 0
    Protected io_sample.Alembic::ABC_Attribute_Sample
    
    Protected *attr.Attribute::Attribute_t
    Protected x=0
    
    Debug "Frame "+Str(frame)
    infos\time = frame
    
    ForEach *Me\props()   
      Alembic::ABC_GetAttributeSampleDescription(*Me\props(),frame,@infos)
      Protected n.s = PeekS(Alembic::ABC_GetAttributeSampleName(@infos),-1,#PB_UTF8)

      SelectElement(*Me\attributes(),x)
      *attr = *Me\attributes()
      If CArray::GetCount(*attr\data)<>infos\nbitems
       CArray::SetCount(*attr\data,infos\nbitems)
      EndIf
    
      io_sample\datas = CArray::GetPtr(*attr\data,0)   
      Alembic::ABC_GetAttributeSample(*Me\props(),@infos,@io_sample)

      x+1
    Next
    
  EndProcedure

   
   
  ;---------------------------------------------------------
  ; Apply Properties
  ;---------------------------------------------------------
  Procedure ApplyProperty(*Me.AlembicObject_t,name.s)
    Define x
    Define nbp
    Define *v.v3f32
    If FindMapElement(*Me\obj\m_attributes(),name)
       If *Me\type = Alembic::#ABC_OBJECT_POINTCLOUD
         Define *geom.Geometry::PointCloudGeometry_t = *Me\obj\geom
         If *geom
           Select name
             Case "Scale"
               nbp = CArray::GetCount(*Me\obj\m_attributes()\data)
               CArray::SetCount(*geom\a_scale,nbp)
               CopyMemory(CArray::GetPtr(*Me\obj\m_attributes()\data,0),CArray::GetPtr(*geom\a_scale,0),CArray::GetCount(*geom\a_scale)*CArray::GetItemSize(*geom\a_scale))

               
             Case "Orientation"
               Debug "################# APPLY ORIENTATION ##############################"
               nbp = CArray::GetCount(*Me\obj\m_attributes()\data)
               Define *tan.v3f32
               Define *nrm.v3f32
               Define *q.q4f32

               CArray::SetCount(*geom\a_normals,nbp)
               CArray::SetCount(*geom\a_tangents,nbp)
       
               For x=0 To nbp-1
                 *nrm = CArray::GetValue(*geom\a_normals,x)
                 *tan = CArray::GetValue(*geom\a_tangents,x)
                 *q = CArray::GetValue(*Me\obj\m_attributes()\data,x)
                 
                 Vector3::Set(*nrm,0,1,0)
                 Vector3::MulByQuaternionInPlace(*nrm,*q)
                 Vector3::Set(*tan,1,0,0)
                 Vector3::MulByQuaternionInPlace(*tan,*q)
               Next
             Case "Color"
               nbp = CArray::GetCount(*Me\obj\m_attributes()\data)

               Define *q.q4f32
               Define q.q4f32
               CArray::SetCount(*geom\a_color,nbp)
               For x=0 To nbp-1
                Color::SetFromOther(CArray::GetValue(*geom\a_color,x),CArray::GetValue(*Me\obj\m_attributes()\data,x))
               Next
              
           EndSelect
         EndIf
       EndIf

     EndIf
  
     
       
;     Protected n.s = PeekS(@*infos\name)
;     
;      
;        
;       
;        
;      EndIf  
   EndProcedure
   
  ;---------------------------------------------------------
  ; Apply Property2
  ;---------------------------------------------------------
  Procedure ApplyProperty2(*Me.AlembicObject_t,name.s,*arr.CArray::CArrayT)
    Define x
    Define nbp
    Define *v.v3f32
    ForEach *Me\attributes()
      If *Me\attributes()\name = name
    ;If FindMapElement(*Me\attributes(),name)
      
       nbp = CArray::GetCount(*arr)
;        CArray::SetCount(*Me\obj\m_attributes()\data,nbp)
;        CopyMemory(CArray::GetPtr(*Me\obj\m_attributes()\data,0),CArray::GetPtr(*arr,0),nbp*CArray::GetItemSize(*arr))
        CArray::Copy(*arr,*Me\attributes()\data)
        Break
;        Else
;          MessageRequester("[Alembic]","Property "+name+" does NOT exists!!!")
       EndIf
     Next
     

;     If FindMapElement(*Me\obj\m_attributes(),name)
;       
;        nbp = CArray::GetCount(*arr)
;        CArray::SetCount(*Me\obj\m_attributes()\data,nbp)
;        CopyMemory(CArray::GetPtr(*Me\obj\m_attributes()\data,0),CArray::GetPtr(*arr,0),nbp*CArray::GetItemSize(*arr))
; 
;                
;      Else
;        MessageRequester("[Alembic]","Property "+name+" does NOT exists!!!")
;      EndIf
   
   EndProcedure
  
  ;---------------------------------------------------------
  ; Init
  ;---------------------------------------------------------
  Procedure Init(*o.AlembicObject_t,*p.AlembicObject_t=#Null)
    
    *o\name = PeekS(Alembic::ABC_GetObjectName(*o\ptr),-1,#PB_Ascii)
    *o\parent = *p
    
    If *o\type = Alembic::#ABC_OBJECT_XFORM
;      Protected *s = Alembic::ABC_InitObject(*o\ptr,Alembic::#ABC_OBJECT_XFORM)

      CreateSample(*o)
      *o\obj = #Null
      *o\initialized = #False
      ;LogProperties(*o)
      UpdateSample(*o,1)
      
    ElseIf *o\type = Alembic::#ABC_OBJECT_POLYMESH
      ; *s = Alembic::ABC_InitObject(*o\ptr,Alembic::#ABC_OBJECT_POLYMESH)
      
      CreateSample(*o)
      ;LogProperties(*o)
      Protected *meshinfos.Alembic::ABC_Polymesh_Topo_Sample_Infos = *o\infos
      Protected *mesh.Polymesh::Polymesh_t = Polymesh::New(*o\name,Shape::#SHAPE_NONE)
;       Protected *node.AlembicNode::AlembicNode_t = AlembicNode::New(*mesh,*o)
;       Stack::AddNode(*mesh\stack,*node)
;       GetProperties(*o)
      *o\obj = *mesh
      *o\initialized = #False
      
      UpdateSample(*o,1)

      Object3D::Freeze(*mesh)
      
  
      ;init transform
;       If *o\parent<>#Null
;         If *o\parent\type = Alembic::#ABC_OBJECT_XFORM
;           Protected *sample.Alembic::ABC_XForm_Sample = *o\parent\sample
;           Protected *t.Transform::Transform_t = *mesh\model
;           Vector3::Set(*t\t\pos,*sample\pos[0],*sample\pos[1],*sample\pos[2])
;           Vector3::Set(*t\t\scl,*sample\scl[0],*sample\scl[1],*sample\scl[2])
;           Quaternion::SetFromAxisAngleValues(*t\t\rot,*sample\ori[0],*sample\ori[1],*sample\ori[2],*sample\ori[3] * #F32_DEG2RAD)
;           Transform::UpdateMatrixFromSRT(*t\t)
;           Matrix4::SetFromOther(*o\obj\model,*t\m)
; ;           Protected *tp.CTransform = newCTransform()
; ;           *o\obj\UpdateTransform(*tp)
;         EndIf
;       EndIf
;       
;       
;       
   ElseIf *o\type = Alembic::#ABC_OBJECT_POINTCLOUD
;      Alembic::ABC_InitObject(*o\ptr,Alembic::#ABC_OBJECT_POINTCLOUD)
    
     CreateSample(*o)
     
     Protected *cloudinfos.Alembic::ABC_PointCloud_Sample_Infos = *o\infos
     Protected *cloud.InstanceCloud::InstanceCloud_t = InstanceCloud::New(*o\name,Shape::#SHAPE_CUBE,*cloudinfos\nbpoints)
;       *node.AlembicNode::AlembicNode_t = AlembicNode::New(*cloud,*o)
;       Stack::AddNode(*cloud\stack,*node)
      *o\obj = *cloud
      *o\initialized = #False
      Protected *cloud_geom.Geometry::PointCloudGeometry_t = *cloud\geom
      ;LogProperties(*o)
      GetProperties(*o)

;       *cloud_geom\PointOnSphere()
      UpdateSample(*o,1)
      
    EndIf  
  EndProcedure
  
  
  ;------------------------------------------------------------------
  ; Destuctor
  ;------------------------------------------------------------------
  Procedure Delete(*o.AlembicObject_t)
    ;glDelete
    If *o\obj<>#Null
      Protected *obj.Object3D::Object3D_t = *o\obj
      ;*obj\abc_obj = #Null
    EndIf
    ClearStructure(*o,AlembicObject_t)
    FreeMemory(*o)
  EndProcedure
  
  
  ;---------------------------------------------
  ;  Constructor
  ;---------------------------------------------
  ;{
  Procedure.i New(*archive,id.i)
    ; ---[ Allocate Memory ]----------------------------------------------------
    Protected *Me.AlembicObject_t = AllocateMemory(SizeOf(AlembicObject_t))
    InitializeStructure(*Me,AlembicObject_t)
    
    *Me\obj = #Null
    *Me\ptr = Alembic::ABC_GetObjectFromArchiveByID(*archive,id)
    If *Me\ptr
      *Me\name = PeekS(Alembic::ABC_GetObjectName(*Me\ptr),-1,#PB_UTF8)
      If Alembic::ABC_ObjectIsXForm(*Me\ptr) 
        *Me\type = Alembic::#ABC_OBJECT_XFORM
        *Me\sample = #Null
        
      ElseIf Alembic::ABC_ObjectIsPolymesh(*Me\ptr)
        *Me\type = Alembic::#ABC_OBJECT_POLYMESH
        *Me\sample = #Null
        
  ;     ElseIf Alembic::ABC_ObjectIsSubD(*Me\ptr)
  ;       Debug "[Alembic] : Object is a SUBD..."
  ;       *Me\type = Alembic::#ABC_OBJECT_SUBD
  ;       *Me\sample = #Null
        
      ElseIf Alembic::ABC_ObjectIsPointCloud(*Me\ptr)
        *Me\type = Alembic::#ABC_OBJECT_POINTCLOUD
        *Me\sample = #Null
        
      Else
        *Me\type = Alembic::#ABC_OBJECT_UNKNOWN
        *Me\sample = #Null
      EndIf
    EndIf
    
    ProcedureReturn *Me
  EndProcedure
EndModule

; Test
Alembic::Init()
Define abc_folder.s = "E:\Projects\RnD\Noodle\abc"
Define dir = ExamineDirectory(#PB_Any, abc_folder, "*.abc")
NewList abc_files.s()
If dir
  While NextDirectoryEntry(dir)
    If FindString(DirectoryEntryName(dir), "Skeleton")
      AddElement(abc_files())
      abc_files() = abc_folder+"\"+DirectoryEntryName(dir)
      Debug abc_files()
    EndIf
    
  Wend  
EndIf

ForEach abc_files()
  Define archive_path.s = abc_files()
  For i=0 To 2
    Define archive.Alembic::IArchive = Alembic::OpenIArchive(archive_path)
  Next
  
Next

Alembic::Terminate()


; IDE Options = PureBasic 5.62 (Windows - x64)
; CursorPosition = 686
; FirstLine = 676
; Folding = --------
; EnableXP