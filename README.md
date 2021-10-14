# Breadth-first Search
Repository for Assignment 3 of CS636 Big Data Systems Fall 2021

## Python BFS
The Python version is located in pygraph/example.py

## C++ BFS
The C++ versions (single-thread and multi-thread) are located in kernel/bfs.cpp. The single-thread version is written in top-down style and the multi-thread version is written in bottom-up style.

Single-thread version: **run_bfs(graph_t& g, vid_t root, bool print_result)**\
Multi-thread version: **run_bfs_ms(graph_t& g, vid_t root, bool print_result)**

### Testing the C++ version
The print_result argument needs to be **True** to print the number of nodes at each level. \
To test the running time of each C++ version, print_result needs to be **False** since the output of bottom-up BFS is a tree and counting the nodes at each level is much more costly beacuse we need to traverse the tree.
