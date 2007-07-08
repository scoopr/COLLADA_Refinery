; refineryinstall.nsi
;
; This script attempts to install an uninstall refinery package.
!include "MUI.nsh"
!include AddToPath.nsh
!include WriteEnvStr.nsh
!include Sections.nsh

;--------------------------------

Name "Refinery Install"
Caption "Refinery Install"
;Icon "${EXEDIR}\icons\collada.ico"
OutFile "..\COLLADA_Refinery_2.1.0.exe"

Var PROGRAM_NAME
SetCompressor /SOLID lzma

InstallDir "$PROGRAMFILES\COLLADA\COLLADA_Refinery"
InstallDirRegKey HKLM "Software\COLLADA_Refinery" "Install_Dir"

;CheckBitmap "${EXEDIR}\icons\classic-cross.bmp"

LicenseText "Pease read the following important information before continuing"
LicenseData "scea-shared-source-lic1.0.txt"

;--------------------------------
; Pages
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "scea-shared-source-lic1.0.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  # These indented statements modify settings for MUI_PAGE_FINISH
    !define MUI_FINISHPAGE_NOAUTOCLOSE
    !define MUI_FINISHPAGE_RUN
    !define MUI_FINISHPAGE_RUN_CHECKED
    !define MUI_FINISHPAGE_RUN_TEXT "Start COLLADA Refinery"
    !define MUI_FINISHPAGE_RUN_FUNCTION "LaunchRefinery"
    !define MUI_FINISHPAGE_SHOWREADME_CHECKED
    !define MUI_FINISHPAGE_SHOWREADME_FUNCTION "LaunchReadme"
    !define MUI_FINISHPAGE_SHOWREADME $INSTDIR\readme_e.txt
  !insertmacro MUI_PAGE_FINISH

;Languages
!insertmacro MUI_LANGUAGE "English"
  
UninstPage uninstConfirm
UninstPage instfiles
AutoCloseWindow false

;--------------------------------
; Sections
Section "" ; empty string makes it hidden, so would starting with -

  StrCpy '$PROGRAM_NAME' "Refinery"

  SetOutPath $INSTDIR
  WriteRegStr HKLM Software\Refinery "Install_Dir" "$INSTDIR"
  
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Refinery" "DisplayName" "COLLADA Refinery"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Refinery" "UninstallString" '"$INSTDIR\refinery-uninst.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Refinery" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Refinery" "NoRepair" 1
  WriteUninstaller "refinery-uninst.exe"
 
SectionEnd

Section "Binaries" Sec1
	; Install the VS 2005 redist
	SetOutPath $INSTDIR
	File vcredist_x86.exe
	ExecWait '"$INSTDIR\vcredist_x86.exe"'
	Delete vcredist_x86.exe

	SetOutPath $INSTDIR\bin\vc8\release
	File /r bin\vc8\release\*.dll
	File /r bin\vc8\release\*.bat
	File /r bin\vc8\release\*.jar
	SetOutPath $INSTDIR\icons
	File icons\*.*
    SetOutPath $INSTDIR\macros
 	File macros\*.pip    
	SetOutPath $INSTDIR
	File savefiles.xsd
	File version.ini
	File readme_e.txt
	File scea-shared-source-lic1.0.txt
	File scea-shared-source-license1.0.pdf
	File update.dat
	File update.exe	
SectionEnd

Section /o "Source" Sec2
	Call DetectMSVS
	Call DetectJDK

	; Install the VS 2005 redist
	SetOutPath $INSTDIR
	File vcredist_x86.exe
	ExecWait '"$INSTDIR\vcredist_x86.exe"'
	Delete vcredist_x86.exe

	SetOutPath $INSTDIR\bin\vc8\release
	File /r bin\vc8\release\*.dll
	File /r bin\vc8\release\*.bat
	File /r bin\vc8\release\*.jar
	
	SetOutPath $INSTDIR\bin\vc8\debug
	File bin\vc8\debug\DevIL.dll
	File bin\vc8\debug\iconv.dll
	File bin\vc8\debug\libcollada141dom12.dll
	File bin\vc8\debug\zip32.dll
	File bin\vc8\debug\refinery.bat

	SetOutPath $INSTDIR\bin\vc7\release
	File bin\vc7\release\DevIL.dll
	File bin\vc7\release\iconv.dll
	File bin\vc7\release\libcollada141dom12.dll
	File bin\vc7\release\zip32.dll
	File bin\vc7\release\refinery.bat

	SetOutPath $INSTDIR\bin\vc7\debug
	File bin\vc7\debug\DevIL.dll
	File bin\vc7\debug\iconv.dll
	File bin\vc7\debug\libcollada141dom12.dll
	File bin\vc7\debug\zip32.dll
	File bin\vc7\debug\refinery.bat

	SetOutPath $INSTDIR\icons
	File icons\*.*
    SetOutPath $INSTDIR\macros
	File macros\*.*  
	SetOutPath $INSTDIR\include
	File include\*.*
    SetOutPath $INSTDIR\projects
	File /r projects\*.sln  
	File /r projects\*.vcproj  
    SetOutPath $INSTDIR\src
	File /r src\*.cpp  
	File /r src\*.java  
	File /r src\*.txt  

    SetOutPath $INSTDIR\external-libs\COLLADA_DOM
	File /r /x .svn external-libs\COLLADA_DOM\*.*  

    SetOutPath $INSTDIR\..\external-libs\devil
	File /r /x .svn ..\external-libs\devil\*.*   
	
    SetOutPath $INSTDIR\..\external-libs\zip32
	File /r /x .svn ..\external-libs\zip32\*.*  

    SetOutPath $INSTDIR\..\external-libs\libxml2
	File /r /x .svn ..\external-libs\libxml2\*.*   

	SetOutPath $INSTDIR
	File savefiles.xsd
	File version.ini
	File readme_e.txt
	File scea-shared-source-lic1.0.txt
	File scea-shared-source-license1.0.pdf
	File update.dat
	File update.exe	
