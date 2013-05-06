// I wrote my own VR920 header files to get around copyright issues.

#ifndef VR920_H
#define VR920_H
// tracker stuff
typedef DWORD (__cdecl *PVUZIX_DWORD)(void);
typedef void  (__cdecl *PVUZIX_VOID)(void);
typedef DWORD (__cdecl *PVUZIX_LONG3)(LONG *, LONG *, LONG *);
typedef void  (__cdecl *PVUZIX_BOOL)(BOOL);

extern PVUZIX_DWORD IWROpenTracker;
extern PVUZIX_LONG3 IWRGetTracking;
extern PVUZIX_VOID IWRZeroSet;
extern PVUZIX_DWORD IWRBeginCalibrate;
extern PVUZIX_BOOL IWREndCalibrate;
extern PVUZIX_BOOL IWRSetFilterState;
extern PVUZIX_VOID IWRCloseTracker;

// VR920 freeze frame stereoscopic 3D stuff
typedef HANDLE (__cdecl *PVUZIX_HANDLE)(void);
typedef void (__cdecl *PVUZIX_CLOSEHANDLE)(HANDLE);
typedef BOOL (__cdecl *PVUZIX_HANDLEBOOL)(HANDLE, BOOL);
typedef BYTE (__cdecl *PVUZIX_BYTEHANDLE)(HANDLE, BOOL);

extern PVUZIX_HANDLE IWRSTEREO_Open;
extern PVUZIX_HANDLEBOOL IWRSTEREO_SetStereo;
extern PVUZIX_HANDLEBOOL IWRSTEREO_SetLR;
extern PVUZIX_CLOSEHANDLE IWRSTEREO_Close;
extern PVUZIX_BYTEHANDLE IWRSTEREO_WaitForAck;

extern HANDLE vr920StereoHandle;

bool VR920_StartStereo3D();
bool VR920_StopStereo3D();
void VR920_CleanupStereo3D();

#endif