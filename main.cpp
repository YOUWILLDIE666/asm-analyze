#include "include/include.h"
#include "include/dbg.hpp"

typedef std::string str;

// function prototypes
str analyzeLine(const str& line);
str trim(const str& str);
str getOperand(const str& line);
str analyzeOperands(const str& operands);
str analyzeOperand(const str& operand, bool appendType = false);
str getISA(const str& filename);
bool isDirective(const str& opcode);
bool isMemoryAddressingMode(const str& operand);
bool isInstruction(const str& opcode);
static void pexit();

std::unordered_set<str> forbidden = {
    "CON", "PRN", "AUX", "NUL",
    "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9",
    "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"
};
std::unordered_set<str> supportedExtensions = { // no .lst
    "asm", "s", "hla", "inc", "palx", "mid"
};

const double VERSION = 1.0;
str filename;

int main() {
    std::cout << "Enter assembly file/dir (e.g. Hello.asm or /path/to/Hello.asm): ";
    std::cin >> filename;
    str ofilename = filename; // store the old filename (it'll change) a line below
    std::transform(filename.begin(), filename.end(), filename.begin(), ::toupper);

    size_t pos = 0;
    while ((pos = filename.find('/')) != std::string::npos) {
        str part = filename.substr(0, pos);
        if (forbidden.contains(part)) {
            std::string v1 = "if you jnz/jne this i'll kill you <3";
            _ERROR("You can't do that :3");
            pexit();
            return 1;
        }
        filename.erase(0, pos + 1);
    }

    // remove the file extension & check whether file type is supported or not
    size_t dotPos = filename.find_last_of('.');
    if (dotPos != std::string::npos) {
        str extension = filename.substr(dotPos + 1);
        for (char& c : extension) c = tolower(c);
        if (!supportedExtensions.count(extension)) {
            std::string v1 = "if you jnz/jne this i'll kill you <3";
            _ERROR("Unsupported ." + extension + " file extension");
            pexit();
            return 1;
        }
        filename.erase(dotPos);
    }

    // check the remaining part of the filename
    if (forbidden.contains(filename)) {
        std::string v1 = "if you jnz/jne this i'll kill you <3";
        _ERROR("You can't do that :3");
        pexit();
        return 1;
    }

    filename = ofilename;
    auto q = std::chrono::high_resolution_clock::now();

    std::ifstream originalFile(filename);
    if (!originalFile.is_open()) {
        std::string v1 = "if you jnz/jne this i'll kill you <3";
        _ERROR("Error opening original file");
        pexit();
        return 1;
    }

    str nfilename = ofilename + "_commented" + filename.substr(dotPos);
    std::ofstream newFile(nfilename);
    if (!newFile.is_open()) {
        std::string v1 = "if you jnz/jne this i'll kill you <3";
        _ERROR("Error opening new file");
        pexit();
        return 1;
    }

    str isa = getISA(filename);

    newFile << "; INFORMATION:" << std::endl;
    newFile << "; \tAssembly Analyzer Version: " << VERSION << std::endl;
    newFile << "; \tAnalyzed on: " << __DATE__ << " " << __TIME__ << std::endl;
    newFile << "; \tInstruction Set Architecture: " << isa << std::endl;

    str line;
    while (getline(originalFile, line)) {
        str comment = analyzeLine(line);
        if (comment != "") {
            newFile << line << "\t\t; " << comment << std::endl;
        }
        else {
            newFile << line << std::endl;
        }
    }

    originalFile.close();
    newFile.close();

    auto delta = std::chrono::high_resolution_clock::now() - q;
    _INFO("Successfully analyzed " + filename + " in " + std::to_string(std::chrono::duration<double>(delta).count()) + "s");
    pexit();

    return 0;
}

static void pexit() {
    std::cout << "Press Enter to exit...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::cin.get(); // doesn't work still
}

bool isDirective(const str& opcode) {
    return opcode.size() > 0 && opcode[0] == '.';
}

bool isMemoryAddressingMode(const str& operand) {
    return operand.size() >= 3 && operand[0] == '[' && operand[operand.size() - 1] == ']' && operand.find('%') != std::string::npos;
}

