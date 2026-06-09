#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

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

string toLower(string value) {
    for (size_t i = 0; i < value.size(); ++i) {
        value[i] = static_cast<char>(tolower(value[i]));
    }
    return value;
}

void calculateMetrics(Process& process) {
    process.turnaroundTime = process.completionTime - process.arrivalTime;
    process.waitingTime = process.turnaroundTime - process.burstTime;
    process.responseTime = process.startTime - process.arrivalTime;
}

vector<string> split(const string& line, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(line);

    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

vector<Process> readProcesses(const string& inputFile) {
    ifstream input(inputFile.c_str());

    if (!input.is_open()) {
        throw runtime_error("Failed to open input file: " + inputFile);
    }

    vector<Process> processes;
    string line;
    int lineNumber = 0;
    int processId = 1;

    while (getline(input, line)) {
        ++lineNumber;

        if (line.empty()) {
            continue;
        }

        vector<string> tokens = split(line, ':');
        if (tokens.size() != 3) {
            stringstream error;
            error << "Invalid input on line " << lineNumber
                  << ": expected burst_time:arrival_time:priority";
            throw runtime_error(error.str());
        }

        Process process;
        process.id = processId++;
        process.burstTime = stoi(tokens[0]);
        process.arrivalTime = stoi(tokens[1]);
        process.priority = stoi(tokens[2]);
        process.remainingTime = process.burstTime;

        if (process.burstTime < 0 || process.arrivalTime < 0) {
            stringstream error;
            error << "Invalid negative value on line " << lineNumber;
            throw runtime_error(error.str());
        }

        processes.push_back(process);
    }

    if (processes.empty()) {
        throw runtime_error("Input file does not contain any processes.");
    }

    return processes;
}

void sortByProcessId(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), [](const Process& first, const Process& second) {
        return first.id < second.id;
    });
}

vector<Process> runFirstComeFirstServed(vector<Process> processes) {
    sort(processes.begin(), processes.end(), [](const Process& first, const Process& second) {
        if (first.arrivalTime == second.arrivalTime) {
            return first.id < second.id;
        }
        return first.arrivalTime < second.arrivalTime;
    });

    int currentTime = 0;

    for (size_t i = 0; i < processes.size(); ++i) {
        if (currentTime < processes[i].arrivalTime) {
            currentTime = processes[i].arrivalTime;
        }

        processes[i].startTime = currentTime;
        currentTime += processes[i].burstTime;
        processes[i].completionTime = currentTime;
        processes[i].remainingTime = 0;
        calculateMetrics(processes[i]);
    }

    sortByProcessId(processes);
    return processes;
}

vector<Process> runNoScheduling(vector<Process> processes) {
    int currentTime = 0;

    for (size_t i = 0; i < processes.size(); ++i) {
        if (currentTime < processes[i].arrivalTime) {
            currentTime = processes[i].arrivalTime;
        }

        processes[i].startTime = currentTime;
        currentTime += processes[i].burstTime;
        processes[i].completionTime = currentTime;
        processes[i].remainingTime = 0;
        calculateMetrics(processes[i]);
    }

    sortByProcessId(processes);
    return processes;
}

vector<Process> runShortestJobFirst(vector<Process> processes) {
    int currentTime = 0;
    int completedProcesses = 0;
    vector<bool> completed(processes.size(), false);

    while (completedProcesses < static_cast<int>(processes.size())) {
        int selectedIndex = -1;

        for (size_t i = 0; i < processes.size(); ++i) {
            if (completed[i] || processes[i].arrivalTime > currentTime) {
                continue;
            }

            if (selectedIndex == -1 ||
                processes[i].burstTime < processes[selectedIndex].burstTime ||
                (processes[i].burstTime == processes[selectedIndex].burstTime &&
                 processes[i].arrivalTime < processes[selectedIndex].arrivalTime) ||
                (processes[i].burstTime == processes[selectedIndex].burstTime &&
                 processes[i].arrivalTime == processes[selectedIndex].arrivalTime &&
                 processes[i].id < processes[selectedIndex].id)) {
                selectedIndex = static_cast<int>(i);
            }
        }

        if (selectedIndex == -1) {
            int nextArrival = -1;
            for (size_t i = 0; i < processes.size(); ++i) {
                if (!completed[i] && (nextArrival == -1 || processes[i].arrivalTime < nextArrival)) {
                    nextArrival = processes[i].arrivalTime;
                }
            }
            currentTime = nextArrival;
            continue;
        }

        Process& process = processes[selectedIndex];
        process.startTime = currentTime;
        currentTime += process.burstTime;
        process.completionTime = currentTime;
        process.remainingTime = 0;
        calculateMetrics(process);

        completed[selectedIndex] = true;
        ++completedProcesses;
    }

    sortByProcessId(processes);
    return processes;
}

