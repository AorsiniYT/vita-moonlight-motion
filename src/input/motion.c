#include <stdbool.h>
#include <time.h>
#include "../config.h"
#include "psp2/motion.h"
#include "Limelight.h"
#include "motion.h"
#include "../debug.h"
#include "vita.h"


/*
if (config.enable_motion_controls) {
    if (motion_enabled_from_host) {
      //Resetting motion causes downwards jerk temporary code to prevent
      if(sceRtcCompareTick(&current, &until) > 0 && !_calibrateGyro){
        
      } 

      move_motion(motionState);
    }
    


    //TODO: Don't calibrate based on a button press
    if((is_pressed(map.btn_tl))){
      if(_calibrateGyro){
        sceMotionReset();
        sceRtcTickAddMicroseconds(&until, &current, MOTION_ACTION_DELAY);
        _motionResetCount = 1;
      }
      _calibrateGyro = false;
    }else{
      _calibrateGyro = true;
      _motionCalibrated = false;
      _motionResetCount = 0;
    }
    memcpy(&deviceQuat_old, &motionState.deviceQuat, sizeof(struct SceFQuaternion));
  }

  inline void move_motion(SceMotionState motionState) {
  // Get the mouse position.
  double delta_x = (deviceQuat_old.y-motionState.deviceQuat.y) * (float)MOUSE_SENSITIVITY;
  double delta_y = (deviceQuat_old.x-motionState.deviceQuat.x) * (float)MOUSE_SENSITIVITY;

  if (delta_x == 0 && delta_y == 0) {
    return;
  }
  //TODO: Thread

  //int x = lround(delta_x * mouse_multiplier);
  //int y = lround(delta_y * mouse_multiplier);

  //LiSendMouseMoveEvent(x, y);

  //Controller number could start at 0, I hope not
  //Angular velocity should be in deg/s
  // -X ... +X : left ... right
  // -Y ... +Y : bottom ... top
  // -Z ... +Z : farther ... closer
  float vx = motionState.angularVelocity.x;
  float vy = motionState.angularVelocity.y;
  float vz = motionState.angularVelocity.z;
  vita_debug_log("vx: %f\nvy: %f\nvz: %f\n", vx, vy, vz);
  LiSendControllerMotionEvent(1, LI_MOTION_TYPE_GYRO, vx, vy, vz);
}

*/

motion_data_state motion_state = {
    .motion_type_gyro_enabled = 0,
    .motion_type_accel_enabled = 0,
    .report_rate_gyro = 1,
    .report_rate_accel = 1,
};

bool vita_motion_init() {

    sceMotionStartSampling();
    sceMotionSetDeadband(false);

    SceUID thid = sceKernelCreateThread("vitainput_motion_gyro_thread", vitainput_motion_gyro_thread, 0, 0x40000, 0, 0, NULL);
    if (thid >= 0) {
        sceKernelStartThread(thid, 0, NULL);
    } else {
      return false;
    }

    thid = sceKernelCreateThread("vitainput_motion_accel_thread", vitainput_motion_accel_thread, 0, 0x40000, 0, 0, NULL);
    if (thid >= 0) {
        sceKernelStartThread(thid, 0, NULL);
        return true;
    }

    return false;
}

int vitainput_motion_gyro_thread(SceSize args, void *argp) {
  while (1) {
    if (motion_state.motion_type_gyro_enabled == true) {
      motion_process_gyro();
    }
    //1000/report rate = delay * 1000 (us)
    sceKernelDelayThread((1000/motion_state.report_rate_gyro) * 1000);
  }


  return 0;
}

int vitainput_motion_accel_thread(SceSize args, void *argp) {
  while (1) {
    if (motion_state.motion_type_accel_enabled == true) {
      motion_process_accel();
    }

    sceKernelDelayThread((1000/motion_state.report_rate_accel) * 1000); // 2 ms
  }

  return 0;
}

void motion_process_gyro() {
  SceMotionState motionState;
  sceMotionGetState(&motionState);

  //Angular velocity should be in deg/s

  float vx = motionState.angularVelocity.x;
  float vy = motionState.angularVelocity.z * -1;
  float vz = motionState.angularVelocity.y;
  vita_debug_log("Gyro: vx: %f vy: %f vz: %f", vx, vy, vz);
  LiSendControllerMotionEvent(0, LI_MOTION_TYPE_GYRO, vx, vy, vz);
}

void motion_process_accel() {
  SceMotionState motionState;
  sceMotionGetState(&motionState);

  float vx = motionState.acceleration.x;
  float vy = motionState.acceleration.z * -1;
  float vz = motionState.acceleration.y;
  //vita_debug_log("Accel: vx: %f\nvy: %f\nvz: %f\n", vx, vy, vz);
  LiSendControllerMotionEvent(0, LI_MOTION_TYPE_ACCEL, vx, vy, vz);

}