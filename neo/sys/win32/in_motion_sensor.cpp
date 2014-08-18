#pragma hdrstop

#include "../../idlib/precompiled.h"
#undef strncmp

#include "../../../dependencies/libfreespace/include/freespace/freespace.h"
#include "../../../dependencies/vrpn/vrpn_Tracker.h"
#include "../../../dependencies/vrpn/quat/quat.h"

#include "in_motion_sensor.h"
#include "MahonyAHRS.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

bool hasHMD = false, hasOculusRift = false, hasHillcrest = false, hasVR920Tracker = false;

#ifdef WIN32
#include <windows.h>

// VR920 stuff
#include "vr920.h"
PVUZIX_DWORD IWROpenTracker = NULL;
PVUZIX_LONG3 IWRGetTracking = NULL;
PVUZIX_VOID IWRZeroSet = NULL;
PVUZIX_DWORD IWRBeginCalibrate = NULL;
PVUZIX_BOOL IWREndCalibrate = NULL;
PVUZIX_BOOL IWRSetFilterState = NULL;
PVUZIX_VOID IWRCloseTracker = NULL;

#include "OVR.h"
using namespace OVR;

#define RADIANS_TO_DEGREES(rad) ((float) rad * (float) (180.0 / idMath::PI))

void IN_MotionSensor_Thread(void);

FreespaceDeviceId device;
float angles[3];


    // *** Oculus HMD Variables
    
    idCVar in_sensorPrediction("in_motionSensorPrediction", "50", CVAR_SYSTEM | CVAR_ARCHIVE | CVAR_FLOAT, "motion sensor prediction (ms)");
	
	ovrHmd hmd;
    //ovrHmdDesc hmdDesc; // not used


vrpn_Tracker_Remote* vrpnTracker = new vrpn_Tracker_Remote("Tracker0@localhost");

void VRPN_CALLBACK handle_tracker(void* userData, const vrpn_TRACKERCB t )
{
	double euler[3];
	q_to_euler((double *)t.quat, euler);
}

HMODULE iwrdrv_dll = NULL;
static void LoadVR920(void) 
{
	hasVR920Tracker = false;
	if (!iwrdrv_dll) {
		iwrdrv_dll = LoadLibrary( "IWEARDRV.DLL" );
		if (iwrdrv_dll) {
			IWROpenTracker = (PVUZIX_DWORD)GetProcAddress(iwrdrv_dll, "IWROpenTracker");
			IWRGetTracking = (PVUZIX_LONG3)GetProcAddress(iwrdrv_dll, "IWRGetTracking");
			IWRZeroSet = (PVUZIX_VOID)GetProcAddress(iwrdrv_dll, "IWRZeroSet");
			IWRBeginCalibrate = (PVUZIX_DWORD)GetProcAddress(iwrdrv_dll, "IWRBeginCalibrate");
			IWREndCalibrate = (PVUZIX_BOOL)GetProcAddress(iwrdrv_dll, "IWREndCalibrate");
			IWRSetFilterState = (PVUZIX_BOOL)GetProcAddress(iwrdrv_dll, "IWRSetFilterState");
			IWRCloseTracker = (PVUZIX_VOID)GetProcAddress(iwrdrv_dll, "IWRCloseTracker");
		} else {
			IWROpenTracker = NULL;
			IWRGetTracking = NULL;
			IWRZeroSet = NULL;
			IWRBeginCalibrate = NULL;
			IWREndCalibrate = NULL;
			IWRSetFilterState = NULL;
			IWRCloseTracker = NULL;
			common->Warning("Vuzix VR920 tracker driver not installed.\n");
		}
	}
	if ((!hasVR920Tracker) && IWROpenTracker && IWROpenTracker()==ERROR_SUCCESS) {
		hasVR920Tracker = true;
		common->Printf("VR920 head tracker started.\n");
	}
}

void FreeVR920(void) 
{
	hasVR920Tracker = false;
	if (iwrdrv_dll) {
		if (IWRCloseTracker)
			IWRCloseTracker();
		FreeLibrary(iwrdrv_dll);
	}
	IWROpenTracker = NULL;
	IWRGetTracking = NULL;
	IWRZeroSet = NULL;
	IWRBeginCalibrate = NULL;
	IWREndCalibrate = NULL;
	IWRSetFilterState = NULL;
	IWRCloseTracker = NULL;
}

