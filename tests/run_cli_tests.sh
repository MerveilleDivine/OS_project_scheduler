#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

BINARY="$ROOT_DIR/scheduler"
INPUT_FILE="$ROOT_DIR/examples/sample_input.txt"

assert_contains() {
    local output="$1"
    local expected="$2"
    local context="$3"

    if [[ "$output" != *"$expected"* ]]; then
        echo "Assertion failed for $context"
        echo "Expected to find: $expected"
        echo "Actual output:"
        echo "$output"
        exit 1
    fi
}

assert_matches() {
    local output="$1"
    local pattern="$2"
    local context="$3"

    if ! printf '%s\n' "$output" | grep -Eq "$pattern"; then
        echo "Assertion failed for $context"
        echo "Expected pattern: $pattern"
        echo "Actual output:"
        echo "$output"
        exit 1
    fi
}

run_scheduler() {
    local algorithm="$1"
    shift
    "$BINARY" -f "$INPUT_FILE" --algorithm "$algorithm" "$@"
}

echo "Compiling scheduler..."
g++ -std=c++11 scheduler.cpp -o scheduler

echo "Testing FCFS..."
FCFS_OUTPUT="$(run_scheduler fcfs)"
assert_contains "$FCFS_OUTPUT" "Scheduling Method: First Come, First Served" "FCFS method name"
assert_matches "$FCFS_OUTPUT" '^P1[[:space:]]+0[[:space:]]+5[[:space:]]+3[[:space:]]+0[[:space:]]+5[[:space:]]+0[[:space:]]+5[[:space:]]+0' "FCFS P1 metrics"
assert_matches "$FCFS_OUTPUT" '^P2[[:space:]]+1[[:space:]]+4[[:space:]]+2[[:space:]]+5[[:space:]]+9[[:space:]]+4[[:space:]]+8[[:space:]]+4' "FCFS P2 metrics"
assert_matches "$FCFS_OUTPUT" '^P5[[:space:]]+3[[:space:]]+3[[:space:]]+1[[:space:]]+16[[:space:]]+19[[:space:]]+13[[:space:]]+16[[:space:]]+13' "FCFS P5 metrics"
assert_contains "$FCFS_OUTPUT" "Average Waiting Time: 7.00 ms" "FCFS average waiting time"
assert_contains "$FCFS_OUTPUT" "Average Turnaround Time: 11.00 ms" "FCFS average turnaround time"

FCFS_REPORT="$(mktemp)"
"$BINARY" -f "$INPUT_FILE" -o "$FCFS_REPORT" --algorithm fcfs > /tmp/scheduler_fcfs_stdout.txt
FCFS_FILE_OUTPUT="$(cat "$FCFS_REPORT")"
assert_contains "$FCFS_FILE_OUTPUT" "Average Waiting Time: 7.00 ms" "FCFS file output"
rm -f "$FCFS_REPORT" /tmp/scheduler_fcfs_stdout.txt

echo "Testing SJF..."
SJF_OUTPUT="$(run_scheduler sjf)"
assert_contains "$SJF_OUTPUT" "Scheduling Method: Shortest Job First - Non-Preemptive" "SJF method name"
assert_contains "$SJF_OUTPUT" "Average Waiting Time: 6.40 ms" "SJF average waiting time"
assert_contains "$SJF_OUTPUT" "Average Turnaround Time: 10.20 ms" "SJF average turnaround time"


echo "Testing Priority Scheduling..."
PRIORITY_OUTPUT="$(run_scheduler priority)"
assert_contains "$PRIORITY_OUTPUT" "Scheduling Method: Priority Scheduling - Non-Preemptive" "Priority method name"
assert_contains "$PRIORITY_OUTPUT" "Average Waiting Time: 6.40 ms" "Priority average waiting time"
assert_contains "$PRIORITY_OUTPUT" "Average Turnaround Time: 10.20 ms" "Priority average turnaround time"


echo "Testing Round Robin..."
RR_OUTPUT="$(run_scheduler rr --quantum 2)"
assert_contains "$RR_OUTPUT" "Scheduling Method: Round Robin - Quantum 2" "Round Robin method name"
assert_contains "$RR_OUTPUT" "Average Waiting Time: 11.40 ms" "Round Robin average waiting time"
assert_contains "$RR_OUTPUT" "Average Turnaround Time: 15.20 ms" "Round Robin average turnaround time"


echo "Testing invalid quantum handling..."
set +e
INVALID_OUTPUT="$(run_scheduler rr --quantum 0 2>&1)"
INVALID_STATUS=$?
set -e
if [[ $INVALID_STATUS -eq 0 ]]; then
    echo "Expected Round Robin with quantum 0 to fail."
    exit 1
fi
assert_contains "$INVALID_OUTPUT" "Round Robin quantum must be greater than 0" "invalid quantum error"

echo "All scheduler CLI tests passed."
