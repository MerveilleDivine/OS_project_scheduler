
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <queue> // for First Come, First Served Scheduling and Round-Robin Scheduling
#include <functional> // for Shortest-Job-First Scheduling and Priority Scheduling
#include <algorithm>



using namespace std;

// Structure for a process
struct Process {
  int burst_time;
  int arrival_time;
  int priority;
  int waiting_time;
  int turnaround_time;
};

// Compare function for Shortest-Job-First Scheduling
struct CompareByBurstTime {
bool operator()(const Process& p1, const Process& p2) const {
return p1.burst_time > p2.burst_time;
}
};

// Compare function for Priority Scheduling
struct CompareByPriority {
bool operator()(const Process& p1, const Process& p2) const {
return p1.priority > p2.priority;
}
};

// Functions
void choose_scheduling_method(vector<Process>& processes);
void run_first_come_first_served_scheduling(vector<Process>& processes);
void run_shortest_job_first_scheduling(vector<Process>& processes);
void run_priority_scheduling(vector<Process>& processes);
void run_round_robin_scheduling(vector<Process>& processes);
void run_no_scheduling(vector<Process>& processes);
void show_result(const vector<Process>& processes);
void end_program(const vector<Process>& processes, const string& output_file);
void show_menu(vector<Process>& processes);



//my_argc is the argument count and my_argv is the argument vector
int main(int my_argc, char** my_argv) {

  // Initialize variables for input and output file names
  string input_file;
  string output_file;

  // Parse the command line arguments
  for (int i = 1; i < my_argc; i++) {
    if (strcmp(my_argv[i], "-f") == 0) {
      input_file = my_argv[i+1];
    } else if (strcmp(my_argv[i], "-o") == 0) {
      output_file = my_argv[i+1];
    }
  }

  try {
    // Open the input file
    ifstream input(input_file);
    if (!input.is_open()) {
      throw runtime_error("Failed to open input file: " + input_file);
    }

    // Read the data from the input file
    vector<Process> processes;
    while (input >> burst_time >> arrival_time >> priority) {
      Process p;
      p.burst_time = burst_time;
      p.arrival_time = arrival_time;
      p.priority = priority;
      processes.push_back(p);
    }

    // to close the input file
    input.close();

   // Show the menu to the user
show_menu(processes);
} catch (const exception& e) {
cerr << "Error: " << e.what() << endl;
}

return 0;

  /* to implement file from cmd line use
  ./nameOfTheFile.extension -f input.txt -o output.txt
  change the filename
*/
}

void choose_scheduling_method(vector<Process>& processes) {
  cout << "Please select a scheduling method: " << endl;
  cout << "1. First Come, First Served Scheduling" << endl;
  cout << "2. Shortest-Job-First Scheduling" << endl;
  cout << "3. Priority Scheduling" << endl;
  cout << "4. Round-Robin Scheduling" << endl;
  cout << "5. No Scheduling (Press any other key to continue without scheduling)" << endl;

  int scheduling_method;
cin >> scheduling_method;

switch (scheduling_method) {
case 1:
// First Come, First Served Scheduling
run_first_come_first_served_scheduling(processes);
break;
case 2:
// Shortest-Job-First Scheduling
run_shortest_job_first_scheduling(processes);
break;
case 3:
// Priority Scheduling
run_priority_scheduling(processes);
break;
case 4:
// Round-Robin Scheduling
run_round_robin_scheduling(processes);
break;
default:
// No Scheduling
run_no_scheduling(processes);
break;
}
}

// Function to run First Come, First Served Scheduling
void run_first_come_first_served_scheduling(vector<Process>& processes) {
    // Sort the processes by arrival time
    sort(processes.begin(), processes.end(), [](const Process& p1, const Process& p2) {
        return p1.arrival_time < p2.arrival_time;
    });

    // Create a queue for the processes
    queue<Process> process_queue;

    // Variables to keep track of current time, completion time and waiting time
    int current_time = 0;
    vector<int> completion_time(processes.size());
    vector<int> waiting_time(processes.size());

    // Loop through the processes
    for (int i = 0; i < processes.size(); i++) {
        // Add the process to the queue if it arrives at the current time
        if (processes[i].arrival_time == current_time) {
            process_queue.push(processes[i]);
        }

        // If there are no processes in the queue, move the current time to the arrival time of the next process
        if (process_queue.empty()) {
            current_time = processes[i].arrival_time;
            process_queue.push(processes[i]);
            continue;
        }

        // Get the first process in the queue
        Process current_process = process_queue.front();
        process_queue.pop();

        // Update the completion time and waiting time for the current process
        completion_time[i] = current_time + current_process.burst_time;
        waiting_time[i] = current_time - current_process.arrival_time;

        // Move the current time to the completion time of the current process
        current_time = completion_time[i];

        // Add the next process to the queue if it arrives at the current time
        for (int j = i+1; j < processes.size(); j++) {
            if (processes[j].arrival_time == current_time) {
                process_queue.push(processes[j]);
            }
        }
    }

    // Show the result
    show_result(processes);
}



// Function to run Shortest-Job-First Scheduling
void run_shortest_job_first_scheduling(vector<Process>& processes) {
  // Sort the processes by burst time
    sort(processes.begin(), processes.end(), CompareByBurstTime());

    // Variables to keep track of current time, completion time and waiting time
    int current_time = 0;
    vector<int> completion_time(processes.size());
    vector<int> waiting_time(processes.size());

    // Loop through the processes
    for (int i = 0; i < processes.size(); i++) {
        // Update the completion time and waiting time for the current process
        completion_time[i] = current_time + processes[i].burst_time;
        waiting_time[i] = current_time - processes[i].arrival_time;

        // Move the current time to the completion time of the current process
        current_time = completion_time[i];
    }

    // Show the result
show_result(processes);
}





// Function to run Priority Scheduling
void run_priority_scheduling(vector<Process>& processes) {
  // sort the processes based on their priority
  sort(processes.begin(), processes.end(), CompareByPriority());
  
  int current_time = 0;
  int waiting_time = 0;
  int turn_around_time = 0;

  for (auto& process : processes) {
    current_time += process.burst_time;
    waiting_time += current_time - process.burst_time - process.arrival_time;
    turn_around_time += current_time - process.arrival_time;
  }

  // Show the results
  cout << "Priority Scheduling" << endl;
  cout << "Average Waiting Time: " << (float) waiting_time / processes.size() << endl;
  cout << "Average Turnaround Time: " << (float) turn_around_time / processes.size() << endl;
}



// Function to run Round-Robin Scheduling
     

