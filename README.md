# XCrypt

XCrypt is a simple command-line encryption and decryption tool using XOR encryption. It allows users to securely encrypt and decrypt files with a key file and ensures data integrity with a checksum.

## Features
- Encrypt (`-e`) and decrypt (`-d`) files using a XOR-based cipher.
- Uses a password for authentication before encryption or decryption.
- Appends a checksum for data integrity verification.
- Fully cross-platform (works on Linux, macOS, and Windows).

## Prerequisites
- C++ compiler (e.g., `g++` from GCC)
- A terminal or command prompt to run the program

## Installation
Clone the repository:
```sh
git clone https://github.com/SiddhPurohit/XCrypt.git
cd xcrypt
```

## Compilation
Compile the program using g++:
```sh
g++ -std=c++11 -o xcrypt xcrypt.cpp
```

## Usage
Run `xcrypt` with the required parameters:
```sh
./xcrypt <mode> <input.txt> <key.txt> <output.txt>
```

### Modes
- `-e` : Encrypts `input.txt` using `key.txt` and saves the result in `output.txt`.
- `-d` : Decrypts `input.txt` using `key.txt` and saves the result in `output.txt`.
- `-help` : Displays usage instructions.

### Example Commands
#### Encrypt a file:
```sh
./xcrypt -e input.txt key.txt output.txt
```
#### Decrypt a file:
```sh
./xcrypt -d input.txt key.txt output.txt
```
#### Display help:
```sh
./xcrypt -help
```

## Authentication
Before encryption or decryption, the program will prompt for a password. The correct password is:
```
hello
```
If an incorrect password is entered, the program will deny access.

## Error Handling
- If files are missing, the program will display an appropriate error.
- If the decryption checksum does not match, an integrity error will be shown.

## Notes
- Ensure `input.txt` and `key.txt` exist before running the program.
- The key file should not be empty and should contain random characters for better security.


