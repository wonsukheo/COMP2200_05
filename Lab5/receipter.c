#include <time.h>
#include <stdio.h>

#include "receipter.h"

#define RECEIPT_WIDTH (50)
#define MESSAGE_MAX_LENGTH (75)
#define ITEM_MAX_PER_RECEIPT (10)

static char s_header_buffer[RECEIPT_WIDTH * 4 + 1];
static char* s_header_buffer_ptr = s_header_buffer;
static char s_item_buffer[(RECEIPT_WIDTH + 1) * ITEM_MAX_PER_RECEIPT];
static char* s_item_buffer_ptr = s_item_buffer;
static char s_pricing_buffer[RECEIPT_WIDTH * 5 + 1];
static char* s_pricing_buffer_ptr = s_pricing_buffer;
static char s_message_buffer[RECEIPT_WIDTH * 2 + 1];
static char* s_message_buffer_ptr = s_message_buffer;
static char s_ending_buffer[RECEIPT_WIDTH * 2 + 1];
static char* s_ending_buffer_ptr = s_ending_buffer;

static size_t s_order_number = 0u;
static size_t s_item_count = 0u;
static double s_order_subtotal = 0.00;
static double s_order_tip = 0.00;

int get_string_length(const char* str_or_null)
{
    const char* str_ptr = str_or_null;

    if (str_or_null == NULL) {
        return -1;
    }
    while (*str_ptr != '\0') {
        str_ptr++;
    }
    return str_ptr - str_or_null;    
}

void print_header(time_t timestamp)
{
    size_t i;
    char header_spacing[RECEIPT_WIDTH + 1];
    char timestamp_string[RECEIPT_WIDTH + 1]; 
    const char RESTAURANT_NAME[] = "Charles' Seafood";

    int year, month, day, hours, minutes, seconds;
    struct tm *gm = gmtime(&timestamp);
    year = gm->tm_year + 1900;
    month = gm->tm_mon + 1;
    day = gm->tm_mday;
    hours = gm->tm_hour;
    minutes = gm->tm_min;
    seconds = gm->tm_sec;
    sprintf(timestamp_string, "%d-%02d-%02d %02d:%02d:%02d", year, month, day, hours, minutes, seconds);

    for (i = 0; i < RECEIPT_WIDTH; i++) {
        header_spacing[i] = '-';
    }
    header_spacing[i] = '\0';

    sprintf(s_header_buffer_ptr, "%s\n", RESTAURANT_NAME);
    s_header_buffer_ptr += get_string_length(RESTAURANT_NAME) + 1;
    sprintf(s_header_buffer_ptr, "%s\n", header_spacing);
    s_header_buffer_ptr += RECEIPT_WIDTH + 1;
    sprintf(s_header_buffer_ptr, "%-44s %05d\n", timestamp_string, s_order_number);
    s_header_buffer_ptr += RECEIPT_WIDTH + 1;
    sprintf(s_header_buffer_ptr, "%s\n", header_spacing);
    s_header_buffer_ptr += RECEIPT_WIDTH + 1;
}

int add_item(const char* name, double price)
{    
    s_item_count++;
    if (s_item_count > 10) {
        fprintf(stderr, "maximum order items per receipt has reached!");
        return FALSE;
    }

    sprintf(s_item_buffer_ptr, "%32.25s %17.2f\n", name, price);    
    s_item_buffer_ptr += RECEIPT_WIDTH + 1;
    s_order_subtotal += price;

    return TRUE;
}

void set_tip(double tip)
{
    s_order_tip = tip;
}