vector<Process> runPriorityScheduling(vector<Process> processes) {
    int currentTime = 0;
    int completedProcesses = 0;
    vector<bool> completed(processes.size(), false);

    while (completedProcesses < static_cast<int>(processes.size())) {
        int selectedIndex = -1;

        for (size_t i = 0; i < processes.size(); ++i) {
            if (completed[i] || processes[i].arrivalTime > currentTime) {
                continue;
            }

            if (selectedIndex == -1 ||
                processes[i].priority < processes[selectedIndex].priority ||
                (processes[i].priority == processes[selectedIndex].priority &&
                 processes[i].arrivalTime < processes[selectedIndex].arrivalTime) ||
                (processes[i].priority == processes[selectedIndex].priority &&
                 processes[i].arrivalTime == processes[selectedIndex].arrivalTime &&
                 processes[i].id < processes[selectedIndex].id)) {
                selectedIndex = static_cast<int>(i);
            }
        }

        if (selectedIndex == -1) {
            int nextArrival = -1;
            for (size_t i = 0; i < processes.size(); ++i) {
                if (!completed[i] && (nextArrival == -1 || processes[i].arrivalTime < nextArrival)) {
                    nextArrival = processes[i].arrivalTime;
                }
            }
            currentTime = nextArrival;
            continue;
        }

        Process& process = processes[selectedIndex];
        process.startTime = currentTime;
        currentTime += process.burstTime;
        process.completionTime = currentTime;
        process.remainingTime = 0;
        calculateMetrics(process);

        completed[selectedIndex] = true;
        ++completedProcesses;
    }

    sortByProcessId(processes);
    return processes;
}

vector<Process> runRoundRobin(vector<Process> processes, int quantum) {
    if (quantum <= 0) {
        throw runtime_error("Round Robin quantum must be greater than 0.");
    }

    sort(processes.begin(), processes.end(), [](const Process& first, const Process& second) {
        if (first.arrivalTime == second.arrivalTime) {
            return first.id < second.id;
        }
        return first.arrivalTime < second.arrivalTime;
    });

    queue<int> readyQueue;
    int currentTime = 0;
    size_t nextProcessIndex = 0;
    int completedProcesses = 0;

    while (completedProcesses < static_cast<int>(processes.size())) {
        if (readyQueue.empty() && nextProcessIndex < processes.size() &&
            currentTime < processes[nextProcessIndex].arrivalTime) {
            currentTime = processes[nextProcessIndex].arrivalTime;
        }

        while (nextProcessIndex < processes.size() &&
               processes[nextProcessIndex].arrivalTime <= currentTime) {
            readyQueue.push(static_cast<int>(nextProcessIndex));
            ++nextProcessIndex;
        }

        if (readyQueue.empty()) {
            continue;
        }

        int processIndex = readyQueue.front();
        readyQueue.pop();
        Process& process = processes[processIndex];

        if (process.startTime == -1) {
            process.startTime = currentTime;
        }

        int timeSlice = min(quantum, process.remainingTime);
        process.remainingTime -= timeSlice;
        currentTime += timeSlice;

        while (nextProcessIndex < processes.size() &&
               processes[nextProcessIndex].arrivalTime <= currentTime) {
            readyQueue.push(static_cast<int>(nextProcessIndex));
            ++nextProcessIndex;
        }

        if (process.remainingTime > 0) {
            readyQueue.push(processIndex);
        } else {
            process.completionTime = currentTime;
            calculateMetrics(process);
            ++completedProcesses;
        }
    }

    sortByProcessId(processes);
    return processes;
}

