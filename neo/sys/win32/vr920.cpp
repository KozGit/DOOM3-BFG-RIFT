#pragma hdrstop

#include "../../idlib/precompiled.h"
#undef strncmp

#include "vr920.h"

HANDLE vr920StereoHandle = INVALID_HANDLE_VALUE;

PVUZIX_HANDLE IWRSTEREO_Open = NULL;
PVUZIX_HANDLEBOOL IWRSTEREO_SetStereo = NULL;
PVUZIX_HANDLEBOOL IWRSTEREO_SetLR = NULL;
PVUZIX_CLOSEHANDLE IWRSTEREO_Close = NULL;
PVUZIX_BYTEHANDLE IWRSTEREO_WaitForAck = NULL;

HMODULE iwrstdrv_dll = NULL;

bool VR920_StartStereo3D() {
	if (vr920StereoHandle!=INVALID_HANDLE_VALUE)
		return true;
	if (!iwrstdrv_dll) {
		iwrstdrv_dll = LoadLibrary( "IWRSTDRV.DLL" );
		if (iwrstdrv_dll) {
			IWRSTEREO_Open = (PVUZIX_HANDLE)GetProcAddress(iwrstdrv_dll, "IWRSTEREO_Open");
			IWRSTEREO_SetStereo = (PVUZIX_HANDLEBOOL)GetProcAddress(iwrstdrv_dll, "IWRSTEREO_SetStereo");
			IWRSTEREO_SetLR = (PVUZIX_HANDLEBOOL)GetProcAddress(iwrstdrv_dll, "IWRSTEREO_SetLR");
			IWRSTEREO_WaitForAck = (PVUZIX_BYTEHANDLE)GetProcAddress(iwrstdrv_dll, "IWRSTEREO_WaitForAck");
			IWRSTEREO_Close = (PVUZIX_CLOSEHANDLE)GetProcAddress(iwrstdrv_dll, "IWRSTEREO_Close");
		} else {
			IWRSTEREO_Open = NULL;
			IWRSTEREO_SetStereo = NULL;
			IWRSTEREO_SetLR = NULL;
			IWRSTEREO_WaitForAck = NULL;
			IWRSTEREO_Close = NULL;
			common->Warning("Vuzix VR920 stereoscopic 3D driver not installed.\n");
			return false;
		}
	}
	if (IWRSTEREO_Open && IWRSTEREO_SetStereo && IWRSTEREO_SetLR) {
		vr920StereoHandle = IWRSTEREO_Open();
		if (vr920StereoHandle!=INVALID_HANDLE_VALUE) {
			common->Printf("VR920 stereoscopic 3D opened.\n");
			IWRSTEREO_SetStereo(vr920StereoHandle, true);
			common->Printf("VR920 stereoscopic 3D started.\n");
		} else {
			DWORD error = GetLastError();
			switch (error) {
				case ERROR_INVALID_FUNCTION: 
					common->Warning("Vuzix VR920 needs firmware update to start 3D mode!");
					break;
				case ERROR_DEV_NOT_EXIST:
					common->Warning("Vuzix VR920 is not plugged into USB!");
					break;
				default:
					common->Warning("Unknown error starting VR920 3D mode!");
			}
			return false;
		}
		return true;
	}
	return false;
}

bool VR920_StopStereo3D() {
	if (vr920StereoHandle==INVALID_HANDLE_VALUE)
		return true;
	if (IWRSTEREO_SetStereo) {
		IWRSTEREO_SetStereo(vr920StereoHandle, false);
		common->Printf("VR920 stereoscopic 3D stopped.\n");
		return true;
	} else return false;
}

void VR920_CleanupStereo3D() {
	common->Printf("Cleaning up VR920 stereoscopic 3D.\n");
	VR920_StopStereo3D();
	if (IWRSTEREO_Close)
		IWRSTEREO_Close(vr920StereoHandle);
	vr920StereoHandle = INVALID_HANDLE_VALUE;
	IWRSTEREO_Open = NULL;
	IWRSTEREO_SetStereo = NULL;
	IWRSTEREO_SetLR = NULL;
	IWRSTEREO_WaitForAck = NULL;
	IWRSTEREO_Close = NULL;
	if (iwrstdrv_dll)
		FreeLibrary(iwrstdrv_dll);
	iwrstdrv_dll = NULL;
}