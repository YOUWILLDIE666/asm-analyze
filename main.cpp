#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>

// function prototypes
std::string analyzeLine(const std::string& line);
std::string trim(const std::string& str);
bool isInstruction(const std::string& opcode);
std::string analyzeOperands(const std::string& operands);
std::string analyzeOperand(const std::string& operand);

// filename (dynamic)
std::string filename;

int main() {
    std::cout << "Enter assembly filename: ";
    std::cin >> filename;
    auto q = std::chrono::high_resolution_clock::now();

    std::ifstream originalFile(filename + ".asm");
    if (!originalFile.is_open()) {
        std::cerr << "Error opening original file" << std::endl;
        return 1;
    }

    std::string newFileName = std::string(filename) + "_commented.asm";
    std::ofstream newFile(newFileName);
    if (!newFile.is_open()) {
        std::cerr << "Error opening new file" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(originalFile, line)) {
        std::string comment = analyzeLine(line);
        newFile << line << "        ; " << comment << std::endl;
    }

    originalFile.close();
    newFile.close();

    auto dur = std::chrono::high_resolution_clock::now() - q;
    std::cout << "Successfully analyzed " << filename << ".asm in " << std::chrono::duration<double>(dur).count() << "s" << std::endl;

    return 0;
}

bool isDirective(const std::string& opcode) {
    return opcode.size() > 0 && opcode[0] == '.';
}

std::string getOperand(const std::string& line) {
    size_t whitespacePos = line.find(' ');
    if (whitespacePos == std::string::npos) {
        return "";
    }

    std::string operand = line.substr(whitespacePos + 1);
    size_t trailingWhitespacePos = operand.find_last_not_of(' ');
    if (trailingWhitespacePos != std::string::npos) {
        operand = operand.substr(0, trailingWhitespacePos + 1);
    }

    return operand;
}

std::string analyzeLine(const std::string& line) {
    if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
        return "";
    }

    std::string trimmedLine = trim(line);
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

    std::istringstream iss(trimmedLine);
    std::string opcode;
    iss >> opcode;

    if (isInstruction(opcode)) {
        std::string operands;
        std::getline(iss, operands);

        std::string operandComment;
        if (opcode == "int") {
            std::istringstream operandIss(operands);
            std::string operand;
            operandIss >> operand;

            if (operand.find("0x") == 0) {
                return std::string("Instruction: int ") + std::string("| Hexadecimal value: ") + operand;
            }
            else {
                return std::string("Instruction: int ") + std::string("| Unknown Operand: ") + operand;
            }
        }
        else {
            operandComment = analyzeOperands(operands);
        }

        return "Instruction: " + opcode + " " + operandComment;
    }
    else if (opcode == "section") {
        std::string sectionName = getOperand(line);
        return "Section " + sectionName + " declared";
    }

    if (isDirective(opcode)) {
        if (opcode == ".string") {
            std::string strValue = getOperand(line);
            strValue.erase(std::remove(strValue.begin(), strValue.end(), '\''), strValue.end());
            return "String constant '" + strValue + "' declared";
        }
    }

    return "Unknown instruction";
}

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
}

bool isInstruction(const std::string& opcode) {
    static const std::vector<std::string> instructions = {
        "mov", "add", "sub", "mul", "div", "jmp", "je", "jne", "jg", "jl", "jge", "jle", "int"
    };

    return std::find(instructions.begin(), instructions.end(), opcode) != instructions.end();
}

std::string analyzeOperands(const std::string& operands) {
    std::istringstream iss(operands);
    std::string operand;
    std::string operandComment;
    while (iss >> operand) {
        std::string comment = analyzeOperand(operand);
        operandComment += comment + " ";
    }

    return operandComment;
}

std::string analyzeOperand(const std::string& operand) {
    if (operand[0] == 'r' && operand[1] == 'e' && operand[2] == 'g') {
        return "Register: " + operand;
    }

    if (operand[0] == '$') {
        return "Immediate: " + operand.substr(1);
    }

    if (operand[0] == '[' && operand[operand.size() - 1] == ']') {
        return "Memory Address: " + operand.substr(1, operand.size() - 2);
    }

    if (std::all_of(operand.begin(), operand.end(), ::isalnum) || operand.find('_') != std::string::npos) {
        return "Label/Variable: " + operand;
    }

    return "Unknown Operand: " + operand;
}
