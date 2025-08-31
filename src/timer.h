#ifndef TIMER_H
#define TIMER_H
#define TASK_INPUT_SIZE 128

// Duration structure for Pomodoro intervals
typedef struct {
    int work_minutes;
    int break_minutes;
    int cycles;
    int log;
    int sound;
    char task_name[TASK_INPUT_SIZE];
} PomodoroConfig;

// timer.h
typedef enum {
    TIMER_COMPLETE = 0,
    TIMER_SKIPPED = 1,
    TIMER_QUIT = 2
} TimerResult;


// Starts a timer for the given number of minutes
TimerResult start_timer(int minutes, int is_final_work);

// Runs the full Pomodoro session based on config
void run_pomodoro(PomodoroConfig config);

// Utility to display countdown (optional)
void display_countdown(int seconds);

#endif
