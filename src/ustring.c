/*

    This file is a part of the Dust Programming Language
    project and distributed under the MIT license.

    Copyright © Kadir Aksoy
    https://github.com/kadir014/Dust


    ustring.c  -  Dust Unicode & String Library
    -------------------------------------------------
    This library utilizes 4byte character arrays as
    main string concept. You can encode & decode your
    string to various encodings to print or perform
    different actions on it.

      Supported encodings are:

        UTF-32 (main 4byte string):
            > utf32_to_utf8()
            > utf32_to_ascii()

        UTF-8 (multibyte string):
            > utf8_to_utf32()

        ASCII (1byte string):
            > ascii_to_utf32()

    Character functions usually starts with either
    'u32c' or ends with 'chr'. String functions
    starts with 'u32'.


    Standard Functions        New Functions
    ======================    ============================

    C STD     4byte String    4byte String    1byte String
    ----------------------    ----------------------------
    strlen   | u32len         u32push       | u8push
    strcmp   | u32isequal     u32rfind      | u8rfind
    strcat   | u32concat      u32join       |
    strcpy   | u32copy        u32replace    |
    strchr   | u32findchr     u32strip      |
    strrchr  | u32rfindchr    u32slice      |
    strstr   | u32find        u32startswith |
    isalnum  | u32cisalnum    u32endswith   |
    isalpha  | u32cisalpha    u32contains   |
    isdigit  | u32cisdigit    u32count      |
    isxdigit | u32cisxdigit   u32countchr   |
    isspace  | u32cisspace    u32isdigit    |

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "dust/ustring.h"


#define ENCODING_ASCII_STRICT false //replace overflowed characters with TMPCHR?
#define ENCODING_ASCII_TMPCHR '?'   //character that is used to replace overflowed chars


/**
 * @brief Checks if character is digit
 * 
 * @param chr Character to check
 * @return (bool) result
 */
bool u32cisdigit(u32char chr) {
    return ((chr >= U'0') && (chr <= U'9'));
}

/**
 * @brief Checks if character is hexedecimal digit
 * 
 * @param chr Character to check
 * @return (bool) result
 */
bool u32cisxdigit(u32char chr) {
    return (u32cisdigit(chr) ||
            (chr >= U'A' && chr <= U'F') ||
            (chr >= U'a' && chr <= U'f'));
}

/**
 * @brief Checks if character is space
 * 
 * @param chr Character to check
 * @return (bool) result
 */
bool u32cisspace(u32char chr) {
    return (chr == U' ' ||
            chr == U'\f' ||
            chr == U'\n' ||
            chr == U'\r' ||
            chr == U'\t' ||
            chr == U'\v');
}

/**
 * @brief Checks if all characters of string is digit
 * 
 * @param str String to check
 * @return (bool) result
 */
bool u32isdigit(u32char *str) {
    while (*str) {
        if (!u32cisdigit(*str)) return false;
        str++;
    }
    return true;
}

//TODO: Documentate
bool u32cisalnum(u32char chr) {
    if (chr <= U'Z' && chr >= U'a' && u32cisdigit(chr)) return true;
    return false;
}

//TODO: Cover all invalid identifier cases
/**
 * @brief Check if string is a valid identifier
 * 
 * @param str String to check
 * @return (bool) result
 */
bool u32isidentifier(u32char *str) {
    size_t i = 0;

    if (str[0] == U'_' || u32cisalnum(str[0])) i++;
    else return false;

    while (i < u32len(str)) {
        if (!u32cisspace(str[i])) i++;
        else return false;
    }
    
    return true;
}

/**
 * @brief Calculate length of the string (not including null-terminating char)
 *
 * @param str String to calculate length
 * @return Length of the string
 */
size_t u32len(u32char *str) {
    u32char *t = str;
    size_t i = 0;
    while (*t++ != '\0') i++;
    return i;
}

