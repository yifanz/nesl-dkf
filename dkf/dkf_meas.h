#ifndef dkf_meas_h
#define dkf_meas_h

#include <stdint.h>
#include <Eigen/Dense>

namespace dkf
{
    class Meas
    {
    public:
        const double LIGHTSPEED = 299792458; // m/s
        // range var is about 0.05 m, so due to timing that's about 1.67e-10
        
        const double var_dij = 1e-12; // around -20 to converge on type 1 only
        const double var_rij = 1.00;
        const double var_Rij = 0.30;
        // cross terms
        const double var_dxr = 0;
        const double var_dxR = 0;
        const double var_rxR = 0;
        
        // measurement wall time
        double walltime = 0;
        // measurement source (i)
        int nodei = 0;
        // measurement destination (j)
        int nodej = 0;
        // processed measurements
        double d_ij = 0;
        double r_ij = 0;
        double R_ij = 0;
        // processed times
        uint64_t T_rsp0 = 0;
        uint64_t  T_rsp1 = 0;
        uint64_t  T_rnd0 = 0;
        uint64_t  T_rnd1 = 0;
        // beaconing raw times
        uint64_t t_bcn_tx = 0;
        uint64_t t_bcn_rx = 0;
        uint64_t t_i = 0;
        uint64_t t_j = 0;
        // sequence number
        uint64_t seq = 0;
        // is this a queued message?
        int queued = 0;
        uint64_t t0 = 0;
        uint64_t t1 = 0;
        uint64_t t2 = 0;
        uint64_t t3 = 0;
        uint64_t t4 = 0;
        uint64_t t5 = 0;

        Meas();
        
        explicit Meas(
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
        
        Meas& operator=(const Meas& other);
        
        Eigen::Vector3d vectorize();
        Eigen::Matrix3d getCovariance();
        
        int getSourceId()
        {
            return nodei;
        }
        
        int getDestId()
        {
            return nodej;
        }
        
        double getTime()
        {
            return walltime;
        }
        
    };
}

#endif