SectionEnd

Section "Create short cut" Sec3
  SetOutPath "$INSTDIR\bin\vc8\release"
  CreateDirectory "$SMPROGRAMS\COLLADA\"
  CreateDirectory "$SMPROGRAMS\COLLADA\COLLADA Refinery"
  CreateShortCut "$SMPROGRAMS\COLLADA\COLLADA Refinery\Readme.lnk" "$PROGRAMFILES\Windows NT\Accessories\wordpad.exe" "$\"$INSTDIR\readme_e.txt$\"" "$PROGRAMFILES\Windows NT\Accessories\wordpad.exe" 0
  CreateShortCut "$SMPROGRAMS\COLLADA\COLLADA Refinery\Refinery.lnk" "$INSTDIR\bin\vc8\release\refinery.bat" "" "$INSTDIR\icons\collada.ico" 0 SW_SHOWMINIMIZED
  CreateShortCut "$SMPROGRAMS\COLLADA\COLLADA Refinery\Uninstall.lnk" "$INSTDIR\refinery-uninst.exe" "" "$INSTDIR\refinery-uninst.exe" 0
  CreateShortCut "$SMPROGRAMS\COLLADA\COLLADA Refinery\Update.lnk" "$INSTDIR\update.exe" "" "$INSTDIR\update.exe" 0
SectionEnd

Section "Environment Variables" Sec4

  ReadEnvStr $3 "COLLADA_EXTERNAL_LIBS_LOCATION"
  StrCmp $3 "" 0 ExternalLibsLocDone
  Push "COLLADA_EXTERNAL_LIBS_LOCATION"
  Push "$INSTDIR\..\external-libs"
  Call WriteEnvStr
  ExternalLibsLocDone:
 
  Push "PATH"
  Push "$INSTDIR\bin\vc8\release"
  Call AddToEnvVar
  
  ReadEnvStr $3 "JDK_HOME"
  StrCmp $3 "" 0 JdkHomeDone
  ReadRegStr $1 HKLM "SOFTWARE\JavaSoft\Java Development Kit" "CurrentVersion"
  ReadRegStr $2 HKLM "SOFTWARE\JavaSoft\Java Development Kit\$1" "JavaHome"
  Push "JDK_HOME"
  Push $2
  Call WriteEnvStr
  JdkHomeDone:

  ReadRegStr $1 HKLM "SOFTWARE\JavaSoft\Java Runtime Environment" "CurrentVersion"
  ReadRegStr $2 HKLM "SOFTWARE\JavaSoft\Java Runtime Environment\$1" "JavaHome"
  Push "PATH"
  Push "$2\bin"
  Call AddToEnvVar

SectionEnd

LangString DESC_Section1 ${LANG_ENGLISH} "Everything you need to run COLLADA Refinery"
LangString DESC_Section2 ${LANG_ENGLISH} "All binaries and everything you need to build conditioners and refinery"
LangString DESC_Section3 ${LANG_ENGLISH} "Add short cuts"
LangString DESC_Section4 ${LANG_ENGLISH} "Set environment variables"

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${Sec1} $(DESC_Section1)
  !insertmacro MUI_DESCRIPTION_TEXT ${Sec2} $(DESC_Section2)
  !insertmacro MUI_DESCRIPTION_TEXT ${Sec3} $(DESC_Section3)
  !insertmacro MUI_DESCRIPTION_TEXT ${Sec4} $(DESC_Section4)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
; Uninstaller

UninstallText "This will uninstall Refinery. Click next to continue."
;UninstallIcon "${EXEDIR}\icons\uninstall.ico"

