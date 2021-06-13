#include <stdio.h>
#include <string.h>

#include "translate.h"

enum { BUFFER_LEN = 512
};

int translate(int argc, const char** argv)
{
    char set1[BUFFER_LEN];
    char set2[BUFFER_LEN];
    char* set1_ptr;
    char* set2_ptr;
    size_t set1_len;
    size_t set2_len;
    char c;
    char ESCAPE_CHAR_CMD[11] = { '\\', 'a', 'b', 'f', 'n', 'r', 't', 'v', '\'', '\"' };
    char ESCAPE_CHAR_C[11] = { '\\', '\a', '\b', '\f', '\n', '\r', '\t', '\v', '\'', '\"' };

    int flag = FALSE;
    ESCAPE_CHAR_CMD[10] = '\0';
    ESCAPE_CHAR_C[10] = '\0';
 

    if (argc == 3) {
        if (strlen(argv[1]) > 511 || strlen(argv[2]) > 511) {
            return -1;/*ERROR_CODE_ARGUMENT_TOO_LONG;*/
        } 
        strncpy(set1, argv[1], BUFFER_LEN);
        set1[511] = '\0';
        strncpy(set2, argv[2], BUFFER_LEN);
        set2[511] = '\0';
    } else if (argc == 4) {
        if (strlen(argv[2]) > 511 || strlen(argv[3]) > 511) {
            return -1;/*ERROR_CODE_ARGUMENT_TOO_LONG;*/
        } 
        strncpy(set1, argv[2], BUFFER_LEN);
        set1[511] = '\0';
        strncpy(set2, argv[3], BUFFER_LEN);
        set2[511] = '\0';
        flag = TRUE;
    }

    set1_len = strlen(set1);
    set2_len = strlen(set2);
     
    if (set1_len > set2_len) {
        size_t i;
        size_t len_diff = set1_len - set2_len;
        char last_c;

        set2_ptr = set2 + set2_len;
        last_c = *(set2_ptr - 1);
        
        for (i = 0; i < len_diff; i++) {
            *set2_ptr++ = last_c;
        }
        *set2_ptr = '\0';
    }
    
    set1_ptr = set1;

    while (*set1_ptr != '\0') {
        if (*set1_ptr == '\\') {
            char ch = *(set1_ptr + 1);
            char* escape_char_cmd_ptr = ESCAPE_CHAR_CMD;

            while (*escape_char_cmd_ptr != '\0') {
                if (*escape_char_cmd_ptr == ch) {
                    *set1_ptr = *(ESCAPE_CHAR_C + (escape_char_cmd_ptr - ESCAPE_CHAR_CMD));
                    strcpy(set1_ptr + 1, set1_ptr + 2);
                    goto success;
                }
                escape_char_cmd_ptr++;
            }
            return -1;
        }
    success:
        set1_ptr++;
    }
    
    set1_ptr = set1;
    while (*set1_ptr != '\0') {
        int difference;
        int i;
        if (*set1_ptr == '-') {
            if (set1_ptr == set1 || set1_ptr == set1 + set1_len - 1) {
                goto success2;
            }
            if (*(set1_ptr + 2) == '-') {
                if (*(set1_ptr + 1) == '-') {
                    strcpy(set1_ptr, set1_ptr + 2);
                    goto success2;
                }
            }  
            if (*(set1_ptr - 2) == '-') { 
                goto success2;
            }

            difference = *(set1_ptr + 1) - *(set1_ptr - 1);

            if (difference < 0) {
                return -1;
            } else if (difference == 0) {
                strcpy(set1_ptr - 1, set1_ptr + 1);
            } else {
                strcpy(set1_ptr + difference - 2, set1_ptr + 1);
                for (i = 0; i < difference - 1; i++) {
                    set1_ptr[i] = *set1_ptr - 1;
                }
            }                     
        }
    success2:
        set1_ptr++;
    }  

    c = getchar();
    while (c != EOF) {
        size_t index;
        if (flag == TRUE) {
            c |= ' ';
        }
        if (strrchr(set1, c) != NULL) {
            index = strrchr(set1, c) - set1;
            c = set2[index];       
        }
        putchar(c);
        c = getchar();
    }
    return 0;    
}