void IN_MotionSensor_Init(void)
{
	//Carl: Don't initialize has* here to false, because they can also be set by command line parameters

	// *** Oculus Sensor Initialization
	ovr_Initialize();

    // Create DeviceManager and first available HMDDevice from it.
    // Sensor object is created from the HMD, to ensure that it is on the
    // correct device.

    //pManager = *DeviceManager::Create();

	// We'll handle it's messages in this case.
	//pManager->SetMessageHandler(this);

    // Release Sensor/HMD in case this is a retry.
    //pSensor.Clear();
    //pHMD.Clear();
	hmd = ovrHmd_Create(0);
    if (hmd)
    {
        // Get more details about the HMD
		//ovrHmd_GetDesc(hmd, &hmdDesc); //not in 0.4.1

		if ((ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position | ovrHmdCap_LowPersistence, ovrTrackingCap_Orientation)
				&& hmd->HmdCaps & ovrHmdCap_ExtendDesktop)) {// make sure rift is running in extended desktop mode
			hasOculusRift = true;
			hasHMD = true;
			common->Printf("Oculus Rift detected!\n");
		}
		else {
			common->Warning("Oculus Rift detected but not used, it lacks some feature(s) required for use or is incorrectly setup!");
			hasOculusRift = false;
			hasHMD = false;
			ovrHmd_Destroy(hmd);
			ovr_Shutdown();
		}
    }
    else
    {            
        common->Warning("Oculus Rift not detected.\n");
		LoadVR920();
	}

	hasHMD = hasHMD || hasVR920Tracker;

	//Hillcrest libfreespace stuff
	LPDWORD dwThreadId=0;
	struct freespace_message message;
	int numIds;
	int rc;
	int i;

	// Initialize the freespace library
	rc = freespace_init();
	if (rc != FREESPACE_SUCCESS) {
		common->Warning("Hillcrest Initialization error. rc=%d\n", rc);
		return;
	}

	/** --- START EXAMPLE INITIALIZATION OF DEVICE -- **/
	rc = freespace_getDeviceList(&device, 1, &numIds);
	if (numIds == 0) {
		common->Warning("MotionSensor: Didn't find any devices.\n");
		return;
	}

	rc = freespace_openDevice(device);
	if (rc != FREESPACE_SUCCESS) {
		common->Warning("MotionSensor: Error opening device: %d\n", rc);
		return;
	}

	rc = freespace_flush(device);
	if (rc != FREESPACE_SUCCESS) {
		common->Warning("MotionSensor: Error flushing device: %d\n", rc);
		return;
	}

	memset(&message, 0, sizeof(message));
	message.messageType = FREESPACE_MESSAGE_DATAMODECONTROLV2REQUEST;
	message.dataModeControlV2Request.packetSelect = 2;
	message.dataModeControlV2Request.modeAndStatus |= 0 << 1;

	rc = freespace_sendMessage(device, &message);
	if (rc != FREESPACE_SUCCESS) {
		common->Warning("freespaceInputThread: Could not send message: %d.\n", rc);
	}
	CreateThread(NULL, //Choose default security
		0, //Default stack size
		(LPTHREAD_START_ROUTINE)&IN_MotionSensor_Thread,
		//Routine to execute
		(LPVOID) &i, //Thread parameter
		0, //Immediately run the thread
		dwThreadId //Thread Id
		);
	hasHillcrest = true;
	hasHMD = true;
}

