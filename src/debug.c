/*
 * This file is part of Moonlight Embedded.
 *
 * Copyright (C) 2017 Sunguk Lee
 *
 * Moonlight is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moonlight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moonlight; if not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <psp2/rtc.h>
#include <stdlib.h>

#include "debug.h"
#include "config.h"
#include "psp2/kernel/threadmgr/thread.h"

pthread_mutex_t print_mutex;

bool vita_debug_init() {
  if (pthread_mutex_init(&print_mutex, NULL) != 0) {
    return false;
  }
  return true;
}

void vita_debug_log(const char *s, ...) {
  if (!config.save_debug_log) {
    return;
  }

  char* buffer = malloc(8192);
  if (!buffer) {
    return;
  }

  pthread_mutex_lock(&print_mutex);

  SceDateTime time;
  sceRtcGetCurrentClock(&time, 0);

  snprintf(buffer, 26, "%04d%02d%02d %02d:%02d:%02d.%06d ",
           time.year, time.month, time.day,
           time.hour, time.minute, time.second,
           time.microsecond);

  va_list va;
  va_start(va, s);
  int len = vsnprintf(&buffer[25], 8000, s, va);
  va_end(va);

  fprintf(config.log_file, "%s", buffer);
  if (buffer[len + 24] != '\n') {
      fprintf(config.log_file, "\n");
  }
  fflush(config.log_file);

  free(buffer);

  pthread_mutex_unlock(&print_mutex);
}

