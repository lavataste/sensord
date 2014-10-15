/*
 * sensord
 *
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
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
#include <sensor.h>
#include <stdbool.h>

static GMainLoop *mainloop;

void callback(unsigned int event_type, sensor_event_data_t *event, void *user_data)
{
	sensor_data_t *data = (sensor_data_t *)event->event_data;
	printf("Accelerometer [%lld] [%6.6f] [%6.6f] [%6.6f]\n\n", data->timestamp, data->values[0], data->values[1], data->values[2]);
}

void printformat()
{
	printf("Usage : ./accelerometer <event> <interval>(optional)\n\n");
	printf("event:\n");
	printf("ROTATION_CHECK\n");
	printf("RAW_DATA_REPORT_ON_TIME\n");
	printf("CALIBRATION_NEEDED\n");
	printf("SET_HORIZON\n");
	printf("SET_WAKEUP\n");
	printf("ORIENTATION_DATA_REPORT_ON_TIME\n");
	printf("LINEAR_ACCELERATION_DATA_REPORT_ON_TIME\n");
	printf("GRAVITY_DATA_REPORT_ON_TIME\n\n");
	printf("interval:\n");
	printf("The time interval should be entered based on the sampling frequency supported by accelerometer driver on the device in ms.If no value for sensor is entered default value by the driver will be used.\n");
}

int main(int argc,char **argv)
{
	int result, handle, start_handle, stop_handle;
	unsigned int event;

	mainloop = g_main_loop_new(NULL, FALSE);
	sensor_type_t type = ACCELEROMETER_SENSOR;
	event_condition_t *event_condition = (event_condition_t*) malloc(sizeof(event_condition_t));
	event_condition->cond_op = CONDITION_EQUAL;
	event_condition->cond_value1 = 100;

	if (argc != 2 && argc != 3) {
		printformat();
		free(event_condition);
		return 0;
	}

	if (strcmp(argv[1], "ROTATION_CHECK") == 0) {
		event = ACCELEROMETER_EVENT_ROTATION_CHECK;
	}
	else if (strcmp(argv[1], "RAW_DATA_REPORT_ON_TIME") == 0) {
		event = ACCELEROMETER_EVENT_RAW_DATA_REPORT_ON_TIME;
	}
	else if (strcmp(argv[1], "CALIBRATION_NEEDED") == 0) {
		event = ACCELEROMETER_EVENT_CALIBRATION_NEEDED;
	}
	else if (strcmp(argv[1], "SET_HORIZON") == 0) {
		event = ACCELEROMETER_EVENT_SET_HORIZON;
	}
	else if (strcmp(argv[1], "SET_WAKEUP") == 0) {
		event = ACCELEROMETER_EVENT_SET_WAKEUP;
	}
	else if (strcmp(argv[1], "ORIENTATION_DATA_REPORT_ON_TIME") == 0) {
		event = ACCELEROMETER_EVENT_ORIENTATION_DATA_REPORT_ON_TIME;
	}
	else if (strcmp(argv[1], "LINEAR_ACCELERATION_DATA_REPORT_ON_TIME") == 0) {
		event = ACCELEROMETER_EVENT_LINEAR_ACCELERATION_DATA_REPORT_ON_TIME;
	}
	else if (strcmp(argv[1], "GRAVITY_DATA_REPORT_ON_TIME") == 0) {
		event = ACCELEROMETER_EVENT_GRAVITY_DATA_REPORT_ON_TIME;
	}
	else {
		printformat();
		free(event_condition);
		return 0;
	}
	if (argc == 3)
		event_condition->cond_value1 = atof(argv[2]);
	
	handle = sf_connect(type);
	result = sf_register_event(handle, event, event_condition, callback, NULL);

	if (result < 0)
		printf("Can't register accelerometer\n");

	start_handle = sf_start(handle,0);

	if (start_handle < 0) {
		printf("Error\n\n\n\n");
		sf_unregister_event(handle, event);
		sf_disconnect(handle);
		return -1;
	}

	g_main_loop_run(mainloop);
	g_main_loop_unref(mainloop);

	sf_unregister_event(handle, event);

	stop_handle = sf_stop(handle);

	if (stop_handle < 0) {
		printf("Error\n\n");
		return -1;
	}

	sf_disconnect(handle);

	free(event_condition);

	return 0;
}