void printReport(const vector<Process>& processes, const string& schedulingMethod, ostream& output) {
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    int totalResponseTime = 0;

    output << "Scheduling Method: " << schedulingMethod << endl;
    output << left << setw(10) << "Process"
           << setw(12) << "Arrival"
           << setw(10) << "Burst"
           << setw(10) << "Priority"
           << setw(10) << "Start"
           << setw(12) << "Complete"
           << setw(10) << "Waiting"
           << setw(12) << "Turnaround"
           << setw(10) << "Response" << endl;

    for (size_t i = 0; i < processes.size(); ++i) {
        const Process& process = processes[i];
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
           << totalWaitingTime / static_cast<double>(processes.size()) << " ms" << endl;
    output << "Average Turnaround Time: "
           << totalTurnaroundTime / static_cast<double>(processes.size()) << " ms" << endl;
    output << "Average Response Time: "
           << totalResponseTime / static_cast<double>(processes.size()) << " ms" << endl;
}

void printHelp() {
    cout << "CPU Scheduler Simulator" << endl;
    cout << "Usage:" << endl;
    cout << "  ./scheduler -f input.txt -o output.txt --algorithm fcfs" << endl;
    cout << "  ./scheduler -f input.txt -o output.txt --algorithm rr --quantum 2" << endl;
    cout << endl;
    cout << "Algorithms: none, fcfs, sjf, priority, rr" << endl;
}

string normalizeAlgorithmName(const string& input) {
    string value = toLower(input);

    if (value == "1" || value == "fcfs" || value == "first-come-first-served") {
        return "fcfs";
    }
    if (value == "2" || value == "sjf" || value == "shortest-job-first") {
        return "sjf";
    }
    if (value == "3" || value == "priority") {
        return "priority";
    }
    if (value == "4" || value == "rr" || value == "round-robin") {
        return "rr";
    }
    if (value == "5" || value == "none" || value == "no-scheduling") {
        return "none";
    }

    return value;
}

string displayNameForAlgorithm(const string& algorithm, int quantum) {
    if (algorithm == "fcfs") {
        return "First Come, First Served";
    }
    if (algorithm == "sjf") {
        return "Shortest Job First - Non-Preemptive";
    }
    if (algorithm == "priority") {
        return "Priority Scheduling - Non-Preemptive";
    }
    if (algorithm == "rr") {
        stringstream name;
        name << "Round Robin - Quantum " << quantum;
        return name.str();
    }
    return "No Scheduling";
}

int main(int argc, char** argv) {
    string inputFile;
    string outputFile;
    string algorithm;
    int quantum = 0;

    for (int i = 1; i < argc; ++i) {
        string argument = argv[i];

        if (argument == "--help" || argument == "-h") {
            printHelp();
            return 0;
        } else if ((argument == "-f" || argument == "--input") && i + 1 < argc) {
            inputFile = argv[++i];
        } else if ((argument == "-o" || argument == "--output") && i + 1 < argc) {
            outputFile = argv[++i];
        } else if ((argument == "-a" || argument == "--algorithm") && i + 1 < argc) {
            algorithm = normalizeAlgorithmName(argv[++i]);
        } else if ((argument == "-q" || argument == "--quantum") && i + 1 < argc) {
            quantum = stoi(argv[++i]);
        } else {
            cerr << "Unknown or incomplete argument: " << argument << endl;
            printHelp();
            return 1;
        }
    }

    if (inputFile.empty()) {
        cout << "Enter input file name: ";
        cin >> inputFile;
    }

    if (algorithm.empty()) {
        cout << "Choose scheduling method:" << endl;
        cout << "1. First Come, First Served" << endl;
        cout << "2. Shortest Job First" << endl;
        cout << "3. Priority Scheduling" << endl;
        cout << "4. Round Robin" << endl;
        cout << "5. No Scheduling" << endl;
        cout << "Option: ";
        cin >> algorithm;
        algorithm = normalizeAlgorithmName(algorithm);
    }

    try {
        vector<Process> processes = readProcesses(inputFile);
        vector<Process> result;

        if (algorithm == "fcfs") {
            result = runFirstComeFirstServed(processes);
        } else if (algorithm == "sjf") {
            result = runShortestJobFirst(processes);
        } else if (algorithm == "priority") {
            result = runPriorityScheduling(processes);
        } else if (algorithm == "rr") {
            if (quantum <= 0) {
                cout << "Enter Round Robin quantum: ";
                cin >> quantum;
            }
            result = runRoundRobin(processes, quantum);
        } else if (algorithm == "none") {
            result = runNoScheduling(processes);
        } else {
            throw runtime_error("Unsupported scheduling algorithm: " + algorithm);
        }

        string methodName = displayNameForAlgorithm(algorithm, quantum);
        printReport(result, methodName, cout);

        if (!outputFile.empty()) {
            ofstream output(outputFile.c_str());
            if (!output.is_open()) {
                throw runtime_error("Failed to open output file: " + outputFile);
            }
            printReport(result, methodName, output);
        }
    } catch (const exception& error) {
        cerr << "Error: " << error.what() << endl;
        return 1;
    }

    return 0;
}
