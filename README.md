# CPU Scheduler Simulator

![C++ CI](https://github.com/MerveilleDivine/OS_project_scheduler/actions/workflows/cpp-ci.yml/badge.svg)

A C++ command-line simulator for comparing classic CPU scheduling algorithms and process-level performance metrics.

This project began as an Operating Systems course project and has been refactored into a clearer C++ application with a structured source layout, CLI regression tests, CMake support, and GitHub Actions CI.

## What it does

The simulator reads process data from a text file, runs a selected CPU scheduling algorithm, and reports process timing metrics. It also supports comparison mode for evaluating several algorithms against the same input.

Implemented algorithms:

- No scheduling, sequential execution in input order
- First Come, First Served
- Shortest Job First, non-preemptive
- Priority Scheduling, non-preemptive
- Round Robin with user-provided quantum

Priority Scheduling currently treats a lower priority number as higher priority.

## Metrics reported

- Start time
- Completion time
- Waiting time
- Turnaround time
- Response time
- Average waiting time
- Average turnaround time
- Average response time
- Gantt chart segments

## Project structure

```text
OS_project_scheduler/
├── include/
├── src/
├── tests/
├── examples/
├── .github/workflows/
├── CMakeLists.txt
└── README.md
```

## Input format

Each process has three values separated by colons:

```text
burst_time:arrival_time:priority
```

Example:

```text
5:0:3
4:1:2
3:1:1
4:2:2
3:3:1
```

A sample input file is included in `examples/sample_input.txt`.

## Build and run

Compile directly with g++:

```bash
g++ -std=c++11 src/main.cpp src/Parser.cpp src/Scheduler.cpp src/Reporter.cpp -Iinclude -o scheduler
```

Run FCFS:

```bash
./scheduler -f examples/sample_input.txt -o output.txt --algorithm fcfs
```

Run Round Robin:

```bash
./scheduler -f examples/sample_input.txt -o output.txt --algorithm rr --quantum 2
```

Run comparison mode:

```bash
./scheduler -f examples/sample_input.txt --compare --quantum 2
```

Supported algorithm values:

```text
none, fcfs, sjf, priority, rr
```

## Testing

Run the CLI regression tests:

```bash
bash tests/run_cli_tests.sh
```

The tests cover the implemented algorithms, comparison mode, invalid quantum handling, and output file generation.

## Build with CMake

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Concepts used

- C++11
- File I/O
- Command-line argument parsing
- Queues and sorting
- Ready-queue scheduling logic
- CPU scheduling metrics
- CLI regression testing
- CMake
- GitHub Actions CI

## Possible extensions

- Add preemptive variants of SJF and Priority Scheduling.
- Export results in CSV or JSON.
- Add richer Gantt chart visualization.
- Add unit tests for scheduler functions in addition to CLI regression tests.

## License

MIT License.
