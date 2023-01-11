#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <queue> // for First Come, First Served Scheduling and Round-Robin Scheduling
#include <functional> // for Shortest-Job-First Scheduling and Priority Scheduling
#include <algorithm>
#include <fstream>
#include <sstream>


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
void choose_scheduling_method(vector<Process>& processes, const string &schedulingMethod);
void run_first_come_first_served_scheduling(vector<Process>& processes, const string &schedulingMethod);
void run_shortest_job_first_scheduling(vector<Process>& processes, const string &schedulingMethod);
void run_priority_scheduling(vector<Process>& processes,const string &schedulingMethod);
void run_round_robin_scheduling(vector<Process>& processes, const string &schedulingMethod);
void run_no_scheduling(vector<Process>& processes,const string &schedulingMethod);
void show_result(const vector<Process>& processes, const string &schedulingMethod );
void end_program(const vector<Process>& processes, const string& output_file);
void show_menu(vector<Process>& processes, const string& output_file);

int main(int my_argc, char** my_argv) {

  // Initialize variables for input and output file names
  string input_file;
  string output_file;
  string schedulingmethodStr;
 
  

  // Parse the command line arguments
  for (int i = 1; i < my_argc; i++) {
    if (strcmp(my_argv[i], "-f") == 0) {
      input_file = my_argv[i+1];
    } else if (strcmp(my_argv[i], "-o") == 0) {
      output_file = my_argv[i+1];
    }
  }

for (int i = 1; i < my_argc; i++) {
    if (!input_file.empty() && !output_file.empty()) {
        break;
    }
    string arg = my_argv[i];
    if (arg == "-f" && (i + 1) < my_argc) {
        input_file = my_argv[i + 1];
    } else if (arg == "-o" && (i + 1) < my_argc) {
        output_file = my_argv[i + 1];
    }
}




   // If input file name was not parsed from command line arguments
  if (input_file.empty()) {
    // Ask for the input file name
    cout << "It seems like you have not parsed a file, please enter the input file name or exit and parse the filename: ";
    cin >> input_file;
  }

  try {
    // Open the input file in a loop until a valid file is found
    ifstream input(input_file);
    while (!input.is_open()) {
      try {
        input.open(input_file);
        if (!input.is_open()) {
          throw runtime_error("Failed to open input file: " + input_file);
        }
      } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;

        cout << "I couldn't find it, enter the input file name again: ";
        cin >> input_file;
      }
    }
  cout << "Welcome to the Process Scheduler!" << endl;
  cout << "There is 4 scheduling method in this program, which one will you use? : FCFS, SJF, Priority, RR -->  ";
  cin >> schedulingmethodStr;

  try {
    // Read the data from the input file

    int burst_time, arrival_time, priority;
    vector<Process> processes;
    string line;
    while (getline(input, line)) {
      stringstream ss(line);
      string token;
      vector<string> tokens;
      while (getline(ss, token, ':')) {
        tokens.push_back(token);
      }
      // create a process using the tokens
      Process p;
      p.burst_time = stoi(tokens[0]);
      p.arrival_time = stoi(tokens[1]);
      p.priority = stoi(tokens[2]);
      processes.push_back(p);
    }
    // Close the input file
    input.close();

    // Show the menu to the user
    show_menu(processes, schedulingmethodStr);
  } catch (const exception& e) {
    cerr << "Error: " << e.what() << endl;
  }

  } catch (const exception& e) {
    cerr << "Error: " << e.what() << endl;
  }
  return 0;
}





void choose_scheduling_method(vector<Process>& processes, const string &schedulingmethodStr) {

cout << "Please confirm the scheduling method (1,2,3,4,5): " << endl;
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
run_first_come_first_served_scheduling(processes, schedulingmethodStr);
break;
case 2:
// Shortest-Job-First Scheduling
run_shortest_job_first_scheduling(processes, schedulingmethodStr);
break;
case 3:
// Priority Scheduling
run_priority_scheduling(processes, schedulingmethodStr);
break;
case 4:
// Round-Robin Scheduling
run_round_robin_scheduling(processes, schedulingmethodStr);
break;
default:
// No Scheduling
run_no_scheduling(processes, schedulingmethodStr);
break;
}
}


// Function to run First Come, First Served Scheduling
void run_first_come_first_served_scheduling(vector<Process>& processes, const string &schedulingmethodStr) {
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
    show_result(processes, schedulingmethodStr);
}



// Function to run Shortest-Job-First Scheduling
void run_shortest_job_first_scheduling(vector<Process>& processes, const string &schedulingmethodStr) {
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
show_result(processes, schedulingmethodStr);
}





