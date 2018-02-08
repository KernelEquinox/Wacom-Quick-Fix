#pragma once

#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <setupapi.h>
#include <regstr.h>
#include <infstr.h>
#include <cfgmgr32.h>
#include <string.h>
#include <malloc.h>
#include <newdev.h>
#include <objbase.h>
#include <strsafe.h>
#include <io.h>
#include <fcntl.h>
#include <afxext.h>
#include <winsvc.h>

#ifndef ARRAYSIZE
#define ARRAYSIZE(a)                (sizeof(a)/sizeof(a[0]))
#endif

typedef int(*DispatchFunc)(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_ LPTSTR argv[]);
typedef int(*CallbackFunc)(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo, _In_ DWORD Index, _In_ LPVOID Context);

typedef struct {
	LPCTSTR         cmd;
	DispatchFunc    func;
	DWORD           shortHelp;
	DWORD           longHelp;
} DispatchEntry;

extern DispatchEntry DispatchTable[];

#ifndef ARRAYSIZE
#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))
#endif

#define INSTANCEID_PREFIX_CHAR TEXT('@') // character used to prefix instance ID's
#define CLASS_PREFIX_CHAR      TEXT('=') // character used to prefix class name
#define WILD_CHAR              TEXT('*') // wild character
#define QUOTE_PREFIX_CHAR      TEXT('\'') // prefix character to ignore wild characters
#define SPLIT_COMMAND_SEP      TEXT(":=") // whole word, indicates end of id's

//
// Devcon.exe command line flags
//
#define DEVCON_FLAG_FORCE       0x00000001

void FormatToStream(_In_ FILE * stream, _In_ DWORD fmt, ...);
void Padding(_In_ int pad);
bool SplitCommandLine(
	_In_ int & argc,
	_In_reads_(argc) LPTSTR * & argv,
	_Out_ int & argc_right,
	_Outref_result_buffer_(argc_right) LPTSTR * & argv_right);
int EnumerateDevices(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PWSTR* argv, _In_ CallbackFunc Callback, _In_ LPVOID Context);
LPTSTR GetDeviceStringProperty(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo, _In_ DWORD Prop);
LPTSTR GetDeviceDescription(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo);
__drv_allocatesMem(object) LPTSTR * GetDevMultiSz(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo, _In_ DWORD Prop);
__drv_allocatesMem(object) LPTSTR * GetRegMultiSz(_In_ HKEY hKey, _In_ LPCTSTR Val);
__drv_allocatesMem(object) LPTSTR * GetMultiSzIndexArray(_In_ __drv_aliasesMem LPTSTR MultiSz);
void DelMultiSz(_In_opt_ __drv_freesMem(object) PZPWSTR Array);
__drv_allocatesMem(object) LPTSTR * CopyMultiSz(_In_opt_ PZPWSTR Array);

BOOL DumpArray(_In_ int pad, _In_ PZPWSTR Array);
BOOL DumpDevice(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo);
BOOL DumpDeviceClass(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo);
BOOL DumpDeviceDescr(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo);
BOOL DumpDeviceStatus(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo);
BOOL DumpDeviceResources(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo);
BOOL DumpDeviceDriverFiles(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo);
BOOL DumpDeviceDriverNodes(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo);
BOOL DumpDeviceHwIds(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo);
BOOL DumpDeviceWithInfo(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo, _In_opt_ LPCTSTR Info);
BOOL DumpDeviceStack(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo);
BOOL DumpDriverPackageData(_In_ LPCTSTR InfName);
BOOL Reboot();
int cmdRestart(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);
int cmdFind(_In_ LPCTSTR BaseName, _In_opt_ LPCTSTR Machine, _In_ DWORD Flags, _In_ int argc, _In_reads_(argc) PTSTR argv[]);


//
// UpdateDriverForPlugAndPlayDevices
//
typedef BOOL(WINAPI *UpdateDriverForPlugAndPlayDevicesProto)(_In_opt_ HWND hwndParent,
	_In_ LPCTSTR HardwareId,
	_In_ LPCTSTR FullInfPath,
	_In_ DWORD InstallFlags,
	_Out_opt_ PBOOL bRebootRequired
	);
typedef BOOL(WINAPI *SetupSetNonInteractiveModeProto)(_In_ BOOL NonInteractiveFlag
	);
typedef BOOL(WINAPI *SetupUninstallOEMInfProto)(_In_ LPCTSTR InfFileName,
	_In_ DWORD Flags,
	_Reserved_ PVOID Reserved
	);

#if _SETUPAPI_VER >= _WIN32_WINNT_WINXP
typedef BOOL(WINAPI *SetupVerifyInfFileProto)(_In_ LPCTSTR InfName,
	_In_opt_ PSP_ALTPLATFORM_INFO_V2 AltPlatformInfo,
	_Inout_ PSP_INF_SIGNER_INFO InfSignerInfo);
#endif

#ifdef _UNICODE
#define UPDATEDRIVERFORPLUGANDPLAYDEVICES "UpdateDriverForPlugAndPlayDevicesW"
#define SETUPUNINSTALLOEMINF "SetupUninstallOEMInfW"
#else
#define UPDATEDRIVERFORPLUGANDPLAYDEVICES "UpdateDriverForPlugAndPlayDevicesA"
#define SETUPUNINSTALLOEMINF "SetupUninstallOEMInfA"
#endif
#define SETUPSETNONINTERACTIVEMODE "SetupSetNonInteractiveMode"
#define SETUPVERIFYINFFILE "SetupVerifyInfFile"

//
// exit codes
//
#define EXIT_OK      (0)
#define EXIT_REBOOT  (1)
#define EXIT_FAIL    (2)
#define EXIT_USAGE   (3)



#define IDS_ENABLED         3000
#define IDS_ENABLED_REBOOT  3001
#define IDS_ENABLE_FAILED   3002
#define IDS_DISABLED        3003
#define IDS_DISABLED_REBOOT 3004
#define IDS_DISABLE_FAILED  3005
#define IDS_RESTARTED       3006
#define IDS_REQUIRES_REBOOT 3007
#define IDS_RESTART_FAILED  3008
#define IDS_REMOVED         3009
#define IDS_REMOVED_REBOOT  3010
#define IDS_REMOVE_FAILED   3011

#define FIND_DEVICE         0x00000001 // display device
#define FIND_STATUS         0x00000002 // display status of device
#define FIND_RESOURCES      0x00000004 // display resources of device
#define FIND_DRIVERFILES    0x00000008 // display drivers used by device
#define FIND_HWIDS          0x00000010 // display hw/compat id's used by device
#define FIND_DRIVERNODES    0x00000020 // display driver nodes for a device.
#define FIND_CLASS          0x00000040 // display device's setup class
#define FIND_STACK          0x00000080 // display device's driver-stack