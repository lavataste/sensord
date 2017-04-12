/*
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

#include <sensor_log.h>
#include <fusion_sensor.h>
#include <vector>

#ifdef ENABLE_AUTO_ROTATION
#include "auto_rotation/auto_rotation_sensor.h"
#endif
#ifdef ENABLE_GRAVITY
#include "gravity/gravity_lowpass_sensor.h"
#include "gravity/gravity_comp_sensor.h"
#endif
#ifdef ENABLE_LINEAR_ACCEL
#include "linear_accel/linear_accel_sensor.h"
#endif

static std::vector<fusion_sensor_t> sensors;

template<typename _sensor>
void create_sensor(const char *name)
{
	fusion_sensor *instance = NULL;
	try {
		instance = new _sensor;
	} catch (std::exception &e) {
		_E("Failed to create %s sensor, exception: %s", name, e.what());
		return;
	} catch (int err) {
		_ERRNO(err, _E, "Failed to create %s sensor device", name);
		return;
	}

	sensors.push_back(instance);
}

extern "C" int create(fusion_sensor_t **fsensors)
{
#ifdef ENABLE_AUTO_ROTATION
	create_sensor<auto_rotation_sensor>("Accelerometer");
#endif

#ifdef ENABLE_GRAVITY
	create_sensor<gravity_lowpass_sensor>("Gravity(Lowpass)");
	create_sensor<gravity_comp_sensor>("Gravity(Complementary)");
#endif

#ifdef ENABLE_LINEAR_ACCEL
	create_sensor<linear_accel_sensor>("Linear Acceleration Sensor");
#endif

	*fsensors = &sensors[0];
	return sensors.size();
}
