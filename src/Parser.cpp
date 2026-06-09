#include "Parser.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

namespace {
vector<string> split(const string& line, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(line);

    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}
}

vector<Process> readProcessesFromFile(const string& inputFile) {
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
