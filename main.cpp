#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <cctype>

using namespace std;

// function prototypes
string analyzeLine(const string& line);
string trim(const string& str);
string getOperand(const string& line);
string analyzeOperands(const string& operands);
string analyzeOperand(const string& operand, bool appendType = false);
string getISA(const string& filename);
bool isDirective(const string& opcode);
bool isMemoryAddressingMode(const string& operand);
bool isInstruction(const string& opcode);

const double __VERSION__ = 1.0;
// file (dynamic)
string filename;

int main() {
    cout << "Enter assembly file/dir (e.g. Hello.asm or /path/to/Hello.asm): ";
    cin >> filename;
    auto q = chrono::high_resolution_clock::now();

    ifstream originalFile(filename);
    if (!originalFile.is_open()) {
        cerr << "Error opening original file" << endl;
        return 1;
    }

    string nfilename = string(filename) + "_commented.asm"; // we'll go with that for now
    ofstream newFile(nfilename);
    if (!newFile.is_open()) {
        cerr << "Error opening new file" << endl;
        return 1;
    }

    string isa = getISA(filename);

    newFile << "; INFORMATION:" << endl;
    newFile << "; \tAssembly Analyzer Version: " << __VERSION__ << endl;
    newFile << "; \tAnalyzed on: " << __DATE__ << " " << __TIME__ << endl;
    newFile << "; \tInstruction Set Architecture: " << isa << endl;

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

    auto delta = chrono::high_resolution_clock::now() - q;
    cout << "Successfully analyzed " << filename << " in " << chrono::duration<double>(delta).count() << "s" << endl;
    cout << "Press Enter to exit...";
    cin.ignore(); cin.get(); // doesn't work (except for the Return key).

    return 0;
}

bool isDirective(const string& opcode) {
    return opcode.size() > 0 && opcode[0] == '.';
}

bool isMemoryAddressingMode(const string& operand) {
    return operand.size() >= 3 && operand[0] == '[' && operand[operand.size() - 1] == ']' && operand.find('%') != string::npos;
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

    size_t spacePos = trimmedLine.find(' ');
    string opcode = trimmedLine.substr(0, spacePos);

    if (isInstruction(opcode)) {
        string operands = trimmedLine.substr(spacePos + 1);

        string operandComment;
        if (opcode == "int") {
            size_t spacePos = operands.find(' ');
            string operand = operands.substr(0, spacePos);

            if (operand.find("0x") == 0)
                return string("Instruction: int ") + string("| Interrupt: ") + operand + string(" | Purpose: Invoke an interrupt handler");
        } else if (opcode == "push") {
            string operand = getOperand(line);
            return "push instruction: pushed " + operand + " into stack";
        } else if (opcode == "pop") {
            string operand = getOperand(line);
            return "pop instruction: popped " + operand + " from stack";
        } else if (opcode == "mov" || opcode == "movq" || opcode == "add" || opcode == "addq" || opcode == "sub" || opcode == "subq") {
            size_t spacePos = operands.find(' ');
            string destOperand = operands.substr(0, spacePos);
            string srcOperand = operands.substr(spacePos + 1);

            string dest = analyzeOperand(destOperand, true);
            string src = analyzeOperand(srcOperand, true);

            return "Instruction: " + opcode + " | Destination: " + dest + " | Source: " + src;
        } else {
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

string analyzeOperands(const string& operands) {
    string operandComment;
    string Operands = operands;
    size_t pos = 0;
    while ((pos = Operands.find(' ')) != string::npos) {
        string operand = Operands.substr(0, pos);
        operandComment += analyzeOperand(operand) + " ";
        Operands.erase(0, pos + 1);
    }

    operandComment += analyzeOperand(Operands);
    return operandComment;
}

string analyzeOperand(const string& operand, bool appendType) {
    if (operand[0] == 'r' && operand[1] == 'e' && operand[2] == 'g') {
        if (appendType) {
            return operand + " (Register) | Purpose: Store temporary results";
        }
        return "Register: " + operand + " | Purpose: Store temporary results";
    }

    if (operand[0] == '$') {
        if (appendType) {
            return operand.substr(1) + " (Immediate)";
        }
        return "Immediate: " + operand.substr(1);
    }

    if (isMemoryAddressingMode(operand)) {
        string labelName = operand.substr(1, operand.size() - 2);
        if (appendType) {
            return labelName + " (Memory Address)";
        }
        return "Memory Address: " + labelName;
    }

    if (operand[0] == '%') {
        if (appendType) {
            return operand.substr(1) + " (Register)";
        }
        return "Register: " + operand.substr(1);
    }

    return "Unknown operand: " + operand;
}

string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
        return str;

    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

bool isInstruction(const string& opcode) {
    vector<string> instructions = { "int", "push", "pop", "mov", "movq", "add", "addq", "sub", "subq" };
    return find(instructions.begin(), instructions.end(), opcode) != instructions.end();
}

string getISA(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return "";
    }

    string isa = "Unknown"; // default value
    string line;

    while (getline(file, line)) {
        line = trim(line);
        if (line.find(".code64") != string::npos || line.find(".x64") != string::npos || line.find(".quad") != string::npos)
            isa = "x86-64";
        else if (line.find(".code32") != string::npos || line.find(".x86") != string::npos)
            isa = "x86";
        else if (line.find(".arm") != string::npos || line.find(".thumb") != string::npos)
            isa = "ARM";
        else if (line.find(".mips") != string::npos || line.find(".mips64") != string::npos)
            isa = "MIPS";
        else
            isa = "Unknown";

        if (isa != "Unknown") break;
    }

    file.close();
    return isa;
}
