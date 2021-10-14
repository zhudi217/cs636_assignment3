#include <cassert>
#include <iostream>
#include "csr.h"

#include <list>
#include <algorithm>
#include <omp.h>

using std::cout;
using std::endl;

bool all_visited(bool *visited, vid_t v_count) {
    for (int i = 0 ; i < v_count ; i++) {
        if (!visited[i]) {
            return false;
        }
    }
    return true;
}

/*** Single-Thread BFS. Top-Down style ***/
void run_bfs(graph_t& g, vid_t root, bool print_result)
{
    csr_t* csr = &g.csr;
    csr_t* csc = &g.csc;
    
    //TODO
    vid_t* offset = csr->offset;
    vid_t* nebrs = csr->nebrs;

    // BFS algorithm
    vid_t v_count = csr->get_vcount();
    bool *visited = new bool[v_count];
    for (int i = 0 ; i < v_count ; i++) {
        visited[i] = false;
    }

    list<vid_t> frontiers;  // Current frontiers
    list<vid_t> new_frontiers;  // New frontiers

    list<vid_t> level_counter;  // Store the number of nodes at each level

    frontiers.push_back(root);

	while (!all_visited(visited, v_count)) {
	    int count = 0;

        while (!frontiers.empty()) {
	        vid_t node = frontiers.front();
	        if (visited[node] == false) {
	            visited[node] = true;
	            count += 1;

	            // Get the neighbor of the current node
                for (int j = offset[node] ; j < offset[node+1] ; j++) {
                    new_frontiers.push_back(nebrs[j]);
                }
	        }
	        frontiers.pop_front();
	    }

	    level_counter.push_back(count);

	    // Swap frontiers with new_frontiers
        swap(frontiers, new_frontiers);
	}   

    //print bfs tree here
    if (print_result) {
        cout << "*** Single-thread Top-Down BFS ***" << endl;
        int level = 0;
        while(!level_counter.empty()) {
            cout << "Level " << level++ << ": " << level_counter.front() << " nodes" << endl;
            level_counter.pop_front();
        }
        cout << endl;
    }
}

/*** Multi-thread version of BFS ***/
void bottom_up_BFS_print(vid_t *parents, vid_t root, vid_t v_count) {
    list<vid_t> queue;

    queue.push_back(root);
    vid_t *distances = new vid_t[v_count];
    for (int i = 0 ; i < v_count ; i++) {
        distances[i] = -1;
    }
    distances[root] = 0;

    while(!queue.empty()) {
        vid_t node = queue.front();
        for (int j = 0; j < v_count; j++) {
            if (parents[j]==node && distances[j]==-1) {
                distances[j] = distances[node] + 1;
                queue.push_back(j);
            }
        }
        queue.pop_front();
    }

    sort(distances, distances+v_count);

    vid_t level = 0;
    vid_t count = 0;
    for (int i = 0 ; i < v_count ; i++) {
        if (distances[i] == level) {
            count ++;
        } else {
            cout << "Level " << level << ": " << count << " nodes" << endl;
            level++;
            count = 1;
        }
    }
    cout << "Level " << level << ": " << count << " nodes" << endl;
}


bool check_frontiers(vid_t *frontiers, vid_t v_count) {
    for(vid_t i = 0; i < v_count ; i++) {
        if(frontiers[i] == 1) {
            return false;
        }
    }
    return true;
}

/*** Paralleled version of BFS. Bottom-up style ***/
void run_bfs_ms(graph_t& g, vid_t root, bool print_result) {
    csr_t* csr = &g.csr;
    csr_t* csc = &g.csc;
    
    //TODO
    vid_t* offset = csr->offset;
    vid_t* nebrs = csr->nebrs;

    vid_t v_count = csr->get_vcount();

    // Define variables
    vid_t *frontiers = new vid_t[v_count];      // Using fixed length array here for parallelism
    vid_t *new_frontiers = new vid_t[v_count];
    vid_t *parents = new vid_t[v_count];

    // Initialization
    for(vid_t i = 0; i < v_count ; i++) {
        frontiers[i] = -1;
        new_frontiers[i] = -1;
        parents[i] = -1;
    }
    frontiers[root] = 1;

    // Iterate over the graph, bottom-up style, use arrays rather than dynamic lists
    while (!check_frontiers(frontiers, v_count)) {
        #pragma omp parallel
        {
            #pragma omp for
            for(int i = 0 ; i < v_count ; i++) {    // loop over all the vertices. i is the current node
                if(parents[i] == -1) {
                    // Get the neighbor of the current node
                    for (int j = offset[i]; j < offset[i+1]; j++) {
                        vid_t node = nebrs[j];
                        if(frontiers[node] == 1) {
                            parents[i] = node;
                            new_frontiers[i] = 1;
                            break;
                        }
                    }            
                }
            }
        }
        swap(frontiers, new_frontiers);
        // Clear new_frontiers
        for(vid_t i = 0; i < v_count ; i++) {
            new_frontiers[i] = -1;
        }
    }

    // Print the result of BFS
    if (print_result) {
        cout << "*** Multi-thread Bottom-up BFS ***" << endl;
        bottom_up_BFS_print(parents, root, v_count);
        cout << endl;
    }
}
