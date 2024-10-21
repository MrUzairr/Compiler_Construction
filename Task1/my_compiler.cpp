#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char *argv[]) {
    // Check if a file name is provided
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    // Get the file name from the command-line arguments
    string filename = argv[1];
    
    // Open the file
    ifstream file(filename);

    // Check if the file was opened successfully
    if (!file) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }

    // Read and display the file content line by line
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }

    // Close the file
    file.close();

    return 0;
}