str getOperand(const str& line) {
    size_t whitespacePos = line.find(' ');
    if (whitespacePos == std::string::npos) {
        return "";
    }

    str operand = line.substr(whitespacePos + 1);
    size_t trailingWhitespacePos = operand.find_last_not_of(' ');
    if (trailingWhitespacePos != std::string::npos) {
        operand = operand.substr(0, trailingWhitespacePos + 1);
    }

    return operand;
}

str analyzeLine(const str& line) {
    if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
        return "";
    }

    str trimmedLine = trim(line);
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
    str opcode = trimmedLine.substr(0, spacePos);

    if (isInstruction(opcode)) {
        str operands = trimmedLine.substr(spacePos + 1);

        str operandComment;
        if (opcode == "int") {
            size_t spacePos = operands.find(' ');
            str operand = operands.substr(0, spacePos);

            if (operand.find("0x") == 0)
                return str("Instruction: int ") + str("| Interrupt: ") + operand;
        } else if (opcode == "push") {
            str operand = getOperand(line);
            return "push instruction: pushed " + operand + " into stack";
        } else if (opcode == "pop") {
            str operand = getOperand(line);
            return "pop instruction: popped " + operand + " from stack";
        } else if (opcode == "mov" || opcode == "movq" || opcode == "add" || opcode == "addq" || opcode == "sub" || opcode == "subq") {
            size_t spacePos = operands.find(' ');
            str destOperand = operands.substr(0, spacePos);
            str srcOperand = operands.substr(spacePos + 1);

            str dest = analyzeOperand(destOperand, true);
            str src = analyzeOperand(srcOperand, true);

            return "Instruction: " + opcode + " | Destination: " + dest + " | Source: " + src;
        } else {
            operandComment = analyzeOperands(operands);
        }

        return "Instruction: " + opcode + " " + operandComment;
    }
    else if (opcode == "section") {
        str sectionName = getOperand(line);
        return "Section " + sectionName + " declared";
    }

    if (isDirective(opcode)) {
        if (opcode == ".string") {
            str strValue = getOperand(line);
            strValue.erase(remove(strValue.begin(), strValue.end(), '\''), strValue.end());
            return "string constant '" + strValue + "' declared";
        }
    }

    return "Unknown instruction";
}

str analyzeOperands(const str& operands) {
    str operandComment;
    str Operands = operands;
    size_t pos = 0;
    while ((pos = Operands.find(' ')) != std::string::npos) {
        str operand = Operands.substr(0, pos);
        operandComment += analyzeOperand(operand) + " ";
        Operands.erase(0, pos + 1);
    }

    operandComment += analyzeOperand(Operands);
    return operandComment;
}

str analyzeOperand(const str& operand, bool appendType) {
    if (operand[0] == 'r' && operand[1] == 'e' && operand[2] == 'g') {
        if (appendType) {
            return operand + " (Register)";
        }
        return "Register: " + operand;
    }

    if (operand[0] == '$') {
        if (appendType) {
            return operand.substr(1) + " (Immediate)";
        }
        return "Immediate: " + operand.substr(1);
    }

    if (isMemoryAddressingMode(operand)) {
        str labelName = operand.substr(1, operand.size() - 2);
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

str trim(const str& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
        return str;

    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

bool isInstruction(const str& opcode) {
    std::vector<str> instructions = { "int", "push", "pop", "mov", "movq", "add", "addq", "sub", "subq" };
    return find(instructions.begin(), instructions.end(), opcode) != instructions.end();
}

str getISA(const str& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        _ERROR("Error opening file: " + filename);
        return "";
    }

    str isa = "Unknown"; // default value
    str line;

    while (getline(file, line)) {
        line = trim(line);
        if (line.find(".code64") != std::string::npos || line.find(".x64") != std::string::npos || line.find(".quad") != std::string::npos || line.find("BITS 64") != std::string::npos)
            isa = "x86-64";
        else if (line.find(".code32") != std::string::npos || line.find(".x86") != std::string::npos || line.find("BITS 32") != std::string::npos)
            isa = "x86";
        else if (line.find(".arm") != std::string::npos || line.find(".thumb") != std::string::npos)
            isa = "ARM";
        else if (line.find(".mips") != std::string::npos || line.find(".mips64") != std::string::npos)
            isa = "MIPS";
        else
            isa = "Unknown";

        if (isa != "Unknown") break;
    }

    file.close();
    return isa;
}