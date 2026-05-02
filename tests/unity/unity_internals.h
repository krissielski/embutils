/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-24 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
 * ========================================================================= */

#ifndef UNITY_INTERNALS_H
#define UNITY_INTERNALS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/*-------------------------------------------------------
 * Int Support
 *-------------------------------------------------------*/

typedef   signed long  UNITY_INT;
typedef unsigned long  UNITY_UINT;
typedef unsigned char  UNITY_UINT8;
typedef unsigned short UNITY_UINT16;
typedef unsigned int   UNITY_UINT32;
typedef   signed char  UNITY_INT8;
typedef   signed short UNITY_INT16;
typedef   signed int   UNITY_INT32;

/*-------------------------------------------------------
 * Output
 *-------------------------------------------------------*/

#ifndef UNITY_OUTPUT_CHAR
#define UNITY_OUTPUT_CHAR(a)     (void)putchar(a)
#endif

#ifndef UNITY_OUTPUT_FLUSH
#define UNITY_OUTPUT_FLUSH()     (void)fflush(stdout)
#endif

#ifndef UNITY_PRINT_EOL
#define UNITY_PRINT_EOL()        UNITY_OUTPUT_CHAR('\n')
#endif

/*-------------------------------------------------------
 * Internal Structs
 *-------------------------------------------------------*/

typedef void (*UnityTestFunction)(void);

#define UNITY_DISPLAY_RANGE_INT  (0x10)
#define UNITY_DISPLAY_RANGE_UINT (0x20)
#define UNITY_DISPLAY_RANGE_HEX  (0x40)

typedef enum {
    UNITY_DISPLAY_STYLE_INT        = 4  + UNITY_DISPLAY_RANGE_INT,
    UNITY_DISPLAY_STYLE_INT8       = 1  + UNITY_DISPLAY_RANGE_INT,
    UNITY_DISPLAY_STYLE_INT16      = 2  + UNITY_DISPLAY_RANGE_INT,
    UNITY_DISPLAY_STYLE_INT32      = 4  + UNITY_DISPLAY_RANGE_INT,
    UNITY_DISPLAY_STYLE_UINT       = 4  + UNITY_DISPLAY_RANGE_UINT,
    UNITY_DISPLAY_STYLE_UINT8      = 1  + UNITY_DISPLAY_RANGE_UINT,
    UNITY_DISPLAY_STYLE_UINT16     = 2  + UNITY_DISPLAY_RANGE_UINT,
    UNITY_DISPLAY_STYLE_UINT32     = 4  + UNITY_DISPLAY_RANGE_UINT,
    UNITY_DISPLAY_STYLE_HEX8       = 1  + UNITY_DISPLAY_RANGE_HEX,
    UNITY_DISPLAY_STYLE_HEX16      = 2  + UNITY_DISPLAY_RANGE_HEX,
    UNITY_DISPLAY_STYLE_HEX32      = 4  + UNITY_DISPLAY_RANGE_HEX,
    UNITY_DISPLAY_STYLE_UNKNOWN
} UNITY_DISPLAY_STYLE_T;

typedef enum {
    UNITY_EQUAL_TO    = 1,
    UNITY_GREATER_THAN = 2,
    UNITY_GREATER_OR_EQUAL = 3,
    UNITY_SMALLER_THAN = 4,
    UNITY_SMALLER_OR_EQUAL = 5
} UNITY_COMPARISON_T;

#ifndef UNITY_EXCLUDE_FLOAT
typedef float  UNITY_FLOAT;
#define UNITY_FLOAT_PRECISION (0.00001f)
#endif

struct UNITY_STORAGE_T {
    const char*  TestFile;
    const char*  CurrentTestName;
    UNITY_UINT   CurrentTestLineNumber;
    UNITY_UINT   NumberOfTests;
    UNITY_UINT   TestFailures;
    UNITY_UINT   TestIgnores;
    UNITY_UINT   CurrentTestFailed;
    UNITY_UINT   CurrentTestIgnored;
};

extern struct UNITY_STORAGE_T Unity;

/*-------------------------------------------------------
 * Test Running Macros
 *-------------------------------------------------------*/

#define UNITY_TEST_ASSERT(condition, line, message) \
    if (condition) {} else { UnityFail((message), (UNITY_UINT)(line)); }

#define UNITY_TEST_ASSERT_NULL(pointer, line, message) \
    UNITY_TEST_ASSERT(((pointer) == NULL), (line), (message))

#define UNITY_TEST_ASSERT_NOT_NULL(pointer, line, message) \
    UNITY_TEST_ASSERT(((pointer) != NULL), (line), (message))

#define UNITY_TEST_ASSERT_EMPTY(pointer, line, message) \
    UNITY_TEST_ASSERT((pointer != NULL && (pointer)[0] == 0), (line), (message))

#define UNITY_TEST_ASSERT_NOT_EMPTY(pointer, line, message) \
    UNITY_TEST_ASSERT((pointer != NULL && (pointer)[0] != 0), (line), (message))

