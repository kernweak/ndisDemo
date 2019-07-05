# Microsoft Developer Studio Project File - Name="FirewallDemo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FirewallDemo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FirewallDemo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FirewallDemo.mak" CFG="FirewallDemo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FirewallDemo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FirewallDemo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FirewallDemo - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "FirewallDemo - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "FirewallDemo - Win32 Release"
# Name "FirewallDemo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddRule.cpp
# End Source File
# Begin Source File

SOURCE=..\common\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=..\common\ColorStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\Common.cpp
# End Source File
# Begin Source File

SOURCE=.\Config.cpp
# End Source File
# Begin Source File

SOURCE=.\FirewallDemo.cpp
# End Source File
# Begin Source File

SOURCE=.\FirewallDemo.rc
# End Source File
# Begin Source File

SOURCE=.\FirewallDemoDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\common\HistogramCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\common\HyperlinkStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\Log.cpp
# End Source File
# Begin Source File

SOURCE=.\Monitor.cpp
# End Source File
# Begin Source File

SOURCE=.\NetState.cpp
# End Source File
# Begin Source File

SOURCE=..\common\PasseckDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\common\SystemTray.cpp
# End Source File
# Begin Source File

SOURCE=..\common\ThemeHelperST.cpp
# End Source File
# Begin Source File

SOURCE=.\XInstall.cpp
# End Source File
# Begin Source File

SOURCE=..\common\XPStyleButtonST.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddRule.h
# End Source File
# Begin Source File

SOURCE=..\common\BtnST.h
# End Source File
# Begin Source File

SOURCE=..\common\ColorStatic.h
# End Source File
# Begin Source File

SOURCE=.\Common.h
# End Source File
# Begin Source File

SOURCE=.\condef.h
# End Source File
# Begin Source File

SOURCE=.\Config.h
# End Source File
# Begin Source File

SOURCE=.\DDKLBLInc.h
# End Source File
# Begin Source File

SOURCE=.\FirewallDemo.h
# End Source File
# Begin Source File

SOURCE=.\FirewallDemoDlg.h
# End Source File
# Begin Source File

SOURCE=..\common\HistogramCtrl.h
# End Source File
# Begin Source File

SOURCE=..\common\HyperlinkStatic.h
# End Source File
# Begin Source File

SOURCE=.\IN_SYSTM.H
# End Source File
# Begin Source File

SOURCE=.\Log.h
# End Source File
# Begin Source File

SOURCE=.\Monitor.h
# End Source File
# Begin Source File

SOURCE=.\NetState.h
# End Source File
# Begin Source File

SOURCE=..\common\PasseckDialog.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\common\SystemTray.h
# End Source File
# Begin Source File

SOURCE=..\common\ThemeHelperST.h
# End Source File
# Begin Source File

SOURCE=.\XInstall.h
# End Source File
# Begin Source File

SOURCE=..\common\XPStyleButtonST.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap11.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BITMAP3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BITMAP4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BITMAP6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BITMAP7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BITMAP9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CONFIG.ICO
# End Source File
# Begin Source File

SOURCE=.\res\EXIT.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Filter.ico
# End Source File
# Begin Source File

SOURCE=.\res\FirewallDemo.ico
# End Source File
# Begin Source File

SOURCE=.\res\FirewallDemo.rc2
# End Source File
# Begin Source File

SOURCE=.\res\FW_LOGO.ico
# End Source File
# Begin Source File

SOURCE=.\res\Green_logo.ico
# End Source File
# Begin Source File

SOURCE=.\res\HELP.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Hide.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\log.ico
# End Source File
# Begin Source File

SOURCE=.\res\LOGO.ICO
# End Source File
# Begin Source File

SOURCE=.\res\main.ico
# End Source File
# Begin Source File

SOURCE=.\res\netstate.ico
# End Source File
# Begin Source File

SOURCE=.\res\NullApp.ico
# End Source File
# Begin Source File

SOURCE=.\res\Orange_logo.ico
# End Source File
# Begin Source File

SOURCE=.\res\Red_logo.ico
# End Source File
# Begin Source File

SOURCE=.\res\REG.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Snap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\teen.ico
# End Source File
# Begin Source File

SOURCE=.\res\top_011.ico
# End Source File
# Begin Source File

SOURCE=.\res\UPDATE.ICO
# End Source File
# Begin Source File

SOURCE=.\res\updateitem.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
