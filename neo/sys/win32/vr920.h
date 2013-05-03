// I wrote my own VR920 header files to get around copyright issues.

#ifndef VR920_H
#define VR920_H

typedef DWORD (__cdecl *PVUZIX_DWORD)(void);
typedef void  (__cdecl *PVUZIX_VOID)(void);
typedef DWORD (__cdecl *PVUZIX_LONG3)(LONG *, LONG *, LONG *);
typedef void  (__cdecl *PVUZIX_BOOL)(BOOL);

typedef HANDLE (__cdecl *PVUZIX_HANDLE)(void);
typedef void (__cdecl *PVUZIX_CLOSEHANDLE)(HANDLE);
typedef BOOL (__cdecl *PVUZIX_HANDLEBOOL)(HANDLE, BOOL);

extern PVUZIX_DWORD IWROpenTracker;
extern PVUZIX_LONG3 IWRGetTracking;
extern PVUZIX_VOID IWRZeroSet;
extern PVUZIX_DWORD IWRBeginCalibrate;
extern PVUZIX_BOOL IWREndCalibrate;
extern PVUZIX_BOOL IWRSetFilterState;
extern PVUZIX_VOID IWRCloseTracker;

#endif