#define UNITY_TEST_ASSERT_EQUAL_INT(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(expected), (UNITY_INT)(actual), (message), (UNITY_UINT)(line), UNITY_DISPLAY_STYLE_INT)

#define UNITY_TEST_ASSERT_EQUAL_INT8(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT8)(expected), (UNITY_INT)(UNITY_INT8)(actual), (message), (UNITY_UINT)(line), UNITY_DISPLAY_STYLE_INT8)

#define UNITY_TEST_ASSERT_EQUAL_INT16(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT16)(expected), (UNITY_INT)(UNITY_INT16)(actual), (message), (UNITY_UINT)(line), UNITY_DISPLAY_STYLE_INT16)

#define UNITY_TEST_ASSERT_EQUAL_INT32(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)(expected), (UNITY_INT)(UNITY_INT32)(actual), (message), (UNITY_UINT)(line), UNITY_DISPLAY_STYLE_INT32)

#define UNITY_TEST_ASSERT_EQUAL_UINT(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(expected), (UNITY_INT)(actual), (message), (UNITY_UINT)(line), UNITY_DISPLAY_STYLE_UINT)

#define UNITY_TEST_ASSERT_EQUAL_UINT8(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8)(expected), (UNITY_INT)(UNITY_UINT8)(actual), (message), (UNITY_UINT)(line), UNITY_DISPLAY_STYLE_UINT8)

#define UNITY_TEST_ASSERT_EQUAL_UINT16(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)(expected), (UNITY_INT)(UNITY_UINT16)(actual), (message), (UNITY_UINT)(line), UNITY_DISPLAY_STYLE_UINT16)

#define UNITY_TEST_ASSERT_EQUAL_UINT32(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)(expected), (UNITY_INT)(UNITY_UINT32)(actual), (message), (UNITY_UINT)(line), UNITY_DISPLAY_STYLE_UINT32)

#define UNITY_TEST_ASSERT_EQUAL_HEX8(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8)(expected), (UNITY_INT)(UNITY_UINT8)(actual), (message), (UNITY_UINT)(line), UNITY_DISPLAY_STYLE_HEX8)

#define UNITY_TEST_ASSERT_EQUAL_HEX16(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)(expected), (UNITY_INT)(UNITY_UINT16)(actual), (message), (UNITY_UINT)(line), UNITY_DISPLAY_STYLE_HEX16)

#define UNITY_TEST_ASSERT_EQUAL_HEX32(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)(expected), (UNITY_INT)(UNITY_UINT32)(actual), (message), (UNITY_UINT)(line), UNITY_DISPLAY_STYLE_HEX32)

#define UNITY_TEST_ASSERT_EQUAL_MEMORY(expected, actual, len, line, message) \
    UnityAssertEqualMemory((void*)(expected), (void*)(actual), (UNITY_UINT32)(len), 1, (message), (UNITY_UINT)(line))

#define UNITY_TEST_ASSERT_EQUAL_STRING(expected, actual, line, message) \
    UnityAssertEqualString((const char*)(expected), (const char*)(actual), (message), (UNITY_UINT)(line))

#define UNITY_TEST_ASSERT_NOT_EQUAL_STRING(expected, actual, line, message) \
    UnityAssertNotEqualString((const char*)(expected), (const char*)(actual), (message), (UNITY_UINT)(line))

#define UNITY_TEST_FAIL(line, message) \
    UnityFail((message), (UNITY_UINT)(line))

#define UNITY_TEST_IGNORE(line, message) \
    UnityIgnore((message), (UNITY_UINT)(line))

/*-------------------------------------------------------
 * Internal Functions (prototypes)
 *-------------------------------------------------------*/

void UnityBegin(const char* filename);
int  UnityEnd(void);
void UnityDefaultTestRun(UnityTestFunction Func, const char* FuncName, const int FuncLineNum);
void UnityConcludeTest(void);

void UnityFail(const char* message, const UNITY_UINT line);
void UnityIgnore(const char* message, const UNITY_UINT line);
void UnityMessage(const char* message, const UNITY_UINT line);

void UnityAssertEqualNumber(const UNITY_INT expected,
                             const UNITY_INT actual,
                             const char* msg,
                             const UNITY_UINT lineNumber,
                             const UNITY_DISPLAY_STYLE_T style);

#ifndef UNITY_PTR_ATTRIBUTE
#define UNITY_PTR_ATTRIBUTE
#endif

void UnityAssertEqualMemory(UNITY_PTR_ATTRIBUTE const void* expected,
                             UNITY_PTR_ATTRIBUTE const void* actual,
                             const UNITY_UINT32 length,
                             const UNITY_UINT32 num_elements,
                             const char* msg,
                             const UNITY_UINT lineNumber);

void UnityAssertEqualString(const char* expected,
                             const char* actual,
                             const char* msg,
                             const UNITY_UINT lineNumber);

void UnityAssertNotEqualString(const char* expected,
                                const char* actual,
                                const char* msg,
                                const UNITY_UINT lineNumber);

#ifdef __cplusplus
}
#endif

#endif /* UNITY_INTERNALS_H */
