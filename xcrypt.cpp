#include <iostream>
#include <fstream>
#include <string>
#include <iterator>

using namespace std;

// XOR encryption/decryption function
string xorCrypt(const string &data, const string &keyString) {
    string result = data;
    size_t keyLen = keyString.size();
    for (size_t i = 0; i < data.size(); i++) {
        result[i] = data[i] ^ keyString[i % keyLen];
    }
    return result;
}

// Simple FNV-1a hash (32-bit)
unsigned int fnvHash(const string &text) {
    const unsigned int OFFSET = 2166136261U;
    const unsigned int PRIME = 16777619U;
    unsigned int hashValue = OFFSET;

    for (size_t i = 0; i < text.length(); i++) { 
        hashValue ^= static_cast<unsigned char>(text[i]);
        hashValue *= PRIME;
    }
    return hashValue;
}


// Append a 4-byte checksum to the data
string addChecksum(const string &plainText) {
    unsigned int hashVal = fnvHash(plainText);
    string newText = plainText;
    newText.push_back(static_cast<char>(hashVal & 0xFF));
    newText.push_back(static_cast<char>((hashVal >> 8) & 0xFF));
    newText.push_back(static_cast<char>((hashVal >> 16) & 0xFF));
    newText.push_back(static_cast<char>((hashVal >> 24) & 0xFF));
    return newText;
}

// Check the checksum
pair<string, bool> checkChecksum(const string &dataWithChecksum) {
    if (dataWithChecksum.size() < 4) {
        return make_pair("", false);
    }
    size_t textLen = dataWithChecksum.size() - 4;
    string originalText = dataWithChecksum.substr(0, textLen);
    unsigned int savedHash = 0;
    savedHash |= static_cast<unsigned char>(dataWithChecksum[textLen]);
    savedHash |= static_cast<unsigned char>(dataWithChecksum[textLen + 1]) << 8;
    savedHash |= static_cast<unsigned char>(dataWithChecksum[textLen + 2]) << 16;
    savedHash |= static_cast<unsigned char>(dataWithChecksum[textLen + 3]) << 24;
    unsigned int calcHash = fnvHash(originalText);
    return make_pair(originalText, (calcHash == savedHash));
}

// Display help menu
void displayHelp(const char *appName) {
    cout << "Usage: " << appName << " <mode> <input.txt> <key.txt> <output.txt>\n";
    cout << "Modes:\n";
    cout << "  -e       Encrypt the input file using the key (appends a checksum)\n";
    cout << "  -d       Decrypt the input file using the key (verifies integrity)\n";
    cout << "  -help    Show this help message\n";
    cout << "\nExamples:\n";
    cout << "  " << appName << " -e input.txt key.txt output.txt   (Encrypts input.txt)\n";
    cout << "  " << appName << " -d input.txt key.txt output.txt   (Decrypts input.txt)\n";
}


// Verify password using FNV-1a. The correct hash is for the password "hello".
bool verifyPassword() {
    string userPass;
    const unsigned int CORRECT_HASH = 0x4f9f2cab;  // Precomputed FNV hash for "hello"
    cout << "Enter password to proceed: ";
    cin >> userPass;
    if (fnvHash(userPass) == CORRECT_HASH) {
        return true;
    } else {
        cerr << "Error: Incorrect password. Access denied.\n";
        return false;
    }

}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Error: No mode specified.\n";
        displayHelp(argv[0]);
        return 1;
    }

    string mode = argv[1];
    if (mode == "-help") {
        displayHelp(argv[0]);
        return 0;
    }
    if (argc != 5) {
        cerr << "Error: Incorrect number of arguments.\n";
        displayHelp(argv[0]);
        return 1;
    }

    // Password authentication
    if (!verifyPassword()) {
        return 1;
    }

    // Get file names from arguments
    string inputFileName = argv[2];
    string keyFileName = argv[3];
    string outputFileName = argv[4];


    // Read data from input file
    ifstream inFile(inputFileName, ios::binary);
    if (!inFile) {
        cerr << "Error: Could not open input file " << inputFileName << endl;
        return 1;
    }
    string fileData((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    // Read key from key file
    ifstream keyFile(keyFileName, ios::binary);
    if (!keyFile) {
        cerr << "Error: Could not open key file " << keyFileName << endl;
        return 1;
    }
    string keyData((istreambuf_iterator<char>(keyFile)), istreambuf_iterator<char>());
    keyFile.close();

    
    string outputText;
    if (mode == "-e") {
        // Append checksum and then encrypt
        string dataWithChecksum = addChecksum(fileData);
        outputText = xorCrypt(dataWithChecksum, keyData);
        cout << "Encryption completed with checksum added.\n";
    } else if (mode == "-d") {
        // Decrypt and verify checksum
        string decryptedData = xorCrypt(fileData, keyData);
        pair<string, bool> resultPair = checkChecksum(decryptedData);
        if (!resultPair.second) {
            cerr << "Error: Checksum mismatch. Data integrity compromised.\n";
            return 1;
        }
        outputText = resultPair.first;
        cout << "Decryption successful and data integrity verified.\n";
    } else {
        cerr << "Error: Invalid mode " << mode << endl;
        displayHelp(argv[0]);
        return 1;
    }


    // Write result to output file (can create it if necessary)
    ofstream outFile(outputFileName, ios::binary);
    if (!outFile) {
        cerr << "Error: Could not open or create output file " << outputFileName << endl;
        return 1;
    }
    outFile << outputText;
    outFile.close();


    cout << "Result saved in " << outputFileName << endl;
    return 0;

}
