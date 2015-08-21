/*
 * sensord
 *
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <common.h>
#include <sf_common.h>
#include <geomagnetic_rv_sensor.h>
#include <sensor_plugin_loader.h>
#include <orientation_filter.h>
#include <cvirtual_sensor_config.h>

#define SENSOR_NAME "GEOMAGNETIC_RV_SENSOR"
#define SENSOR_TYPE_GEOMAGNETIC_RV		"GEOMAGNETIC_ROTATION_VECTOR"

#define MIN_DELIVERY_DIFF_FACTOR 0.75f

#define MS_TO_US 1000

#define ELEMENT_VENDOR											"VENDOR"
#define ELEMENT_DEFAULT_SAMPLING_TIME							"DEFAULT_SAMPLING_TIME"

geomagnetic_rv_sensor::geomagnetic_rv_sensor()
: m_accel_sensor(NULL)
, m_magnetic_sensor(NULL)
, m_fusion_sensor(NULL)
, m_time(0)
{
	cvirtual_sensor_config &config = cvirtual_sensor_config::get_instance();

	sensor_hal *fusion_sensor_hal = sensor_plugin_loader::get_instance().get_sensor_hal(FUSION_SENSOR);
	if (!fusion_sensor_hal)
		m_hardware_fusion = false;
	else
		m_hardware_fusion = true;

	m_name = string(SENSOR_NAME);
	register_supported_event(GEOMAGNETIC_RV_RAW_DATA_EVENT);

	if (!config.get(SENSOR_TYPE_GEOMAGNETIC_RV, ELEMENT_VENDOR, m_vendor)) {
		ERR("[VENDOR] is empty\n");
		throw ENXIO;
	}

	INFO("m_vendor = %s", m_vendor.c_str());

	if (!config.get(SENSOR_TYPE_GEOMAGNETIC_RV, ELEMENT_DEFAULT_SAMPLING_TIME, &m_default_sampling_time)) {
		ERR("[DEFAULT_SAMPLING_TIME] is empty\n");
		throw ENXIO;
	}

	INFO("m_default_sampling_time = %d", m_default_sampling_time);

	m_interval = m_default_sampling_time * MS_TO_US;
}

geomagnetic_rv_sensor::~geomagnetic_rv_sensor()
{
	INFO("geomagnetic_rv_sensor is destroyed!\n");
}

bool geomagnetic_rv_sensor::init()
{
	m_accel_sensor = sensor_plugin_loader::get_instance().get_sensor(ACCELEROMETER_SENSOR);
	m_magnetic_sensor = sensor_plugin_loader::get_instance().get_sensor(GEOMAGNETIC_SENSOR);

	m_fusion_sensor = sensor_plugin_loader::get_instance().get_sensor(FUSION_SENSOR);

	if (!m_accel_sensor || !m_magnetic_sensor || !m_fusion_sensor) {
		ERR("Failed to load sensors,  accel: 0x%x, mag: 0x%x, fusion: 0x%x",
			m_accel_sensor, m_magnetic_sensor, m_fusion_sensor);
		return false;
	}

	INFO("%s is created!\n", sensor_base::get_name());

	return true;
}

sensor_type_t geomagnetic_rv_sensor::get_type(void)
{
	return GEOMAGNETIC_RV_SENSOR;
}

bool geomagnetic_rv_sensor::on_start(void)
{
	AUTOLOCK(m_mutex);

	if (!m_hardware_fusion) {
		m_accel_sensor->add_client(ACCELEROMETER_RAW_DATA_EVENT);
		m_accel_sensor->add_interval((intptr_t)this, (m_interval/MS_TO_US), false);
		m_accel_sensor->start();
		m_magnetic_sensor->add_client(GEOMAGNETIC_RAW_DATA_EVENT);
		m_magnetic_sensor->add_interval((intptr_t)this, (m_interval/MS_TO_US), false);
		m_magnetic_sensor->start();
	}

	m_fusion_sensor->register_supported_event(FUSION_EVENT);
	m_fusion_sensor->register_supported_event(FUSION_GEOMAGNETIC_ROTATION_VECTOR_ENABLED);
	m_fusion_sensor->add_client(FUSION_EVENT);
	m_fusion_sensor->add_interval((intptr_t)this, (m_interval/MS_TO_US), false);
	m_fusion_sensor->start();

	activate();
	return true;
}

bool geomagnetic_rv_sensor::on_stop(void)
{
	AUTOLOCK(m_mutex);

	if (!m_hardware_fusion) {
		m_accel_sensor->delete_client(ACCELEROMETER_RAW_DATA_EVENT);
		m_accel_sensor->delete_interval((intptr_t)this, false);
		m_accel_sensor->stop();
		m_magnetic_sensor->delete_client(GEOMAGNETIC_RAW_DATA_EVENT);
		m_magnetic_sensor->delete_interval((intptr_t)this, false);
		m_magnetic_sensor->stop();
	}

	m_fusion_sensor->delete_client(FUSION_EVENT);
	m_fusion_sensor->delete_interval((intptr_t)this, false);
	m_fusion_sensor->unregister_supported_event(FUSION_EVENT);
	m_fusion_sensor->unregister_supported_event(FUSION_GEOMAGNETIC_ROTATION_VECTOR_ENABLED);
	m_fusion_sensor->stop();

	deactivate();
	return true;
}

bool geomagnetic_rv_sensor::add_interval(int client_id, unsigned int interval)
{
	AUTOLOCK(m_mutex);

	if (!m_hardware_fusion) {
		m_accel_sensor->add_interval(client_id, interval, false);
		m_magnetic_sensor->add_interval(client_id, interval, false);
	}

	m_fusion_sensor->add_interval(client_id, interval, false);

	return sensor_base::add_interval(client_id, interval, false);
}

bool geomagnetic_rv_sensor::delete_interval(int client_id)
{
	AUTOLOCK(m_mutex);

	if (!m_hardware_fusion) {
		m_accel_sensor->delete_interval(client_id, false);
		m_magnetic_sensor->delete_interval(client_id, false);
	}

	m_fusion_sensor->delete_interval(client_id, false);

	return sensor_base::delete_interval(client_id, false);
}

void geomagnetic_rv_sensor::synthesize(const sensor_event_t &event, vector<sensor_event_t> &outs)
{
	unsigned long long diff_time;

	sensor_event_t rv_event;

	if (event.event_type == FUSION_EVENT) {
		diff_time = event.data.timestamp - m_time;

		if (m_time && (diff_time < m_interval * MIN_DELIVERY_DIFF_FACTOR))
			return;

		m_time = get_timestamp();
		rv_event.sensor_id = get_id();
		rv_event.event_type = GEOMAGNETIC_RV_RAW_DATA_EVENT;
		rv_event.data.accuracy = SENSOR_ACCURACY_GOOD;
		rv_event.data.timestamp = m_time;
		rv_event.data.value_count = 4;
		rv_event.data.values[0] = event.data.values[1];
		rv_event.data.values[1] = event.data.values[2];
		rv_event.data.values[2] = event.data.values[3];
		rv_event.data.values[3] = event.data.values[0];

		push(rv_event);
	}

	return;
}

int geomagnetic_rv_sensor::get_sensor_data(unsigned int event_type, sensor_data_t &data)
{
	sensor_data_t fusion_data;

	if (event_type != GEOMAGNETIC_RV_RAW_DATA_EVENT)
		return -1;

	m_fusion_sensor->get_sensor_data(FUSION_GEOMAGNETIC_ROTATION_VECTOR_ENABLED, fusion_data);

	data.accuracy = SENSOR_ACCURACY_GOOD;
	data.timestamp = get_timestamp();
	data.value_count = 4;
	data.values[0] = fusion_data.values[1];
	data.values[1] = fusion_data.values[2];
	data.values[2] = fusion_data.values[3];
	data.values[3] = fusion_data.values[0];

	return 0;
}

bool geomagnetic_rv_sensor::get_properties(sensor_properties_s &properties)
{
	properties.vendor = m_vendor;
	properties.name = SENSOR_NAME;
	properties.min_range = -1;
	properties.max_range = 1;
	properties.resolution = 0.000001;
	properties.fifo_count = 0;
	properties.max_batch_count = 0;
	properties.min_interval = 1;

	return true;
}

extern "C" sensor_module* create(void)
{
	geomagnetic_rv_sensor *sensor;

	try {
		sensor = new(std::nothrow) geomagnetic_rv_sensor;
	} catch (int err) {
		ERR("Failed to create module, err: %d, cause: %s", err, strerror(err));
		return NULL;
	}

	sensor_module *module = new(std::nothrow) sensor_module;
	retvm_if(!module || !sensor, NULL, "Failed to allocate memory");

	module->sensors.push_back(sensor);
	return module;
}
