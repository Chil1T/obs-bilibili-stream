#define AppPublisher "Little Lemon"
#ifndef AppName
#define AppName "bilibili-stream-for-obs"
#endif
#ifndef AppVersion
#define AppVersion "0.0.0"
#endif
#ifndef ProjectRoot
#define ProjectRoot "..\.."
#endif
#ifndef BuildConfig
#define BuildConfig "RelWithDebInfo"
#endif
#ifndef OutputDir
#define OutputDir "..\..\release"
#endif
#ifndef OutputBaseName
#define OutputBaseName AppName + "-" + AppVersion + "-windows-x64-installer"
#endif

[Setup]
AppId={{86C230F7-BD58-4F81-A59C-B6261FF06A5F}
AppName=Bilibili Stream for OBS
AppVersion={#AppVersion}
AppPublisher={#AppPublisher}
DefaultDirName={commonappdata}\obs-studio\plugins\{#AppName}
DisableDirPage=yes
DisableProgramGroupPage=yes
OutputDir={#OutputDir}
OutputBaseFilename={#OutputBaseName}
Compression=lzma2
SolidCompression=yes
PrivilegesRequired=admin
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
UninstallDisplayName=Bilibili Stream for OBS

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "{#ProjectRoot}\release\{#BuildConfig}\{#AppName}\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Code]
function InitializeSetup(): Boolean;
var
  ResultCode: Integer;
begin
  Exec(ExpandConstant('{cmd}'), '/C tasklist /FI "IMAGENAME eq obs64.exe" | find /I "obs64.exe" >NUL', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
  if ResultCode = 0 then begin
    MsgBox('请先退出 OBS Studio，再安装插件。', mbError, MB_OK);
    Result := False;
  end else begin
    Result := True;
  end;
end;
