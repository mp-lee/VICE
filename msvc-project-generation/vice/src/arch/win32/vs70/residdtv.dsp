# Microsoft Developer Studio Project File - Name="residdtv" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=residdtv - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "residdtv.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "residdtv.mak" CFG="residdtv - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "residdtv - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "residdtv - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "residdtv - Win32 DX Release" (based on "Win32 (x86) Static Library")
!MESSAGE "residdtv - Win32 DX Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "residdtv - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "libs\residdtv\Release"
# PROP Intermediate_Dir "libs\residdtv\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "NDEBUG" /D PACKAGE=\"residdtv\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\msvc" /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "NDEBUG" /D PACKAGE=\"residdtv\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "WIN32" /d "IDE_COMPILE" /d "NODIRECTX"
# ADD RSC /l 0x409 /i "..\msvc" /i "..\\" /i "..\..\..\\" /d "NDEBUG" /d "WIN32" /d "IDE_COMPILE" /d "NODIRECTX"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "residdtv - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "libs\residdtv\Debug"
# PROP Intermediate_Dir "libs\residdtv\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "_DEBUG" /D PACKAGE=\"residdtv\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "..\msvc" /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "_DEBUG" /D PACKAGE=\"residdtv\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WIN32" /d "IDE_COMPILE" /d "NODIRECTX"
# ADD RSC /l 0x409 /i "..\msvc" /i "..\\" /i "..\..\..\\" /d "_DEBUG" /d "WIN32" /d "IDE_COMPILE" /d "NODIRECTX"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "residdtv - Win32 DX Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DXRelease"
# PROP BASE Intermediate_Dir "DXRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "libs\residdtv\DXRelease"
# PROP Intermediate_Dir "libs\residdtv\DXRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"residdtv\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\msvc" /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"residdtv\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "WIN32" /d "IDE_COMPILE"
# ADD RSC /l 0x409 /i "..\msvc" /i "..\\" /i "..\..\..\\" /d "NDEBUG" /d "WIN32" /d "IDE_COMPILE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "residdtv - Win32 DX Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DXDebug"
# PROP BASE Intermediate_Dir "DXDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "libs\residdtv\DXDebug"
# PROP Intermediate_Dir "libs\residdtv\DXDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"residdtv\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "..\msvc" /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"residdtv\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WIN32" /d "IDE_COMPILE"
# ADD RSC /l 0x409 /i "..\msvc" /i "..\\" /i "..\..\..\\" /d "_DEBUG" /d "WIN32" /d "IDE_COMPILE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF

# Begin Target

# Name "residdtv - Win32 Release"
# Name "residdtv - Win32 Debug"
# Name "residdtv - Win32 DX Release"
# Name "residdtv - Win32 DX Debug"
# Begin Source File

SOURCE="..\..\..\resid-dtv\envelope.cc"

!IF  "$(CFG)" == "residdtv - Win32 Release"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\envelope.cc"
InputName=envelope

"libs\residdtv\Release\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MT /W3 /GX /O2 /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\Release\residdtv.pch" /Fo"libs\residdtv\Release\\" /Fd"libs\residdtv\Release\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 Debug"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\envelope.cc"
InputName=envelope

"libs\residdtv\Debug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MTd /W3 /GX /Z7 /Od /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\Debug\residdtv.pch" /Fo"libs\residdtv\Debug\\" /Fd"libs\residdtv\Debug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 DX Release"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\envelope.cc"
InputName=envelope

"libs\residdtv\DXRelease\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MT /W3 /GX /O2 /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\DXRelease\residdtv.pch" /Fo"libs\residdtv\DXRelease\\" /Fd"libs\residdtv\DXRelease\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 DX Debug"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\envelope.cc"
InputName=envelope

"libs\residdtv\DXDebug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MTd /W3 /GX /Z7 /Od /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\DXDebug\residdtv.pch" /Fo"libs\residdtv\DXDebug\\" /Fd"libs\residdtv\DXDebug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ENDIF

# End Source File
# Begin Source File

SOURCE="..\..\..\resid-dtv\extfilt.cc"

!IF  "$(CFG)" == "residdtv - Win32 Release"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\extfilt.cc"
InputName=extfilt

"libs\residdtv\Release\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MT /W3 /GX /O2 /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\Release\residdtv.pch" /Fo"libs\residdtv\Release\\" /Fd"libs\residdtv\Release\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 Debug"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\extfilt.cc"
InputName=extfilt

"libs\residdtv\Debug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MTd /W3 /GX /Z7 /Od /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\Debug\residdtv.pch" /Fo"libs\residdtv\Debug\\" /Fd"libs\residdtv\Debug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 DX Release"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\extfilt.cc"
InputName=extfilt

"libs\residdtv\DXRelease\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MT /W3 /GX /O2 /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\DXRelease\residdtv.pch" /Fo"libs\residdtv\DXRelease\\" /Fd"libs\residdtv\DXRelease\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 DX Debug"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\extfilt.cc"
InputName=extfilt

"libs\residdtv\DXDebug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MTd /W3 /GX /Z7 /Od /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\DXDebug\residdtv.pch" /Fo"libs\residdtv\DXDebug\\" /Fd"libs\residdtv\DXDebug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ENDIF

# End Source File
# Begin Source File

SOURCE="..\..\..\resid-dtv\filter.cc"

!IF  "$(CFG)" == "residdtv - Win32 Release"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\filter.cc"
InputName=filter

"libs\residdtv\Release\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MT /W3 /GX /O2 /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\Release\residdtv.pch" /Fo"libs\residdtv\Release\\" /Fd"libs\residdtv\Release\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 Debug"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\filter.cc"
InputName=filter

