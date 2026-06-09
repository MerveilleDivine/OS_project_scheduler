---

# CPU Scheduler Simulator

A C++ command-line simulator for comparing classic CPU scheduling algorithms and their process-level performance metrics.

This project began as an **Operating Systems / CPE351 course project** and is being improved into a more polished engineering portfolio project.

It implements multiple scheduling strategies, reads process data from a file, lets the user choose a scheduling method, and writes results to both the terminal and an optional output file.

---

## 📌 Project Objectives

The goal is to **simulate the service of jobs/processes by the CPU** in a multi-user system.

The simulator:

* Reads process data from a **text file** using `-f` or `--input`.
* Writes results to an **output file** using `-o` or `--output`.
* Allows the user to select a scheduling algorithm from the CLI.
* Computes and reports:

  * Start time per process
  * Completion time per process
  * Waiting time per process
  * Average waiting time
  * Turnaround time per process
  * Average turnaround time
  * Response time per process
  * Average response time

---

## ⚙️ Scheduling Methods Implemented

1. **None** — sequential execution in input order
2. **First Come, First Served (FCFS)** — non-preemptive
3. **Shortest Job First (SJF)** — non-preemptive and arrival-time aware
4. **Priority Scheduling** — non-preemptive and arrival-time aware
5. **Round Robin (RR)** — preemptive with user-provided quantum

> Priority Scheduling currently treats a **lower priority number** as higher priority.

---

## 📂 Input Format

Each process has three values separated by `:`:

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

A sample input file is included at:

```text
examples/sample_input.txt
```

---

## 🚀 How to Compile & Run

### Compile directly with g++

```bash
g++ -std=c++11 scheduler.cpp -o scheduler
```

### Run FCFS

```bash
./scheduler -f examples/sample_input.txt -o output.txt --algorithm fcfs
```

### Run Round Robin

```bash
./scheduler -f examples/sample_input.txt -o output.txt --algorithm rr --quantum 2
```

Supported algorithms:

```text
none, fcfs, sjf, priority, rr
```

You can also use the longer CLI form:

```bash
./scheduler --input examples/sample_input.txt --output output.txt --algorithm sjf
```

---

## 🧪 Sample Output

```text
Scheduling Method: First Come, First Served
Process   Arrival     Burst     Priority  Start     Complete    Waiting   Turnaround  Response
P1        0           5         3         0         5           0         5           0
P2        1           4         2         5         9           4         8           4
P3        1           3         1         9         12          8         11          8
P4        2           4         2         12        16          10        14          10
P5        3           3         1         16        19          13        16          13
Average Waiting Time: 7.00 ms
Average Turnaround Time: 11.00 ms
Average Response Time: 7.00 ms
```

---

## ✅ Testing

This repository includes CLI regression tests for the scheduler.

```bash
bash tests/run_cli_tests.sh
```

The tests compile the program and validate expected metrics for:

* FCFS
* SJF
* Priority Scheduling
* Round Robin
* Invalid Round Robin quantum handling
* Output file generation

---

## 🔁 Continuous Integration

The repository includes a GitHub Actions workflow that:

1. Checks out the repository.
2. Compiles the scheduler directly with `g++`.
3. Runs CLI regression tests.
4. Configures the project with CMake.
5. Builds with CMake.
6. Runs CTest.

---

## 🏗 Build with CMake

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

---

## 🛠 Tech & Concepts Used

* **C++11**
* **Data structures:**

  * Queues
  * Sorting algorithms
  * Ready-queue scheduling logic
* **Operating system concepts:**

  * CPU scheduling algorithms
  * Waiting time
  * Turnaround time
  * Response time
  * Completion time
  * Preemptive and non-preemptive scheduling
* **File I/O**
* **Command-line argument parsing**
* **CLI regression testing**
* **CMake**
* **GitHub Actions CI**

---

## 📚 Learning Outcomes

* Designed a C++ scheduling simulation around clear process execution metrics.
* Applied core OS scheduling concepts in a working command-line program.
* Implemented file parsing, CLI argument handling, and output report generation.
* Added regression tests to protect algorithm correctness.
* Added CI automation to make the repository easier to review and maintain.

---

## 🔮 Possible Extensions

* Add preemptive variants of SJF and Priority Scheduling.
* Visualize scheduling with Gantt charts.
* Export results in CSV/JSON for further analysis.
* Split the project into `src/`, `include/`, and `tests/` directories for a larger production-style structure.

---

## 📜 License

MIT License — free to use, adapt, and share.

---
