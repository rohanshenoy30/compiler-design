# Compiler Design Lab - Student ID: 230905290_D32

A comprehensive collection of C programming assignments and practical implementations for a Compiler Design and Lab (CDL) course. This repository contains solutions to various lexical analysis, syntax analysis, and file processing tasks.

## Overview

This course covers the fundamentals of compiler design, including:
- **Lexical Analysis**: Tokenization and pattern matching
- **Syntax Analysis**: Parsing and grammar validation
- **Basic File I/O**: Text processing and analysis

## Repository Structure

```
cdlaballcodes/
├── lab1/          # File I/O and Basic Text Processing
├── lab2/          # Advanced Text Processing
├── lab3/          # Lexical Analysis - Token Classification
├── lab4/          # Character Classification
├── lab5/          # Lex (Lexer) Programs
├── lab6/          # Additional Analysis Programs
├── lab7/          # Input Processing
├── lab8/          # Advanced Input Handling
├── lab9/          # Supplementary Labs
├── lab10/         # Yacc (Parser) Programs
├── midsem/        # Mid-semester Exam Code
└── exam/          # Exam-related Implementations
```

## Detailed Lab Descriptions

### Lab 1: File I/O and Text Processing
**Objectives**: Learn file operations and basic string processing
- **q1.c**: Count characters and lines in a file
- **q2.c**: Copy file contents with modifications
- **q3.c**: Merge multiple files
- **q4.c**: Search and replace operations

### Lab 2: Advanced Text Processing
**Objectives**: Enhance file manipulation skills
- Implementation of copy, search, and pattern matching operations
- Input/output redirection examples

### Lab 3: Lexical Analysis - Token Classification
**Objectives**: Implement basic lexical analyzer
- **q1.c**: Identify keywords, identifiers, and operators
- Classify arithmetic, relational, and logical operators
- Recognize reserved words in C

### Lab 4: Character Classification
**Objectives**: Character-level processing
- Digit identification and classification
- Character type validation

### Lab 5: Lex Programs (Lexical Analyzer Generator)
**Objectives**: Learn automated lexical analysis with Lex/Flex
- **q1.l**: Count vowels and consonants
- **q2.l**: Identify operators and keywords
- **q3.l**: Token classification
- **q4.l**: String and number recognition
- **q5.l**: Complex pattern matching

### Lab 6: Additional Analysis Programs
**Objectives**: Further lexical analysis implementations
- Custom pattern matching algorithms
- Advanced tokenization techniques

### Lab 7-9: Input Processing and Advanced Topics
**Objectives**: Various specialized text processing tasks
- File input handling
- Stream processing
- Data validation

### Lab 10: Yacc Programs (Parser Generator)
**Objectives**: Learn syntax analysis and parsing
- **q1.y**: Variable declaration parsing
- **q2.y**: Expression parsing and validation
- **q3.y**: Statement parsing
- **q4.y**: Complex grammar rules
- Corresponding **.l** files for lexical analysis

### Exam & Midsem Folders
Complete exam solutions demonstrating course concepts in practical scenarios.

## Technologies & Tools

- **Language**: C
- **Lexer Generator**: Lex/Flex (`*.l` files)
- **Parser Generator**: Yacc/Bison (`*.y` files)
- **Compiler**: GCC
- **Build**: Makefile support

## Key Concepts Covered

1. **Lexical Analysis**
   - Pattern recognition with regular expressions
   - Token identification and classification
   - Lex/Flex usage

2. **Syntax Analysis**
   - Grammar specification
   - Parser generation with Yacc/Bison
   - Syntax tree construction

3. **File Processing**
   - File I/O operations
   - Text manipulation
   - Stream handling

4. **C Programming**
   - Data structures
   - String processing
   - File operations
   - Memory management

## How to Compile and Run

### C Programs
```bash
gcc -o program_name program_name.c
./program_name
```

### Lex Programs
```bash
flex program_name.l
gcc -o program_name lex.yy.c -lfl
./program_name
```

### Yacc Programs
```bash
yacc -d program_name.y
flex program_name.l
gcc -o program_name y.tab.c lex.yy.c -lfl
./program_name
```

## Prerequisites

- GCC compiler
- Flex (Lex implementation)
- Bison (Yacc implementation)
- Basic knowledge of C programming
- Understanding of regular expressions

## Installation (macOS/Linux)

```bash
# macOS
brew install flex bison

# Ubuntu/Debian
sudo apt-get install flex bison build-essential

# CentOS/RHEL
sudo yum install flex bison gcc
```

## Notes

- Each lab builds upon previous concepts
- Solutions progress from basic file I/O to complex parsing
- Exam folders contain integrated solutions
- Some programs require input files in the same directory

## Course Information

- **Student ID**: 230905290_D32
- **Course**: Compiler Design Lab (CDL)
- **Institution**: MIT Studies - 6th Semester

## Additional Resources

- Input/output files are provided in respective lab directories (e.g., `source.txt`, `inp.txt`)
- Sample test cases are available in text files within each lab folder
- Generated parser tables and lexer files (`.tab.c`, `.tab.h`, `lex.yy.c`) are included

---

**Last Updated**: 2026

For queries or improvements, refer to the specific lab implementation files in their respective directories.
