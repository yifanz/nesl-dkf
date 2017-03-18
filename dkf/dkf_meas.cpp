#include "dkf_meas.h"

namespace dkf
{
    Meas::Meas(
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
               ):
    walltime(walltime),
    nodei(nodei),
    nodej(nodej),
    seq(seq),
    t0(t0),
    t1(t1),
    t2(t2),
    t3(t3),
    t4(t4),
    t5(t5)
    {
        t_bcn_tx = t0;
        t_bcn_rx = t1;
        t_j = t5;
        t_i = t4;
        
        // calculate processed measurements
        d_ij = t5 - t4;
        T_rnd0 = t3-t0;
        T_rnd1 = t5-t2;
        T_rsp0 = t2-t1;
        T_rsp1 = t4-t3;
        
        r_ij = (LIGHTSPEED/2)*(T_rnd1 - T_rsp1);
        R_ij = (LIGHTSPEED)*(T_rnd0*T_rnd1 - T_rsp0*T_rsp1)/(T_rnd0 + T_rnd1 + T_rsp0 + T_rsp1);
    }
    
    Eigen::Vector3d Meas::vectorize()
    {
        Eigen::Vector3d z(d_ij, r_ij, R_ij);
        
        return z;
    }
    
    Eigen::Matrix3d Meas::getCovariance()
    {
        Eigen::Matrix3d r;
        
        r <<
        var_dij, var_rxR, var_dxR,
        var_rxR, var_rij, var_rxR,
        var_dxR, var_rxR, var_Rij;
        
        return r;
    }
    
}
