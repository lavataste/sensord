/*
 * sensord
 *
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
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

#ifndef __LINEAR_ACCEL_SENSOR_H__
#define __LINEAR_ACCEL_SENSOR_H__

#include <fusion_sensor.h>
#include <sensor_types.h>

class linear_accel_sensor : public fusion_sensor {
public:
	linear_accel_sensor();
	virtual ~linear_accel_sensor();

	int get_sensor_info(const sensor_info2_t **info);
	int get_required_sensors(const required_sensor_s **sensors);

	int update(uint32_t id, sensor_data_t *data, int len);
	int get_data(sensor_data_t **data, int *len);

private:
	float m_x;
	float m_y;
	float m_z;
	float m_gx;
	float m_gy;
	float m_gz;
	int m_accuracy;
	unsigned long long m_time;
};

#endif /* __LINEAR_ACCEL_SENSOR_H__ */