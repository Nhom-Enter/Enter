#include <iostream>
#include <queue>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Process {
    string name;
    int arrival, burst;
    int remaining;
    int waiting = 0;
    int turnaround = 0;
    int completion = 0;
};

struct Execution {
    string pname;
    int start, end;
};

bool cmpArrival(const Process& a, const Process& b) {
    return a.arrival < b.arrival;
}

int main() {
    int n, quantum;
    cout << "Nhap so tien trinh: ";
    cin >> n;
    cout << "Nhap quantum: ";
    cin >> quantum;

    vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        cout << "Nhap ten, burst, arrival cho tien trinh " << i + 1 << ": ";
        cin >> processes[i].name >> processes[i].burst >> processes[i].arrival;
        processes[i].remaining = processes[i].burst;
    }

    sort(processes.begin(), processes.end(), cmpArrival);

    queue<int> ready;
    vector<bool> visited(n, false);
    vector<Execution> gantt;

    int time = 0, done = 0;
    while (done < n) {
        for (int i = 0; i < n; i++) {
            if (!visited[i] && processes[i].arrival <= time) {
                ready.push(i);
                visited[i] = true;
            }
        }

        if (ready.empty()) {
            time++;
            continue;
        }

        int idx = ready.front();
        ready.pop();

        int start = time;
        int execTime = min(quantum, processes[idx].remaining);
        processes[idx].remaining -= execTime;
        time += execTime;
        gantt.push_back({ processes[idx].name, start, time });

        for (int i = 0; i < n; i++) {
            if (!visited[i] && processes[i].arrival <= time) {
                ready.push(i);
                visited[i] = true;
            }
        }

        if (processes[idx].remaining > 0) {
            ready.push(idx);
        }
        else {
            processes[idx].completion = time;
            processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
            processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
            done++;
        }
    }

    double avgWT = 0, avgTAT = 0;
    cout << "\nKet qua Round Robin Scheduling:\n";
    cout << left << setw(8) << "Tien"
        << setw(8) << "Burst"
        << setw(8) << "Arrival"
        << setw(12) << "Waiting"
        << setw(12) << "Turnaround"
        << setw(12) << "Completion" << endl;

    for (auto& p : processes) {
        cout << left << setw(8) << p.name
            << setw(8) << p.burst
            << setw(8) << p.arrival
            << setw(12) << p.waiting
            << setw(12) << p.turnaround
            << setw(12) << p.completion << endl;
        avgWT += p.waiting;
        avgTAT += p.turnaround;
    }

    cout << "\nThoi gian cho trung binh = " << avgWT / n;
    cout << "\nThoi gian quay vong trung binh = " << avgTAT / n << endl;

    cout << "\nGantt Chart:\n";
    for (auto& g : gantt) {
        cout << "| " << g.pname << " ";
    }
    cout << "|\n0";
    for (auto& g : gantt) {
        cout << setw(4) << g.end;
    }
    cout << endl;

    return 0;
}
