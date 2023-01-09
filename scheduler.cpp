
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <vector>


using namespace std;

// Structure for a process
struct Process {
  int burst_time;
  int arrival_time;
  int priority;
};

// Functions
void select_scheduling_method(vector<Process>& processes);
void run_preemptive_mode(const vector<Process>& processes);
void run_non_preemptive_mode(const vector<Process>& processes);
void result(const vector<Process>& processes);
void end_program(const vector<Process>& processes, const string& output_file);
void menu(vector<Process>& processes);


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

     // to display the menu to the user
    menu(processes);

    
    
  } catch (const exception& e) {
    // Exception handling code
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
      break;
    case 2:
      // Shortest-Job-First Scheduling
      break;
    case 3:
      // Priority Scheduling
      break;
    case 4:
      // Round-Robin Scheduling
      break;
    default:
      // No Scheduling
      break;
  }
}
     

