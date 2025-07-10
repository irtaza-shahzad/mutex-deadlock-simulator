# Deadlock Simulation using Pthreads in C

This is a simulation of **resource allocation and deadlock** in a system with shared memory using POSIX threads (`pthreads`) and `mutex` locks.

## Overview

We simulate 5 shared resources (`R1` to `R5`) and 4 threads (`T1` to `T4`) that perform mathematical operations using these resources. Each thread locks a combination of resources in a fixed but different order — which creates a potential for deadlock.

## How It Works

- Each shared resource is protected using a `pthread_mutex_t`.
- Each thread performs basic mathematical operations using shared integers.
- Threads sleep before acquiring additional locks to increase the chance of a deadlock.
- A separate thread (`deadlock_func`) detects a deadlock if threads hang too long (e.g., 5 seconds).

## Threads & Resources

| Thread | Resources Locked         | Operation               |
|--------|--------------------------|--------------------------|
| T1     | R1 → R2 → R3             | `(R1 + R2) + R3`         |
| T2     | R2 → R3 → R4             | `(R2 - R3) - R4`         |
| T3     | R3 → R4 → R5             | `(R3 * R4) * R5`         |
| T4     | R4 → R5 → R1             | `(R4 / R5) / R1`         |

This ordering can lead to a **circular wait**, thus simulating a **deadlock** scenario.

## Example Output

```bash
Shared Resources: 
R1 = 2, R2 = 6, R3 = 12, R4 = 13, R5 = 12

[Thread 1]: Trying to lock R1
[Thread 2]: Trying to lock R2
[Thread 3]: Trying to lock R3
[Thread 4]: Trying to lock R4
...
[WARNING]: Deadlock Detected!
Exiting...
```

## Deadlock Simulation Method
***To simulate a realistic deadlock:***

- Each thread performs a sleep(1) after acquiring its first lock to let other threads interleave and acquire locks in a conflicting order.
- This creates a circular wait condition, the final ingredient for deadlock.

## Deadlock Detection
A separate thread is created that waits 5 seconds:

```bash
sleep(5);
printf("[WARNING]: Deadlock Detected! Exiting...\n");
exit(EXIT_FAILURE);
```
If the main threads are blocked due to circular wait, this thread will detect the stall and exit the program.

## File Structure
```bash
deadlock-sim/
│
├── deadlock-sim.c       # Source code
├── deadlock-sim         # Compiled binary
├── README.md            # Project documentation
├── .gitignore           # Git ignore rules
```
## Build and Run
```bash
gcc deadlock-sim.c -o deadlock-sim
./deadlock-sim
```
## Requirements
- GCC
- pthreads
- Linux/Unix-based system
