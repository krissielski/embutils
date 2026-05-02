/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-24 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
 * ========================================================================= */

#ifndef UNITY_FRAMEWORK_H
#define UNITY_FRAMEWORK_H
#define UNITY_FRAMEWORK_VERSION_MAJOR 2
#define UNITY_FRAMEWORK_VERSION_MINOR 6
#define UNITY_FRAMEWORK_VERSION_BUILD 0
#define UNITY_FRAMEWORK_VERSION       "2.6.0"

#ifdef __cplusplus
extern "C"
{
#endif

#include "unity_internals.h"

/*-------------------------------------------------------
 * Basic Fail / Ignore / Pass
 *-------------------------------------------------------*/

#define TEST_FAIL_MESSAGE(message)                                                               UNITY_TEST_FAIL(__LINE__, (message))
#define TEST_FAIL()                                                                              UNITY_TEST_FAIL(__LINE__, NULL)
#define TEST_IGNORE_MESSAGE(message)                                                             UNITY_TEST_IGNORE(__LINE__, (message))
#define TEST_IGNORE()                                                                            UNITY_TEST_IGNORE(__LINE__, NULL)
#define TEST_MESSAGE(message)                                                                    UnityMessage((message), __LINE__)
#define TEST_ONLY()

/*-------------------------------------------------------
 * Test Asserts (simple)
 *-------------------------------------------------------*/

#define TEST_ASSERT(condition)                                                                   UNITY_TEST_ASSERT(       (condition), __LINE__, " Expression Evaluated To FALSE")
#define TEST_ASSERT_TRUE(condition)                                                              UNITY_TEST_ASSERT(       (condition), __LINE__, " Expected TRUE Was FALSE")
#define TEST_ASSERT_UNLESS(condition)                                                            UNITY_TEST_ASSERT(      !(condition), __LINE__, " Expression Evaluated To TRUE")
#define TEST_ASSERT_FALSE(condition)                                                             UNITY_TEST_ASSERT(      !(condition), __LINE__, " Expected FALSE Was TRUE")
#define TEST_ASSERT_NULL(pointer)                                                                UNITY_TEST_ASSERT_NULL(    (pointer), __LINE__, " Expected NULL")
#define TEST_ASSERT_NOT_NULL(pointer)                                                            UNITY_TEST_ASSERT_NOT_NULL((pointer), __LINE__, " Expected Non-NULL")
#define TEST_ASSERT_EMPTY(pointer)                                                               UNITY_TEST_ASSERT_EMPTY(   (pointer), __LINE__, " Expected Empty String")
#define TEST_ASSERT_NOT_EMPTY(pointer)                                                           UNITY_TEST_ASSERT_NOT_EMPTY((pointer),__LINE__, " Expected Non-Empty String")

/*-------------------------------------------------------
 * Test Asserts (numerical)
 *-------------------------------------------------------*/

#define TEST_ASSERT_EQUAL_INT(expected, actual)                                                  UNITY_TEST_ASSERT_EQUAL_INT((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_INT8(expected, actual)                                                 UNITY_TEST_ASSERT_EQUAL_INT8((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_INT16(expected, actual)                                                UNITY_TEST_ASSERT_EQUAL_INT16((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_INT32(expected, actual)                                                UNITY_TEST_ASSERT_EQUAL_INT32((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_UINT(expected, actual)                                                 UNITY_TEST_ASSERT_EQUAL_UINT( (expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_UINT8(expected, actual)                                                UNITY_TEST_ASSERT_EQUAL_UINT8( (expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_UINT16(expected, actual)                                               UNITY_TEST_ASSERT_EQUAL_UINT16( (expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_UINT32(expected, actual)                                               UNITY_TEST_ASSERT_EQUAL_UINT32( (expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL(expected, actual)                                                      UNITY_TEST_ASSERT_EQUAL_INT((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_NOT_EQUAL(expected, actual)                                                  UNITY_TEST_ASSERT(((expected) != (actual)), __LINE__, " Expected Not-Equal")
#define TEST_ASSERT_EQUAL_HEX(expected, actual)                                                  UNITY_TEST_ASSERT_EQUAL_HEX32((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_HEX8(expected, actual)                                                 UNITY_TEST_ASSERT_EQUAL_HEX8((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_HEX16(expected, actual)                                                UNITY_TEST_ASSERT_EQUAL_HEX16((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_HEX32(expected, actual)                                                UNITY_TEST_ASSERT_EQUAL_HEX32((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_EQUAL_size_t(expected, actual)                                               UNITY_TEST_ASSERT_EQUAL_UINT((expected), (actual), __LINE__, NULL)

/*-------------------------------------------------------
 * Test Asserts (memory)
 *-------------------------------------------------------*/

#define TEST_ASSERT_EQUAL_MEMORY(expected, actual, len)                                          UNITY_TEST_ASSERT_EQUAL_MEMORY((expected), (actual), (len), __LINE__, NULL)

/*-------------------------------------------------------
 * Test Asserts (string)
 *-------------------------------------------------------*/

#define TEST_ASSERT_EQUAL_STRING(expected, actual)                                               UNITY_TEST_ASSERT_EQUAL_STRING((expected), (actual), __LINE__, NULL)
#define TEST_ASSERT_NOT_EQUAL_STRING(expected, actual)                                           UNITY_TEST_ASSERT_NOT_EQUAL_STRING((expected), (actual), __LINE__, NULL)

/*-------------------------------------------------------
 * Control Flow Macros
 *-------------------------------------------------------*/

#define UNITY_BEGIN()    UnityBegin(__FILE__)
#define UNITY_END()      UnityEnd()
#define RUN_TEST(func)   UnityDefaultTestRun(func, #func, __LINE__)

#ifdef __cplusplus
}
#endif

#endif /* UNITY_FRAMEWORK_H */
