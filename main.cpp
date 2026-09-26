#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string>
#include <iomanip>
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

struct Slice {
    int id;
    int start;
    int end;
};

struct Result {
    string name;
    double avgWaiting;
    double avgTurnaround;
    double avgResponse;
};

void printGantt(const vector<Slice> &timeline) {
    cout << "Gantt Chart:" << endl;

    // Top row
    for (const Slice &s : timeline) {
        if (s.id == -1) {
            cout << "| idle ";
        } else {
            cout << "| P" << s.id << " ";
        }
    }
    cout << "|" << endl;

    // Bottom row
    if (!timeline.empty()) {
        cout << timeline[0].start;
        cout << string(5 - to_string(timeline[0].start).size(), ' ');

        for (const Slice &s : timeline) {
            cout << s.end;

            int width = to_string(s.end).size();
            cout << string(5 - width, ' ');
        }

        cout << endl;
    }
}

Result fcfs(vector<Process> processes) {
    sort(processes.begin(), processes.end(),
         [](Process a, Process b) {
             return a.arrivalTime < b.arrivalTime;
         });

    int currentTime = 0;
    vector<Slice> timeline;

    double totalWaiting = 0;
    double totalTurnaround = 0;
    double totalResponse = 0;

    for (Process &p : processes) {

        if (currentTime < p.arrivalTime) {
            timeline.push_back({
                -1,
                currentTime,
                p.arrivalTime
            });

            currentTime = p.arrivalTime;
        }

        p.responseTime =
            currentTime - p.arrivalTime;

        int startTime = currentTime;

        currentTime += p.burstTime;

        timeline.push_back({
            p.id,
            startTime,
            currentTime
        });

        p.completionTime = currentTime;

        p.turnaroundTime =
            p.completionTime - p.arrivalTime;

        p.waitingTime =
            p.turnaroundTime - p.burstTime;

        totalWaiting += p.waitingTime;
        totalTurnaround += p.turnaroundTime;
        totalResponse += p.responseTime;
    }

    cout << "--- FCFS ---" << endl;
    cout << "PID  AT  BT  CT  TAT  WT  RT" << endl;

    for (const Process &p : processes) {
        cout << " " << p.id << "   "
             << p.arrivalTime << "   "
             << p.burstTime << "   "
             << p.completionTime << "   "
             << p.turnaroundTime << "    "
             << p.waitingTime << "   "
             << p.responseTime << endl;
    }

    cout << endl;
    printGantt(timeline);
    cout << endl;

    int n = processes.size();

    return {
        "FCFS",
        totalWaiting / n,
        totalTurnaround / n,
        totalResponse / n
    };
}

Result sjf(vector<Process> processes) {
    int currentTime = 0;
    int completed = 0;
    int n = processes.size();

    vector<Slice> timeline;

    double totalWaiting = 0;
    double totalTurnaround = 0;
    double totalResponse = 0;

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
            int idleStart = currentTime;
            currentTime++;

            if (!timeline.empty() &&
                timeline.back().id == -1) {

                timeline.back().end = currentTime;
            } else {
                timeline.push_back({
                    -1,
                    idleStart,
                    currentTime
                });
            }

            continue;
        }

        processes[idx].responseTime =
            currentTime -
            processes[idx].arrivalTime;

        int startTime = currentTime;

        currentTime += processes[idx].burstTime;

        timeline.push_back({
            processes[idx].id,
            startTime,
            currentTime
        });

        processes[idx].completionTime =
            currentTime;

        processes[idx].turnaroundTime =
            processes[idx].completionTime -
            processes[idx].arrivalTime;

        processes[idx].waitingTime =
            processes[idx].turnaroundTime -
            processes[idx].burstTime;

        totalWaiting += processes[idx].waitingTime;
        totalTurnaround += processes[idx].turnaroundTime;
        totalResponse += processes[idx].responseTime;

        processes[idx].isCompleted = true;
        completed++;
    }

    cout << "--- SJF ---" << endl;
    cout << "PID  AT  BT  CT  TAT  WT  RT" << endl;

    for (const Process &p : processes) {
        cout << " " << p.id << "   "
             << p.arrivalTime << "   "
             << p.burstTime << "   "
             << p.completionTime << "   "
             << p.turnaroundTime << "    "
             << p.waitingTime << "   "
             << p.responseTime << endl;
    }

    cout << endl;
    printGantt(timeline);
    cout << endl;

    return {
        "SJF",
        totalWaiting / n,
        totalTurnaround / n,
        totalResponse / n
    };
}

Result priorityScheduling(vector<Process> processes) {
    int currentTime = 0;
    int completed = 0;
    int n = processes.size();

    vector<Slice> timeline;

    double totalWaiting = 0;
    double totalTurnaround = 0;
    double totalResponse = 0;

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
            int idleStart = currentTime;
            currentTime++;

            if (!timeline.empty() &&
                timeline.back().id == -1) {

                timeline.back().end = currentTime;
            } else {
                timeline.push_back({
                    -1,
                    idleStart,
                    currentTime
                });
            }

            continue;
        }

        processes[idx].responseTime =
            currentTime -
            processes[idx].arrivalTime;

        int startTime = currentTime;

        currentTime += processes[idx].burstTime;

        timeline.push_back({
            processes[idx].id,
            startTime,
            currentTime
        });

        processes[idx].completionTime =
            currentTime;

        processes[idx].turnaroundTime =
            processes[idx].completionTime -
            processes[idx].arrivalTime;

        processes[idx].waitingTime =
            processes[idx].turnaroundTime -
            processes[idx].burstTime;

        totalWaiting += processes[idx].waitingTime;
        totalTurnaround += processes[idx].turnaroundTime;
        totalResponse += processes[idx].responseTime;

        processes[idx].isCompleted = true;
        completed++;
    }

    cout << "--- Priority ---" << endl;
    cout << "PID  AT  BT  CT  TAT  WT  RT" << endl;

    for (const Process &p : processes) {
        cout << " " << p.id << "   "
             << p.arrivalTime << "   "
             << p.burstTime << "   "
             << p.completionTime << "   "
             << p.turnaroundTime << "    "
             << p.waitingTime << "   "
             << p.responseTime << endl;
    }

    cout << endl;
    printGantt(timeline);
    cout << endl;

    return {
        "Priority",
        totalWaiting / n,
        totalTurnaround / n,
        totalResponse / n
    };
}

