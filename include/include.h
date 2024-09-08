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

template <typename StreamType>
void checkExistence(const std::string& fname, StreamType& file) {
    if (!std::filesystem::exists(fname)) {
        _ERROR("File " + fname + " does not exist.");
        return;
    }

    if (!file.good()) {
        _ERROR("Error reading file " + fname);
        return;
    }
}

#endif  // INCLUDE_H