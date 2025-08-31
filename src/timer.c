#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "timer.h"
#include "log_session.h"

void drawProgressBar(int elapsed, int total, const char* label, const char* color) {
    int width = 30;
    int filled = (elapsed * width) / total;

    printf("\r%s%s: %02d:%02d [", color, label, (total - elapsed) / 60, (total - elapsed) % 60);
    for (int i = 0; i < width; i++) {
        if (i < filled) printf("#");
        else printf(" ");
    }
    printf("] %d%%\033[0m", (elapsed * 100) / total);
    fflush(stdout);
}


TimerResult start_timer(int minutes, int is_final_work) {
    int seconds = minutes * 60;
    int paused = 0;
    int skipped = 0;

    while (seconds > 0 && !skipped) {
        if (_kbhit()) {
            char ch = _getch();
            switch (ch) {
                case 'p':
                    paused = 1;
                    printf("\033[33m\nPaused. Press 'r' to resume.\033[0m");
                    break;
                case 'r':
                    if (paused) {
                        paused = 0;
                        printf("\033[32m\nResumed.\033[0m");
                    }
                    break;
                case 's':
                    skipped = 1;
                    printf("\033[34m\nSession skipped.\033[0m");
                    break;
                case 'q':
                    printf("\033[31m\nQuitting Pomodoro.\033[0m");
                    Sleep(2000);
                    exit(0);
            }
        }

        if (!paused && !skipped) {
            // printf("\r\033[32mTime left: %02d:%02d\033[0m", seconds / 60, seconds % 60);
            int elapsed = (minutes * 60) - seconds;
            const char* label = is_final_work ? "\033[31mFinal Work\033[0m" : (minutes <= 5 ? "Break" : "Work");
            const char* color = (minutes <= 5) ? "\033[33m" : "\033[32m"; // Yellow for break, green for work
            drawProgressBar(elapsed, minutes * 60, label, color);

            fflush(stdout);
            Sleep(1000);
            seconds--;
        } else {
            Sleep(500);
        }
    }

    if (skipped) {
        return is_final_work ? TIMER_QUIT : TIMER_SKIPPED;
    }
    else
    {
        printf("\n\033[31mTime's up!\033[0m\n");
        return TIMER_COMPLETE;
    }
}

void run_pomodoro(PomodoroConfig config) {
    for (int i = 0; i < config.cycles; i++) {
        int is_final = (i == config.cycles - 1);

        printf("\n\033[32mCycle %d - Work session for task \"%s\"\033[0m\n\n", i + 1, config.task_name);
        if (config.log) {
            log_session("Work", config.work_minutes, config.task_name);
        }
        int result = start_timer(config.work_minutes, is_final);
        if (result == TIMER_QUIT) {
            printf("\n\033[31mFinal work session skipped. Exiting Pomodoro.\033[0m\n");
            if (config.sound) {
                printf("\a");
            }
            Sleep(2000);
            break;
        }
        if (result == TIMER_COMPLETE && config.sound) {
            printf("\a");
        }

        printf("\n\033[33mCycle %d - Break session\033[0m\n", i + 1);
        if (config.log) {
            log_session("Break", config.break_minutes, config.task_name);
        }
        result = start_timer(config.break_minutes, 0);
        if (result == TIMER_QUIT) {
            printf("\n\033[31mUnexpected quit during break. Exiting Pomodoro.\033[0m\n");
            if (config.sound) {
                printf("\a");
            }
            Sleep(2000);
            break;
        }
        if (result == TIMER_COMPLETE && config.sound) {
            printf("\a");
        }
    }
}
