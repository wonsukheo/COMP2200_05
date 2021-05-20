#include <assert.h>

#define NULL ((void*)0)

const char* get_longest_safe_zone_or_null(const char* const cab_start_location, const size_t cab_length, const char* const cluster_start_locations[], const size_t cluster_lengths[], const size_t cluster_count, size_t* out_longest_safe_area_length)
{
    const char* cab_travel_ptr = cab_start_location;
    const char* longest_safe_zone_start_ptr = NULL;
    size_t longest_safe_zone_length = 0;
    size_t current_safe_zone_length = 0;
    size_t overlapped_cluster_count = 0;
    size_t i;
    size_t j;

    if (cab_length == 0) {
        out_longest_safe_area_length = 0;
        return NULL;
    }
    
    if (cluster_count == 0) {
        *out_longest_safe_area_length = cab_length;
        return cab_start_location;
    }
    
    for (i = 0; i <= cab_length; i++) {
        for (j = 0; j < cluster_count; j++) {
            if (cab_travel_ptr == cluster_start_locations[j]) {
                overlapped_cluster_count++;
            }
            if (cab_travel_ptr == cluster_start_locations[j] + cluster_lengths[j]) {
                overlapped_cluster_count--;
            }
        }

        if ((overlapped_cluster_count & 1) != 0 || i == cab_length) {
            if (current_safe_zone_length >= longest_safe_zone_length) {
                longest_safe_zone_length = current_safe_zone_length;
                longest_safe_zone_start_ptr = cab_travel_ptr - longest_safe_zone_length;
            }
            current_safe_zone_length = 0;
        } else {
            current_safe_zone_length++;
        }
        cab_travel_ptr++;
    }

    *out_longest_safe_area_length = longest_safe_zone_length;
    return longest_safe_zone_start_ptr;
}

int get_travel_time(const char* const cab_start_location, const size_t cab_length, const char* const cluster_start_locations[], const size_t cluster_lengths[], const size_t cluster_count)
{
    const char* cab_travel_ptr = cab_start_location;
    size_t safe_zone = 0;
    size_t not_safe_zone = 0;
    const double SAFE_ZONE_TRAVEL_SPEED_MIN_PER_BYTE = 0.1;
    const double NOT_SAFE_ZONE_TRAVEL_SPEED_MIN_PER_BYTE = 0.2; 
    double travel_time = 0.0;
    size_t overlapped_cluster_count = 0;
    size_t i;
    size_t j;	
    
    if (cluster_count == 0) {
        return (int)(SAFE_ZONE_TRAVEL_SPEED_MIN_PER_BYTE * cab_length);
    }
    if (cab_length == 0) {
        return -1;
    }

    for (i = 0; i < cab_length; i++) {
        for(j = 0; j < cluster_count; j++) {
            if (cab_travel_ptr == cluster_start_locations[j]) {
                overlapped_cluster_count++;
            }
            if (cab_travel_ptr == cluster_start_locations[j] + cluster_lengths[j]) {
                overlapped_cluster_count--;
            }
        }
        
        if ((overlapped_cluster_count & 1) == 0) {
            safe_zone++;
        } else {
            not_safe_zone++;
        }
       
        cab_travel_ptr++;
    }

    assert(safe_zone + not_safe_zone == cab_length);

    travel_time = (safe_zone * SAFE_ZONE_TRAVEL_SPEED_MIN_PER_BYTE + not_safe_zone * NOT_SAFE_ZONE_TRAVEL_SPEED_MIN_PER_BYTE);
    return (int)((travel_time + 0.5) * 10) / 10;
}
