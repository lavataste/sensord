/*
 * sensord
 *
 * Copyright (c) 2014-15 Samsung Electronics Co., Ltd.
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
#include <time.h>
#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <sensor_internal.h>
#include <stdbool.h>
#include <sensor_common.h>
#include <unistd.h>
#include <string.h>

#include "check-sensor.h"

static GMainLoop *mainloop;

int get_event_driven(sensor_type_t sensor_type, char str[])
{
	switch (sensor_type) {
	case ACCELEROMETER_SENSOR:
		if (strcmp(str, "RAW_DATA_REPORT_ON_TIME") == 0)
			return ACCELEROMETER_RAW_DATA_EVENT;
		break;
	case GYROSCOPE_SENSOR:
		if (strcmp(str, "RAW_DATA_REPORT_ON_TIME") == 0)
			return GYROSCOPE_RAW_DATA_EVENT;
		break;
	case PRESSURE_SENSOR:
		if (strcmp(str, "RAW_DATA_REPORT_ON_TIME") == 0)
			return PRESSURE_RAW_DATA_EVENT;
		break;
	case GEOMAGNETIC_SENSOR:
		if (strcmp(str, "RAW_DATA_REPORT_ON_TIME") == 0)
			return GEOMAGNETIC_RAW_DATA_EVENT;
		break;
	case LIGHT_SENSOR:
		if (strcmp(str, "RAW_DATA_REPORT_ON_TIME") == 0)
			return LIGHT_LUX_DATA_EVENT;
		break;
	case TEMPERATURE_SENSOR:
		if (strcmp(str, "RAW_DATA_REPORT_ON_TIME") == 0)
			return TEMPERATURE_RAW_DATA_EVENT;
		break;
	case PROXIMITY_SENSOR:
		if (strcmp(str, "EVENT_CHANGE_STATE") == 0)
			return PROXIMITY_CHANGE_STATE_EVENT;
		break;
	case ORIENTATION_SENSOR:
		if (strcmp(str, "RAW_DATA_REPORT_ON_TIME") == 0)
			return ORIENTATION_RAW_DATA_EVENT;
		break;
	case GRAVITY_SENSOR:
		if (strcmp(str, "RAW_DATA_REPORT_ON_TIME") == 0)
			return GRAVITY_RAW_DATA_EVENT;
		break;
	case LINEAR_ACCEL_SENSOR:
		if (strcmp(str, "RAW_DATA_REPORT_ON_TIME") == 0)
			return LINEAR_ACCEL_RAW_DATA_EVENT;
		break;
	case ROTATION_VECTOR_SENSOR:
		if (strcmp(str, "RAW_DATA_REPORT_ON_TIME") == 0)
			return ROTATION_VECTOR_RAW_DATA_EVENT;
		break;
	case GEOMAGNETIC_RV_SENSOR:
		if (strcmp(str, "RAW_DATA_REPORT_ON_TIME") == 0)
			return GEOMAGNETIC_RV_RAW_DATA_EVENT;
		break;
	case GAMING_RV_SENSOR:
		if (strcmp(str, "RAW_DATA_REPORT_ON_TIME") == 0)
			return GAMING_RV_RAW_DATA_EVENT;
		break;
	}

	return -1;
}

void callback(sensor_t sensor, unsigned int event_type, sensor_data_t *data, void *user_data)
{
	sensor_type_t sensor_type = event_type >> 16;

	switch (sensor_type) {
	case ACCELEROMETER_SENSOR:
		printf("Accelerometer [%lld] [%6.6f] [%6.6f] [%6.6f]\n", data->timestamp, data->values[0], data->values[1], data->values[2]);
		break;
	case GYROSCOPE_SENSOR:
		printf("Gyroscope [%lld] [%6.6f] [%6.6f] [%6.6f]\n", data->timestamp, data->values[0], data->values[1], data->values[2]);
		break;
	case PRESSURE_SENSOR:
		printf("Pressure [%lld] [%6.6f] [%6.6f] [%6.6f]\n", data->timestamp, data->values[0], data->values[1], data->values[2]);
		break;
	case GEOMAGNETIC_SENSOR:
		printf("Geomagnetic [%lld] [%6.6f] [%6.6f] [%6.6f]\n", data->timestamp, data->values[0], data->values[1], data->values[2]);
		break;
	case LIGHT_SENSOR:
		printf("Light [%lld] [%6.6f]\n", data->timestamp, data->values[0]);
		break;
	case TEMPERATURE_SENSOR :
		printf("Temperature [%lld] [%6.6f]\n", data->timestamp, data->values[0]);
		break;
	case PROXIMITY_SENSOR:
		printf("Proximity [%lld] [%6.6f]\n", data->timestamp, data->values[0]);
		break;
	case ORIENTATION_SENSOR :
		printf("Orientation [%lld] [%6.6f] [%6.6f] [%6.6f]\n", data->timestamp, data->values[0], data->values[1], data->values[2]);
		break;
	case GRAVITY_SENSOR:
		printf("Gravity [%lld] [%6.6f] [%6.6f] [%6.6f]\n", data->timestamp, data->values[0], data->values[1], data->values[2]);
		break;
	case LINEAR_ACCEL_SENSOR:
		printf("Linear acceleration [%lld] [%6.6f] [%6.6f] [%6.6f]\n", data->timestamp, data->values[0], data->values[1], data->values[2]);
		break;
	case ROTATION_VECTOR_SENSOR:
		printf("Rotation vector [%lld] [%6.6f] [%6.6f] [%6.6f] [%6.6f]\n", data->timestamp, data->values[0], data->values[1], data->values[2], data->values[3]);
		break;
	case GEOMAGNETIC_RV_SENSOR:
		printf("Geomagnetic RV [%lld] [%6.6f] [%6.6f] [%6.6f] [%6.6f]\n", data->timestamp, data->values[0], data->values[1], data->values[2], data->values[3]);
		break;
	case GAMING_RV_SENSOR:
		printf("Gaming RV [%lld] [%6.6f] [%6.6f] [%6.6f] [%6.6f]\n", data->timestamp, data->values[0], data->values[1], data->values[2], data->values[3]);
		break;
	default:
		return;
	}
}

int check_sensor(sensor_type_t sensor_type, unsigned int event, int interval)
{
	int handle, result, start_handle, stop_handle;

	mainloop = g_main_loop_new(NULL, FALSE);

	sensor_t sensor = sensord_get_sensor(sensor_type);
	handle = sensord_connect(sensor);

	result = sensord_register_event(handle, event, interval, 0, callback, NULL);

	if (result < 0) {
		printf("Can't register sensor\n");
		return -1;
	}

	start_handle = sensord_start(handle, 0);

	if (start_handle < 0) {
		printf("Error\n\n\n\n");
		sensord_unregister_event(handle, event);
		sensord_disconnect(handle);
		return -1;
	}

	g_main_loop_run(mainloop);
	g_main_loop_unref(mainloop);

	result = sensord_unregister_event(handle, event);

	if (result < 0) {
		printf("Error\n\n");
		return -1;
	}

	stop_handle = sensord_stop(handle);

	if (stop_handle < 0) {
		printf("Error\n\n");
		return -1;
	}

	sensord_disconnect(handle);

	return 0;
}