// touchabsolute.h
// Header para touchabsolute.c
#ifndef TOUCHABSOLUTE_H
#define TOUCHABSOLUTE_H

#include <stdbool.h>

void touchabsolute_enable(bool enable);
bool touchabsolute_is_enabled();
void touchabsolute_process(void (*send_mouse_event)(int x, int y, bool down));

#endif // TOUCHABSOLUTE_H
