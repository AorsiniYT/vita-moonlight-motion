/*
 * This file is part of Moonlight Embedded.
 *
 * Copyright (C) 2015 Iwan Timmer
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

#include "connection.h"
#include "Limelight.h"
#include "config.h"
#include "power/vita.h"
#include "input/vita.h"
#include "video/vita.h"
#include "audio/vita.h"

#include <stdio.h>
#include <stdbool.h>

#include "debug.h"

static int connection_status = LI_DISCONNECTED;

char* connection_failed_stage_name = "";


void pause_output() {
  vitainput_stop();
  vitavideo_stop();
  vitaaudio_stop();
}

void stop_output() {
  vitainput_stop();
  vitapower_stop();
  vitavideo_stop();
  vitaaudio_stop();
}

void start_output() {
  vitainput_start();
  vitapower_start();
  vitavideo_start();
  vitaaudio_start();
}

void connection_connection_started() {
  if (connection_status != LI_PAIRED) {
    vita_debug_log("connection_connection_started error: %d\n", connection_status);
    return;
  }
  vita_debug_log("connection started\n");
  connection_status = LI_CONNECTED;
  start_output();
  vitavideo_hide_poor_net_indicator();
}

static void connection_connection_terminated(int error_code) {
  if (connection_status != LI_PAIRED && connection_status != LI_CONNECTED &&
      connection_status != LI_MINIMIZED) {
    vita_debug_log("connection_connection_terminated error: %d\n", connection_status);
  }

  switch (error_code) {
    case ML_ERROR_GRACEFUL_TERMINATION:
      printf("Connection has been terminated gracefully.\n");
      break;
    case ML_ERROR_NO_VIDEO_TRAFFIC:
      printf("No video received from host. Check the host PC's firewall and port forwarding rules.\n");
      break;
    case ML_ERROR_NO_VIDEO_FRAME:
      printf("Your network connection isn't performing well. Reduce your video bitrate setting or try a faster connection.\n");
      break;
    case ML_ERROR_UNEXPECTED_EARLY_TERMINATION:
      printf("The connection was unexpectedly terminated by the host due to a video capture error. Make sure no DRM-protected content is playing on the host.\n");
      break;
    case ML_ERROR_PROTECTED_CONTENT:
      printf("The connection was terminated by the host due to DRM-protected content. Close any DRM-protected content on the host and try again.\n");
      break;
    default:
      printf("Connection terminated with error: %d\n", error_code);
      break;
  }

  if (connection_status == LI_CONNECTED) {
    stop_output();
  }
  vita_debug_log("connection terminated\n");
  connection_status = LI_DISCONNECTED;
}

int connection_reset() {
  if (connection_status != LI_DISCONNECTED) {
    vita_debug_log("connection_reset error: %d\n", connection_status);
    return -1;
  }
  connection_status = LI_READY;
  return 0;
}

int connection_paired() {
  if (connection_status != LI_READY && connection_status != LI_PAIRED &&
      connection_status != LI_CONNECTED) {
    vita_debug_log("connection_paired error: %d\n", connection_status);
    return -1;
  }
  connection_status = LI_PAIRED;
  return 0;
}

int connection_minimize() {
  if (connection_status != LI_CONNECTED) {
    vita_debug_log("connection_minimize error: %d\n", connection_status);
    return -1;
  }
  pause_output();
  connection_status = LI_MINIMIZED;
  return 0;
}

int connection_resume() {
  if (connection_status != LI_MINIMIZED) {
    vita_debug_log("connection_resume error: %d\n", connection_status);
    return -1;
  }
  start_output();
  connection_status = LI_CONNECTED;
  return 0;
}

int connection_terminate() {
  if (connection_status != LI_PAIRED && connection_status != LI_CONNECTED &&
      connection_status != LI_MINIMIZED) {
    vita_debug_log("connection_terminate error: %d\n", connection_status);
    return -1;
  }
  LiStopConnection();
  //connection_connection_terminated(0);
  return 0;
}

void connection_stage_starting(int stage) {
  const char* connection_stage_name = LiGetStageName(stage);
  vita_debug_log("connection_stage_starting - stage: %s\n", connection_stage_name);
}
void connection_stage_complate(int stage) {
  const char* connection_stage_name = LiGetStageName(stage);
  vita_debug_log("connection_stage_complete - stage: %d\n", connection_stage_name);
}

void connection_stage_failed(int stage, int code) {
  connection_failed_stage_name = (char *) LiGetStageName(stage);
  vita_debug_log("connection_stage_failed - stage: %s, %d\n", connection_failed_stage_name, code);
}

bool connection_is_ready() {
  return connection_status != LI_DISCONNECTED;
}

bool connection_is_connected() {
  return connection_status == LI_CONNECTED;
}

int connection_get_status() {
  return connection_status;
}

void connection_status_update(int status) {
  switch (status) {
    case CONN_STATUS_POOR:
      vitavideo_show_poor_net_indicator();
      break;
    case CONN_STATUS_OKAY:
      vitavideo_hide_poor_net_indicator();
      break;
  }
}

CONNECTION_LISTENER_CALLBACKS connection_callbacks = {
  .stageStarting = connection_stage_starting,
  .stageComplete = connection_stage_complate,
  .stageFailed = connection_stage_failed,
  .connectionStarted = connection_connection_started,
  .connectionTerminated = connection_connection_terminated,
  .connectionStatusUpdate = connection_status_update,
  .logMessage = vita_debug_log,
};
