#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "input_utils.h"
// Prompts the user for input, parses it as a positive integer, or falls back to a default value
long parse_input(const char *prompt, int default_value) {
    char input[INPUT_SIZE];
    char *endptr;
    long value;

    printf("%s (default %d): ", prompt, default_value);
    if (fgets(input, INPUT_SIZE, stdin)) {
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) == 0) return default_value;

        value = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\0' || value <= 0) {
            printf("Invalid input. Using default: %d\n", default_value);
            return default_value;
        }

        return value;
    }

    return default_value;
}

void parse_string_input(const char *prompt, char *output, const char *default_value) {
    char input[INPUT_SIZE];

    printf("%s (default \"%s\"): ", prompt, default_value);

    if (fgets(input, INPUT_SIZE, stdin)) {
        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) {
            snprintf(output, INPUT_SIZE, "%s", default_value);
        } else {
            snprintf(output, INPUT_SIZE, "%s", input);
        }
    } else {
        // fgets failed, use default
        snprintf(output, INPUT_SIZE, "%s", default_value);
    }
}


int parse_yes_no_input(const char *prompt, int default_value) {
    char input[INPUT_SIZE];
    printf("%s (y/n, default %s): ", prompt, default_value ? "y" : "n");

    if (fgets(input, INPUT_SIZE, stdin)) {
        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0) return default_value;

        for (int i = 0; input[i]; i++) {
            input[i] = tolower((unsigned char)input[i]);
        }

        if (strcmp(input, "y") == 0 || strcmp(input, "yes") == 0) return 1;
        if (strcmp(input, "n") == 0 || strcmp(input, "no") == 0) return 0;

        printf("Invalid input. Using default: %s\n", default_value ? "yes" : "no");
        return default_value;
    }

    return default_value;
}
