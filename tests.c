/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust


  WARNING: Use `tests_run.py` to run tests
  
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "dust/ustring.h"
#include "dust/tokenizer.h"
#include "dust/parser.h"


char *CURRENT_TEST;
int TESTS = 0;
int FAILS = 0;

/**
 * @brief Compare two integers and print result message
 * 
 * @param value Value
 * @param expect Expected value
 */
void expect_int(int value, int expect) {
    TESTS++;
    if (value == expect) {
        printf("[PASSED] %s\n", CURRENT_TEST);
    }
    else {
        printf("[FAILED] %s: Expected (int)%d but got (int)%d\n",
                CURRENT_TEST,
                expect, value);
        FAILS++;
    }
}

/**
 * @brief Compare two float and print result message
 * 
 * @param value Value
 * @param expect Expected value
 */
void expect_float(float value, float expect) {
    TESTS++;
    if (value == expect)
        printf("[PASSED] %s\n", CURRENT_TEST);

    else
        printf("[FAILED] %s: Expected (float)%f but got (float)%f\n",
                CURRENT_TEST,
                expect, value);
        FAILS++;
}

/**
 * @brief Expect true boolean value
 * 
 * @param value Value
 */
void expect_true(bool value) {
    TESTS++;
    if (value) {
        printf("[PASSED] %s\n", CURRENT_TEST);
    }
    else {
        printf("[FAILED] %s: Expected true\n", CURRENT_TEST);
        FAILS++;
    }
}

/**
 * @brief Expect false boolean value
 * 
 * @param value Value
 */
void expect_false(bool value) {
    TESTS++;
    if (!value) {
        printf("[PASSED] %s\n", CURRENT_TEST);
    }
    else {
        printf("[FAILED] %s: Expected false\n", CURRENT_TEST);
        FAILS++;
    }
}

/**
 * @brief Compare two 4byte strings and print result message
 * 
 * @param value Value
 * @param expect Expected value
 */
void expect_u32string(u32char *value, u32char *expect) {
    TESTS++;
    if (u32isequal(value, expect))
        printf("[PASSED] %s\n", CURRENT_TEST);

    else
        printf("[FAILED] %s: Expected (u32string)'%s' but got (u32string)'%s'\n",
                CURRENT_TEST,
                utf32_to_utf8(expect), utf32_to_utf8(value));
        FAILS++;
}

/**
 * @brief Compare two strings and print result message
 * 
 * @param value Value
 * @param expect Expected value
 */
void expect_string(char *value, char *expect) {
    TESTS++;
    if (!strcmp(value, expect))
        printf("[PASSED] %s\n", CURRENT_TEST);

    else
        printf("[FAILED] %s: Expected (string)'%s' but got (string)'%s'\n",
                CURRENT_TEST,
                expect, value);
        FAILS++;
}


/*
  TEST SUITES
*/

void TEST__u32count() {
    u32char *str = U"hello, this is the test thesuite!";
    expect_int(u32count(str, U"the"), 2);
}

void TEST__u32countchr() {
    u32char *str = U"hello, this is the test suite!i";
    expect_int(u32countchr(str, U'i'), 4);
}

void TEST__u32len() {
    u32char *str = U"hello, this is the test suite!";
    expect_int(u32len(str), 31);
}

void TEST__u32isequal() {
    u32char *str1 = U"hello, this is the test suite!";
    u32char *str2 = U"hello, this is the test suite! ";
    expect_false(u32isequal(str1, str2));
}

void TEST__u32concat() {
    u32char str[40] = U"hello, this is";
    u32concat(str, U" the test suite!");
    expect_u32string(U"hello, this is the test suite!", str);
}

void TEST__u32copy() {
    u32char str[40] = U"";
    u32copy(str, U"hello, this is the test suite!");
    expect_u32string(U"hello, this is the test suite!", str);
}

void TEST__u32findchr() {
    u32char *str = U"hello, this is the test suite!";
    expect_int(u32findchr(str, U't'), 7);
}

void TEST__u32rfindchr() {
    u32char *str = U"hello, this is the test suite!";
    expect_int(u32rfindchr(str, U't'), 2);
}

