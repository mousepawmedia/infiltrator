; Script generated by the Inno Script Studio Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Infiltrator (x64)"
#define MyAppVersion "1.0"
#define MyAppPublisher "MousePaw Games"
#define MyAppURL "http://www.mousepawgames.com/"
#define MyAppExeName "infiltrator.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{2E043E46-5544-4F86-B532-42AC5D5189FB}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\MousePaw Games\Infiltrator
DefaultGroupName=MousePaw Games\Infiltrator
LicenseFile=LICENSE.txt
OutputDir=installer
OutputBaseFilename=InfiltratorSetupX64
SetupIconFile=infiltrator.ico
Compression=lzma
SolidCompression=yes; SourceDir=E:\infiltrator-git
WizardImageFile=installer\installerimage-infiltrator-dinnerparty.bmp

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "bin\Release\Winx64\bin\infiltrator.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libasprintf-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libatk-1.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libatkmm-1.6-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libatomic-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libbz2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libcairo-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libcairo-gobject-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libcairomm-1.0-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libcairo-script-interpreter-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libcharset-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libcroco-0.6-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libepoxy-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libexpat-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libffi-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libfontconfig-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libfreetype-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgailutil-3-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgcc_s_seh-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgdk_pixbuf-2.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgdk-3-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgdkmm-3.0-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgettextlib-0-19-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgettextpo-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgettextsrc-0-19-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgio-2.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgiomm-2.4-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libglib-2.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libglibmm_generate_extra_defs-2.4-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libglibmm-2.4-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgmodule-2.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgmp-10.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgmpxx-4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgobject-2.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgomp-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgomp-plugin-host_nonshm-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgthread-2.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgtk-3-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgtkmm-3.0-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libgtkreftestprivate-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libharfbuzz-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libharfbuzz-gobject-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libharfbuzz-icu-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libiconv-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libintl-8.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libjasper-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libjpeg-8.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libjson-glib-1.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\liblzma-5.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\liblzo2-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libminizip-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libpango-1.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libpangocairo-1.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libpangoft2-1.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libpangomm-1.4-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libpangowin32-1.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libpixman-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libpng16-16.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libquadmath-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\librsvg-2-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libsigc-2.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libssp-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libtiff-5.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libtiffxx-5.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libturbojpeg-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libvtv_stubs-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libvtv-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\bin\libxml2-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Winx64\share\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
