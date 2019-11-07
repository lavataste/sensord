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

#include "sensor_listener_proxy.h"

#include <channel.h>
#include <message.h>
#include <command_types.h>
#include <sensor_log.h>
#include <sensor_types.h>

#include "sensor_handler.h"
#include "sensor_policy_monitor.h"

using namespace sensor;

sensor_listener_proxy::sensor_listener_proxy(uint32_t id,
			std::string uri, sensor_manager *manager, ipc::channel *ch)
: m_id(id)
, m_uri(uri)
, m_manager(manager)
, m_ch(ch)
, m_started(false)
, m_passive(false)
, m_pause_policy(SENSORD_PAUSE_ALL)
, m_axis_orientation(SENSORD_AXIS_DISPLAY_ORIENTED)
, m_last_accuracy(SENSOR_ACCURACY_UNDEFINED)
{
	sensor_policy_monitor::get_instance().add_listener(this);
}

sensor_listener_proxy::~sensor_listener_proxy()
{
	sensor_policy_monitor::get_instance().remove_listener(this);
	stop();
}

uint32_t sensor_listener_proxy::get_id(void)
{
	return m_id;
}

int sensor_listener_proxy::update(const char *uri, ipc::message *msg)
{
	retv_if(!m_ch || !m_ch->is_connected(), OP_CONTINUE);

	update_event(msg);
	update_accuracy(msg);

	return OP_CONTINUE;
}

void sensor_listener_proxy::update_event(ipc::message *msg)
{
	/* TODO: check axis orientation */
	msg->header()->type = CMD_LISTENER_EVENT;
	msg->header()->err = OP_SUCCESS;

	m_ch->send(msg);
}

void sensor_listener_proxy::update_accuracy(ipc::message *msg)
{
	sensor_data_t *data = reinterpret_cast<sensor_data_t *>(msg->body());

	if (data->accuracy == m_last_accuracy)
		return;

	m_last_accuracy = data->accuracy;

	sensor_data_t acc_data;
	acc_data.accuracy = m_last_accuracy;

	ipc::message *acc_msg = new(std::nothrow) ipc::message();
	retm_if(!acc_msg, "Failed to allocate memory");

	acc_msg->header()->type = CMD_LISTENER_ACC_EVENT;
	acc_msg->header()->err = OP_SUCCESS;
	acc_msg->enclose(&acc_data, sizeof(acc_data));

	m_ch->send(acc_msg);
}

int sensor_listener_proxy::start(bool policy)
{
	int ret;
	sensor_handler *sensor = m_manager->get_sensor(m_uri);
	retv_if(!sensor, -EINVAL);
	retvm_if(m_started && !policy, OP_SUCCESS, "Sensor is already started");

	_D("Listener[%d] try to start", get_id());

	ret = sensor->start(this);
	retv_if (ret < 0, OP_ERROR);

	/* m_started is changed only when it is explicitly called by user,
	 * not automatically determined by any pause policy. */
	if (policy)
		return OP_SUCCESS;

	m_started = true;
	return OP_SUCCESS;
}

int sensor_listener_proxy::stop(bool policy)
{
	sensor_handler *sensor = m_manager->get_sensor(m_uri);
	retv_if(!sensor, -EINVAL);
	retvm_if(!m_started && !policy, OP_SUCCESS, "Sensor is already stopped");

	_D("Listener[%d] try to stop", get_id());

	int ret = sensor->stop(this);
	retv_if(ret < 0, OP_ERROR);

	/* attributes and m_started are changed only when it is explicitly called by user,
	 * not automatically determined by any policy. */
	if (policy)
		return OP_SUCCESS;

	/* unset attributes */
	set_interval(POLL_MAX_HZ_MS);
	delete_batch_latency();

	m_started = false;
	return OP_SUCCESS;
}

int sensor_listener_proxy::set_interval(unsigned int interval)
{
	sensor_handler *sensor = m_manager->get_sensor(m_uri);
	retv_if(!sensor, -EINVAL);

	_D("Listener[%d] try to set interval[%d]", get_id(), interval);

	return sensor->set_interval(this, interval);
}

int sensor_listener_proxy::set_max_batch_latency(unsigned int max_batch_latency)
{
	sensor_handler *sensor = m_manager->get_sensor(m_uri);
	retv_if(!sensor, -EINVAL);

	_D("Listener[%d] try to set max batch latency[%d]", get_id(), max_batch_latency);

	return sensor->set_batch_latency(this, max_batch_latency);
}

int sensor_listener_proxy::delete_batch_latency(void)
{
	sensor_handler *sensor = m_manager->get_sensor(m_uri);
	retv_if(!sensor, -EINVAL);

	_I("Listener[%d] try to delete batch latency", get_id());

	return sensor->delete_batch_latency(this);
}

int sensor_listener_proxy::set_passive_mode(bool passive)
{
	/* TODO: passive mode */
	m_passive = passive;
	return OP_SUCCESS;
}

int sensor_listener_proxy::set_attribute(int attribute, int value)
{
	sensor_handler *sensor = m_manager->get_sensor(m_uri);
	retv_if(!sensor, -EINVAL);

	_D("Listener[%d] try to set attribute[%d, %d]", get_id(), attribute, value);

	if (attribute == SENSORD_ATTRIBUTE_PAUSE_POLICY) {
		m_pause_policy = value;
		return OP_SUCCESS;
	} else if (attribute == SENSORD_ATTRIBUTE_AXIS_ORIENTATION) {
		m_axis_orientation = value;
		return OP_SUCCESS;
	} else if (attribute == SENSORD_ATTRIBUTE_FLUSH) {
		return flush();
	}

	return sensor->set_attribute(this, attribute, value);
}

int sensor_listener_proxy::set_attribute(int attribute, const char *value, int len)
{
	sensor_handler *sensor = m_manager->get_sensor(m_uri);
	retv_if(!sensor, -EINVAL);

	_D("Listener[%d] try to set string attribute[%d], len[%d]", get_id(), attribute, len);

	return sensor->set_attribute(this, attribute, value, len);
}

int sensor_listener_proxy::flush(void)
{
	sensor_handler *sensor = m_manager->get_sensor(m_uri);
	retv_if(!sensor, -EINVAL);

	return sensor->flush(this);
}

int sensor_listener_proxy::get_data(sensor_data_t **data, int *len)
{
	sensor_handler *sensor = m_manager->get_sensor(m_uri);
	retv_if(!sensor, -EINVAL);

	return sensor->get_cache(data, len);
}

std::string sensor_listener_proxy::get_required_privileges(void)
{
	sensor_handler *sensor = m_manager->get_sensor(m_uri);
	retv_if(!sensor, "");

	sensor_info info = sensor->get_sensor_info();
	return info.get_privilege();
}

void sensor_listener_proxy::on_policy_changed(int policy, int value)
{
	ret_if(m_started == false);
	ret_if(policy != SENSORD_ATTRIBUTE_PAUSE_POLICY);
	ret_if(m_pause_policy == SENSORD_PAUSE_NONE);

	_D("power_save_state[%d], listener[%d] pause policy[%d]",
			value, get_id(), m_pause_policy);

	if (value & m_pause_policy)
		stop(true);
	if (!(value & m_pause_policy))
		start(true);
}
