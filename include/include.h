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
void ce(const std::fstream& file, const std::string& fname) {
    if (!std::filesystem::exists(file)) {
        _ERROR("File " + fname + " does not exist.");
        return;
    }

    if (!file.is_open()) {
        if (!file.open()) {
            _ERROR("Failed to open file " + fname + " for reading.");
            return;
        }
    } else if (!file.good()) {
        _ERROR("Error reading file " + fname);
        return;
    } else {
        _ERROR("An error occured");
        return;
    }
}

#endif  // INCLUDE_H