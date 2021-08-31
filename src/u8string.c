/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>


typedef wchar_t u8char;


#if defined(_WIN32)
#include <io.h>
#include <fcntl.h>
#include <locale.h>

void u8terminal() {
    setmode(_fileno(stdout), _O_U8TEXT);
    setlocale(LC_ALL, "en_US.UTF-8");
    system(" ");
}

#else

#include <locale.h>
void u8terminal() {
    setlocale(LC_ALL, "");
}

#endif


/*
  Returns length of the string

  u8char *str  ->  String
*/
size_t u8len(u8char *str) {
    return wcslen(str);
}

/*
  Returns size of the string in bytes

  u8char *str  ->  String
*/
size_t u8size(u8char *str) {
    return wcslen(str) * sizeof(u8char);
}

/*
  Joins two string and returns it

  u8char *str1  ->  String on the beginning
  u8char *str2  ->  String on the end
*/
u8char *u8join(u8char *str1, u8char *str2) {
    u8char *result = malloc(u8size(str1) + u8size(str2) + 1);
    wcscpy(result, str1);
    wcscat(result, str2);
    return result;
}

/*
  Trims leading and trailing whitespaces and returns it

  u8char *str  ->  String
*/
u8char *u8strip(u8char *str) {
    u8char *end;

    while (iswspace(*str)) str++;
    if (*str == 0) return str;

    end = str + wcslen(str) - 1;
    while (end > str && iswspace(*end)) end--;
    end[1] = L'\0';

    return str;
}

/*
  Returns uppercase version of string

  u8char *str  ->  String
*/
u8char *u8upper(u8char *str) {
    u8char *dstr = malloc(u8size(str)+1);
    wcscpy(dstr, str);
    int i = 0;
    while (i < wcslen(str)-1) {
        dstr[i] = towupper(str[i]);
        i++;
    }
    return dstr;
}

/*
  Returns lowercase version of string

  u8char *str  ->  String
*/
u8char *u8lower(u8char *str) {
    u8char *dstr = malloc(u8size(str)+1);
    wcscpy(dstr, str);
    int i = 0;
    while (i < wcslen(str)-1) {
        dstr[i] = towlower(str[i]);
        i++;
    }
    return dstr;
}

/*
  Returns a null-terminated string from single character

  u8char *chr   ->  Character
  u8char *dest  ->  Empty string buffer to fill in
*/
u8char *u8char_to_u8string(u8char *chr, u8char *dest) {
    dest[0] = *chr;
    dest[1] = L'\0';
    return dest;
}

/*
  Finds index of a substring's first occurence

  u8char *str     ->  String
  u8char *substr  ->  Substring to search
*/
int u8find(u8char *str, u8char *substr) {
    int i = 0;
    int j = 0;
    int l = wcslen(substr);
    char f = 0;
    
    while (i < wcslen(str)) {
        j = 0;
        while (j < l) {
            if (str[i+j] == substr[j]) {
                f = 1;
            }
            else {
                f = 0;
                break;
            }
            j++;
        }
        if (f == 1) {
            return i;
        }
        i++;
    }

    return -1;
}

/*
  Checks whether string contains the substring

  u8char *str     ->  String
  u8char *substr  ->  Substring to check
*/
bool u8contains(u8char *str, u8char *substr) {
    if (u8find(str, substr) == -1) {
        return false;
    }
    else {
        return true;
    }
}

/*
  Checks if string starting with substring

  u8char *str     ->  String
  u8char *substr  ->  Substring to check
*/
bool u8startswith(u8char *str, u8char *substr) {
    if (u8find(str, substr) == 0) {
        return true;
    } else {
        return false;
    }
}

/*
  Checks if string starting with substring

  u8char *str     ->  String
  u8char *substr  ->  Substring to check
*/
bool u8endswith(u8char *str, u8char *substr) {
    int index = u8find(str, substr);
    if (index == -1) {
        return false;
    }
    else if (index == wcslen(str) - wcslen(substr)) {
        return true;
    }
    else {
        return false;
    }
}

/*
  Checks if two strings are the same

  u8char *str1  ->  First string
  u8char *str2  ->  Second string
*/
bool u8isequal(u8char *str1, u8char *str2) {
    if (wcscmp(str1, str2) == 0) {
        return true;
    }
    else {
        return false;
    }
}

/*
  Checks if all characters of the strings are digits

  u8char *str  ->  String
*/
bool u8isdigit(u8char *str) {
    int i = 0;

    while (i < wcslen(str)) {
        if (str[i] >= L'0' && str[i] <= L'9') {
            i++;
            continue;
        } else {
            return false;
        }
    }

    return true;
}

/*
  Checks if string is an identifier

  u8char *str  ->  String
*/
bool u8isidentifier(u8char *str) {
    int i = 0;

    if (str[i] == L'_' || iswalnum(str[i]) != 0) {
        i++;
    }
    else {
        return false;
    }

    while (i < wcslen(str)) {
        if (str[i] == L'_' || iswalnum(str[i]) != 0) {
            i++;
        } else {
            return false;
        }
    }

    return true;
}

/*
  Checks if string is empty
  (or containing only whitespaces)

  u8char *str  ->  String
*/
bool u8isempty(u8char *str) {
    int l = wcslen(str);
    if (l == 0) {
        return true;
    }
    else {
        int i = 0;
        while (i < l) {
            if (!iswspace(str[i])) {
                return false;
            }
            i++;
        }
        return true;
    }
}

/*
  Fill string with some other string

  u8char *dest  ->  String to fill in
  u8char *str   ->  String to fill with
  int amount    ->  Amount to fill
*/
u8char *u8fill(u8char *dest, u8char *str, int amount) {
    int i = 0;
    for (i = 0; i < amount; i++) {
        dest = u8join(dest, str);
    }

    return dest;
}

/*
  Replace every occurence of one substring
  with another

  u8char *str  ->  String to replace on
  u8char *old  ->  Old string to replace
  u8char *new  ->  New string to replace with
*/
u8char *u8replace(u8char *str, u8char *old, u8char *new) {
    u8char *result;
    int i, cnt = 0;
    int newlen = wcslen(new);
    int oldlen = wcslen(old);
  
    for (i = 0; str[i] != '\0'; i++) {
        if (wcsstr(&str[i], old) == &str[i]) {
            cnt++;
            i += oldlen - 1;
        }
    }
  
    result = (u8char *)malloc((i + cnt * (newlen - oldlen) + 1)*sizeof(u8char));

    i = 0;
    while (*str) {
        if (wcsstr(str, old) == str) {
            wcscpy(&result[i], new);
            i += newlen;
            str += oldlen;
        }
        else
            result[i++] = *str++;
    }
  
    result[i] = '\0';
    return result;
}

/*
  Get a slice of string

  u8char *str  ->  String
  int start    ->  Start position to slice
  int end      ->  End position to slice
*/
u8char *u8slice(u8char *str, int start, int end) {
    //TODO: check if string is long enough for slice indices
    u8char *result = L"";

    while (start <= end) {
        u8char sbuf[2];
        sbuf[0] = str[start];
        sbuf[1] = L'\0';
        result = u8join(result, sbuf);

        start++;
    }

    return result;
}

/*
  Converts narrow string to unicode string

  char *nstr  ->  Narrow string to convert
*/
u8char *ctou8(char *nstr) {
    size_t len = strlen(nstr) * sizeof(u8char);
    u8char *ws = (u8char *)malloc(len);
    swprintf(ws, len, L"%hs", nstr);
    return ws;
}