#include "dkf_meas.h"

namespace dkf
{
    Meas::Meas():
    walltime(0),
    nodei(0),
    nodej(0),
    seq(0),
    t0(0),
    t1(0),
    t2(0),
    t3(0),
    t4(0),
    t5(0),
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
    }
    
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
    t5(t5),
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

    Meas& Meas::operator=(const Meas& other)
    {
        this->walltime = other.walltime;
        this->nodei = other.nodei;
        this->nodej = other.nodej;
        this->seq = other.seq;
        this->t0 = other.t0;
        this->t1 = other.t1;
        this->t2 = other.t2;
        this->t3 = other.t3;
        this->t4 = other.t4;
        this->t5 = other.t5;
        this->d_ij = other.d_ij;
        this->r_ij = other.r_ij;
        this->R_ij = other.R_ij;
        this->T_rsp0 = other.T_rsp0;
        this->T_rsp1 = other.T_rsp1;
        this->T_rnd0 = other.T_rnd0;
        this->T_rnd1 = other.T_rnd1;
        this->t_bcn_tx = other.t_bcn_tx;
        this->t_bcn_rx = other.t_bcn_rx;
        this->t_i = other.t_i;
        this->t_j = other.t_j;
        return *this;
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