void IN_MotionSensor_Thread()
{
	int rc;
	struct freespace_message message;
	static uint16_t lastseq = 0;

	while(1)
	{
		rc = freespace_readMessage(device, &message, 10 /* 10 ms timeout */);
		if (rc == FREESPACE_ERROR_TIMEOUT ||
			rc == FREESPACE_ERROR_INTERRUPTED) {
				continue;
		}

		if (message.messageType == FREESPACE_MESSAGE_BODYFRAME && 
			message.bodyFrame.sequenceNumber != lastseq) {

			lastseq = message.bodyFrame.sequenceNumber;

			MahonyAHRSupdateIMU(message.bodyFrame.angularVelX / 1000.0f, 
				message.bodyFrame.angularVelY / 1000.0f,
				message.bodyFrame.angularVelZ / 1000.0f,
				message.bodyFrame.linearAccelX ,
				message.bodyFrame.linearAccelY ,
				message.bodyFrame.linearAccelZ );

			// convert quaternion to euler angles
			float m11 = (2.0f * q0 * q0) + (2.0f * q1 * q1) - 1.0f;
			float m12 = (2.0f * q1 * q2) + (2.0f * q0 * q3);
			float m13 = (2.0f * q1 * q3) - (2.0f * q0 * q2);
			float m23 = (2.0f * q2 * q3) + (2.0f * q0 * q1);
			float m33 = (2.0f * q0 * q0) + (2.0f * q3 * q3) - 1.0f;

			float euler[3];
			euler[ROLL]  = atan2f(m23, m33);
			euler[PITCH] = asinf(-m13);
			euler[YAW]   = atan2f(m12, m11);

			// store angles in radians
			angles[ROLL] =    RADIANS_TO_DEGREES(euler[ROLL]);
			angles[PITCH] =   RADIANS_TO_DEGREES(euler[PITCH]);
			angles[YAW] =     RADIANS_TO_DEGREES(euler[YAW]);
		}
	}
}

void IN_MotionSensor_Read(float &roll, float &pitch, float &yaw)
{
		if (IN_MotionSensor_CanReadOrientation()) {
			// Query the HMD for the sensor state at a given time. "0.0" means "most recent time".
			ovrTrackingState ts = ovrHmd_GetTrackingState(hmd, 0.0);
			ovrPosef pose = ts.HeadPose.ThePose;
			Quatf orient = pose.Orientation;
			float y = 0.0f, p = 0.0f, r = 0.0f;
			orient.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&y, &p, &r);
			roll =   -RADIANS_TO_DEGREES(r); // ???
			pitch =  -RADIANS_TO_DEGREES(p); // should be degrees down
			yaw =     RADIANS_TO_DEGREES(y); // should be degrees left
		} else if (hasVR920Tracker && IWRGetTracking) {
			LONG y=0, p=0, r=0;
			if (IWRGetTracking(&y, &p, &r)==ERROR_SUCCESS) {
				yaw = y * 180.0f/32767.0f;
				pitch = p * -180.0f/32767.0f;
				roll = r * 180.0f/32767.0f;
			}
		} else {
			roll  = angles[ROLL];
			pitch = angles[PITCH];
			yaw   = angles[YAW];
		}
}

bool IN_MotionSensor_CanReadOrientation() {
	if (hasOculusRift && hmd && (hmd->TrackingCaps & ovrTrackingCap_Orientation)) {
		return true;
	}
	else {
		return false;
	}
}

void IN_MotionSensor_ReadPosition(float &x, float &y, float &z) {
	if (IN_MotionSensor_CanReadPosition()) {
		ovrTrackingState ts = ovrHmd_GetTrackingState(hmd, 0.0);
		ovrPosef pose = ts.HeadPose.ThePose;
		ovrVector3f pos = pose.Position;
		x = pos.x;
		y = pos.y;
		z = pos.z;
	}
}

bool IN_MotionSensor_CanReadPosition() {
	if (hasOculusRift && hmd && (hmd->TrackingCaps & ovrTrackingCap_Position)) {
		return true;
	}
	else {
		return false;
	}
}

void IN_MotionSensor_ResetTrackingOrigin() {
	if (hasOculusRift && hmd) {
		ovrHmd_RecenterPose(hmd);
	}
}

//Console command to reset the oculus tracking origin
void vr_reset_origin_f(const idCmdArgs &args) {
	IN_MotionSensor_ResetTrackingOrigin();
}
static idCommandLink vr_reset_origin("vr_reset_origin", vr_reset_origin_f, "Resets the Oculus tracking origin");

#else
void IN_MotionSensor_Init(void){}
void IN_MotionSensor_Move(void){}
#endif