/**
 * @brief Pushes one character to the end of string
 * 
 * @param str String to push character
 * @param chr Character to push
 * @return New string with the character
 */
char *u8push(char *str, char chr) {
    size_t len = strlen(str);
    char *newstr = (char *)realloc(str, sizeof(char)*len+sizeof(char)*2);
    newstr[len] = chr;
    newstr[len+1] = '\0';
    return newstr;
}

/**
 * @brief Pushes one character to the end of string
 * 
 * @param str String to push character
 * @param chr Character to push
 * @return New string with the character
 */
u32char *u32push(u32char *str, u32char chr) {
    size_t len = u32len(str);
    u32char *newstr = (u32char *)realloc(str, sizeof(u32char)*len+sizeof(u32char)*2);
    newstr[len] = chr;
    newstr[len+1] = '\0';
    return newstr;
}

u32char *u32pushl(u32char *str, u32char chr) {
    size_t len = u32len(str);
    u32char *newstr = (u32char *)malloc(sizeof(u32char)*len+sizeof(u32char)*2);

    u32copy(newstr, str);
    newstr[len] = chr;
    newstr[len+1] = U'\0';

    return newstr;
}

/**
 * @brief Encode 4byte UTF-32 string to multibyte UTF-8 string
 * 
 * @param str 4byte string to encode
 * @return Multibyte UTF-8 encoded string
 */
char *utf32_to_utf8(u32char *str) {
    u32char *tstr = str;

    size_t i = u32len(str);
    size_t j = 0;

    char *u8str = (char *)malloc(sizeof(char));
    u8str[0] = '\0';

    for (; i; i--, tstr++) {
        // outside of UTF-32 code point
        if (*tstr > 0x10FFFF) {
            printf("outside of utf32 code point!\n");
        };

        if (*tstr < 0x7F) {
            u8str = u8push(u8str, *tstr);
            j++;
        }
        else if (*tstr < 0x7FF) {
            u8str = u8push(u8str, 0xC0 | *tstr >> 6);
            u8str = u8push(u8str, 0x80 | *tstr & 0x3f);
            j += 2;
        }
        else if (*tstr < 0xFFFF) {
            u8str = u8push(u8str, 0xE0 | *tstr >> 12);
            u8str = u8push(u8str, 0x80 | *tstr >> 6 & 0x3f);
            u8str = u8push(u8str, 0x80 | *tstr & 0x3f);
            j += 3;
        }
        else {
            u8str = u8push(u8str, 0xF0 | *tstr >> 18);
            u8str = u8push(u8str, 0x80 | *tstr >> 12 & 0x3f);
            u8str = u8push(u8str, 0x80 | *tstr >> 6 & 0x3f);
            u8str = u8push(u8str, 0x80 | *tstr & 0x3f);
            j += 4;
        }
    }

    u8str[j+1] = U'\0';
    return u8str;
}

/**
 * @brief Encode 4byte UTF-32 string to 1byte ASCII string
 * 
 * @param str 4byte string to encode
 * @return ASCII encoded string
 */
char *utf32_to_ascii(u32char *str) {
    u32char *tstr = str;

    size_t i = u32len(str);
    size_t j = 0;

    char *u8str = (char *)malloc(sizeof(char));
    u8str[0] = '\0';

    for (; i; i--, tstr++) {
        if (*tstr < 0x7F) {
            u8str = u8push(u8str, *tstr);
            j++;
        }
        else if (*tstr < 0xFF) {
            u8str = u8push(u8str, 0xC0 | *tstr >> 6);
            u8str = u8push(u8str, 0x80 | *tstr & 0x3f);
            j += 2;
        }
        else {
            #if !ENCODING_ASCII_STRICT
            u8str = u8push(u8str, ENCODING_ASCII_TMPCHR);
            j++;
            #endif
        }
    }

    return u8str;
}

/**
 * @brief Encode multibyte UTF-8 string to 4byte UTF-32 string
 * 
 * @param str String to encode
 * @return 4byte UTF-32 encoded string
 */
