#include "dkf_meas.h"

// TODO Copied directly from the Matlab code. What units are the raw timestamps in?
#define TO_SECONDS(t) (t / 63.8976 / 1e9)

namespace dkf
{
    const double Meas::LIGHTSPEED = 299792458;
    const double Meas::var_dij = 1e-12;
    const double Meas::var_rij = 1.00;
    const double Meas::var_Rij = 0.30;
    const double Meas::var_dxr = 0;
    const double Meas::var_dxR = 0;
    const double Meas::var_rxR = 0;
    
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
    t0(TO_SECONDS(t0)),
    t1(TO_SECONDS(t1)),
    t2(TO_SECONDS(t2)),
    t3(TO_SECONDS(t3)),
    t4(TO_SECONDS(t4)),
    t5(TO_SECONDS(t5)),
    d_ij(0),
    r_ij(0),
    R_ij(0),
    T_rsp0(0),
    T_rsp1(0),
    T_rnd0(0),
    T_rnd1(0),
    t_bcn_tx(0),
    t_bcn_rx(0),
    t_i(0),
    t_j(0)
    {
        t_bcn_tx = this->t0;
        t_bcn_rx = this->t1;
        t_j = this->t5;
        t_i = this->t4;
        
        // calculate processed measurements
        d_ij = this->t5 - this->t4;
        T_rnd0 = this->t3 - this->t0;
        T_rnd1 = this->t5 - this->t2;
        T_rsp0 = this->t2 - this->t1;
        T_rsp1 = this->t4 - this->t3;
        
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
