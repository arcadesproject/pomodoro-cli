#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "timer.h"
#include "input_utils.h"

#define DEFAULT_WORK 25
#define DEFAULT_BREAK 5
#define DEFAULT_CYCLES 4
#define DEFAULT_LOG 0
#define DEFAULT_SOUND 1
#define DEFAULT_TASK_NAME "Session"
#define APP_COLS 88
#define APP_ROWS 32

#define IDI_MAINICON 101

// ---------------- Console helpers ----------------
void disableCursorBlink() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void enableCursorBlink() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// ---------------- Hidden window proc ----------------
LRESULT CALLBACK HiddenWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// ---------------- CLI program ----------------
int real_main(int argc, char *argv[]) {
    SetConsoleTitle("Pomodoro Timer");
    disableCursorBlink();

    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        printf("Pomodoro Timer - Focus and Break Cycles\n");
        printf("Usage:\n");
        printf("  pomodoro [work_minutes break_minutes cycles] [--log] [--task \"Task Name\"]\n");
        printf("Options:\n");
        printf("  --log           Enable session logging to pomodoro_log.txt\n");
        printf("  --task [name]   Set a custom task name for logging and display\n");
        printf("  --silent        Run without sound\n");
        printf("  --default       Run with default settings (25/5/4)\n");
        printf("  --help          Show this help message\n");
        printf("Controls during timer:\n");
        printf("  p               Pause the timer\n");
        printf("  r               Resume if paused\n");
        printf("  s               Skip the current part\n");
        printf("  q               Quit the entire Pomodoro program\n");
        printf("Examples:\n");
        printf("  pomodoro 50 10 3 --log --task \"Writing\"\n");
        return 0;
    }

    system("cls");

    printf("\033[31m ____   __   _  _   __  ____   __  ____   __  \033[0m\n");
    printf("\033[31m(  _ \\ /  \\ ( \\/ ) /  \\(    \\ /  \\(  _ \\ /  \\ \033[0m\n");
    printf("\033[31m ) __/(  O )/ \\/ \\(  O )) D ((  O ))   /(  O )\033[0m\n");
    printf("\033[31m(__)   \\__/ \\_)(_/ \\__/(____/ \\__/(__\\_) \\__/ \033[0m\n");

    PomodoroConfig config = {
        DEFAULT_WORK, DEFAULT_BREAK, DEFAULT_CYCLES,
        DEFAULT_LOG, DEFAULT_SOUND, DEFAULT_TASK_NAME
    };
    
    if (argc == 2 && strcmp(argv[1], "--default") == 0) {
        run_pomodoro(config);
        return 0;
    }
    
    int has_durations = 0;
    for (int i = 1; i < argc - 2; i++) {
        if (isdigit((unsigned char)argv[i][0]) &&
            isdigit((unsigned char)argv[i+1][0]) &&
            isdigit((unsigned char)argv[i+2][0])) {
            config.work_minutes = atoi(argv[i]);
            config.break_minutes = atoi(argv[i+1]);
            config.cycles = atoi(argv[i+2]);
            has_durations = 1;
            i += 2;
        } 
        else if (strcmp(argv[i], "--log") == 0) {
            config.log = 1;
        } 
        else if (strcmp(argv[i], "--task") == 0 && i + 1 < argc) {
            snprintf(config.task_name, sizeof(config.task_name), "%s", argv[i + 1]);
            i++;
        } 
        else if (strcmp(argv[i], "--task") == 0) {
            fprintf(stderr, "Error: --task requires a name argument\n");
        }
        else if (strcmp(argv[i], "--silent") == 0) {
            config.sound = 0;
        }
    }

    if (!has_durations) {
        printf("\n");
        config.work_minutes = parse_input("Enter work duration in minutes", DEFAULT_WORK);
        config.break_minutes = parse_input("Enter break duration in minutes", DEFAULT_BREAK);
        config.cycles = parse_input("Enter number of cycles", DEFAULT_CYCLES);
        config.log = parse_yes_no_input("Enable logging?", DEFAULT_LOG);
        config.sound = parse_yes_no_input("Enable sound?", DEFAULT_SOUND);
        parse_string_input("Enter task name", config.task_name, DEFAULT_TASK_NAME);
    }

    printf("\033[34mStarting Pomodoro: %d min work, %d min break, %d cycles | Task: \"%s\"\033[0m\n",
        config.work_minutes, config.break_minutes, config.cycles, config.task_name);

    run_pomodoro(config);
    enableCursorBlink();
    return 0;
}

void configure_console(int cols, int rows) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Step 1: shrink the window to 1x1 temporarily
    SMALL_RECT sr;
    sr.Left = 0;
    sr.Top = 0;
    sr.Right = 1;
    sr.Bottom = 1;
    SetConsoleWindowInfo(hConsole, TRUE, &sr);

    // Step 2: set buffer size exactly what you want
    COORD bufferSize = { (SHORT)cols, (SHORT)rows };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    // Step 3: resize window to desired size
    sr.Right = cols - 1;
    sr.Bottom = rows - 1;
    SetConsoleWindowInfo(hConsole, TRUE, &sr);
}

void set_console_font(int fontWidth, int fontHeight, const wchar_t* fontName) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };

    GetCurrentConsoleFontEx(hConsole, FALSE, &cfi); // get current settings
    cfi.dwFontSize.X = fontWidth;  // width in pixels
    cfi.dwFontSize.Y = fontHeight; // height in pixels
    wcscpy_s(cfi.FaceName, 32, fontName); // font name, e.g., L"Consolas"
    
    SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
}


// ---------------- Entry point ----------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {

    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
        freopen_s(&fp, "CONIN$",  "r", stdin);
    } else {
        AllocConsole();
        configure_console(APP_COLS, APP_ROWS);
        set_console_font(0, 17, L"Consolas");
    }


    // Register hidden window for taskbar icon
    WNDCLASS wc = {0};
    wc.lpfnWndProc   = HiddenWndProc;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.lpszClassName = "PomodoroHiddenClass";
    wc.hIcon         = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAINICON));
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        wc.lpszClassName,
        "Pomodoro Timer",      // window title (appears on hover)
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, wc.hInstance, NULL
    );

    ShowWindow(hwnd, SW_SHOWMINNOACTIVE); // show minimized, so it appears in taskbar

    // Run CLI logic
    int ret =  real_main(__argc, __argv); // forward command-line args

    // Destroy hidden window
    DestroyWindow(hwnd);
    return ret;
}
