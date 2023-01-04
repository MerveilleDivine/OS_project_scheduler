#include <iostream>
#include <cstring>

using namespace std;

int main(int argc, char** argv) {
    // Initialize variables for input and output file names
    char* input_file = NULL;
    char* output_file = NULL;

    // Parse the command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            input_file = argv[i+1];
        } else if (strcmp(argv[i], "-o") == 0) {
            output_file = argv[i+1];
        }
    }

    // Print the input and output file names
    cout << "Input file: " << input_file << endl;
    cout << "Output file: " << output_file << endl;

    return 0;
}


/* to run this part in cmd line
  ./filename -f input.txt -o output.txt
  don't forget to change the filename
*/

