/*
 * sensorctl
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

#pragma once /* __TEST_BENCH_H__ */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#define FAIL(left, comp, right) \
do { \
	_E("[   FAIL   ] "); \
	std::ostringstream os; \
	os << __FUNCTION__ << "(" << __LINE__ << ") : " \
		<< #left << "(" << left << ") " \
		<< #comp << " " << #right << "(" << right << ")"; \
	std::cout << os.str() << std::endl; \
	test_bench::push_failure(__FUNCTION__, __LINE__, os.str()); \
} while (0)

#define PASS(left, comp, right) \
do { \
	if (test_option::verbose) { \
		_I("[   PASS   ] "); \
		std::ostringstream os; \
		os << __FUNCTION__ << "(" << __LINE__ << ") : " \
			<< #left << "(" << left << ") " \
			<< #comp << " " << #right << "(" << right << ")"; \
		std::cout << os.str() << std::endl; \
	} \
} while (0)

#define ASSERT(left, comp, right) \
do { \
	if (!((left) comp (right))) { \
		FAIL(left, comp, right); \
		return false; \
	} \
	PASS(left, comp, right); \
} while (0)


#define EXPECT(left, comp, right) \
do { \
	if (!((left) comp (right))) { \
		FAIL(left, comp, right); \
	} else { \
		PASS(left, comp, right); \
	} \
} while (0)

#define ASSERT_FREE(expr, X) \
	do { if (expr) { free (X); X = NULL; } } while (0)

#define ASSERT_TRUE(condition) ASSERT(condition, ==, true)
#define ASSERT_FALSE(condition) ASSERT(condition, ==, false)
#define ASSERT_EQ(left, right) ASSERT(left, ==, right)
#define ASSERT_NE(left, right) ASSERT(left, !=, right)
#define ASSERT_LT(left, right) ASSERT(left, <, right)
#define ASSERT_LE(left, right) ASSERT(left, <=, right)
#define ASSERT_GT(left, right) ASSERT(left, >, right)
#define ASSERT_GE(left, right) ASSERT(left, >=, right)
#define ASSERT_NEAR(left, right, err) \
do { \
	ASSERT(left, >=, (right - (err))); \
	ASSERT(left, <=, (right + (err))); \
} while (0)

#define EXPECT_TRUE(condition) EXPECT(condition, ==, true)
#define EXPECT_FALSE(condition) EXPECT(condition, ==, false)
#define EXPECT_EQ(left, right) EXPECT(left, ==, right)
#define EXPECT_NE(left, right) EXPECT(left, !=, right)
#define EXPECT_LT(left, right) EXPECT(left, <, right)
#define EXPECT_LE(left, right) EXPECT(left, <=, right)
#define EXPECT_GT(left, right) EXPECT(left, >, right)
#define EXPECT_GE(left, right) EXPECT(left, >=, right)
#define EXPECT_NEAR(left, right, err) \
do { \
	EXPECT(left, >=, (right - (err))); \
	EXPECT(left, <=, (right + (err))); \
} while (0)

#define TESTCASE(group, name) \
class test_case_##group##_##name : public test_case { \
public: \
	test_case_##group##_##name() \
	: test_case(#group, #name) \
	{ \
		register_func(static_cast<test_case::test_func>(&test_case_##group##_##name::test)); \
	} \
	bool test(void); \
} test_case_##group##_##name##_instance; \
bool test_case_##group##_##name::test(void)

template<typename T>
class Deleter {
public:
	T*& get()
	{
		return storage;
	}
    ~Deleter() {
        if(storage) {
            free(storage);
        }
    }

private:
	T *storage{nullptr};
};

/*
 * Declaration of test_option
 */
class test_option {
public:
	static bool verbose;
	static bool shuffle; /* TODO */
	static bool show_list;
	static int repeat; /* TODO */
	static std::string filter;
	static std::string output; /* TODO */
	static int interval;
	static int latency;
	static int powersave;

	static bool set_options(int argc, char *argv[]);
};

/*
 * Declaration of test_result
 */
class test_result {
public:
	test_result(const std::string &_function, long _line, const std::string &_msg)
	: function(_function)
	, line(_line)
	, msg(_msg) { }

	std::string function;
	long line;
	std::string msg;
};

/*
 * Declaration of test_case
 */
class test_case {
public:
	test_case(const std::string &group, const std::string &name);

	void run_testcase(void);

	const std::string& group() const { return m_group; }
	const std::string& name() const { return m_name; }
	const std::string& fullname() const { return m_fullname; }

protected:
	typedef bool (test_case::*test_func)();

	void started(void);
	void stopped(bool result);
	void register_func(test_func func);

private:
	const std::string m_group;
	const std::string m_name;
	const std::string m_fullname;
	test_func m_func;
};

/*
 * Declaration of test_bench
 */
class test_bench {
public:
	test_bench()
	: m_failure_count(0)
	, m_stop(false)
	{}

	static void show_testcases(void);

	static void register_testcase(const std::string &group, test_case *testcase);

	static void run_all_testcases(void);
	static void stop_all_testcases(void);

	static void push_failure(const std::string &function, long line, const std::string &msg);

private:
	static test_bench& instance();

	void add_failure(const std::string &function, long line, const std::string &msg);

	void started(void);
	void stopped(void);
	void show_failures(void);

	void add_testcase(const std::string &group, test_case *testcase);

	bool filter(const std::string &name, const std::string &filter);
	bool filter_name(const std::string &name);
	bool filter_group(const std::string &group);

	void run(void);
	void stop(void);

	void show(void);

	unsigned int count(void);

	std::multimap<const std::string, test_case *> testcases;
	std::vector<test_result> results;
	int m_failure_count;
	bool m_stop;
};
