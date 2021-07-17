/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#include <stdlib.h>
#include <ctype.h>
#include <wchar.h>
#include <io.h>
#include <fcntl.h>
#include <locale.h>
#include <stdio.h>

typedef wchar_t u8char;
typedef char byte;


void u8winterminal() {
    setmode(_fileno(stdout), _O_U8TEXT);
    setlocale(LC_ALL, "");
    system(" ");
}


size_t u8len(u8char *str) {
    return wcslen(str);
}

size_t u8size(u8char *str) {
    return wcslen(str) * sizeof(u8char);
}

u8char *u8join(u8char *str1, u8char *str2) {
    u8char *result = malloc(u8size(str1) + u8size(str2) + 1);
    wcscpy(result, str1);
    wcscat(result, str2);
    return result;
}

u8char *u8strip(u8char *str) {
    u8char *end;

    while (iswspace(*str)) str++;
    if (*str == 0) return str;

    end = str + wcslen(str) - 1;
    while (end > str && iswspace(*end)) end--;
    end[1] = L'\0';

    return str;
}

u8char *u8upper(u8char *str) {
    u8char *dstr = malloc(u8size(str)+1);
    wcscpy(dstr, str);
    int i = 0;
    while (i < wcslen(str)-1) {
        dstr[i] = toupper(str[i]);
        i++;
    }
    return dstr;
}

u8char *u8lower(u8char *str) {
    u8char *dstr = malloc(u8size(str)+1);
    wcscpy(dstr, str);
    int i = 0;
    while (i < wcslen(str)-1) {
        dstr[i] = tolower(str[i]);
        i++;
    }
    return dstr;
}

u8char *u8char_to_u8string(u8char *chr, u8char *dest) {
    dest[0] = *chr;
    dest[1] = L'\0';
    return dest;
}

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

byte u8contains(u8char *str, u8char *substr) {
    if (u8find(str, substr) == -1) {
        return 0;
    }
    else {
        return 1;
    }
}

byte u8startswith(u8char *str, u8char *substr) {
    if (u8find(str, substr) == 0) {
        return 1;
    } else {
        return 0;
    }
}

byte u8endswith(u8char *str, u8char *substr) {
    int index = u8find(str, substr);
    if (index == -1) {
        return 0;
    }
    else if (index == wcslen(str) - wcslen(substr)) {
        return 1;
    }
    else {
        return 0;
    }
}

byte u8isequal(u8char *str1, u8char *str2) {
    if (wcscmp(str1, str2) == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

byte u8isdigit(u8char *str) {
    int i = 0;

    while (i < wcslen(str)) {
        if (str[i] >= L'0' && str[i] <= L'9') {
            i++;
            continue;
        } else {
            return 0;
        }
    }

    return 1;
}

byte u8isidentifier(u8char *str) {
    int i = 0;

    if (str[i] == L'_' ||
       (str[i] >= L'a' && str[i] <= L'z') ||
       (str[i] >= L'A' && str[i] <= L'Z')) {
        i++;
    }
    else {
        return 0;
    }

    while (i < wcslen(str)) {
        if (str[i] == L'_' ||
           (str[i] >= L'0' && str[i] <= L'9') ||
           (str[i] >= L'a' && str[i] <= L'z') ||
           (str[i] >= L'A' && str[i] <= L'Z')) {
            i++;
            continue;
        } else {
            return 0;
        }
    }

    return 1;
}

byte u8isempty(u8char *str) {
    int l = wcslen(str);
    if (l == 0) {
        return 1;
    }
    else {
        int i = 0;
        while (i < l) {
            if (!iswspace(str[i])) {
                return 0;
            }
            i++;
        }
        return 1;
    }
}