Result roundRobin(vector<Process> processes, int quantum) {
    int currentTime = 0;
    int completed = 0;
    int n = processes.size();

    queue<int> q;
    vector<bool> inQueue(n, false);
    vector<Slice> timeline;

    double totalWaiting = 0;
    double totalTurnaround = 0;
    double totalResponse = 0;

    // Initialize remaining time
    for (Process &p : processes) {
        p.remainingTime = p.burstTime;
    }

    // Add processes arriving at time 0
    for (int i = 0; i < n; i++) {
        if (processes[i].arrivalTime == 0) {
            q.push(i);
            inQueue[i] = true;
        }
    }

    while (completed < n) {

        // CPU is idle
        if (q.empty()) {
            int idleStart = currentTime;
            currentTime++;

            for (int i = 0; i < n; i++) {
                if (processes[i].arrivalTime <= currentTime &&
                    processes[i].remainingTime > 0 &&
                    !inQueue[i]) {

                    q.push(i);
                    inQueue[i] = true;
                }
            }

            if (!timeline.empty() &&
                timeline.back().id == -1) {

                timeline.back().end = currentTime;
            } else {
                timeline.push_back({
                    -1,
                    idleStart,
                    currentTime
                });
            }

            continue;
        }

        int idx = q.front();
        q.pop();
        inQueue[idx] = false;

        // First time process gets CPU
        if (processes[idx].responseTime == -1) {
            processes[idx].responseTime =
                currentTime -
                processes[idx].arrivalTime;
        }

        int startTime = currentTime;

        int runTime =
            min(quantum,
                processes[idx].remainingTime);

        currentTime += runTime;

        processes[idx].remainingTime -= runTime;

        // Every quantum gets its own slice
        timeline.push_back({
            processes[idx].id,
            startTime,
            currentTime
        });

        // Add newly arrived processes
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime &&
                processes[i].remainingTime > 0 &&
                !inQueue[i] &&
                i != idx) {

                q.push(i);
                inQueue[i] = true;
            }
        }

        // Process finished
        if (processes[idx].remainingTime == 0) {

            processes[idx].completionTime =
                currentTime;

            processes[idx].turnaroundTime =
                processes[idx].completionTime -
                processes[idx].arrivalTime;

            processes[idx].waitingTime =
                processes[idx].turnaroundTime -
                processes[idx].burstTime;

            totalWaiting += processes[idx].waitingTime;
            totalTurnaround += processes[idx].turnaroundTime;
            totalResponse += processes[idx].responseTime;

            processes[idx].isCompleted = true;

            completed++;
        }

        // Process still has work
        else {
            q.push(idx);
            inQueue[idx] = true;
        }
    }

    cout << "--- Round Robin ---" << endl;
    cout << "PID  AT  BT  CT  TAT  WT  RT" << endl;

    for (const Process &p : processes) {
        cout << " " << p.id << "   "
             << p.arrivalTime << "   "
             << p.burstTime << "   "
             << p.completionTime << "   "
             << p.turnaroundTime << "    "
             << p.waitingTime << "   "
             << p.responseTime << endl;
    }

    cout << endl;
    printGantt(timeline);
    cout << endl;

    return {
        "Round Robin",
        totalWaiting / n,
        totalTurnaround / n,
        totalResponse / n
    };
}

void printComparison(const vector<Result> &results) {
    cout << "==============================================" << endl;
    cout << "         SCHEDULING COMPARISON" << endl;
    cout << "==============================================" << endl;

    cout << left
         << setw(15) << "Algorithm"
         << setw(18) << "Avg Waiting"
         << setw(20) << "Avg Turnaround"
         << setw(15) << "Avg Response"
         << endl;

    cout << "--------------------------------------------------------------"
         << endl;

    cout << fixed << setprecision(2);

    for (const Result &r : results) {
        cout << left
             << setw(15) << r.name
             << setw(18) << r.avgWaiting
             << setw(20) << r.avgTurnaround
             << setw(15) << r.avgResponse
             << endl;
    }

    cout << endl;
}

int main() {
vector<Process> processes = {
    {1, 0, 5, 3},
    {2, 1, 3, 4},
    {3, 2, 8, 1},
    {4, 3, 6, 2}
};

    Result fcfsResult =
        fcfs(processes);

    Result sjfResult =
        sjf(processes);

    Result priorityResult =
        priorityScheduling(processes);

    Result rrResult =
        roundRobin(processes, 2);

    vector<Result> results = {
        fcfsResult,
        sjfResult,
        priorityResult,
        rrResult
    };

    printComparison(results);

    return 0;
}