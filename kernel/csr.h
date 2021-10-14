#pragma once

#include "stdint.h"
using namespace std;

typedef uint32_t vid_t;

enum enumGraph {
    eCSR = 0,
};

class csr_t {
 public:
    vid_t  v_count;
    vid_t  e_count;
    vid_t  dst_size;
    vid_t* offset;
    vid_t* nebrs;
    int64_t flag;

 public:
    csr_t() {
        //TODO initialize the members to a default value
    };
    void init(vid_t a_vcount, vid_t a_dstsize, void* a_offset, void* a_nebrs, int64_t a_flag, vid_t edge_count) {
        v_count = a_vcount;
        dst_size = a_dstsize;
        offset = (vid_t*)a_offset;
        nebrs = (vid_t*)a_nebrs;
        e_count = offset[edge_count];
        flag = a_flag;

    }
    vid_t get_vcount() {
        return v_count;
    }
    vid_t get_ecount() {
        return e_count;
    }
    vid_t get_degree(vid_t index) {
        return offset[index + 1] - offset[index];
    }

    vid_t* get_offset(){
        return offset;
    }

    vid_t* get_nebrs(){
        return nebrs;
    }
};

class edge_t {
 public:
     vid_t src;
     vid_t dst;
};

class coo_t {
 public:
     edge_t* edges;
     vid_t dst_size;
     vid_t v_count;
     vid_t e_count;
     coo_t() {
         edges = 0;
         dst_size = 0;
         v_count = 0;
         e_count = 0;
     }
     void init(vid_t a_vcount, vid_t a_dstsize, vid_t a_ecount, edge_t* a_edges) {
         v_count = a_vcount;
         e_count = a_ecount;
         dst_size = a_dstsize;
         edges = a_edges;
     }
};

class graph_t {
 public:
    csr_t csr;
    csr_t csc;
    coo_t coo;
 public:
    void init(vid_t a_vcount, vid_t a_dstsize, void* a_offset, void* a_nebrs, void* a_offset1, void* a_nebrs1, int64_t flag, int64_t num_vcount) {
        // I guess we need to implement something here
        csr.init(a_vcount, a_dstsize, a_offset, a_nebrs, flag, num_vcount);
        // Okay nobody cares about CSC
    }

    vid_t get_vcount() {
        return csr.v_count;
    }
    vid_t get_edge_count() {
        return csr.e_count;
    }
};

