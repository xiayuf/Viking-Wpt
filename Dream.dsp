# Microsoft Developer Studio Project File - Name="Dream" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Dream - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Dream.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Dream.mak" CFG="Dream - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Dream - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "KYROSBASE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "KYROSBASE_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /machine:I386 /out:"Viking Wpt version/eVOL.dll"
# Begin Target

# Name "Dream - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Aimbot.cpp
# End Source File
# Begin Source File

SOURCE=.\apihook.cpp
# End Source File
# Begin Source File

SOURCE=.\attack.cpp
# End Source File
# Begin Source File

SOURCE=.\autodirection.cpp
# End Source File
# Begin Source File

SOURCE=.\bind.cpp
# End Source File
# Begin Source File

SOURCE=.\calcscreen.cpp
# End Source File
# Begin Source File

SOURCE=.\client.cpp
# End Source File
# Begin Source File

SOURCE=.\color.cpp
# End Source File
# Begin Source File

SOURCE=.\console.cpp
# End Source File
# Begin Source File

SOURCE=.\cvars.cpp
# End Source File
# Begin Source File

SOURCE=.\drawing.cpp
# End Source File
# Begin Source File

SOURCE=.\gateways.cpp
# End Source File
# Begin Source File

SOURCE=.\gpatch.cpp
# End Source File
# Begin Source File

SOURCE=.\gui.cpp
# End Source File
# Begin Source File

SOURCE=.\gui2.cpp
# End Source File
# Begin Source File

SOURCE=.\idhook.cpp
# End Source File
# Begin Source File

SOURCE=.\interpreter.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\menu.cpp
# End Source File
# Begin Source File

SOURCE=.\nospread.cpp
# End Source File
# Begin Source File

SOURCE=.\opengl.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\parsemsg.cpp
# End Source File
# Begin Source File

SOURCE=.\players.cpp
# End Source File
# Begin Source File

SOURCE=.\sprites.cpp
# End Source File
# Begin Source File

SOURCE=.\stringfinder.cpp
# End Source File
# Begin Source File

SOURCE=.\textures.cpp
# End Source File
# Begin Source File

SOURCE=.\timehandling.cpp
# End Source File
# Begin Source File

SOURCE=.\tinystrings.cpp
# End Source File
# Begin Source File

SOURCE=.\trace.cpp
# End Source File
# Begin Source File

SOURCE=.\waypoint.cpp
# End Source File
# Begin Source File

SOURCE=.\weaponslist.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Aimbot.h
# End Source File
# Begin Source File

SOURCE=.\apihook.h
# End Source File
# Begin Source File

SOURCE=.\attack.h
# End Source File
# Begin Source File

SOURCE=.\autodirection.h
# End Source File
# Begin Source File

SOURCE=.\bind.h
# End Source File
# Begin Source File

SOURCE=.\calcscreen.h
# End Source File
# Begin Source File

SOURCE=.\client.h
# End Source File
# Begin Source File

SOURCE=.\color.h
# End Source File
# Begin Source File

SOURCE=.\console.h
# End Source File
# Begin Source File

SOURCE=.\cvars.h
# End Source File
# Begin Source File

SOURCE=.\drawing.h
# End Source File
# Begin Source File

SOURCE=.\gateways.h
# End Source File
# Begin Source File

SOURCE=.\gpatch.h
# End Source File
# Begin Source File

SOURCE=.\gui.h
# End Source File
# Begin Source File

SOURCE=.\gui2.h
# End Source File
# Begin Source File

SOURCE=.\idhook.h
# End Source File
# Begin Source File

SOURCE=.\interpreter.h
# End Source File
# Begin Source File

SOURCE=.\menu.h
# End Source File
# Begin Source File

SOURCE=.\nospread.h
# End Source File
# Begin Source File

SOURCE=.\opengl.h
# End Source File
# Begin Source File

SOURCE=.\misc\parsemsg.h
# End Source File
# Begin Source File

SOURCE=.\players.h
# End Source File
# Begin Source File

SOURCE=.\misc\sprites.h
# End Source File
# Begin Source File

SOURCE=.\sprites.h
# End Source File
# Begin Source File

SOURCE=.\stringfinder.h
# End Source File
# Begin Source File

SOURCE=.\timehandling.h
# End Source File
# Begin Source File

SOURCE=.\tinystrings.h
# End Source File
# Begin Source File

SOURCE=.\trace.h
# End Source File
# Begin Source File

SOURCE=.\waypoint.h
# End Source File
# Begin Source File

SOURCE=.\weaponslist.h
# End Source File
# Begin Source File

SOURCE=.\xorstr.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "engine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\engine\cdll_dll.h
# End Source File
# Begin Source File

SOURCE=.\engine\cdll_engine_dll.h
# End Source File
# Begin Source File

SOURCE=.\engine\cdll_int.h
# End Source File
# Begin Source File

SOURCE=.\engine\cl_dll.h
# End Source File
# Begin Source File

SOURCE=.\engine\cl_entity.h
# End Source File
# Begin Source File

SOURCE=.\engine\const.h
# End Source File
# Begin Source File

SOURCE=.\engine\crc.h
# End Source File
# Begin Source File

