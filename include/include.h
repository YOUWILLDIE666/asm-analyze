// Import libraries/whataver to use them in main.cpp

#ifndef INCLUDE_H
#define INCLUDE_H

#include <unordered_set>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <cctype>

#include "dbg.hpp"

template <typename _>
void ce(const std::string& part, std::fstream& file) {
    if (!std::filesystem::exists(part)) {
        _ERROR("File " + part + " does not exist.");
        return;
    }
/*
    file.open(part);
    if (!file.is_open()) {
        _ERROR("Failed to open file " + part + " for reading.");
        return;
    }
*/
    if (!file.good()) {
        _ERROR("Error reading file " + part);
        return;
    }
}

#endif  // INCLUDE_H