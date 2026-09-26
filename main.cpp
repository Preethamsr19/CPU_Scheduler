#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
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
    int remainingTime = 0;
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

    cout << endl;
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

    cout << endl;
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

    cout << endl;
}

void roundRobin(vector<Process> processes, int quantum) {
    int currentTime = 0;
    int completed = 0;
    int n = processes.size();

    queue<int> q;
    vector<bool> inQueue(n, false);

    // Initialize remaining burst time
    for (Process &p : processes) {
        p.remainingTime = p.burstTime;
    }

    // Add processes that have arrived at time 0
    for (int i = 0; i < n; i++) {
        if (processes[i].arrivalTime == 0) {
            q.push(i);
            inQueue[i] = true;
        }
    }

    while (completed < n) {

        // If queue is empty, advance time until a process arrives
        if (q.empty()) {
            currentTime++;

            for (int i = 0; i < n; i++) {
                if (processes[i].arrivalTime <= currentTime &&
                    processes[i].remainingTime > 0 &&
                    !inQueue[i]) {

                    q.push(i);
                    inQueue[i] = true;
                }
            }

            continue;
        }

        // Get the first process from the queue
        int idx = q.front();
        q.pop();
        inQueue[idx] = false;

        // Set response time only on its first execution
        if (processes[idx].responseTime == -1) {
            processes[idx].responseTime =
                currentTime - processes[idx].arrivalTime;
        }

        // Run for one quantum or until process finishes
        int runTime = min(quantum, processes[idx].remainingTime);

        currentTime += runTime;
        processes[idx].remainingTime -= runTime;

        // Add processes that arrived during this time slice
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime &&
                processes[i].remainingTime > 0 &&
                !inQueue[i] &&
                i != idx) {

                q.push(i);
                inQueue[i] = true;
            }
        }

        // Check whether current process has finished
        if (processes[idx].remainingTime == 0) {

            processes[idx].completionTime = currentTime;

            processes[idx].turnaroundTime =
                processes[idx].completionTime -
                processes[idx].arrivalTime;

            processes[idx].waitingTime =
                processes[idx].turnaroundTime -
                processes[idx].burstTime;

            processes[idx].isCompleted = true;

            completed++;
        }
        else {
            // Process still has work, so put it at the back
            q.push(idx);
            inQueue[idx] = true;
        }
    }

    cout << "--- Round Robin ---" << endl;
    cout << "PID  AT  BT  CT  TAT  WT  RT" << endl;

    for (const Process &p : processes) {
        cout << " " << p.id << "   " << p.arrivalTime << "   " << p.burstTime
             << "   " << p.completionTime << "   " << p.turnaroundTime
             << "    " << p.waitingTime << "   " << p.responseTime << endl;
    }

    cout << endl;
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

    // Round Robin with time quantum = 2
    roundRobin(processes, 2);

    return 0;
}