u32char *utf8_to_utf32(char *str) {
    u32char *u32str = (u32char *)malloc(sizeof(u32char));
    u32str[0] = U'\0';
    size_t len = strlen(str);

    char *cursor = str;
    while (*cursor != str[len]) {
        u32char chr = 0;

        // pass ASCII characters
        if ((unsigned char)(*cursor) < 128) {
            chr = *(cursor++);
        }
        else {
            char continuation = 0;

            if ((*cursor & 0xE0) == 0xC0) {
                chr = *cursor & 0x1F;
                continuation = 1;
            }
            else if ((*cursor & 0xF0) == 0xE0) {
                chr = *cursor & 0xF;
                continuation = 2;
            }
            else if ((*cursor & 0xF8) == 0xF0) {
                chr = *cursor & 0x7;
                continuation = 3;
            }
            else if ((*cursor & 0xFC) == 0xF8) {
                chr = *cursor & 0x3;
                continuation = 4;
            }
            else if ((*cursor & 0xFE) == 0xFC) {
                chr = *cursor & 0x1;
                continuation = 5;
            }

            for ((cursor)++; continuation > 0; continuation--, (cursor)++) {
                if ((*cursor & 0xC0) != 0x80) {
                    chr = -1;
                    break;
                }

                chr = (chr << 6) | (*cursor & 0x3F);
            }
        }

        u32str = u32push(u32str, chr);
    }

    return u32str;
}

u32char *ascii_to_utf32(char *str) {
    u32char *u32str = (u32char *)malloc(sizeof(u32char));
    u32str[0] = U'\0';
    size_t len = strlen(str);

    for (size_t i = 0; i < len; i++) {
        u32str = u32push(u32str, str[i]);
    }

    return u32str;
}

/**
 * @brief Checks if all characters in the string are blank
 * 
 * @param str String to check
 * @return (bool) result
 */
bool u32isempty(u32char *str) {
    size_t len = u32len(str);
    if (len == 0) return true;

    size_t i;
    for (i = 0; i < len; i++) {
        if (str[i] != U' ') return false;
    }
    return true;
}

/**
 * @brief Checks if two strings are the same
 * 
 * @param str1 First string to check
 * @param str2 Second string to check
 * @return (bool) result
 */
bool u32isequal(u32char *str1, u32char *str2) {
    size_t len = u32len(str1);
    if (len != u32len(str2)) return false;

    //! decleration in for loop segfaults ????
    size_t i;
    for (i = 0; i < len; i++) {
        if (str1[i] != str2[i]) return false;
    }
    return true;
}

/**
 * @brief Checks if string starts with substring
 * 
 * @param str String to check on
 * @param substr Substring to check
 * @return (bool) result
 */
bool u32startswith(u32char *str, u32char *substr) {
    return (u32find(str, substr) == 0);
}

/**
 * @brief Checks if string ends with substring
 * 
 * @param str String to check on
 * @param substr Substring to check
 * @return (bool) result
 */
bool u32endswith(u32char *str, u32char *substr) {
    size_t i = u32rfind(str, substr);
    if (i < 0) return false;
    return (i == u32len(str) - u32len(substr));
}

/**
 * @brief Checks if string contains substring
 * 
 * @param str String to check on
 * @param substr Substring to check
 * @return (bool) result
 */
bool u32contains(u32char *str, u32char *substr) {
    return (u32find(str, substr) != -1);
}

/**
 * @brief Contcatenate string to destination string
 *        (destination string must be large enough)
 * 
 * @param dest Destination string
 * @param src String to append on destination
 */
void u32concat(u32char *dest, u32char *src) {
    u32char *s = dest + u32len(dest);

    while (*src) *s++ = *src++;

    *s = U'\0';
}

/**
 * @brief Copy string onto destination string
 *        (destination string must be large enough)
 * 
 * @param dest Destination string
 * @param src String to copy
 */
