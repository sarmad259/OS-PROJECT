# Systems Programming Collection

A collection of **C programs** demonstrating fundamental concepts in **network programming** and **operating system scheduling algorithms**.

---

## What This Is

### Network Programming Components

- **Multi-client Server:**  
  Demonstrates concurrent client handling using the `select()` system call for multiplexed I/O.

- **Single-client Server:**  
  Basic TCP server implementation for one-to-one communication.

- **Network Clients:**  
  Client programs for both local and remote server connections.

- **Real-time Communication:**  
  Interactive message exchange between clients and servers.

### CPU Scheduling Simulator

Implementation of 5 core CPU scheduling policies used in operating systems:

- **FCFS (First-Come-First-Served):** Processes execute in arrival order.
- **SJF (Shortest Job First):** Prioritizes processes with shortest execution time.
- **Priority Scheduling:** Executes highest priority processes first.
- **Round Robin:** Time-sliced execution with configurable quantum.
- **Priority Round Robin:** Combines priority-based selection with round-robin execution.

---

## Technical Concepts Demonstrated

### Network Programming

- TCP socket creation and management  
- Client-server architecture  
- Multiplexed I/O using `select()`  
- Connection handling and cleanup  
- Cross-network communication protocols  

### Operating Systems

- Process scheduling algorithms  
- Performance metrics calculation (waiting time, turnaround time)  
- Comparative algorithm analysis  
- File I/O and data parsing  
- Algorithm complexity demonstration  

---

## Purpose

Educational implementations showcasing practical applications of systems programming concepts commonly taught in computer science courses covering networks and operating systems.

---

Feel free to explore the source code for each component and understand the working through well-commented programs.

---
