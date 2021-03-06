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

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <stdlib.h> /* size_t */
#include <atomic>
#include <memory>

#define MAX_MSG_CAPACITY (32*1024)
#define MAX_HEADER_RESERVED 3

namespace ipc {

typedef struct message_header {
	uint64_t id { 0 };
	uint32_t type { 0 };
	size_t length { 0 };
	int32_t err { 0 };
	void *ancillary[MAX_HEADER_RESERVED] { nullptr };
} message_header;

class message {
public:
	template <class... Args>
	static std::shared_ptr<message> create(Args&&... args)
		noexcept(noexcept(message(std::forward<Args>(args)...)))
	{
		return std::shared_ptr<message>(new (std::nothrow) message(std::forward<Args>(args)...));
	}

	message(size_t capacity = MAX_MSG_CAPACITY);
	message(const void *msg, size_t size);
	message(const message &msg);
	message(int err);
	~message();

	void enclose(const void *msg, const size_t size);
	void enclose(int error);
	void disclose(void *msg, const size_t size);

	uint32_t type(void);
	void set_type(uint32_t type);

	size_t size(void);

	void ref(void);
	void unref(void);
	int  ref_count(void);

	message_header *header(void);
	char *body(void);

private:
	message_header m_header;
	size_t m_size;
	size_t m_capacity;

	char *m_msg;
};

}

#endif /* __MESSAGE_H__ */
