#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid){};

// -TODO: Return this process's ID
int Process::Pid() { return pid_; }

// -TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    // long hertz = sysconf(_SC_CLK_TCK);
      // calculation from https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
    vector<string> tokinized_line = LinuxParser::PidActiveJiffies(pid_);
    if (tokinized_line.size() > 22){
        long system_uptime = LinuxParser::UpTime();
        long hertz = sysconf(_SC_CLK_TCK);
        long utime = std::stol(tokinized_line[13]); // cpu time spent in user code, measured in clock ticks
        long stime = std::stol(tokinized_line[14]); // cpu time spent in kernel code, measured in clock ticks
        long cutime = std::stol(tokinized_line[15]); // waited for childern's cpu time spent in users code in clock ticks
        long cstime = std::stol(tokinized_line[16]); // waited for childern's cpu time spent in kernel code in clock ticks
        long startime = std::stol(tokinized_line[21]); // time when the process started, measured in clock ticks
        
        long total_time = utime + stime + cutime + cstime;
        float seconds = system_uptime - (startime / hertz);
        float cpu_usage = 100 * ((total_time / hertz) /seconds);
        return cpu_usage;
    }
    // return LinuxParser::ActiveJiffies(pid_);
    return 0; 
}

// -TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(pid_);
        // return string(); 
}

// -TODO: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(pid_);
}


// -TODO: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(pid_);
}

// -TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    vector<string> tokinized_stats = LinuxParser::PidActiveJiffies(pid_);
    long hertz = sysconf(_SC_CLK_TCK);
    long clockticks = std::stol(tokinized_stats[21]);
    return clockticks / hertz; 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    // LinuxParser::Ram(pid_);
    // this->Ram();
    return std::stof(LinuxParser::Ram(this->pid_)) < std::stold(LinuxParser::Ram(a.pid_)) ? true : false;
    // return true; 
    }