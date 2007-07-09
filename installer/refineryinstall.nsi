; refineryinstall.nsi
;
; This script attempts to install an uninstall refinery package.

;--------------------------------

Name "Refinery Install"
Caption "Refinery Install"
;Icon "${EXEDIR}\icons\install.ico"
OutFile "refineryinstall.exe"

;SetDateSave on
;SetDatablockOptimize on
;CRCCheck on
;SilentInstall normal
;BGGradient 000000 800000 FFFFFF
;InstallColors FF8080 000030
;XPStyle on

SetCompressor /SOLID lzma

InstallDir "$PROGRAMFILES\Refinery"
InstallDirRegKey HKLM "Software\Refinery" "Install_Dir"

;CheckBitmap "${EXEDIR}\icons\classic-cross.bmp"

LicenseText "Pease read the following important information before continuing"
LicenseData "scea-shared-source-lic1.0.txt"

;--------------------------------

Page license
Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles
  
;--------------------------------

AutoCloseWindow false
ShowInstDetails show

;--------------------------------

Section "" ; empty string makes it hidden, so would starting with -

  SetOutPath $INSTDIR
  WriteRegStr HKLM Software\Refinery "Install_Dir" "$INSTDIR"
  
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Refinery" "DisplayName" "Refinery"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Refinery" "UninstallString" '"$INSTDIR\refinery-uninst.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Refinery" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Refinery" "NoRepair" 1
  WriteUninstaller "refinery-uninst.exe"
  
SectionEnd

SectionGroup "Refinery with Conditioners"

Section "Refinery"
	File refinery.jar
    ;File launcher.exe
	;File launcher_cl.exe
	File libloader.dll
	File DevIL.dll
	File conditionerlist.txt
	File preferences.xml
	File savefiles.xsd
	File update.exe
	File update.dat
	File version.ini
	
	SetOutPath $INSTDIR\icons
	File icons\*.*

SectionEnd

Section "macros"
    SetOutPath $INSTDIR\macros
	File macros\*.*  
SectionEnd

Section "savefiles"
    SetOutPath $INSTDIR\savefiles
	File savefiles\*.*  
SectionEnd

SectionGroupEnd

Section "Conditioner Executables"

     SetOutPath $INSTDIR\bin
     File bin\*.exe
     
SectionEnd

Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Refinery"
  CreateShortCut "$SMPROGRAMS\Refinery\Uninstall.lnk" "$INSTDIR\refinery-uninst.exe" "" "$INSTDIR\refinery-uninst.exe" 0
  ;CreateShortCut "$SMPROGRAMS\Refinery\Refinery (MakeNSISW).lnk" "$EXEDIR\refineryinstall.exe" "" "$EXEDIR\refineryinstall.exe" 0
  ;CreateShortCut "$SMPROGRAMS\Refinery\Refinery.lnk" "$EXEDIR\launcher.exe" "" "$EXEDIR\launcher.exe" 0
  CreateShortCut "$SMPROGRAMS\Refinery\Update.lnk" "$INSTDIR\update.exe" "" "$INSTDIR\update.exe" 0
  
SectionEnd
;--------------------------------

; Uninstaller

UninstallText "This will uninstall Refinery. Click next to continue."
;UninstallIcon "${EXEDIR}\icons\uninstall.ico"

Section "Uninstall"

  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Refinery"
  DeleteRegKey HKLM Software\Refinery

  ; Remove files and uninstaller
  Delete $INSTDIR\*.*
  Delete $INSTDIR\bin\*.*
  RMDir  "$INSTDIR\bin"
  Delete $INSTDIR\icons\*.*
  RMDir  "$INSTDIR\icons"
  Delete $INSTDIR\macros\*.*
  RMDir  "$INSTDIR\macros"
  Delete $INSTDIR\savefiles\*.*
  RMDir  "$INSTDIR\savefiles"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Refinery\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\Refinery"
  RMDir "$INSTDIR"
  
SectionEnd
