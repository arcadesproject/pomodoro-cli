#ifndef INPUT_UTILS_H
#define INPUT_UTILS_H

#define INPUT_SIZE 16

long parse_input(const char *prompt, int default_value);

void parse_string_input(const char *prompt, char *output, const char *default_value);

int parse_yes_no_input(const char *prompt, int default_value);

#endif