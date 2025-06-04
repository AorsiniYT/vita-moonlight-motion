// touchabsolute.c
// Lógica de mouse absoluto (touch-to-absolute-mouse) para Moonlight Vita Motion
// Inspirado en vita-moonlight/src/input/vita.c

#include "touchabsolute.h"
#include "../config.h"
#include "../debug.h"
#include <psp2/touch.h>
#include <psp2/ctrl.h>
#include <stdbool.h>
#include <string.h>

static bool absolute_mouse_enabled = false;
static int last_abs_x = -1, last_abs_y = -1;

void touchabsolute_enable(bool enable) {
    absolute_mouse_enabled = enable;
    if (!enable) {
        last_abs_x = -1;
        last_abs_y = -1;
    }
}

bool touchabsolute_is_enabled() {
    return absolute_mouse_enabled;
}

// Convierte coordenadas de touchpad a coordenadas absolutas de mouse
static void touch_to_absolute(int tx, int ty, int *mx, int *my) {
    // El touchpad frontal de Vita es 1920x1088, la pantalla es 960x544
    *mx = tx * 960 / 1920;
    *my = ty * 544 / 1088;
}

// Procesa el touchpad y envía eventos de mouse absoluto
void touchabsolute_process(void (*send_mouse_event)(int x, int y, bool down)) {
    if (!absolute_mouse_enabled) return;
    SceTouchData touch;
    memset(&touch, 0, sizeof(touch));
    sceTouchPeek(SCE_TOUCH_PORT_FRONT, &touch, 1);
    if (touch.reportNum > 0) {
        int mx, my;
        touch_to_absolute(touch.report[0].x, touch.report[0].y, &mx, &my);
        if (mx != last_abs_x || my != last_abs_y) {
            send_mouse_event(mx, my, true);
            last_abs_x = mx;
            last_abs_y = my;
        }
    } else {
        if (last_abs_x != -1 && last_abs_y != -1) {
            send_mouse_event(last_abs_x, last_abs_y, false); // Levanta el click
            last_abs_x = -1;
            last_abs_y = -1;
        }
    }
}
