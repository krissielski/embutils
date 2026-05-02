/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-24 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
 * ========================================================================= */

#include "unity.h"
#include <stdio.h>
#include <string.h>

struct UNITY_STORAGE_T Unity;

/*-------------------------------------------------------
 * Output Helpers
 *-------------------------------------------------------*/

static void UnityPrint(const char* string)
{
    const char* pch = string;
    if (pch != NULL) {
        while (*pch) {
            UNITY_OUTPUT_CHAR(*pch);
            pch++;
        }
    }
}

static void UnityPrintNumber(const UNITY_INT number, const UNITY_DISPLAY_STYLE_T style)
{
    UNITY_UINT abs_number;
    char buf[32];
    int  i = 30;

    buf[31] = '\0';

    if ((style & UNITY_DISPLAY_RANGE_INT) && (number < 0)) {
        abs_number = (UNITY_UINT)(-number);
        UNITY_OUTPUT_CHAR('-');
    } else {
        abs_number = (UNITY_UINT)number;
    }

    if (style & UNITY_DISPLAY_RANGE_HEX) {
        UNITY_OUTPUT_CHAR('0');
        UNITY_OUTPUT_CHAR('x');
        /* Print hex */
        unsigned long n = (unsigned long)abs_number;
        /* determine width */
        int width = 2;
        if (style == UNITY_DISPLAY_STYLE_HEX16) width = 4;
        if (style == UNITY_DISPLAY_STYLE_HEX32) width = 8;
        char hex[9];
        int j;
        for (j = width - 1; j >= 0; j--) {
            int nibble = (int)(n & 0xF);
            hex[j] = (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);
            n >>= 4;
        }
        hex[width] = '\0';
        UnityPrint(hex);
        return;
    }

    /* Decimal */
    if (abs_number == 0) {
        UNITY_OUTPUT_CHAR('0');
        return;
    }
    while (abs_number > 0 && i > 0) {
        buf[i--] = (char)('0' + (abs_number % 10));
        abs_number /= 10;
    }
    UnityPrint(&buf[i + 1]);
}

static void UnityPrintExpectedAndActualStrings(const char* expected, const char* actual)
{
    UnityPrint(" Expected ");
    if (expected != NULL) {
        UNITY_OUTPUT_CHAR('\'');
        UnityPrint(expected);
        UNITY_OUTPUT_CHAR('\'');
    } else {
        UnityPrint("NULL");
    }
    UnityPrint(" Was ");
    if (actual != NULL) {
        UNITY_OUTPUT_CHAR('\'');
        UnityPrint(actual);
        UNITY_OUTPUT_CHAR('\'');
    } else {
        UnityPrint("NULL");
    }
}

/*-------------------------------------------------------
 * Test Lifecycle
 *-------------------------------------------------------*/

void UnityBegin(const char* filename)
{
    Unity.TestFile       = filename;
    Unity.NumberOfTests  = 0;
    Unity.TestFailures   = 0;
    Unity.TestIgnores    = 0;
    Unity.CurrentTestFailed  = 0;
    Unity.CurrentTestIgnored = 0;
}

int UnityEnd(void)
{
    UNITY_PRINT_EOL();
    UnityPrint("-----------------------");
    UNITY_PRINT_EOL();
    UnityPrintNumber((UNITY_INT)Unity.NumberOfTests, UNITY_DISPLAY_STYLE_UINT);
    UnityPrint(" Tests ");
    UnityPrintNumber((UNITY_INT)Unity.TestFailures,  UNITY_DISPLAY_STYLE_UINT);
    UnityPrint(" Failures ");
    UnityPrintNumber((UNITY_INT)Unity.TestIgnores,   UNITY_DISPLAY_STYLE_UINT);
    UnityPrint(" Ignored");
    UNITY_PRINT_EOL();
    if (Unity.TestFailures == 0) {
        UnityPrint("OK");
    } else {
        UnityPrint("FAIL");
    }
    UNITY_PRINT_EOL();
    UNITY_OUTPUT_FLUSH();
    return (int)Unity.TestFailures;
}

