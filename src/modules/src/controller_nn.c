/*
Neural Network Controller.

Tao Chen, 07/05/2018
*/

#include <math.h>

#include "log.h"
#include "math3d.h"
#include "linear_algebra.h"
#include "stabilizer_types.h"

// static m_matrix weights;
// static v_vector biases; 

void stateControllerInit(void) 
{
	// read in the weights and biases
}

bool stateControllerTest(void) { return true; }

void stateControllerReset(void) {}

void networkEvaluate(control_t *control, const struct m_matrix *state) 
{
	// assuming for now the network is a 1-hidden-layer-128-neuron structure
	// and tanh activation on hidden layers and linear activation on output layer 

	// struct m_matrix hidden_layer_1 = matrix_mul(state);

	return;
}

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
	struct vec r_error = vsub(setpointPos, statePos);

	// Velocity Error (ev)
	struct vec v_error = vsub(setpointVel, stateVel);

	// Orientation (rotatonal matrix)
	struct quat q = mkquat(state->attitudeQuaternion.x, state->attitudeQuaternion.y, state->attitudeQuaternion.z, state->attitudeQuaternion.w);
	struct mat33 R = quat2rotmat(q);

	// // Angular velocity 
	float stateAttitudeRateRoll = radians(sensors->gyro.x);
	float stateAttitudeRatePitch = radians(sensors->gyro.y);
	float stateAttitudeRateYaw = radians(sensors->gyro.z);

	// // the state vector
	float state_array[18] = {r_error.x, r_error.y, r_error.z, v_error.x, v_error.y, v_error.z, 
					   R.m[0][0], R.m[0][1], R.m[0][2],
					   R.m[1][0], R.m[1][1], R.m[1][2],
					   R.m[2][0], R.m[2][1], R.m[2][2], 
					   stateAttitudeRateRoll, stateAttitudeRatePitch, stateAttitudeRateYaw};

	struct m_matrix state_vec = make_vector(18, state_array);

	networkEvaluate(control, &state_vec);
}
