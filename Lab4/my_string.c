#include "my_string.h"

#define NULL ((void*)0)

static char* s_original_str = NULL;

size_t get_str_len(const char* str)
{
    const char* ptr = str;
    while (*ptr++ != '\0') {
    }
    
    return ptr - str - 1;
}

void reverse(char* str)
{
    size_t str_len = get_str_len(str);
    char* ptr = str + str_len - 1;
    
    char temp;
    size_t i = 0;
    while (i++ < str_len / 2) {
        temp = *str;
        *str = *ptr;
        *ptr = temp;
        
        str++;
        ptr--;
    }        
}

int index_of(const char* str, const char* word)
{
    const char* str_ptr = str;
    const char* word_ptr = word;
    size_t word_len = get_str_len(word);
    size_t count = 0;

    if (word_len == 0) {
        return 0;
    }
    while (*str_ptr != '\0') {
        if (*str_ptr == *word_ptr) {
            count++;
            word_ptr++;
            str_ptr++;
        } else { 
            count = 0;
            word_ptr = word;

            if (*str_ptr++ == *word_ptr) {
                count++;
                word_ptr++;
            }
        }
        
        if (count == word_len) {
            return str_ptr - word_len - str;   
        }
    }
    
    return -1;     
}

void reverse_by_words(char* str)
{
    char* str_ptr = str;
    char* word_ptr = str;

    while (*str_ptr != '\0') {
        if (*str_ptr == ' ') {
            *str_ptr = '\0';
            reverse(word_ptr);
         
            *str_ptr = ' ';
            str_ptr++;
            word_ptr = str_ptr;
        } else {
            str_ptr++;
        }
   }
   reverse(word_ptr);
}

char* tokenize(char* str_or_null, const char* delims)
{
    char* str_ptr = str_or_null;
    size_t delims_count = get_str_len(delims);
    size_t str_ptr_count = 0;

    if (str_or_null == NULL) {
        str_ptr = s_original_str;
    }
    /*
    if (str_ptr == NULL && s_original_str == NULL) {
        return NULL;
    }
    */
    while (*str_ptr != '\0') {
        size_t i;
        for (i = 0; i < delims_count; i++) {
            if (*str_ptr == delims[i]) {
                *str_ptr = '\0';
                s_original_str = str_ptr + 1;

                if (str_ptr_count == 0) {
                    return tokenize(NULL, delims);
                }
                return str_ptr - str_ptr_count;
            }
        }
        str_ptr_count++;
        str_ptr++;
    }
    if (*str_ptr == '\0' && str_ptr_count != 0) {
        return str_ptr - str_ptr_count;
    }
    return NULL;
}

char* reverse_tokenize(char* str_or_null, const char* delims)
{
    char* str_ptr = str_or_null;
    size_t delims_count = get_str_len(delims);
    size_t str_ptr_count = 0;

    if (str_or_null == NULL) {
        str_ptr = s_original_str;
    }
    /*
    if (str_ptr == NULL && s_original_str == NULL) {
        return NULL;
    }
    */
    while (*str_ptr != '\0') {
        size_t i;
        for (i = 0; i < delims_count; i++) {
            if (*str_ptr == delims[i]) {
                *str_ptr = '\0';
                s_original_str = str_ptr + 1;
                reverse(str_ptr - str_ptr_count);

                if (str_ptr_count == 0) {
                    return reverse_tokenize(NULL, delims);
                }
                return str_ptr - str_ptr_count;
            }
        }
        str_ptr_count++;
        str_ptr++;
    }
    if (*str_ptr == '\0' && str_ptr_count != 0) {
        reverse(str_ptr - str_ptr_count);
        return str_ptr - str_ptr_count;
    }
    return NULL;       
}
