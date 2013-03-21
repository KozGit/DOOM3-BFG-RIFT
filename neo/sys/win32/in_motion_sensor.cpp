#pragma hdrstop

#include "../../idlib/precompiled.h"

#include "../../../dependencies/libfreespace/include/freespace/freespace.h"
#include "../../../dependencies/vrpn/vrpn_Tracker.h"
#include "../../../dependencies/vrpn/quat/quat.h"
#include "in_motion_sensor.h"
#include "MahonyAHRS.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef WIN32
#include <windows.h>

#define RADIANS_TO_DEGREES(rad) ((float) rad * (float) (180.0 / idMath::PI))

void IN_MotionSensor_Thread(void);

FreespaceDeviceId device;
float angles[3];

vrpn_Tracker_Remote* vrpnTracker = new vrpn_Tracker_Remote("Tracker0@localhost");

void VRPN_CALLBACK handle_tracker(void* userData, const vrpn_TRACKERCB t )
{
	double euler[3];
	q_to_euler((double *)t.quat, euler);
}

void IN_MotionSensor_Init(void)
{

	LPDWORD dwThreadId=0;
	struct freespace_message message;
	int numIds;
	int rc;
	int i;

	// Initialize the freespace library
	rc = freespace_init();
	if (rc != FREESPACE_SUCCESS) {
		//printf("Initialization error. rc=%d\n", rc);
		return;
	}

	/** --- START EXAMPLE INITIALIZATION OF DEVICE -- **/
	rc = freespace_getDeviceList(&device, 1, &numIds);
	if (numIds == 0) {
		//printf("MotionSensor: Didn't find any devices.\n");
		return;
	}

	rc = freespace_openDevice(device);
	if (rc != FREESPACE_SUCCESS) {
		//printf("MotionSensor: Error opening device: %d\n", rc);
		return;
	}

	rc = freespace_flush(device);
	if (rc != FREESPACE_SUCCESS) {
		//printf("MotionSensor: Error flushing device: %d\n", rc);
		return;
	}

	memset(&message, 0, sizeof(message));
	message.messageType = FREESPACE_MESSAGE_DATAMODECONTROLV2REQUEST;
	message.dataModeControlV2Request.packetSelect = 2;
	message.dataModeControlV2Request.modeAndStatus |= 0 << 1;

	rc = freespace_sendMessage(device, &message);
	if (rc != FREESPACE_SUCCESS) {
		//printf("freespaceInputThread: Could not send message: %d.\n", rc);
	}

	CreateThread(NULL, //Choose default security
		0, //Default stack size
		(LPTHREAD_START_ROUTINE)&IN_MotionSensor_Thread,
		//Routine to execute
		(LPVOID) &i, //Thread parameter
		0, //Immediately run the thread
		dwThreadId //Thread Id
		);
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
	roll  = angles[ROLL];
	pitch = angles[PITCH];
	yaw   = angles[YAW];
}

#else
void IN_MotionSensor_Init(void){}
void IN_MotionSensor_Move(void){}
#endif