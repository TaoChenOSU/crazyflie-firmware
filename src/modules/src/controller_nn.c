/*
Neural Network Controller.

Tao Chen, 07/05/2018
*/

#include <math.h>

#include "log.h"
#include "math3d.h"

void stateControllerInit(void) {
	// read in the network config file
	
}

bool stateControllerTest(void) {}

void stateControllerReset(void) {}

void stateController(control_t *control, setpoint_t *setpoint, 
										 const sensorData_t *sensors, 
										 const state_t *state,
										 const uint32_t tick)
{
	struct vec setpointPos = mkvec(setpoint->position.x, setpoint->position.y, setpoint->position.z);
	struct vec setpointVel = mkvec(setpoint->velocity.x, setpoint->velocity.y, setpoint->velocity.z);
	struct vec statePos = mkvec(state->position.x, state->position.y, state->position.z);
	struct vec stateVel = mkvec(state->velocity.x, state->velocity.y, state->velocity.z);

	// Position Error (ep)
	r_error = vsub(setpointPos, statePos);

	// Velocity Error (ev)
	v_error = vsub(setpointVel, stateVel);

	// Orientation (rotatonal matrix)
	struct quat q = mkquat(state->attitudeQuaternion.x, state->attitudeQuaternion.y, state->attitudeQuaternion.z, state->attitudeQuaternion.w);
	struct mat33 R = quat2rotmat(q);

	// Angular velocity 
	float stateAttitudeRateRoll = radians(sensors->gyro.x);
	float stateAttitudeRatePitch = radians(sensors->gyro.y);
	float stateAttitudeRateYaw = radians(sensors->gyro.z);


}