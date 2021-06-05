#ifndef RECEIPTER_H
#define RECEIPTER_H

#define TRUE (1)
#define FALSE (0)

int get_string_length(const char* str_or_null);

void print_header(time_t timestamp);

int add_item(const char* name, double price);

int print_price(void);

void set_tip(double tip);

void set_message(const char* message);

void print_ending(void);

void reset_receipt(void);

int print_receipt(const char* filename, time_t timestamp);

#endif /* RECEIPTER_H */
