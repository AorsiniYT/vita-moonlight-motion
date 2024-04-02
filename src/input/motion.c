
#include "Limelight.h"
#include "psp2/kernel/threadmgr/thread.h"
#include "psp2common/types.h"
#include "sys/_stdint.h"
#include <stdbool.h>
#include "../config.h"
#include "vita.h"



/* void vita_motion_init() {
    SceUID thid = sceKernelCreateThread("vitainput_motion_thread", vitainput_thread, 0, 0x40000, 0, 0, NULL);
    if (thid >= 0) {
        sceKernelStartThread(thid, 0, NULL);
        return true;
    }

    return false;
}

int vitainput_thread(SceSize args, void *argp) {
  while (1) {
    if (active_input_thread) {
      vitainput_process();
    }

    sceKernelDelayThread(2000); // 2 ms
  }

  return 0;
} */