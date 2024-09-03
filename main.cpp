#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>

using namespace std;

// function prototypes
string analyzeLine(const string& line);
string trim(const string& str);
string getOperand(const string& line);
string analyzeOperands(const string& operands);
string analyzeOperand(const string& operand);
bool isDirective(const string& opcode);
bool isInstruction(const string& opcode);

// filename (dynamic)
string filename;

int main() {
    cout << "Enter assembly filename: ";
    cin >> filename;
    auto q = chrono::high_resolution_clock::now();

    ifstream originalFile(filename + ".asm");
    if (!originalFile.is_open()) {
        cerr << "Error opening original file" << endl;
        return 1;
    }

    string newFileName = string(filename) + "_commented.asm";
    ofstream newFile(newFileName);
    if (!newFile.is_open()) {
        cerr << "Error opening new file" << endl;
        return 1;
    }

    string line;
    while (getline(originalFile, line)) {
        string comment = analyzeLine(line);
        if (comment != "") {
            newFile << line << "\t\t; " << comment << endl;
        } else {
            newFile << line << endl;
        }
    }

    originalFile.close();
    newFile.close();

    auto dur = chrono::high_resolution_clock::now() - q;
    cout << "Successfully analyzed " << filename << ".asm in " << chrono::duration<double>(dur).count() << "s" << endl;
    cout << "Press any key to exit...";
    cin.ignore();
    cin.get();

    return 0;
}

bool isDirective(const string& opcode) {
    return opcode.size() > 0 && opcode[0] == '.';
}

string getOperand(const string& line) {
    size_t whitespacePos = line.find(' ');
    if (whitespacePos == string::npos) {
        return "";
    }

    string operand = line.substr(whitespacePos + 1);
    size_t trailingWhitespacePos = operand.find_last_not_of(' ');
    if (trailingWhitespacePos != string::npos) {
        operand = operand.substr(0, trailingWhitespacePos + 1);
    }

    return operand;
}

string analyzeLine(const string& line) {
    if (line.find_first_not_of(" \t\r\n") == string::npos) {
        return "";
    }

    string trimmedLine = trim(line);
    if (trimmedLine[0] == ';') {
        if (!trimmedLine.empty()) {
            return "ASSEMBLY ANALYZER: " + trimmedLine.substr(1);
        }
        else {
            return "";
        }
    }

    if (!trimmedLine.empty() && trimmedLine[trimmedLine.size() - 1] == ':') {
        return "Label: " + trimmedLine.substr(0, trimmedLine.size() - 1);
    }

    istringstream iss(trimmedLine);
    string opcode;
    iss >> opcode;

    if (isInstruction(opcode)) {
        string operands;
        getline(iss, operands);

        string operandComment;
        if (opcode == "int") {
            istringstream operandIss(operands);
            string operand;
            operandIss >> operand;

            if (operand.find("0x") == 0) {
                return string("Instruction: int ") + string("| Hexadecimal value: ") + operand;
            }
            else {
                return string("Instruction: int ") + string("| Unknown Operand: ") + operand;
            }
        }
        else {
            operandComment = analyzeOperands(operands);
        }

        return "Instruction: " + opcode + " " + operandComment;
    }
    else if (opcode == "section") {
        string sectionName = getOperand(line);
        return "Section " + sectionName + " declared";
    }

    if (isDirective(opcode)) {
        if (opcode == ".string") {
            string strValue = getOperand(line);
            strValue.erase(remove(strValue.begin(), strValue.end(), '\''), strValue.end());
            return "String constant '" + strValue + "' declared";
        }
    }

    return "Unknown instruction";
}

string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
}

bool isInstruction(const string& opcode) {
    static const vector<string> instructions = {
        "mov", "add", "sub", "mul", "div", "jmp", "je", "jne", "jg", "jl", "jge", "jle", "int"
    };

    return find(instructions.begin(), instructions.end(), opcode) != instructions.end();
}

string analyzeOperands(const string& operands) {
    istringstream iss(operands);
    string operand;
    string operandComment;
    while (iss >> operand) {
        string comment = analyzeOperand(operand);
        operandComment += comment + " ";
    }

    return operandComment;
}

string analyzeOperand(const string& operand) {
    if (operand[0] == 'r' && operand[1] == 'e' && operand[2] == 'g') {
        return "Register: " + operand;
    }

    if (operand[0] == '$') {
        return "Immediate: " + operand.substr(1);
    }

    if (operand[0] == '[' && operand[operand.size() - 1] == ']') {
        return "Memory Address: " + operand.substr(1, operand.size() - 2);
    }

    if (all_of(operand.begin(), operand.end(), ::isalnum) || operand.find('_') != string::npos) {
        return "Label/Variable: " + operand;
    }

    return "Unknown Operand: " + operand;
}
