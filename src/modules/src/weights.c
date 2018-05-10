// define the memory for learned weights

#include <errno.h>
/* FreeRtos includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "crtp.h"
#include "weights.h"

// Global variables
float weights_memory[WEIGHT_BUFFER_OUT_LEN][WEIGHT_BUFFER_IN_LEN];
uint8_t weights_received_memory[WEIGHTS_MEMORY_SIZE];

static bool isInit = false;

// makes sure the current weights don't get accessed while it's getting renewed
static xSemaphoreHandle lockWeights;

// weights task (there's one currently)
enum WeightsCommand_e {
  COMMAND_RENEW_WEIGHTS   = 0,
};

// Private function
static void crtpWeightsTask(void * prm);

static int renew_weights(uint8_t dim_out, uint8_t dim_in);

void crtpWeightsInit(void)
{
  if (isInit) {
    return;
  }

  // start the weights task
  xTaskCreate(crtpWeightsTask, WEIGHTS_TASK_NAME,
              WEIGHTS_TASK_STACKSIZE, NULL, WEIGHTS_TASK_PRI, NULL);

  lockWeights = xSemaphoreCreateMutex();

  isInit = true;
}

void crtpWeightsTask(void * prm)
{
  int ret;
  CRTPPacket p;
  crtpInitTaskQueue(CRTP_PORT_WEIGHTS);

  while(1) {
    crtpReceivePacketBlock(CRTP_PORT_WEIGHTS, &p);

    switch(p.data[0])
    {
      case COMMAND_RENEW_WEIGHTS:
        ret = renew_weights(p.data[1], p.data[2]);
        break;
      default:
        ret = ENOEXEC;
        break;
    }

    // answer
    p.data[3] = ret;
    p.size = 4;
    crtpSendPacket(&p);
  }
}

int renew_weights(uint8_t dim_out, uint8_t dim_in)
{
  if (weights_received_memory[0] != dim_out
    || weights_received_memory[1] != dim_in) {
      return ENOEXEC;
  }
  float *weights_pure_data = (float*)(&weights_received_memory[2]);
  for(uint8_t i = 0; i < dim_out; i++){
    for(int8_t j = 0; j < dim_in; j++){
      weights_memory[i][j] = weights_pure_data[i*dim_in+j];
    }
  }
  return 0;
}