"libs\residdtv\Debug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MTd /W3 /GX /Z7 /Od /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\Debug\residdtv.pch" /Fo"libs\residdtv\Debug\\" /Fd"libs\residdtv\Debug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 DX Release"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\filter.cc"
InputName=filter

"libs\residdtv\DXRelease\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MT /W3 /GX /O2 /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\DXRelease\residdtv.pch" /Fo"libs\residdtv\DXRelease\\" /Fd"libs\residdtv\DXRelease\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 DX Debug"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\filter.cc"
InputName=filter

"libs\residdtv\DXDebug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MTd /W3 /GX /Z7 /Od /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\DXDebug\residdtv.pch" /Fo"libs\residdtv\DXDebug\\" /Fd"libs\residdtv\DXDebug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ENDIF

# End Source File
# Begin Source File

SOURCE="..\..\..\resid-dtv\sid.cc"

!IF  "$(CFG)" == "residdtv - Win32 Release"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\sid.cc"
InputName=sid

"libs\residdtv\Release\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MT /W3 /GX /O2 /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\Release\residdtv.pch" /Fo"libs\residdtv\Release\\" /Fd"libs\residdtv\Release\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 Debug"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\sid.cc"
InputName=sid

"libs\residdtv\Debug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MTd /W3 /GX /Z7 /Od /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\Debug\residdtv.pch" /Fo"libs\residdtv\Debug\\" /Fd"libs\residdtv\Debug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 DX Release"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\sid.cc"
InputName=sid

"libs\residdtv\DXRelease\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MT /W3 /GX /O2 /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\DXRelease\residdtv.pch" /Fo"libs\residdtv\DXRelease\\" /Fd"libs\residdtv\DXRelease\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 DX Debug"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\sid.cc"
InputName=sid

"libs\residdtv\DXDebug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MTd /W3 /GX /Z7 /Od /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\DXDebug\residdtv.pch" /Fo"libs\residdtv\DXDebug\\" /Fd"libs\residdtv\DXDebug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ENDIF

# End Source File
# Begin Source File

SOURCE="..\..\..\resid-dtv\version.cc"

!IF  "$(CFG)" == "residdtv - Win32 Release"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\version.cc"
InputName=version

"libs\residdtv\Release\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MT /W3 /GX /O2 /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\Release\residdtv.pch" /Fo"libs\residdtv\Release\\" /Fd"libs\residdtv\Release\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 Debug"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\version.cc"
InputName=version

"libs\residdtv\Debug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MTd /W3 /GX /Z7 /Od /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\Debug\residdtv.pch" /Fo"libs\residdtv\Debug\\" /Fd"libs\residdtv\Debug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 DX Release"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\version.cc"
InputName=version

"libs\residdtv\DXRelease\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MT /W3 /GX /O2 /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\DXRelease\residdtv.pch" /Fo"libs\residdtv\DXRelease\\" /Fd"libs\residdtv\DXRelease\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 DX Debug"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\version.cc"
InputName=version

"libs\residdtv\DXDebug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MTd /W3 /GX /Z7 /Od /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\DXDebug\residdtv.pch" /Fo"libs\residdtv\DXDebug\\" /Fd"libs\residdtv\DXDebug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ENDIF

# End Source File
# Begin Source File

SOURCE="..\..\..\resid-dtv\voice.cc"

!IF  "$(CFG)" == "residdtv - Win32 Release"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\voice.cc"
InputName=voice

"libs\residdtv\Release\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MT /W3 /GX /O2 /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\Release\residdtv.pch" /Fo"libs\residdtv\Release\\" /Fd"libs\residdtv\Release\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 Debug"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\voice.cc"
InputName=voice

"libs\residdtv\Debug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MTd /W3 /GX /Z7 /Od /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\Debug\residdtv.pch" /Fo"libs\residdtv\Debug\\" /Fd"libs\residdtv\Debug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 DX Release"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\voice.cc"
InputName=voice

"libs\residdtv\DXRelease\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MT /W3 /GX /O2 /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\DXRelease\residdtv.pch" /Fo"libs\residdtv\DXRelease\\" /Fd"libs\residdtv\DXRelease\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 DX Debug"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\voice.cc"
InputName=voice

"libs\residdtv\DXDebug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MTd /W3 /GX /Z7 /Od /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\DXDebug\residdtv.pch" /Fo"libs\residdtv\DXDebug\\" /Fd"libs\residdtv\DXDebug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ENDIF

# End Source File
# Begin Source File

SOURCE="..\..\..\resid-dtv\wave.cc"

!IF  "$(CFG)" == "residdtv - Win32 Release"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\wave.cc"
InputName=wave

"libs\residdtv\Release\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MT /W3 /GX /O2 /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\Release\residdtv.pch" /Fo"libs\residdtv\Release\\" /Fd"libs\residdtv\Release\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 Debug"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\wave.cc"
InputName=wave

"libs\residdtv\Debug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MTd /W3 /GX /Z7 /Od /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\Debug\residdtv.pch" /Fo"libs\residdtv\Debug\\" /Fd"libs\residdtv\Debug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 DX Release"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\wave.cc"
InputName=wave

"libs\residdtv\DXRelease\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MT /W3 /GX /O2 /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\DXRelease\residdtv.pch" /Fo"libs\residdtv\DXRelease\\" /Fd"libs\residdtv\DXRelease\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "residdtv - Win32 DX Debug"

# Begin Custom Build
InputPath="..\..\..\resid-dtv\wave.cc"
InputName=wave

"libs\residdtv\DXDebug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MTd /W3 /GX /Z7 /Od /EHsc /I "..\msvc"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\residdtv\DXDebug\residdtv.pch" /Fo"libs\residdtv\DXDebug\\" /Fd"libs\residdtv\DXDebug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ENDIF

# End Source File
# End Target
# End Project
