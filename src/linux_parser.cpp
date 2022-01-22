#include <dirent.h>
#include <iomanip>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string readValueFromFile(string searchKey, string fileName){
  string line;
  string key;
  string value;
  std::ifstream filestream(fileName);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == searchKey) {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }      
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  // string line;
  // string key;
  // string value;
  // std::ifstream filestream(kOSPath);
  // if (filestream.is_open()) {
  //   while (std::getline(filestream, line)) {
  //     std::replace(line.begin(), line.end(), ' ', '_');
  //     std::replace(line.begin(), line.end(), '=', ' ');
  //     std::replace(line.begin(), line.end(), '"', ' ');
  //     std::istringstream linestream(line);
  //     while (linestream >> key >> value) {
  //       if (key == "PRETTY_NAME") {
  //         std::replace(value.begin(), value.end(), '_', ' ');
  //         return value;
  //       }
  //     }
  //   }
  // }
  // return value;
  return readValueFromFile("PRETTY_NAME", kOSPath);
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// -TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  float memTotal;
  float memFree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()){
    for (int i=0; i < 2; i++){
      std::getline(filestream, line);
      std::istringstream lineStream(line);
      lineStream >> key >> value;
      if (key == "MemTotal:"){
        memTotal = std::stof(value);
      } else if (key == "MemFree:"){
        memFree = std::stoi(value);
      }
    }
  }
  return (memTotal-memFree)/memTotal;
}

// -TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime = 5;
  string line; 
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream lineStream(line);
    lineStream >> uptime;
  }
  return uptime;
}

string LinuxParser::readLineFromStats(string key){
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string line_key;
  string value;
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> line_key >> value;
      if (line_key == key){
        return value;
      }
    }
  }
  return "0"; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
vector<string> LinuxParser::PidActiveJiffies(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  string line_key;
  string value;
  vector<string> tokinized_line{};
  if (filestream.is_open()){
    if (std::getline(filestream, line)){
      std::istringstream linestream(line);
      std::stringstream stream(line);
      string token;
      while (std::getline(stream, token, ' ')){
        tokinized_line.push_back(token);
      }
    }
  }
  return tokinized_line;
  // // calculation from https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  // if (tokinized_line.size() > 22){
  //   long system_uptime = UpTime();
  //   long hertz = sysconf(_SC_CLK_TCK);
  //   long utime = std::stol(tokinized_line[13]); // cpu time spent in user code, measured in clock ticks
  //   long stime = std::stol(tokinized_line[14]); // cpu time spent in kernel code, measured in clock ticks
  //   long cutime = std::stol(tokinized_line[15]); // waited for childern's cpu time spent in users code in clock ticks
  //   long cstime = std::stol(tokinized_line[16]); // waited for childern's cpu time spent in kernel code in clock ticks
  //   long startime = std::stol(tokinized_line[21]); // time when the process started, measured in clock ticks
    
  //   long total_time = utime + stime + cutime + cstime;
  //   long seconds = system_uptime - (startime / hertz);
  //   long cpu_usage = 100 * ((total_time / hertz) /seconds);
  //   return cpu_usage;
  // }
  // return "0"; 
  // return 0; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// -TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> split_values {}; 
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string token;
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::stringstream stream(line);
    while (std::getline(stream, token, ' ')){
      if (token != "" && token != "cpu")
        split_values.push_back(token);
    }
  }
  return split_values;
}

// -TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return std::stoi(LinuxParser::readLineFromStats("processes"));
  // string line;
  // std::ifstream filestream(kProcDirectory + kStatFilename);
  // string key;
  // string value;
  // if (filestream.is_open()){
  //   while (std::getline(filestream, line)){
  //     std::istringstream linestream(line);
  //     linestream >> key >> value;
  //     if (key == "processes"){
  //       return std::stoi(value);
  //     }
  //   }
  // }
  // return 0; 
}

// -TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return std::stoi(LinuxParser::readLineFromStats("procs_running"));
  // return 0; 
  }

// -TODO: Read and return the command associated with a process
// -REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) {
  // long uptime = 5;
  string line; 
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
    // std::istringstream lineStream(line);
    // lineStream >> uptime;
  }
  // return uptime;
  return string();
}

// -TODO: Read and return the memory used by a process
// -REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value;
  float memTotal;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    // for (int i=0; i < 2; i++){
    while(std::getline(filestream, line)){
      std::istringstream lineStream(line);
      lineStream >> key >> value;
      if (key == "VmSize:"){
        memTotal = std::stof(value);
        // convet kb to mb
        std::stringstream ss;
        memTotal /= 1000;
        ss << std::fixed << std::setprecision(2) << memTotal;
        return ss.str();
      } 
    }
  }
  // return "0.00"; 
}

// -TODO: Read and return the user ID associated with a process
// -REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  string key =  "Uid:";
  string line_key;
  string value;
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> line_key >> value;
      if (line_key == key){
        return value;
      }
    }
  }
  // return "0"; 
  return string(); 
}

// -TODO: Read and return the user associated with a process
// -REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  string line;
  std::ifstream filestream(kPasswordPath);
  string token;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)){
      vector<string> split_line = {};
      std::stringstream stream(line);
      for (int i=0; i<3; i++){
        std::getline(stream, token, ':');
        split_line.push_back(token);
      }
      if (split_line.size() == 3 && split_line[2] == uid){
        return split_line[0];  
      }
    }
  }
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
