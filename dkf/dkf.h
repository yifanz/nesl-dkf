#ifndef dkf_h
#define dkf_h

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    void dkf_init();

    void dkf_iterate(
            double walltime,
            int nodei,
            int nodej,
            uint64_t seq,
            uint64_t t0,
            uint64_t t1,
            uint64_t t2,
            uint64_t t3,
            uint64_t t4,
            uint64_t t5
            );
    
    // Debugging
    void _dkf_print_result();

#ifdef __cplusplus
}
#endif

#endif