SOURCE=.\engine\custom.h
# End Source File
# Begin Source File

SOURCE=.\engine\customentity.h
# End Source File
# Begin Source File

SOURCE=.\engine\cvardef.h
# End Source File
# Begin Source File

SOURCE=.\engine\dlight.h
# End Source File
# Begin Source File

SOURCE=.\engine\edict.h
# End Source File
# Begin Source File

SOURCE=.\engine\eiface.h
# End Source File
# Begin Source File

SOURCE=.\engine\entity_state.h
# End Source File
# Begin Source File

SOURCE=.\engine\entity_types.h
# End Source File
# Begin Source File

SOURCE=.\engine\event_args.h
# End Source File
# Begin Source File

SOURCE=.\engine\event_flags.h
# End Source File
# Begin Source File

SOURCE=.\engine\in_buttons.h
# End Source File
# Begin Source File

SOURCE=.\engine\keydefs.h
# End Source File
# Begin Source File

SOURCE=.\engine\particledef.h
# End Source File
# Begin Source File

SOURCE=.\engine\pm_defs.h
# End Source File
# Begin Source File

SOURCE=.\engine\pm_info.h
# End Source File
# Begin Source File

SOURCE=.\engine\pmtrace.h
# End Source File
# Begin Source File

SOURCE=.\engine\progdefs.h
# End Source File
# Begin Source File

SOURCE=.\engine\progs.h
# End Source File
# Begin Source File

SOURCE=.\engine\r_efx.h
# End Source File
# Begin Source File

SOURCE=.\engine\shake.h
# End Source File
# Begin Source File

SOURCE=.\engine\studio.h
# End Source File
# Begin Source File

SOURCE=.\engine\studio_event.h
# End Source File
# Begin Source File

SOURCE=.\engine\triangleapi.h
# End Source File
# Begin Source File

SOURCE=.\engine\usercmd.h
# End Source File
# Begin Source File

SOURCE=.\engine\util_vector.h
# End Source File
# Begin Source File

SOURCE=.\engine\weaponinfo.h
# End Source File
# Begin Source File

SOURCE=.\engine\wrect.h
# End Source File
# End Group
# Begin Group "common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\common\beamdef.h
# End Source File
# Begin Source File

SOURCE=.\common\cl_entity.h
# End Source File
# Begin Source File

SOURCE=.\common\com_model.h
# End Source File
# Begin Source File

SOURCE=.\common\con_nprint.h
# End Source File
# Begin Source File

SOURCE=.\common\const.h
# End Source File
# Begin Source File

SOURCE=.\common\crc.h
# End Source File
# Begin Source File

SOURCE=.\common\cvardef.h
# End Source File
# Begin Source File

SOURCE=.\common\demo_api.h
# End Source File
# Begin Source File

SOURCE=.\common\director_cmds.h
# End Source File
# Begin Source File

SOURCE=.\common\dlight.h
# End Source File
# Begin Source File

SOURCE=.\common\dll_state.h
# End Source File
# Begin Source File

SOURCE=.\common\engine_launcher_api.h
# End Source File
# Begin Source File

SOURCE=.\common\entity_state.h
# End Source File
# Begin Source File

SOURCE=.\common\entity_types.h
# End Source File
# Begin Source File

SOURCE=.\common\event_api.h
# End Source File
# Begin Source File

SOURCE=.\common\event_args.h
# End Source File
# Begin Source File

SOURCE=.\common\event_flags.h
# End Source File
# Begin Source File

SOURCE=.\common\exefuncs.h
# End Source File
# Begin Source File

SOURCE=.\common\hltv.h
# End Source File
# Begin Source File

SOURCE=.\common\in_buttons.h
# End Source File
# Begin Source File

SOURCE=.\common\interface.cpp
# End Source File
# Begin Source File

SOURCE=.\common\interface.h
# End Source File
# Begin Source File

SOURCE=.\common\itrackeruser.h
# End Source File
# Begin Source File

SOURCE=.\common\ivoicetweak.h
# End Source File
# Begin Source File

SOURCE=.\common\mathlib.h
# End Source File
# Begin Source File

SOURCE=.\common\net_api.h
# End Source File
# Begin Source File

SOURCE=.\common\netadr.h
# End Source File
# Begin Source File

SOURCE=.\common\nowin.h
# End Source File
# Begin Source File

SOURCE=.\common\particledef.h
# End Source File
# Begin Source File

SOURCE=.\common\pmtrace.h
# End Source File
# Begin Source File

SOURCE=.\common\qfont.h
# End Source File
# Begin Source File

SOURCE=.\common\r_efx.h
# End Source File
# Begin Source File

SOURCE=.\common\ref_params.h
# End Source File
# Begin Source File

SOURCE=.\common\screenfade.h
# End Source File
# Begin Source File

SOURCE=.\common\studio_event.h
# End Source File
# Begin Source File

SOURCE=.\common\triangleapi.h
# End Source File
# Begin Source File

SOURCE=.\common\usercmd.h
# End Source File
# Begin Source File

SOURCE=.\common\weaponinfo.h
# End Source File
# End Group
# Begin Group "misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\misc\r_studioint.h
# End Source File
# End Group
# End Target
# End Project
