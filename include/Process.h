#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;
    int startTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
    int responseTime;

    Process()
        : id(0), arrivalTime(0), burstTime(0), remainingTime(0), priority(0),
          startTime(-1), completionTime(0), waitingTime(0), turnaroundTime(0),
          responseTime(0) {}
};

struct GanttSegment {
    int processId;
    int startTime;
    int endTime;

    GanttSegment() : processId(0), startTime(0), endTime(0) {}
    GanttSegment(int id, int start, int end)
        : processId(id), startTime(start), endTime(end) {}
};

struct ScheduleResult {
    std::string algorithmName;
    std::vector<Process> processes;
    std::vector<GanttSegment> ganttChart;
};

#endif
