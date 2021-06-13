#include <stdio.h>
#include <string.h>

#include "translate.h"

#define LENGTH (512)

int do_magic(int argc, char* set1, char* set2, const char** argv, int flag)
{   
    char escape_char_cmd[10] = { '\\', 'a', 'b', 'f', 'n', 'r', 't', 'v', '\'', '\"' };
    char escape_char_c[10] = { '\\', '\a', '\b', '\f', '\n', '\r', '\t', '\v', '\'', '\"' };
    size_t set1_length;
    size_t set2_length;
    size_t i;
    size_t j;

    /* error code return */
    if (flag == 0) {
        if (argc != 3) {
            fprintf(stdout, "%s", "ERROR_CODE_WRONG_ARGUMENTS_NUMBER"); 
            return 1;
        }
        if (strlen(argv[1]) > LENGTH || strlen(argv[2]) > LENGTH) {
            fprintf(stdout, "%s", "ERROR_CODE_ARGUMENT_TOO_LONG");
            return 4;
        }

        strncpy(set1, argv[1], LENGTH);
        set1[LENGTH] = '\0';
        strncpy(set2, argv[2], LENGTH);
        set2[LENGTH] = '\0';
    } else if (flag == 1) {
        if (argc != 4) {
            fprintf(stdout, "%s", "ERROR_CODE_WRONG_ARGUMENTS_NUMBER"); 
            return 1;
        }
        if (strlen(argv[2]) > LENGTH || strlen(argv[3]) > LENGTH) {
            fprintf(stdout, "%s", "ERROR_CODE_ARGUMENT_TOO_LONG");
            return 4;
        }

        strncpy(set1, argv[2], LENGTH);
        set1[LENGTH] = '\0';
        strncpy(set2, argv[3], LENGTH);
        set2[LENGTH] = '\0';
    }    
    set1_length = strlen(set1);
    set2_length = strlen(set2);

    /* check escape char */
    for (i = 0; i < set1_length; ++i) {
        if (set1[i] == '\\') {
            for (j = 0; j < 10; ++j) {
                if (set1[i + 1] == escape_char_cmd[j]) {
                    set1[i] = escape_char_c[j];
                        
                    strcpy(&set1[i + 1], &set1[i + 2]);
                    break;
                } 
            }   
            if (j == 10) {
                fprintf(stdout, "%s", "ERROR_CODE_INVALID_FORMAT");
                return 3;    
            }
        }
    }
    for (i = 0; i < set2_length; ++i) {
        if (set2[i] == '\\') {
            for (j = 0; j < 10; ++j) {
                if (set2[i + 1] == escape_char_cmd[j]) {
                    set2[i] = escape_char_c[j];
                    strcpy(&set2[i + 1], &set2[i + 2]);
                    break;
                }
            }    
         
            if (j == 10) {
                fprintf(stdout, "%s", "ERROR_CODE_INVALID_FORMAT");
                return 3;    
            }
        }
    }  
    /* char - char */
    for (i = 1; i < set1_length - 1; ++i) {
        if (set1[i] == '-') {
            if (set1[i - 1] != set1[i + 1]) {
                int difference_ascii = (int)set1[i + 1] - (int)set1[i - 1];	
                char temp_array[511];          
                strcpy(temp_array, &set1[i + 2]);            
                
                if (difference_ascii < 0) {
                    fprintf(stdout, "%s", "ERROR_CODE_INVALID_RANGE");
                    return 5;
                }
                if (set1_length + difference_ascii - 2 > 511) {
                    fprintf(stdout, "%s", "ERROR_CODE_ARGUMENT_TOO_LONG");
                    return 4;
                }
                for (j = i; j < difference_ascii + i; ++j) {
                    set1[j] = (char)((int)set1[j - 1] + 1);
                }
                
                strcpy(&set1[i + difference_ascii], temp_array);
                i += difference_ascii;
                set1_length = strlen(set1);
            }
            if (set1[i + 1] == '-' && set1[i + 2] == '-') {
                strcpy(&set1[i], &set1[i + 2]);
                set1[set1_length - 1] = '\0';
                set1[set1_length - 2] = '\0';
            }
            if (set1[i - 1] == set1[i + 1]) {
                strcpy(&set1[i - 1], &set1[i + 1]);
                set1[set1_length - 1] = '\0';
                set1[set1_length - 2] = '\0';            
            }
        }    
    }    
    for (i = 1; i < set2_length - 1; ++i) {
        if (set2[i] == '-') {
            if (set2[i - 1] != set2[i + 1]) {
                int difference_ascii = (int)set2[i + 1] - (int)set2[i - 1];	
                char temp_array[511];
           
                strcpy(temp_array, &set2[i + 2]);
            
                if (difference_ascii < 0) {
                    fprintf(stdout, "%s", "ERROR_CODE_INVALID_RANGE");
                    return 5;
                }
                if (set1_length + difference_ascii - 2 > 511) {
                    fprintf(stdout, "%s", "ERROR_CODE_ARGUMENT_TOO_LONG");
                    return 4;
                }
                for (j = i; j < difference_ascii + i; ++j) {
                    set2[j] = (char)((int)set2[j - 1] + 1);
                }
                strcpy(&set2[i + difference_ascii], temp_array);
                i += difference_ascii;
                set2_length = strlen(set1);
            }
            if (set2[i + 1] == '-' && set2[i + 2] == '-') {
                strcpy(&set2[i], &set2[i + 2]);
                set2[set2_length - 1] = '\0';
                set2[set2_length - 2] = '\0';
            }
            if (set2[i - 1] == set2[i + 1]) {
                strcpy(&set2[i - 1], &set2[i + 1]);
                set2[set2_length - 1] = '\0';
                set2[set2_length - 2] = '\0';            
            }           
        }
    }
    
    /* extend set2_len */
    set1_length = strlen(set1);
    set2_length = strlen(set2);
    if (set1_length > set2_length) {
        size_t i;
        for (i = set2_length; i < set1_length; ++i) {
            set2[i] = set2[i - 1];
        }
        set2[set1_length] = '\0';
    }        

    return 0;  
}