Section "Uninstall"

  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Refinery"
  DeleteRegKey HKLM Software\Refinery

  Push "$INSTDIR"
  Push ""
  Call un.RmDirsButOne
  Delete "$INSTDIR\*.*"
  RMDir "$INSTDIR"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\COLLADA\COLLADA Refinery\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\COLLADA\COLLADA Refinery"
  RMDir "$SMPROGRAMS\COLLADA"
  RMDir "$INSTDIR"
 
  Push "COLLADA_EXTERNAL_LIBS_LOCATION"
  Push "$INSTDIR\..\external-libs"
  Call un.RemoveFromEnvVar
 
  Push "PATH"
  Push "$INSTDIR\bin"
  Call un.RemoveFromEnvVar
  
SectionEnd

;--------------------------------
; Function
 
Function .onInit
  Call DetectJRE
   
  StrCpy '$PROGRAM_NAME' "Refinery"

  ReadRegStr $R0 HKLM \
  "Software\Microsoft\Windows\CurrentVersion\Uninstall\Refinery" \
  "UninstallString"
  StrCmp $R0 "" done
 
  MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
  "Refinery is already installed. $\n$\nClick `OK` to remove the \
  previous version or `Cancel` to cancel this upgrade." \
  IDOK uninst
  Abort
  
;Run the uninstaller
uninst:
  ClearErrors
  ExecWait '$R0 _?=$INSTDIR' ;Do not copy the uninstaller to a temp file
 
  IfErrors no_remove_uninstaller
    ;You can either use Delete /REBOOTOK in the uninstaller or add some code
    ;here to remove the uninstaller. Use a registry key to check
    ;whether the user has chosen to uninstall. If you are using an uninstaller
    ;components page, make sure all sections are uninstalled.
  no_remove_uninstaller:
  
done:
 
  StrCpy $1 ${Sec1} ; Group 1 - Option 1 is selected by default

FunctionEnd

Function .onSelChange

  !insertmacro StartRadioButtons $1
    !insertmacro RadioButton ${Sec1}
    !insertmacro RadioButton ${Sec2}
  !insertmacro EndRadioButtons
	
FunctionEnd


Function un.RmDirsButOne
 Exch $R0 ; exclude dir
 Exch
 Exch $R1 ; route dir
 Push $R2
 Push $R3
 
  FindFirst $R3 $R2 "$R1\*.*"
  IfErrors Exit
 
  Top:
   StrCmp $R2 "." Next
   StrCmp $R2 ".." Next
   StrCmp $R2 $R0 Next
   IfFileExists "$R1\$R2\*.*" 0 Next
    RmDir /r "$R1\$R2"
 
   #Goto Exit ;uncomment this to stop it being recursive
 
   Next:
    ClearErrors
    FindNext $R3 $R2
    IfErrors Exit
   Goto Top
 
  Exit:
  FindClose $R3
 
 Pop $R3
 Pop $R2
 Pop $R1
 Pop $R0
FunctionEnd


Function DetectJDK
  Push $1
  Push $2
  ReadRegStr $1 HKLM "SOFTWARE\JavaSoft\Java Development Kit" "CurrentVersion"
  StrCmp $1 "" NoFound
  ReadRegStr $2 HKLM "SOFTWARE\JavaSoft\Java Development Kit\$1" "JavaHome"
  StrCmp $2 "" NoFound
  Goto DetectJDKEnd
NoFound:
  MessageBox MB_OK "JDK not found, Please install JDK if you want to compile Refinery"
  Goto DetectJDKEnd
DetectJDKEnd:  
  Pop $2
  Pop $1
FunctionEnd

Function DetectJRE
  Push $1
  Push $2
  ReadRegStr $1 HKLM "SOFTWARE\JavaSoft\Java Runtime Environment" "CurrentVersion"
  StrCmp $1 "" NoFound
  ReadRegStr $2 HKLM "SOFTWARE\JavaSoft\Java Runtime Environment\$1" "JavaHome"
  StrCmp $2 "" NoFound
  Goto DetectJREEnd
NoFound:
  MessageBox MB_OK "JRE not found, Please install JRE\nYou can't run Refinery without JRE"
  Abort
  Goto DetectJREEnd
DetectJREEnd:  
  Pop $2
  Pop $1
FunctionEnd

Function DetectMSVS
  Push $1
  ReadRegStr $1 HKLM "SOFTWARE\Microsoft\VisualStudio\7.1" "InstallDir"
  StrCmp $1 "" NoVS71 DetectMSVSEnd
NoVS71:
  ReadRegStr $1 HKLM "SOFTWARE\Microsoft\VisualStudio\8.0" "InstallDir"
  StrCmp $1 "" NoVS80 DetectMSVSEnd
NoVS80:
  MessageBox MB_OK "Microsoft Visual Studio not found, Please install Microsoft Visual Studio 7.1 or 8.0 if you want to compile"
  Goto DetectMSVSEnd
DetectMSVSEnd:  
  Pop $1
FunctionEnd

Function LaunchRefinery
  ExecShell "" "$SMPROGRAMS\COLLADA\COLLADA Refinery\Refinery.lnk"
FunctionEnd

Function LaunchReadme
  ExecShell "" "$SMPROGRAMS\COLLADA\COLLADA Refinery\Readme.lnk"
FunctionEnd