void UnityDefaultTestRun(UnityTestFunction Func, const char* FuncName, const int FuncLineNum)
{
    Unity.CurrentTestName       = FuncName;
    Unity.CurrentTestLineNumber = (UNITY_UINT)FuncLineNum;
    Unity.NumberOfTests++;
    Unity.CurrentTestFailed  = 0;
    Unity.CurrentTestIgnored = 0;
    Func();
    UnityConcludeTest();
}

void UnityConcludeTest(void)
{
    if (Unity.CurrentTestIgnored) {
        Unity.TestIgnores++;
    } else if (Unity.CurrentTestFailed) {
        Unity.TestFailures++;
    } else {
        UnityPrint(Unity.TestFile);
        UNITY_OUTPUT_CHAR(':');
        UnityPrintNumber((UNITY_INT)Unity.CurrentTestLineNumber, UNITY_DISPLAY_STYLE_UINT);
        UNITY_OUTPUT_CHAR(':');
        UnityPrint(Unity.CurrentTestName);
        UnityPrint(":PASS");
        UNITY_PRINT_EOL();
    }
}

/*-------------------------------------------------------
 * Assertion Helpers
 *-------------------------------------------------------*/

void UnityFail(const char* message, const UNITY_UINT line)
{
    Unity.CurrentTestFailed = 1;
    UnityPrint(Unity.TestFile);
    UNITY_OUTPUT_CHAR(':');
    UnityPrintNumber((UNITY_INT)line, UNITY_DISPLAY_STYLE_UINT);
    UNITY_OUTPUT_CHAR(':');
    UnityPrint(Unity.CurrentTestName);
    UnityPrint(":FAIL");
    if (message != NULL) {
        UNITY_OUTPUT_CHAR(':');
        UnityPrint(message);
    }
    UNITY_PRINT_EOL();
    UNITY_OUTPUT_FLUSH();
}

void UnityIgnore(const char* message, const UNITY_UINT line)
{
    Unity.CurrentTestIgnored = 1;
    UnityPrint(Unity.TestFile);
    UNITY_OUTPUT_CHAR(':');
    UnityPrintNumber((UNITY_INT)line, UNITY_DISPLAY_STYLE_UINT);
    UNITY_OUTPUT_CHAR(':');
    UnityPrint(Unity.CurrentTestName);
    UnityPrint(":IGNORE");
    if (message != NULL) {
        UNITY_OUTPUT_CHAR(':');
        UnityPrint(message);
    }
    UNITY_PRINT_EOL();
}

void UnityMessage(const char* message, const UNITY_UINT line)
{
    UnityPrint(Unity.TestFile);
    UNITY_OUTPUT_CHAR(':');
    UnityPrintNumber((UNITY_INT)line, UNITY_DISPLAY_STYLE_UINT);
    UNITY_OUTPUT_CHAR(':');
    UnityPrint("INFO:");
    UnityPrint(message);
    UNITY_PRINT_EOL();
}

void UnityAssertEqualNumber(const UNITY_INT expected,
                             const UNITY_INT actual,
                             const char* msg,
                             const UNITY_UINT lineNumber,
                             const UNITY_DISPLAY_STYLE_T style)
{
    if (expected != actual) {
        Unity.CurrentTestFailed = 1;
        UnityPrint(Unity.TestFile);
        UNITY_OUTPUT_CHAR(':');
        UnityPrintNumber((UNITY_INT)lineNumber, UNITY_DISPLAY_STYLE_UINT);
        UNITY_OUTPUT_CHAR(':');
        UnityPrint(Unity.CurrentTestName);
        UnityPrint(":FAIL: Expected ");
        UnityPrintNumber(expected, style);
        UnityPrint(" Was ");
        UnityPrintNumber(actual, style);
        if (msg != NULL) {
            UNITY_OUTPUT_CHAR(' ');
            UnityPrint(msg);
        }
        UNITY_PRINT_EOL();
        UNITY_OUTPUT_FLUSH();
    }
}

