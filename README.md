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

### **Example Command**

```bash
./scheduler -f input.txt -o output.txt
```

---

### **Sample Screen Output**

```
Scheduling Method: First Come, First Served
Process Waiting Times:
P1: 0 ms
P2: 4 ms
P3: 8 ms
P4: 10 ms
P5: 13 ms
Average Waiting Time: 7.0 ms

Process Turnaround Times:
P1: 5 ms
P2: 9 ms
P3: 11 ms
P4: 14 ms
P5: 16 ms
Average Turnaround Time: 11.0 ms
```

---

## 🚀 How to Compile & Run

### **Compile**

```bash
g++ -std=c++11 scheduler.cpp -o scheduler
```

### **Run**

```bash
./scheduler -f input.txt -o output.txt
```

If no file arguments are given, the program prompts for them.

---

## 🛠 Tech & Concepts Used

* **C++ (C++11 standard)**
* **Data Structures:**

  * Queues
  * Priority Queues
  * Sorting algorithms
* **Operating System Concepts:**

  * CPU Scheduling Algorithms
  * Turnaround & Waiting Time calculation
  * Preemptive vs Non-preemptive scheduling logic (Non-preemptive implemented in this version)
* **File I/O in C++**

---

## 📚 Learning Outcomes

* Designed a **modular C++ program** from scratch without special libraries.
* Applied **core OS scheduling concepts** in a working simulation.
* Implemented **robust file parsing** and **user interaction**.
* Practiced **Git version control discipline** by committing daily progress during development.

---

## 🔮 Possible Extensions

* Add **Preemptive** variants of SJF and Priority scheduling.
* Visualize scheduling with **Gantt charts**.
* Export results in **CSV/JSON** for further analysis.

---

## 📜 License

MIT License — free to use, adapt, and share.

---
