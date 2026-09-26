#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;
    int completionTime = 0;
    int turnaroundTime = 0;
    int waitingTime = 0;
    int responseTime = -1;
};

void fcfs(vector<Process> processes) {
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;

    for (Process &p : processes) {
        if (currentTime < p.arrivalTime) {
            currentTime = p.arrivalTime;
        }

        p.responseTime = currentTime - p.arrivalTime;
        currentTime += p.burstTime;
        p.completionTime = currentTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
    }

    cout << "--- FCFS ---" << endl;
    cout << "PID  AT  BT  CT  TAT  WT  RT" << endl;
    for (const Process &p : processes) {
        cout << " " << p.id << "   " << p.arrivalTime << "   " << p.burstTime
             << "   " << p.completionTime << "   " << p.turnaroundTime
             << "    " << p.waitingTime << "   " << p.responseTime << endl;
    }
}

int main() {
    vector<Process> processes = {
        {1, 0, 5, 2},
        {2, 1, 3, 1},
        {3, 2, 8, 3},
        {4, 3, 6, 2}
    };

    fcfs(processes);
    return 0;
}