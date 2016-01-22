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

#ifndef _DBUS_UTIL_H_
#define _DBUS_UTIL_H_

enum dbus_ret{
	DBUS_INIT = -1,
	DBUS_FAILED = 0,
	DBUS_SUCCESS = 1
};

void init_dbus(void);
void fini_dbus(void);

#endif /* _DBUS_UTIL_H_ */
