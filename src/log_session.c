#include <stdio.h>
#include <time.h>

#define TIME_CHARS 64

// Logs a Pomodoro session with timestamp, session type, duration, and optional task description
void log_session(const char *session_type, int duration, const char *task) {
    
    // Open the log file in append mode so new entries are added to the end
    FILE *log_file = fopen("pomodoro_log.txt", "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        return;
    }

    // Get the current time and convert it to local time format
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // Format the local time into a readable string like "2025-08-30 14:30"
    char time_str[TIME_CHARS];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M", t);

    // Write the session details to the log file, including task if provided
    if (task != NULL && task[0] != '\0') {
        fprintf(log_file, "%s - %s session started (%d min) - Task: \"%s\"\n",
                time_str, session_type, duration, task);
    } else {
        fprintf(log_file, "%s - %s session started (%d min)\n",
                time_str, session_type, duration);
    }

    // Close the log file to ensure the data is saved properly
    fclose(log_file);
}
