//
//  main.cpp
//  dkf
//
//  Created by nlogn on 3/17/17.
//  Copyright © 2017 nesl. All rights reserved.
//

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "dkf.h"

int main(int argc, char *argv[])
{
    FILE *csv_file;
    
    dkf_init();
    
    csv_file = fopen("ntbtiming.csv", "r");
    
    double walltime;
    int nodei;
    int nodej;
    uint64_t seq;
    uint64_t t0;
    uint64_t t1;
    uint64_t t2;
    uint64_t t3;
    uint64_t t4;
    uint64_t t5;
    float discard;
    
    int limit = 1000;
    while(limit-- &&
          fscanf(csv_file,
                 "%lf, %d, %d, %" SCNu64 ", %" SCNu64 ", %" SCNu64 ", %" SCNu64 ", %" SCNu64 ", %" SCNu64 ", %" SCNu64 ", %f\n",
                 &walltime,
                 &nodei,
                 &nodej,
                 &seq,
                 &t0,
                 &t1,
                 &t2,
                 &t3,
                 &t4,
                 &t5,
                 &discard
                 )) {
              //printf("%lf, %d, %llu\n", walltime, nodei, t0);
              dkf_iterate(
                          walltime,
                          nodei,
                          nodej,
                          seq,
                          t0,
                          t1,
                          t2,
                          t3,
                          t4,
                          t5
                          );
          }
    _dkf_print_result();
    return 0;
}