void u32copy(u32char *dest, u32char *src) {
    while (*src) *dest++ = *src++;
    *dest = U'\0';
}

/**
 * @brief Find the first occurrence of string
 *        in another string
 * 
 * @param src String to search on
 * @param str String to find
 * @return Index of the found string (-1 if not found)
 */
size_t u32find(u32char *src, u32char *str) {
    size_t i = 0;
    size_t len = u32len(str);

    while (src[i] != '\0') {
        bool a = 0;
        for (size_t j = 0; j < len; j++) {
            if (src[i+j] != str[j]) {
                a = 0;
                break;
            }
            else a = 1;
        }
        if (a) return i;
        i++;
    }

    return -1;
}

/**
 * @brief Find the first occurence of character
 *        in a string
 * 
 * @param src String to search on
 * @param chr Character to find
 * @return Index of the found character (-1 if not found)
 */
size_t u32findchr(u32char *src, u32char chr) {
    size_t i = 0;

    for (; *src != '\0'; ++src) {
        if (*src == chr) return i;
        i++;
    }

    return -1;
}

/**
 * @brief Find the last occurrence of string
 *        in another string
 * 
 * @param src Sstring to search on
 * @param str String to find
 * @return Index of the found string (-1 if not found)
 */
size_t u32rfind(u32char *src, u32char *str) {
    size_t len = u32len(str);
    size_t i = u32len(src)-len;

    while (i > 0) {
        bool a = 0;
        for (size_t j = 0; j < len; j++) {
            if (src[i+j] != str[j]) {
                a = 0;
                break;
            }
            else a = 1;
        }
        if (a) return i;
        i--;
    }

    return -1;
}

/**
 * @brief Find the last occurrence of string
 *        in another string
 * 
 * @param src String to search on
 * @param str String to find
 * @return Index of the found string (-1 if not found)
 */
size_t u8rfind(char *src, char *str) {
    size_t len = strlen(str);
    size_t i = strlen(src)-len;

    while (i > 0) {
        bool a = 0;
        for (size_t j = 0; j < len; j++) {
            if (src[i+j] != str[j]) {
                a = 0;
                break;
            }
            else a = 1;
        }
        if (a) return i;
        i--;
    }

    return -1;
}

/**
 * @brief Find the last occurence of character
 *        in a string
 * 
 * @param src String to search on
 * @param chr Character to find
 * @return Index of the found character (-1 if not found)
 */
size_t u32rfindchr(u32char *src, u32char chr) {
    size_t i = u32len(src)-1;

    while (i > 0) {
        if (src[i] == chr) return i;
        i--;
    }

    return -1;
}

/**
 * @brief Count how many times the substring occures in string
 * 
 * @param str String that contains the substring
 * @param substr Substring to count
 * @return Count of occurences (0 if none)
 */
size_t u32count(u32char *str, u32char *substr) {
    size_t count = 0;
    size_t sublen = u32len(substr);

    for (size_t i = 0; i < u32len(str) - sublen; i++) {
        bool a = false;
        for (size_t j = 0; j < sublen; j++) {
            if (str[i+j] == substr[j]) a = true;
            else {
                a = false;
                break;
            }
        }
        if (a) {
            count++;
            i += sublen;
        }
    }

    return count;
}

/**
 * @brief Count how many times the character occures in string
 * 
 * @param str String that contains the character
 * @param chr Character to count
 * @return Count of occurences (0 if none)
 */
size_t u32countchr(u32char *str, u32char chr) {
    size_t count = 0;

    while (*str) {
        if (*str == chr) count++;
        str++;
    }

    return count;
}

/**
 * @brief Join two strings and return the result
 * 
 * @param str1 First string
 * @param str2 Second string
 * @return New string
 */
u32char *u32join(u32char *str1, u32char *str2) {
    u32char *result = (u32char *)malloc(  sizeof(u32char)*u32len(str1)
                                        + sizeof(u32char)*u32len(str2)
                                        + 1);
    u32copy(result, str1);
    u32concat(result, str2);
    
    return result;
}

