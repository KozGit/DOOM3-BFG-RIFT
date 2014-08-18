#ifndef IN_MOTION_SENSOR
#define IN_MOTION_SENSOR

void IN_MotionSensor_Init(void);
void IN_MotionSensor_Read(float &roll, float &pitch, float &yaw);
void IN_MotionSensor_ReadPosition(float &x, float &y, float &z);
bool IN_MotionSensor_CanReadPosition();
bool IN_MotionSensor_CanReadOrientation();
void IN_MotionSensor_ResetTrackingOrigin();

#endif