---

CPU Scheduler Simulation

**Course Project — Operating Systems / CPE351**
This project is a complete simulation of CPU process scheduling algorithms, developed as part of a **C++ programming exam**.

It implements **multiple scheduling strategies** and computes key performance metrics, meeting the given specification for reading input from a file, allowing user-driven scheduling choice, and writing results to both screen and file.

---

## 📌 Project Objectives

The goal was to **simulate the service of jobs (processes) by the CPU** in a multi-user system.
The simulator:

* Reads process data from a **text file** (`-f` argument).
* Writes results to an **output file** (`-o` argument) and to the **screen**.
* Allows the user to select a scheduling method at runtime.
* Computes and reports:

  * Waiting time per process
  * Average waiting time
  * Turnaround time per process
  * Average turnaround time
  * Response time per process
  * Average response time

---

## ⚙️ Scheduling Methods Implemented

1. **None** (No scheduling, sequential execution)
2. **First Come, First Served (FCFS)** — Non-preemptive
3. **Shortest Job First (SJF)** — Non-preemptive
4. **Priority Scheduling** — Non-preemptive
5. **Round Robin (RR)** — User-provided quantum

---

## 📂 Input & Output Format

### **Input File Format** (`input.txt`):

Each process has three values separated by `:`:

```
burst_time:arrival_time:priority
```

Example:

```
5:0:3
4:1:2
3:1:1
4:2:2
3:3:1
```

---

### **Example Commands**

```bash
./scheduler -f examples/sample_input.txt -o output.txt --algorithm fcfs
./scheduler -f examples/sample_input.txt -o output.txt --algorithm rr --quantum 2
```

Supported algorithms:

```text
none, fcfs, sjf, priority, rr
```

---

### **Sample Screen Output**

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

## 🚀 How to Compile & Run

### **Compile directly with g++**

```bash
g++ -std=c++11 scheduler.cpp -o scheduler
```

### **Run**

```bash
./scheduler -f examples/sample_input.txt -o output.txt --algorithm fcfs
```

### **Build with CMake**

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
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

A GitHub Actions workflow also builds and tests the project automatically on pushes and pull requests.

---

## 🛠 Tech & Concepts Used

* **C++ (C++11 standard)**
* **Data Structures:**

  * Queues
  * Sorting algorithms
  * Ready-queue scheduling logic
* **Operating System Concepts:**

  * CPU Scheduling Algorithms
  * Turnaround, waiting, response, start, and completion time calculation
  * Preemptive vs Non-preemptive scheduling logic
* **File I/O in C++**
* **CLI regression testing**
* **GitHub Actions CI**

---

## 📚 Learning Outcomes

* Designed a **modular C++ scheduling simulation** around clear process execution metrics.
* Applied **core OS scheduling concepts** in a working command-line program.
* Implemented **file parsing**, **CLI argument handling**, and **output report generation**.
* Added **regression tests** to protect algorithm correctness.
* Added **CI automation** to make the repository easier to review and maintain.

---

## 🔮 Possible Extensions

* Add **Preemptive** variants of SJF and Priority scheduling.
* Visualize scheduling with **Gantt charts**.
* Export results in **CSV/JSON** for further analysis.
* Split the project into `src/`, `include/`, and `tests/` directories for a larger production-style structure.

---

## 📜 License

MIT License — free to use, adapt, and share.

---
