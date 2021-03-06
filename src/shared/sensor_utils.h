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

#ifndef __SENSOR_UTILS_H__
#define __SENSOR_UTILS_H__

#include <time.h>
#include <sensor_types.h>
#include <string>
#include <vector>

namespace sensor {

namespace utils {
	const char *get_uri(sensor_type_t type);
	const char *get_privilege(std::string uri);
	sensor_type_t get_type(std::string uri);

	unsigned long long get_timestamp(void);
	unsigned long long get_timestamp(timeval *t);

	const char* get_client_name(void);
	bool get_proc_name(pid_t pid, char *process_name);

	std::vector<std::string> tokenize(const std::string &in, const char *delim);
}

}

#endif /* __SENSOR_UTILS_H__ */
