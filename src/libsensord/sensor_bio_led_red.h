/*
 * libsensord
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

#ifndef __SENSOR_BIO_LED_RED_H__
#define __SENSOR_BIO_LED_RED_H__

//! Pre-defined events for the bio led red sensor
//! Sensor Plugin developer can add more event to their own headers

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup SENSOR_BIO_LED_RED HRM LED RED Sensor
 * @ingroup SENSOR_FRAMEWORK
 *
 * These APIs are used to control the HRM LED RED sensor.
 * @{
 */

enum bio_led_green_event_type {
	BIO_LED_RED_RAW_DATA_EVENT	= (BIO_LED_RED_SENSOR << 16) | 0x0001,
};

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
//! End of a file