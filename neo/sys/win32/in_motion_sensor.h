#ifndef IN_MOTION_SENSOR
#define IN_MOTION_SENSOR

void IN_MotionSensor_Init(void);
void IN_MotionSensor_Read(float &roll, float &pitch, float &yaw);

#endif