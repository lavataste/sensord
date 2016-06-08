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

#ifndef _SENSOR_EVENT_POLLER_H_
#define _SENSOR_EVENT_POLLER_H_

#include <map>
#include <poller.h>
#include <physical_sensor.h>

typedef std::multimap<int, physical_sensor *> fd_sensors_t;

class sensor_event_poller {
public:
	sensor_event_poller();
	virtual ~sensor_event_poller();

	bool poll(void);

private:
	poller m_poller;
	fd_sensors_t m_fd_sensors;

private:
	void init_sensor_map(void);
	void init_fd(void);
	void init_signal_fd(void);

	bool add_poll_fd(int fd);
	bool read_fd(int fd, std::vector<uint32_t> &ids);
	bool process_event(int fd, const std::vector<uint32_t> &ids);
};

#endif /* _SENSOR_EVENT_POLLER_H_ */