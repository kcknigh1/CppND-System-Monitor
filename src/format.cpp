#include <string>
#include <cstdio>

#include "format.h"

#include <iostream>

using std::string;
using std::to_string;

// -TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds[[maybe_unused]]) { 
    char buffer[9];
    int hours = seconds/3600;
    int minutes = seconds%3600/60;
    int remaing_seconds = seconds%60;
    sprintf(buffer, "%02d:%02d:%02d", hours, minutes, remaing_seconds);
    return buffer; 
}