/*
 * sensord
 *
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __COMMAND_TYPES_H__
#define __COMMAND_TYPES_H__

#include <sensor_types.h>
#include "sensor_info.h"

#define SENSOR_CHANNEL_PATH		"/run/.sensord.socket"
#define MAX_BUF_SIZE 4096

/* TODO: OOP - create serializer interface */
enum cmd_type_e {
	CMD_DONE = -1,
	CMD_NONE = 0,

	/* Manager */
	CMD_MANAGER_SENSOR_LIST,

	/* Listener */
	CMD_LISTENER_EVENT,
	CMD_LISTENER_ACC_EVENT,
	CMD_LISTENER_CONNECT,
	CMD_LISTENER_DISCONNECT,
	CMD_LISTENER_START,
	CMD_LISTENER_STOP,
	CMD_LISTENER_ATTR_INT,
	CMD_LISTENER_ATTR_STR,
	CMD_LISTENER_GET_DATA,

	/* Provider(Dyanmic/External sensor) */
	CMD_PROVIDER_CMD_EVENT,
	CMD_PROVIDER_CONNECT,
	CMD_PROVIDER_DISCONNECT,
	CMD_PROVIDER_POST,

	CMD_HAS_PRIVILEGE,

	CMD_CNT,
};

typedef struct {
	int sensor_cnt;
	char data[0];
} cmd_manager_sensor_list_t;

typedef struct {
	int listener_id;
	char sensor[NAME_MAX];
} cmd_listener_connect_t;

typedef struct {
	int listener_id;
} cmd_listener_disconnect_t;

typedef struct {
	int listener_id;
} cmd_listener_start_t;

typedef struct {
	int listener_id;
} cmd_listener_stop_t;

typedef struct  {
	int listener_id;
	int attribute;
	int value;
} cmd_listener_attr_int_t;

typedef struct  {
	int listener_id;
	int attribute;
	int len;
	char value[0];
} cmd_listener_attr_str_t;

typedef struct {
	int listener_id;
	int len;
	char data[0];
} cmd_listener_get_data_t;

typedef struct {
	int provider_id;
	int sensor_id;
	char sensor[NAME_MAX];
	char s_info[0];
} cmd_provider_connect_t;

typedef struct {
	int provider_id;
	char sensor[NAME_MAX];
} cmd_provider_disconnect_t;

typedef struct {
	int provider_id;
	char sensor[NAME_MAX];
	sensorhub_data_t base_data;
} cmd_provider_post_t;

typedef struct {
	char sensor[NAME_MAX];
} cmd_has_privilege_t ;

#endif /* __COMMAND_TYPES_H__ */