/**
 * @brief Replace every occurence of one substring with
 *        another on source string
 * 
 * @param str String to operate on
 * @param oldstr Old substring to replace
 * @param newstr 4New substring to replace with
 * @return New replaced string
 */
u32char *u32replace(u32char *str, u32char *oldstr, u32char *newstr) {
    u32char *result;
    int i, cnt = 0;
    int newlen = u32len(newstr);
    int oldlen = u32len(oldstr);
  
    for (i = 0; str[i] != U'\0'; i++) {
        if (u32find(str+i, oldstr) == 0) {
            cnt++;
            i += oldlen - 1;
        }
    }
  
    result = (u32char *)malloc((i + cnt * (newlen - oldlen) + 1) * 
                                sizeof(u32char));

    i = 0;
    u32char *ptr = result;
    while (*str != U'\0') {
        if (u32find(str, oldstr) == 0) {
            u32copy(ptr+i, newstr);
            i += newlen;
            str += oldlen;
        }
        else
            result[i++] = *str++;
    }
  
    result[i] = U'\0';
    return result;
}

/**
 * @brief Trims leading and trailing whitespaces
 * 
 * @param str String to strip
 * @return Stripped string 
 */
u32char *u32strip(u32char *str) {
    u32char *end;

    while (u32cisspace(*str)) str++;
    if (*str == 0) return str;

    end = str + u32len(str) - 1;
    while (end > str && u32cisspace(*end)) end--;
    end[1] = U'\0';

    return str;
}

//TODO: check if string is long enough for slice indices
/**
 * @brief Get a slice of string
 * 
 * @param str String to get slice from
 * @param start Start position to slice
 * @param end End position to slice
 * @return Sliced portion of the string
 */
u32char *u32slice(u32char *str, size_t start, size_t end) {
    u32char *result = U"";

    while (start <= end) {
        u32char sbuf[2];
        sbuf[0] = str[start];
        sbuf[1] = U'\0';
        result = u32join(result, sbuf);

        start++;
    }

    return result;
}

//TODO: find a way better then u32join
/**
 * @brief Fill string with some other string
 * 
 * @param dest String to fill in
 * @param str String to fill with
 * @param amount Amount to fill
 * @return New filled string
 */
u32char *u32fill(u32char *dest, u32char *str, size_t amount) {
    for (size_t i = 0; i < amount; i++) {
        dest = u32join(dest, str);
    }

    return dest;
}

//TODO: better conversion functions like in STD strto... family
/**
 * @brief Convert string into decimal integer
 * 
 * @param str String to convert
 * @param base Number base
 * @return Converted integer
 */
int u32toint(u32char *str, int base) {
    size_t len = u32len(str);

    int result = 0;
    int i = 0;
    while (str[i] != U'\0') {
        int digit = str[i] - 48;
        result += digit * pow(base, len-i-1);
        i++;
    }

    return result;
}

/**
 * @brief Convert string into float
 * 
 * @param str String to convert
 * @return Converted float
 */
double u32tofloat(u32char *str) {
    double result;
    sscanf(utf32_to_ascii(str), "%lf", &result);
    return result;
}

/**
 * @brief Read file into multibyte UTF-8 encoded string
 * 
 * @param filepath Filepath of the file
 * @return Multibyte UTF-8 encoded string
 */
char *u8readfile(char *filepath) {
    FILE *f = fopen(filepath, "r");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *string = (char *)malloc(fsize + 1);
    fread(string, 1, fsize, f);
    fclose(f);

    string[fsize] = '\0';

    return string;
}

/**
 * @brief Read file into 4byte UTF-32 encoded string
 * 
 * @param filepath Filepath of the file
 * @return 4byte UTF-32 encoded string
 */
u32char *u32readfile(char *filepath) {
    return utf8_to_utf32(u8readfile(filepath));
}