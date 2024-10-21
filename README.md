# Compiler Construction Project

This repository contains the code and documentation for the **Compiler Construction** course project. The goal of this project is to design and implement a custom programming language with features such as conditional statements, loops, function support, data types, error handling, and more.

## Course Details
- **Course Name:** Compiler Construction  
- **Instructor:** Sir Laeeq ul Zaman  
- **Institution:** University of Engineering and Technology Lahore  

## Project Description
This project focuses on building a simple compiler with the following features:
- **Custom Syntax:** Use of modified keywords (e.g., `Agar` instead of `if`).
- **Data Types:** Support for integer, float, double, string, bool, and char.
- **Control Flow:** Conditional statements (`if-else`) and loops (`while`, `for`).
- **Logical Expressions:** Support for logical operators like `&&`, `||`, `==`, and `!=`.
- **Symbol Table:** Efficient storage and lookup of variables using hash maps.
- **Error Handling:** Friendly syntax error messages with line numbers and hints.
- **File Handling:** Ability to read source code from files provided via command-line arguments.
- **Function Support:** Basic function declarations and calls.
- **Comments:** Support for single-line comments (`//`).

## How to Run
1. **Clone the Repository:**
   ```bash
   git clone https://github.com/MrUzairr/Compiler_Construction.git

2. **Navigate to the Project Directory:**
   cd https://github.com/MrUzairr/Compiler_Construction
3. **Compile the Code:** Use the g++ compiler to build the program:
   g++ main.cpp -o compiler
4. **Run the Program:**
   ./compiler test.txt
5. **Sample Input File (test.txt):**
   Agar (x > 10) {
    int y;
    y = x + 5;
} else {
    return 0;
}

## Project Structure
  ├── README.md           # Project documentation
  ├── main.cpp            # Main compiler source code
  ├── Lexer.cpp           # Lexical analyzer
  ├── Parser.cpp          # Syntax parser
  ├── SymbolTable.cpp     # Symbol table implementation
  ├── test.txt            # Sample input file

## Future Enhancements
- Support for Multi-Line Comments.
- Type Checking between variables.
- Optimizations for faster compilation.

## Instructor
Sir Laeeq ul Zaman

## Contributor
Muhammad Uzair 2021-CS-17

## Acknowledgments
Special thanks to Sir Laeeq ul Zaman for his guidance and support throughout the course.


---

### **How to Create a README.md on GitHub:**
1. **Go to your repository** on GitHub.
2. Click on **"Add file"** → **"Create new file"**.
3. Name the file **README.md**.
4. Paste the above content into the file.
5. Click **Commit new file**.

---

### **Explanation:**
- **Project Overview:** Describes the purpose and features of your compiler.
- **Course and Instructor Details:** Adds relevance to your academic project.
- **How to Run:** Provides clear instructions to compile and run the code.
- **Project Structure:** Displays the directory layout for easy navigation.
- **Acknowledgments:** Gives credit to your instructor for guidance.


