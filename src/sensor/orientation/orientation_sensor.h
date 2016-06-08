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

#ifndef _ORIENTATION_SENSOR_H_
#define _ORIENTATION_SENSOR_H_

#include <virtual_sensor.h>
#include <sensor_types.h>
#include <sensor_fusion.h>

class orientation_sensor : public virtual_sensor {
public:
	orientation_sensor();
	virtual ~orientation_sensor();

	/* initialize sensor */
	bool init(void);

	/* sensor info */
	virtual sensor_type_t get_type(void);
	virtual unsigned int get_event_type(void);
	virtual const char* get_name(void);

	virtual bool get_sensor_info(sensor_info &info);

	/* synthesize event */
	virtual void synthesize(const sensor_event_t& event);

	bool add_interval(int client_id, unsigned int interval, bool is_processor);
	bool delete_interval(int client_id, bool is_processor);

	/* get data */
	virtual int get_data(sensor_data_t **data, int *length);
private:
	sensor_base *m_rotation_vector_sensor;

	float m_azimuth;
	float m_pitch;
	float m_roll;
	int m_accuracy;
	unsigned long long m_time;
	unsigned long m_interval;

	virtual bool set_interval(unsigned long interval);
	virtual bool set_batch_latency(unsigned long latency);

	virtual bool on_start(void);
	virtual bool on_stop(void);

	int rotation_to_orientation(const float *rotation, float &azimuth, float &pitch, float &roll);
};

#endif /* _ORIENTATION_SENSOR_H_ */