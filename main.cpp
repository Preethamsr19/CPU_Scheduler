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
    bool isCompleted = false;
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

void sjf(vector<Process> processes) {
    int currentTime = 0;
    int completed = 0;
    int n = processes.size();

    while (completed < n) {
        int idx = -1;
        int minBurst = 1e9;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime &&
                !processes[i].isCompleted &&
                processes[i].burstTime < minBurst) {

                idx = i;
                minBurst = processes[i].burstTime;
            }
        }

        if (idx == -1) {
            currentTime++;
            continue;
        }

        processes[idx].responseTime =
            currentTime - processes[idx].arrivalTime;

        currentTime += processes[idx].burstTime;

        processes[idx].completionTime = currentTime;

        processes[idx].turnaroundTime =
            processes[idx].completionTime - processes[idx].arrivalTime;

        processes[idx].waitingTime =
            processes[idx].turnaroundTime - processes[idx].burstTime;

        processes[idx].isCompleted = true;
        completed++;
    }

    cout << "--- SJF ---" << endl;
    cout << "PID  AT  BT  CT  TAT  WT  RT" << endl;

    for (const Process &p : processes) {
        cout << " " << p.id << "   " << p.arrivalTime << "   " << p.burstTime
             << "   " << p.completionTime << "   " << p.turnaroundTime
             << "    " << p.waitingTime << "   " << p.responseTime << endl;
    }
}

void priorityScheduling(vector<Process> processes) {
    int currentTime = 0;
    int completed = 0;
    int n = processes.size();

    while (completed < n) {
        int idx = -1;
        int minPriority = 1e9;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime &&
                !processes[i].isCompleted &&
                processes[i].priority < minPriority) {

                idx = i;
                minPriority = processes[i].priority;
            }
        }

        if (idx == -1) {
            currentTime++;
            continue;
        }

        processes[idx].responseTime =
            currentTime - processes[idx].arrivalTime;

        currentTime += processes[idx].burstTime;

        processes[idx].completionTime = currentTime;

        processes[idx].turnaroundTime =
            processes[idx].completionTime - processes[idx].arrivalTime;

        processes[idx].waitingTime =
            processes[idx].turnaroundTime - processes[idx].burstTime;

        processes[idx].isCompleted = true;
        completed++;
    }

    cout << "--- Priority ---" << endl;
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
    sjf(processes);
    priorityScheduling(processes);

    return 0;
}