void UnityAssertEqualMemory(UNITY_PTR_ATTRIBUTE const void* expected,
                             UNITY_PTR_ATTRIBUTE const void* actual,
                             const UNITY_UINT32 length,
                             const UNITY_UINT32 num_elements,
                             const char* msg,
                             const UNITY_UINT lineNumber)
{
    UNITY_PTR_ATTRIBUTE const unsigned char* exp_ptr = (const unsigned char*)expected;
    UNITY_PTR_ATTRIBUTE const unsigned char* act_ptr = (const unsigned char*)actual;
    UNITY_UINT32 total = length * num_elements;

    if (expected == actual) return;
    if (expected == NULL || actual == NULL) {
        UnityFail(" Memory pointers NULL", lineNumber);
        return;
    }
    if (memcmp(expected, actual, total) != 0) {
        UNITY_UINT32 i;
        Unity.CurrentTestFailed = 1;
        UnityPrint(Unity.TestFile);
        UNITY_OUTPUT_CHAR(':');
        UnityPrintNumber((UNITY_INT)lineNumber, UNITY_DISPLAY_STYLE_UINT);
        UNITY_OUTPUT_CHAR(':');
        UnityPrint(Unity.CurrentTestName);
        UnityPrint(":FAIL: Memory mismatch at byte ");
        for (i = 0; i < total; i++) {
            if (exp_ptr[i] != act_ptr[i]) {
                UnityPrintNumber((UNITY_INT)i, UNITY_DISPLAY_STYLE_UINT);
                UnityPrint(" Expected 0x");
                UnityPrintNumber((UNITY_INT)exp_ptr[i], UNITY_DISPLAY_STYLE_HEX8);
                UnityPrint(" Was 0x");
                UnityPrintNumber((UNITY_INT)act_ptr[i], UNITY_DISPLAY_STYLE_HEX8);
                break;
            }
        }
        if (msg != NULL) {
            UNITY_OUTPUT_CHAR(' ');
            UnityPrint(msg);
        }
        UNITY_PRINT_EOL();
        UNITY_OUTPUT_FLUSH();
    }
}

void UnityAssertEqualString(const char* expected,
                             const char* actual,
                             const char* msg,
                             const UNITY_UINT lineNumber)
{
    if (expected == actual) return;
    if (expected == NULL || actual == NULL || strcmp(expected, actual) != 0) {
        Unity.CurrentTestFailed = 1;
        UnityPrint(Unity.TestFile);
        UNITY_OUTPUT_CHAR(':');
        UnityPrintNumber((UNITY_INT)lineNumber, UNITY_DISPLAY_STYLE_UINT);
        UNITY_OUTPUT_CHAR(':');
        UnityPrint(Unity.CurrentTestName);
        UnityPrint(":FAIL:");
        UnityPrintExpectedAndActualStrings(expected, actual);
        if (msg != NULL) {
            UNITY_OUTPUT_CHAR(' ');
            UnityPrint(msg);
        }
        UNITY_PRINT_EOL();
        UNITY_OUTPUT_FLUSH();
    }
}

void UnityAssertNotEqualString(const char* expected,
                                const char* actual,
                                const char* msg,
                                const UNITY_UINT lineNumber)
{
    if (expected == actual) {
        UnityFail(" Strings should not be equal", lineNumber);
        return;
    }
    if (expected != NULL && actual != NULL && strcmp(expected, actual) == 0) {
        Unity.CurrentTestFailed = 1;
        UnityPrint(Unity.TestFile);
        UNITY_OUTPUT_CHAR(':');
        UnityPrintNumber((UNITY_INT)lineNumber, UNITY_DISPLAY_STYLE_UINT);
        UNITY_OUTPUT_CHAR(':');
        UnityPrint(Unity.CurrentTestName);
        UnityPrint(":FAIL: Strings should differ but both are '");
        UnityPrint(actual);
        UNITY_OUTPUT_CHAR('\'');
        if (msg != NULL) {
            UNITY_OUTPUT_CHAR(' ');
            UnityPrint(msg);
        }
        UNITY_PRINT_EOL();
        UNITY_OUTPUT_FLUSH();
    }
}
