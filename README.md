# Pomodoro Timer CLI

A command-line Pomodoro timer for Windows, implemented in C. Supports customizable work and break durations, multiple cycles, logging, task naming, and optional sound notifications. The application can run in the terminal or as a minimized GUI taskbar icon.

## Features

* Customizable work and break durations.
* Multiple Pomodoro cycles.
* Optional logging of sessions to `pomodoro_log.txt`.
* Custom task names for tracking.
* Sound notifications at the end of each session.
* Cross-terminal support (CMD, PowerShell, Explorer launches).
* Console window configuration (font, size) for better readability.
* Responsive to runtime controls:

  * `p` – Pause
  * `r` – Resume
  * `s` – Skip current session
  * `q` – Quit the application

## Requirements

* Windows 10 or later.
* C99 compatible compiler (MSVC or Clang recommended).
* CMake 3.10+ for building.
* Optional: PowerShell or CMD for running from terminal.

## Building the Application

1. Clone the repository:

```bash
git clone https://github.com/yourusername/pomodoro-cli.git
cd pomodoro-cli
```

2. Create a build directory and run CMake:

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

3. The executable will be located in `bin/pomodoro.exe`.

## Running the Application

Run the timer from a terminal:

```bash
bin\pomodoro.exe [work_minutes break_minutes cycles] [--log] [--task "Task Name"]
```

### Options

| Option          | Description                                     |
| --------------- | ----------------------------------------------- |
| `--log`         | Enable session logging to `pomodoro_log.txt`.   |
| `--task [name]` | Set a custom task name for logging and display. |
| `--silent`      | Disable sound notifications.                    |
| `--default`     | Run with default settings (25/5/4).             |
| `--help`        | Show usage instructions.                        |

### Example

```bash
bin\pomodoro.exe 50 10 3 --log --task "Writing"
```

## Controls During Timer

* `p` – Pause the timer.
* `r` – Resume if paused.
* `s` – Skip the current session.
* `q` – Quit the Pomodoro application.

## Logging

If logging is enabled, session details are written to `pomodoro_log.txt` in the following format:

```
YYYY-MM-DD HH:MM - Work session started (25 min) - Task: "Writing"
YYYY-MM-DD HH:MM - Break session started (5 min)
```

## Development Notes

* Built as a `WIN32` application to allow a hidden taskbar icon while running the CLI.
* Uses `AttachConsole` and `AllocConsole` to handle both terminal and Explorer launches.
* Input is handled safely with bounded buffers to prevent overflow.
* Compatible with Clang and MSVC; AddressSanitizer can be enabled during development for memory checking.
* CMake manages build targets and supports `clang-tidy` for static analysis.

## Recommended Tools

* **Compiler:** MSVC or Clang on Windows.
* **Static Analysis:** `clang-tidy`, `clang-analyzer`.
* **Memory Checking:** AddressSanitizer on Clang or `Dr. Memory` on Windows.
* **Build System:** CMake 3.10+.

## License

This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.