int print_price(void)
{
    double order_tax = s_order_subtotal * 0.05f;

    if (s_item_count < 1) {
        fprintf(stderr, "no order items found");
        return FALSE;
    }

    sprintf(s_pricing_buffer_ptr, "\n");
    s_pricing_buffer_ptr++;
    sprintf(s_pricing_buffer_ptr, "%32.25s %17.2f\n", "Subtotal", s_order_subtotal);
    s_pricing_buffer_ptr += RECEIPT_WIDTH + 1;

    if (s_order_tip != 0.00f) {
        sprintf(s_pricing_buffer_ptr, "%32.25s %17.2f\n", "Tip", s_order_tip);
        s_pricing_buffer_ptr += RECEIPT_WIDTH + 1;
    }

    sprintf(s_pricing_buffer_ptr, "%32.25s %17.2f\n", "Tax", order_tax);
    s_pricing_buffer_ptr += RECEIPT_WIDTH + 1;
    sprintf(s_pricing_buffer_ptr, "%32.25s %17.2f\n", "Total", s_order_subtotal + s_order_tip + order_tax);
    s_pricing_buffer_ptr += RECEIPT_WIDTH + 1;
    sprintf(s_pricing_buffer_ptr, "\n");
    s_pricing_buffer_ptr++;
    
    return TRUE;
}

void set_message(const char* message)
{
    size_t message_length = get_string_length(message);
    
    if (message_length > RECEIPT_WIDTH) {
        sprintf(s_message_buffer_ptr, "%.50s\n", message);
        s_message_buffer_ptr += RECEIPT_WIDTH + 1;
        sprintf(s_message_buffer_ptr, "%.25s\n", message + 50);
        if (message_length > MESSAGE_MAX_LENGTH) {
            s_message_buffer_ptr += 25 + 1;
        } else {
            s_message_buffer_ptr += message_length - 50 + 1;
        }
    } else {
        sprintf(s_message_buffer_ptr, "%-s\n", message);
        s_message_buffer_ptr += message_length + 1;
    }        
}

void print_ending(void) 
{
    size_t i;
    char ending_spacing[RECEIPT_WIDTH + 1];
    for (i = 0; i < RECEIPT_WIDTH; i++) {
        ending_spacing[i] = '=';
    }
    ending_spacing[i] = '\0';

    sprintf(s_ending_buffer_ptr, "%s\n", ending_spacing);
    s_ending_buffer_ptr += RECEIPT_WIDTH + 1;
    
    sprintf(s_ending_buffer_ptr, "%50s", "Tax#-51234");
    s_ending_buffer_ptr += RECEIPT_WIDTH + 1;
    *s_ending_buffer_ptr = '\0';
}

void reset_receipt(void)
{
    s_header_buffer_ptr = s_header_buffer;
    s_item_buffer_ptr = s_item_buffer;
    s_pricing_buffer_ptr = s_pricing_buffer;
    s_message_buffer_ptr = s_message_buffer;
    s_ending_buffer_ptr = s_ending_buffer;
    
    s_item_count = 0u;
    s_order_subtotal = 0.00;
    s_order_tip = 0.00;
}

int print_receipt(const char* filename, time_t timestamp)
{
    FILE* stream;

    print_header(timestamp);
    print_price();
    print_ending();

    if (s_item_count <= 0) {
        reset_receipt();
        return FALSE;
    }

    stream = fopen(filename, "w+");
    if (stream == NULL) {
        perror("error while creating receipt file");
        return FALSE;
    }    
    fwrite(s_header_buffer, 1, s_header_buffer_ptr - s_header_buffer, stream);
    fwrite(s_item_buffer, 1, s_item_buffer_ptr - s_item_buffer, stream);
    fwrite(s_pricing_buffer, 1, s_pricing_buffer_ptr - s_pricing_buffer, stream);
    fwrite(s_message_buffer, 1, s_message_buffer_ptr - s_message_buffer, stream);
    fwrite(s_ending_buffer, 1, s_ending_buffer_ptr - s_ending_buffer, stream);
    fflush(stream);

    if (fclose(stream) != 0) {
        perror("error while closing");
    }

    s_order_number++;
    reset_receipt();

    return TRUE;
}
