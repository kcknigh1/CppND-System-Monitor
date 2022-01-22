#include <linux_parser.h>
#include <vector>
#include <string.h>
#include "processor.h"


// -TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<std::string> tokens = LinuxParser::CpuUtilization();
    int user = std::stoi(tokens[0]);
    int nice = std::stoi(tokens[1]);
    int system = std::stoi(tokens[2]);
    int idle = std::stoi(tokens[3]);
    int iowait = std::stoi(tokens[4]);
    int irq = std::stoi(tokens[5]);
    int softirq = std::stoi(tokens[6]);
    int steal = std::stoi(tokens[7]);

    // int PrevIdle = previdle + previowait;
    int Idle = idle + iowait;

    // int PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsortirq + prevsteal;
    int NonIdle = user + nice + system + irq + softirq + steal;

    // int PrevTotal = PrevIdle + PrevNonIdle;
    int Total = Idle + NonIdle;

    float totald = Total - PrevTotal;
    float idled = Idle - PrevIdle;

    PrevIdle = Idle;
    PrevNonIdle = NonIdle;
    PrevTotal = Total;

    return (totald - idled)/totald;
}