#include "Reporter.h"

#include <iomanip>
#include <ostream>
#include <string>
#include <vector>

using namespace std;

namespace {
double averageWaitingTime(const ScheduleResult& result) {
    int total = 0;
    for (size_t i = 0; i < result.processes.size(); ++i) {
        total += result.processes[i].waitingTime;
    }
    return total / static_cast<double>(result.processes.size());
}

double averageTurnaroundTime(const ScheduleResult& result) {
    int total = 0;
    for (size_t i = 0; i < result.processes.size(); ++i) {
        total += result.processes[i].turnaroundTime;
    }
    return total / static_cast<double>(result.processes.size());
}

double averageResponseTime(const ScheduleResult& result) {
    int total = 0;
    for (size_t i = 0; i < result.processes.size(); ++i) {
        total += result.processes[i].responseTime;
    }
    return total / static_cast<double>(result.processes.size());
}

void printGanttChart(const vector<GanttSegment>& ganttChart, ostream& output) {
    if (ganttChart.empty()) {
        return;
    }

    output << "Gantt Chart:" << endl;

    for (size_t i = 0; i < ganttChart.size(); ++i) {
        output << "| ";
        if (ganttChart[i].processId == 0) {
            output << "Idle";
        } else {
            output << "P" << ganttChart[i].processId;
        }
        output << " ";
    }
    output << "|" << endl;

    output << ganttChart.front().startTime;
    for (size_t i = 0; i < ganttChart.size(); ++i) {
        output << setw(6) << ganttChart[i].endTime;
    }
    output << endl;
}
}

void printReport(const ScheduleResult& result, ostream& output) {
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    int totalResponseTime = 0;

    output << "Scheduling Method: " << result.algorithmName << endl;
    output << left << setw(10) << "Process"
           << setw(12) << "Arrival"
           << setw(10) << "Burst"
           << setw(10) << "Priority"
           << setw(10) << "Start"
           << setw(12) << "Complete"
           << setw(10) << "Waiting"
           << setw(12) << "Turnaround"
           << setw(10) << "Response" << endl;

    for (size_t i = 0; i < result.processes.size(); ++i) {
        const Process& process = result.processes[i];
        output << left << setw(10) << ("P" + to_string(process.id))
               << setw(12) << process.arrivalTime
               << setw(10) << process.burstTime
               << setw(10) << process.priority
               << setw(10) << process.startTime
               << setw(12) << process.completionTime
               << setw(10) << process.waitingTime
               << setw(12) << process.turnaroundTime
               << setw(10) << process.responseTime << endl;

        totalWaitingTime += process.waitingTime;
        totalTurnaroundTime += process.turnaroundTime;
        totalResponseTime += process.responseTime;
    }

    output << fixed << setprecision(2);
    output << "Average Waiting Time: "
           << totalWaitingTime / static_cast<double>(result.processes.size()) << " ms" << endl;
    output << "Average Turnaround Time: "
           << totalTurnaroundTime / static_cast<double>(result.processes.size()) << " ms" << endl;
    output << "Average Response Time: "
           << totalResponseTime / static_cast<double>(result.processes.size()) << " ms" << endl;
    output << endl;
    printGanttChart(result.ganttChart, output);
}

void printComparisonReport(const vector<ScheduleResult>& results, ostream& output) {
    output << "Scheduling Algorithm Comparison" << endl;
    output << left << setw(42) << "Algorithm"
           << setw(18) << "Avg Waiting"
           << setw(20) << "Avg Turnaround"
           << setw(18) << "Avg Response" << endl;

    output << fixed << setprecision(2);
    for (size_t i = 0; i < results.size(); ++i) {
        output << left << setw(42) << results[i].algorithmName
               << setw(18) << averageWaitingTime(results[i])
               << setw(20) << averageTurnaroundTime(results[i])
               << setw(18) << averageResponseTime(results[i]) << endl;
    }
}
