/*
 *  Copyright (c) 2016-2017 Samsung Electronics Co., Ltd.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef __SAVITZKYGOLAYFILTER15_H__
#define __SAVITZKYGOLAYFILTER15_H__

#include "timestamp.h"

/************************************************************************
 * stores Savitzky-Golay filter state.
 */
class savitzky_golay_filter15 {
public:

	savitzky_golay_filter15();

	~savitzky_golay_filter15();

	/************************************************************************
	 * filters input data.
	 *
	 * @param value
	 *              data to filter.
	 * @result filtered data.
	 */
	double filter(double value);

	/************************************************************************
	 * resets Savitzky-Golay filter to initial state.
	 */
	void reset(void);

private:
	double *m_array;
	bool m_empty;
};

#endif /* __SAVITZKYGOLAYFILTER15_H__ */