int translate(int argc, const char** argv) 
{
    char set1[LENGTH];
    char set2[LENGTH];
    char* set1_ptr = set1;
    int c;
    int count = 0;
    int flag = 0;
    int errormessage;
    if (argc == 4) {
        if (argv[1][0] == '-') {
            if (argv[1][1] == 'i') {
                flag = 1;
            } else if (argv[1][1] != 'i') {
                fprintf(stdout, "%s", "ERROR_CODE_INVALID_FLAG");
                return 2;
            }
        }
    } 
    switch (flag) {
    case 0: { 
        errormessage = do_magic(argc, set1, set2, argv, flag);
        if (errormessage != 0) {
            return errormessage;
        }
        /* translate code */
 
        while (TRUE) {
            c = getchar();
            if (c == EOF) {
                break;
            }
            set1_ptr = strrchr(set1, c);
            if (set1_ptr != NULL) {
                c = set2[set1_ptr - set1];
            }
            count++;
            putchar(c);
        }
        return errormessage;
    } break;

    case 1: {
        errormessage = do_magic(argc, set1, set2, argv, flag);
        if (errormessage != 0) {
            return errormessage;
        }
        /* translate code */
        while (*set1_ptr != '\0') {
            *set1_ptr |= 32;
            set1_ptr++;
        }
        while (TRUE) {
            c = getchar();
            if (c == EOF) {
                break;
            }
            c |= 32;
            set1_ptr = strrchr(set1, c);
            if (set1_ptr != NULL) {
                c = set2[set1_ptr - set1];
            }
            count++;
            putchar(c);
        }       
        return errormessage;
    } break;
    
    default: 
        break;
    }

    return 0;
}
