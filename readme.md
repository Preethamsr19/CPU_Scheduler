# CPU Process Scheduling Simulator

A C++ simulator implementing four CPU scheduling algorithms, with
per-process metrics, Gantt chart visualization, and a comparative
benchmark across all four.

## Build and Run

```
g++ -std=c++17 main.cpp -o scheduler
./scheduler
```

## Algorithms

- **FCFS** — runs processes in arrival order, non-preemptive.
- **SJF** — picks the shortest burst among processes that have arrived.
- **Priority** — same selection loop, ordered by priority (lower value = higher priority).
- **Round Robin** — preemptive; each process gets a fixed quantum before returning to the queue.

## Metrics

For each process the simulator computes completion time, turnaround time,
waiting time and response time, and reports the average of each per algorithm.

## Sample Output

```
| P1 | P2 | P3 | P1 | P4 | P2 | P3 | P1 | P4 | P3 | P4 | P3 |
0    2    4    6    8    10   11   13   14   16   18   20   22
```

## Comparison

```
Algorithm      Avg Waiting   Avg Turnaround   Avg Response
FCFS           5.75          11.25            5.75
SJF            5.25          10.75            5.25
Priority       7.75          13.25            7.75
Round Robin    9.75          15.25            2.00
```

SJF gives the lowest average waiting time because always running the shortest available job keeps the other processes from waiting too long. Priority performs worse on this process set because it ignores burst length entirely — P2 gets priority and runs before longer jobs, while P3 has the lowest priority and waits until the end. Round Robin makes the opposite trade-off: its waiting time is highest because processes are repeatedly preempted and re-queued, but its response time is lowest at 2.00 because every process gets CPU time quickly. This makes Round Robin useful when responsiveness matters, while SJF can be more effective when reducing average waiting time is the priority.