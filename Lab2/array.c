#include <stdio.h>
#include <limits.h>
#include "array.h"

int get_index_of(const int numbers[], const size_t element_count, const int num)
{
    size_t i;
    
    for (i = 0; i < element_count; i++) {
        if (numbers[i] == INT_MIN) {
            return -1;
        }
        if (numbers[i] == num) {
            return i;
        }
    }

    return -1;
}

int get_last_index_of(const int numbers[], const size_t element_count, const int num)
{
    int i;

    for (i = element_count - 1; i >= 0; i--) {
        if (numbers[i] == num) {
            return i;
        }
    }

    return -1;    
}

int get_max_index(const int numbers[], const size_t element_count)
{
    size_t i;
    int max = numbers[0];
    int index = 0;
    
    if (element_count == 0 || max == INT_MIN) {
        return -1;
    }

    for (i = 1; i < element_count - 1; i++) {
        if (numbers[i] > max) {
            index = i;
            max = numbers[i];
        }
    }
    
    return index;
}

int get_min_index(const int numbers[], const size_t element_count)
{
    size_t i;
    int min = numbers[0];
    int index = 0;
    
    if (element_count == 0 || min == INT_MIN) {
        return -1;
    }

    for (i = 1; i < element_count; i++) {
        if (numbers[i] < min) {
            index = i;
            min = numbers[i];
        }
    }
    
    return index;
}

int is_all_positive(const int numbers[], const size_t element_count)
{
    size_t i;
    
    if (numbers[0] == INT_MIN || element_count == 0) {
        return FALSE;
    }

    for (i = 0; i < element_count; i++) {
        if (numbers[i] < 0) {
            return FALSE;
        }
    }
    
    return TRUE;
}

int has_even(const int numbers[], const size_t element_count)
{
    size_t i;
 
    for (i = 0; i < element_count; i++) {
        if (numbers[i] % 2 == 0) {
            return TRUE;
        }
    }
    
    return FALSE;
}

int insert(int numbers[], const size_t element_count, const int num, const size_t pos)
{
    size_t i;
    
    if (pos > element_count) {
        return FALSE;
    }

    for (i = element_count; i > pos; i--) {
        numbers[i] = numbers[i - 1];
    }
    numbers[pos] = num;
    
    return TRUE;
} 

int remove_at(int numbers[], const size_t element_count, size_t index)
{
    size_t i;
    
    if (index >= element_count) {
        return FALSE;
    }

    for (i = index; i < element_count; i++) {
        numbers[i] = numbers[i + 1];
    }
    
    return TRUE;    
}
