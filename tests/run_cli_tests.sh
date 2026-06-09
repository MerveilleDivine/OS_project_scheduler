#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

BINARY="${SCHEDULER_BINARY:-$ROOT_DIR/scheduler}"
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

build_if_needed() {
    if [[ -x "$BINARY" ]]; then
        return
    fi

    echo "Compiling scheduler..."
    g++ -std=c++11 src/main.cpp src/Parser.cpp src/Scheduler.cpp src/Reporter.cpp -Iinclude -o scheduler
    BINARY="$ROOT_DIR/scheduler"
}

run_and_assert() {
    local context="$1"
    local expected="$2"
    shift 2

    echo "Testing $context..."
    local output
    output="$($@)"
    assert_contains "$output" "$expected" "$context"
    assert_contains "$output" "Average Waiting Time:" "$context includes waiting metric"
    assert_contains "$output" "Average Turnaround Time:" "$context includes turnaround metric"
    assert_contains "$output" "Average Response Time:" "$context includes response metric"
}

build_if_needed

run_and_assert "FCFS" \
    "Scheduling Method: First Come, First Served" \
    "$BINARY" -f "$INPUT_FILE" --algorithm fcfs

run_and_assert "SJF" \
    "Scheduling Method: Shortest Job First - Non-Preemptive" \
    "$BINARY" -f "$INPUT_FILE" --algorithm sjf

run_and_assert "Priority Scheduling" \
    "Scheduling Method: Priority Scheduling - Non-Preemptive" \
    "$BINARY" -f "$INPUT_FILE" --algorithm priority

run_and_assert "Round Robin" \
    "Scheduling Method: Round Robin - Quantum 2" \
    "$BINARY" -f "$INPUT_FILE" --algorithm rr --quantum 2

assert_contains "$($BINARY -f "$INPUT_FILE" --algorithm fcfs)" "Gantt Chart:" "Gantt chart output"

echo "Testing output file generation..."
REPORT_FILE="$(mktemp)"
"$BINARY" -f "$INPUT_FILE" -o "$REPORT_FILE" --algorithm fcfs > /tmp/scheduler_fcfs_stdout.txt
REPORT_OUTPUT="$(cat "$REPORT_FILE")"
assert_contains "$REPORT_OUTPUT" "Scheduling Method: First Come, First Served" "output file method name"
assert_contains "$REPORT_OUTPUT" "Average Waiting Time:" "output file average waiting metric"
rm -f "$REPORT_FILE" /tmp/scheduler_fcfs_stdout.txt

echo "Testing comparison mode..."
COMPARE_OUTPUT="$($BINARY -f "$INPUT_FILE" --compare --quantum 2)"
assert_contains "$COMPARE_OUTPUT" "Scheduling Algorithm Comparison" "comparison title"
assert_contains "$COMPARE_OUTPUT" "First Come, First Served" "comparison FCFS row"
assert_contains "$COMPARE_OUTPUT" "Shortest Job First - Non-Preemptive" "comparison SJF row"
assert_contains "$COMPARE_OUTPUT" "Priority Scheduling - Non-Preemptive" "comparison Priority row"
assert_contains "$COMPARE_OUTPUT" "Round Robin - Quantum 2" "comparison RR row"

set +e
INVALID_OUTPUT="$($BINARY -f "$INPUT_FILE" --algorithm rr --quantum 0 2>&1)"
INVALID_STATUS=$?
set -e
if [[ $INVALID_STATUS -eq 0 ]]; then
    echo "Expected Round Robin with quantum 0 to fail."
    exit 1
fi
assert_contains "$INVALID_OUTPUT" "Round Robin quantum must be greater than 0" "invalid quantum error"

echo "All scheduler CLI tests passed."
