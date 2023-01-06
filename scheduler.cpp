
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>

using namespace std;

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

    // Open the output file
    ofstream output(output_file);
    if (!output.is_open()) {
      throw runtime_error("Failed to open output file: " + output_file);
    }

    // Print the input and output file names
    cout << "Input file: " << input_file << endl;
    cout << "Output file: " << output_file << endl;

    // Read from the input file and write to the output file
    string line;
    while (getline(input, line)) {
      output << line << endl;
    }
  } catch (const exception& e) {
    // Exception handling code goes here
    cerr << "Error: " << e.what() << endl;
    return 1;
  }

  return 0;

  /* to implement file from cmd line use
  ./nameOfTheFile.extension -f input.txt -o output.txt
  don't forget to change the filename
*/

}