void TEST__u32find() {
    u32char *str = U"hello, this is the test suite!";
    expect_int(u32find(str, U"the"), 15);
}

void TEST__u32rfind() {
    u32char *str = U"hello, this is the test suite!";
    expect_int(u32rfind(str, U"the"), 14);
}

void TEST__u32cisalnum() {
    u32char chr = U'a';
    expect_true(u32cisalnum(chr));
}

void TEST__u32cisdigit() {
    u32char chr1 = U'5';
    expect_true(u32cisdigit(chr1));

    u32char chr2 = U'a';
    expect_false(u32cisdigit(chr2));
}

void TEST__u32cisxdigit() {
    u32char chr1 = U'5';
    expect_true(u32cisxdigit(chr1));

    u32char chr2 = U'a';
    expect_true(u32cisxdigit(chr2));

    u32char chr3 = U't';
    expect_false(u32cisxdigit(chr3));
}

void TEST__u32cisspace() {
    u32char chr1 = U'a';
    expect_false(u32cisspace(chr1));

    u32char chr2 = U' ';
    expect_true(u32cisspace(chr2));
}

void TEST__u32push() {
    u32char *str = (u32char *)malloc(sizeof(u32char));
    str[0] = U'\0';
    str = u32push(str, U'o');
    expect_u32string(str, U"o");
}

void TEST__u32join() {
    u32char *str = U"hello ";
    expect_u32string(u32join(str, U"world"), U"hello world");
}

void TEST__u32replace() {
    u32char *str = U"hello ive been working on this forever.";
    expect_u32string(u32replace(str, U"e", U"ğ"), U"hğllo ivğ bğğn working on this forğvğr.");
}

void TEST__u32strip() {
    u32char str[12] = U"     hello ";
    expect_u32string(u32strip(str), U"hello");
}

void TEST__u32slice() {
    u32char *str = U"hello world";
    expect_u32string(u32slice(str, 5, 10), U"world");
}

void TEST__u32startswith() {
    u32char *str = U"hello world";
    expect_true(u32startswith(str, U"hello"));
}

void TEST__u32endswith() {
    u32char *str = U"hello world";
    expect_true(u32endswith(str, U"orld"));
}

void TEST__u32contains() {
    u32char *str = U"hello world";
    expect_true(u32contains(str, U"orl"));
}

void TEST__u32isdigit() {
    u32char *str = U"136071324";
    expect_true(u32isdigit(str));
}


int main() {
    CURRENT_TEST = "u32count   ";   TEST__u32count();
    CURRENT_TEST = "u32countchr";   TEST__u32countchr();
    CURRENT_TEST = "u32len";        TEST__u32len();
    CURRENT_TEST = "u32isequal";    TEST__u32isequal();
    CURRENT_TEST = "u32concat";     TEST__u32concat();
    CURRENT_TEST = "u32copy";       TEST__u32copy();
    CURRENT_TEST = "u32findchr";    TEST__u32findchr();
    CURRENT_TEST = "u32rfindchr";   TEST__u32rfindchr();
    CURRENT_TEST = "u32find";       TEST__u32find();
    CURRENT_TEST = "u32rfind";      TEST__u32rfind();
    CURRENT_TEST = "u32cisalnum";   TEST__u32cisalnum();
    CURRENT_TEST = "u32cisdigit";   TEST__u32cisdigit();
    CURRENT_TEST = "u32cisxdigit";  TEST__u32cisxdigit();
    CURRENT_TEST = "u32cisspace";   TEST__u32cisspace();
    CURRENT_TEST = "u32push";       TEST__u32push();
    CURRENT_TEST = "u32join";       TEST__u32join();
    CURRENT_TEST = "u32replace";    TEST__u32replace();
    CURRENT_TEST = "u32strip";      TEST__u32strip();
    CURRENT_TEST = "u32slice";      TEST__u32slice();
    CURRENT_TEST = "u32startswith"; TEST__u32startswith();
    CURRENT_TEST = "u32endswith";   TEST__u32endswith();
    CURRENT_TEST = "u32contains";   TEST__u32contains();
    CURRENT_TEST = "u32isdigit";    TEST__u32isdigit();

    printf("tests: %d\n", TESTS);
    printf("fails: %d\n", FAILS);

    return 0;
}