// Function to run Priority Scheduling
void run_priority_scheduling(vector<Process>& processes, const string &schedulingmethodStr) {
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
  show_result(processes, schedulingmethodStr);
}



// Function to run Round-Robin Scheduling
void run_round_robin_scheduling(vector<Process>& processes, const string &schedulingmethodStr) {
    string output_file;
    // Initialize variables for time and quantum
    int time = 0;
    int quantum;
    cout << "Please enter the quantum time: ";
    cin >> quantum;

    // Initialize a queue for the processes
    queue<Process> process_queue;

    // Initialize a vector for the result
    vector<Process> result;

    // Iterate through the processes
    for (int i = 0; i < processes.size(); i++) {
        // Check if the current process has arrived
        if (processes[i].arrival_time <= time) {
            process_queue.push(processes[i]);
        } else {
            // Keep track of the time until the next process arrives
            time = processes[i].arrival_time;
            process_queue.push(processes[i]);
        }

        // Run the process for the quantum time
        Process current_process = process_queue.front();
        process_queue.pop();
        time += min(current_process.burst_time, quantum);
        current_process.burst_time -= min(current_process.burst_time, quantum);

        // If the process is not completed, put it back in the queue
        if (current_process.burst_time > 0) {
            process_queue.push(current_process);
        } else {
            result.push_back(current_process);
        }

        // Check for any other processes that have arrived
        while (i < processes.size() - 1 && processes[i + 1].arrival_time <= time) {
            process_queue.push(processes[++i]);
        }
    }

    // Push any remaining processes in the queue to the result
    while (!process_queue.empty()) {
        Process current_process = process_queue.front();
        process_queue.pop();
        result.push_back(current_process);
    }

    // Show the result
    show_result(result, schedulingmethodStr);
    // End the program
    end_program(result, output_file);
}





// Function to run No Scheduling
void run_no_scheduling(vector<Process>& processes, const string &schedulingmethodStr) {
    string output_file;
// Initialize the waiting time and turnaround time for each process
for (Process& p : processes) {
p.waiting_time = 0;
p.turnaround_time = p.burst_time;
}

// Calculate the average waiting time and turnaround time
double total_waiting_time = 0;
double total_turnaround_time = 0;

for (const Process& p : processes) {
total_waiting_time += p.waiting_time;
total_turnaround_time += p.turnaround_time;
}
double average_waiting_time = total_waiting_time / processes.size();
double average_turnaround_time = total_turnaround_time / processes.size();

// Show the result
show_result(processes, schedulingmethodStr);
cout << "Average Waiting Time: " << average_waiting_time << endl;
cout << "Average Turnaround Time: " << average_turnaround_time << endl;

// End the program
end_program(processes, output_file);
}


// Function to show the menu to the user
void show_menu(vector<Process>& processes, const string &schedulingmethodStr) {
cout << "Please choose an option: " << endl;
cout << "1. Schedule the processes" << endl;
cout << "2. Show the result" << endl;
cout << "3. End the program" << endl;

int option;
cin >> option;

switch (option) {
case 1:
choose_scheduling_method(processes, schedulingmethodStr);
break;
case 2:
show_result(processes, schedulingmethodStr);
break;
case 3:
// Exit the program
break;
default:
cout << "Invalid option. Please try again." << endl;
show_menu(processes,schedulingmethodStr );
break;
}
}


void show_result(const vector<Process>& processes, const string &schedulingmethodStr) {
    cout << "Scheduling Method: " <<schedulingmethodStr << endl;
    cout << "Process Waiting Times:" << endl;
    int totalWaitingTime = 0;
    for (int i = 0; i < processes.size(); i++) {
        cout << "P" << i+1 << ": " << processes[i].waiting_time << " ms" << endl;
        totalWaitingTime += processes[i].waiting_time;
    }
    float avgWaitingTime = totalWaitingTime / (float)processes.size();
    cout << "Average Waiting Time: " << avgWaitingTime << " ms" << endl;

    cout << "Process Turnaround Times:" << endl;
    int totalTurnaroundTime = 0;
    for (int i = 0; i < processes.size(); i++) {
        cout << "P" << i+1 << ": " << processes[i].turnaround_time << " ms" << endl;
        totalTurnaroundTime += processes[i].turnaround_time;
    }
    float avgTurnaroundTime = totalTurnaroundTime / (float)processes.size();
    cout << "Average Turnaround Time: " << avgTurnaroundTime << " ms" << endl;
}




// Function to end the program
void end_program(const vector<Process>& processes, const string& output_file) {
  // Open the output file
  ofstream output(output_file);

  // Write the data to the output file
  for (const auto& p : processes) {
    output << p.burst_time << " " << p.arrival_time << " " << p.priority << endl;
  }

  // Close the output file
  output.close();
}

