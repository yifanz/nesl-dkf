#ifndef dkf_meas_h
#define dkf_meas_h

#include <stdint.h>
#include <Eigen/Dense>

namespace dkf
{
    class Meas
    {
    public:
        static const double LIGHTSPEED; // m/s
        // range var is about 0.05 m, so due to timing that's about 1.67e-10
        
        static const double var_dij; // around -20 to converge on type 1 only
        static const double var_rij;
        static const double var_Rij;
        // cross terms
        static const double var_dxr;
        static const double var_dxR;
        static const double var_rxR;
        
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
        double T_rsp0 = 0;
        double  T_rsp1 = 0;
        double  T_rnd0 = 0;
        double  T_rnd1 = 0;
        // beaconing raw times
        double t_bcn_tx = 0;
        double t_bcn_rx = 0;
        double t_i = 0;
        double t_j = 0;
        // sequence number
        uint64_t seq = 0;
        // is this a queued message?
        int queued = 0;
        double t0 = 0;
        double t1 = 0;
        double t2 = 0;
        double t3 = 0;
        double t4 = 0;
        double t5 = 0;

        Meas() = default;

        Meas(
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
