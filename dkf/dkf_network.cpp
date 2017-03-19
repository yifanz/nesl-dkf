#include "dkf_network.h"
#include "dkf_log.h"

#include <vector>
#include "dkf_helpers.hpp"

#define LIGHTSPEED 299792458.0

namespace dkf
{
    Network::Network()
    {
        nodes.emplace_back(0, "ntb-alpha");
        nodes.emplace_back(1, "ntb-bravo");
        nodes.emplace_back(2, "ntb-charlie");
        nodes.emplace_back(3, "ntb-delta");
        nodes.emplace_back(4, "ntb-echo");
        nodes.emplace_back(5, "ntb-foxtrot");
        nodes.emplace_back(6, "ntb-golf");
        nodes.emplace_back(7, "ntb-hotel");
        
        // Fully connected
        neighbors = {
            {0,1,2,3,4,5,6,7},
            {0,1,2,3,4,5,6,7},
            {0,1,2,3,4,5,6,7},
            {0,1,2,3,4,5,6,7},
            {0,1,2,3,4,5,6,7},
            {0,1,2,3,4,5,6,7},
            {0,1,2,3,4,5,6,7},
            {0,1,2,3,4,5,6,7}
        };
        
        DLOG("Network Constructed");
    }
    
    void Network::next_meas(dkf::Meas meas)
    {
        if (first_meas) {
            first_meas = false;
            meas_last = meas;
        } else {
            double dt_ref = meas.getTime() - meas_last.getTime();
            meas_last = meas;
            
            auto h = [=](Eigen::VectorXcd s) {
                return this->measurementFcn(s, meas);
            };
            
            publishmeas_forneigh(meas, h);
            checkekf_p1_forall();
        }
        
    }
    
    void Network::init_x_P_forall(Eigen::VectorXd x, Eigen::MatrixXd P)
    {
        for (int k = 0; k < nodes.size(); k++) {
            nodes[k].init_x_P(x, P);
        }
    }
    
    Eigen::MatrixXd Network::getInitialVar()
    {
        Eigen::VectorXd diag;
        diag.resize(nodes.size()*5, 1);
        diag.setZero(nodes.size()*5);
        
        for (int k = 0; k < nodes.size(); k++) {
            auto initialVar = nodes[k].getInitialVar();
            diag(k*5) = initialVar(0);
            diag(k*5+1) = initialVar(1);
            diag(k*5+2) = initialVar(2);
            diag(k*5+3) = initialVar(3);
            diag(k*5+4) = initialVar(4);
        }
        
        return diag.asDiagonal();
    }
    
    void Network::publishmeas_forneigh(dkf::Meas meas,
                                       std::function<Eigen::Vector3cd(Eigen::VectorXcd)> h)
    {
        int srcIndex = meas.getSourceId();
        //int destIndex = meas.getDestId();
        
        // Assume everyone is a neighbor to every
        for (int i : neighbors[srcIndex]) {
            nodes[i].set_meas(meas, h);
        }
    }
    
    void Network::checkekf_p1_forall()
    {
        for (int i = 0; i < nodes.size(); i++) {
            nodes[i].checkekf_p1();
        }
    }
    
    Eigen::Vector3cd Network::measurementFcn(Eigen::VectorXcd s, dkf::Meas meas)
    {
        int srcIdx = meas.getSourceId();
        int dstIdx = meas.getDestId();
        int stateSize = 5;
        int start_i = srcIdx * stateSize;
        int start_j = dstIdx * stateSize;
        
        Eigen::Vector3cd pi(s(start_i), s(start_i+1), s(start_i+2));
        Eigen::Vector3cd pj(s(start_j), s(start_j+1), s(start_j+2));
        std::complex<double> oi = s(start_i+3);
        std::complex<double> oj = s(start_j+3);
        std::complex<double> bi = s(start_i+4);
        std::complex<double> bj = s(start_j+4);
        
        Eigen::Vector3cd y;

        // [y; ( sqrt(sum((pj-pi).^2)) + 2.0 )/c + (oj - oi)];
        Eigen::Vector3cd _tmp_vec3 = pj-pi;
        _tmp_vec3 = _tmp_vec3.array().pow(2).matrix();
        std::complex<double> p_ij = zbound(_tmp_vec3.sum());
        p_ij = sqrt(p_ij);
        y(0) = ((p_ij + 2.0)/LIGHTSPEED) + (oj-oi);
        
        // [y; sqrt(sum((pj-pi).^2)) + (c*meas.T_rsp1/2)*(bj - bi)*1e-9];
        y(1) = p_ij + (LIGHTSPEED * meas.T_rsp1/2)*(bj-bi)*1e-9;
        
        // err = c*( 1e-9*(bi-bj)*(meas.T_rnd0*meas.T_rnd1 - meas.T_rsp0*meas.T_rsp1 ))/( (1+ 1e-9*(bi-bj))*meas.T_rnd0 + meas.T_rnd1 + meas.T_rsp0 + (1+ 1e-9*(bi-bj))*meas.T_rsp1 );
        // y = [y; sqrt(sum((pj-pi).^2)) + err ];
        std::complex<double> err = LIGHTSPEED*( 1e-9*(bi-bj)*(meas.T_rnd0*meas.T_rnd1 - meas.T_rsp0*(double)meas.T_rsp1 ))/( (1.+ 1e-9*(bi-bj))*(double)meas.T_rnd0 + (double)meas.T_rnd1 + (double)meas.T_rsp0 + (1. + 1e-9*(bi-bj))*(double)meas.T_rsp1 );
        y(2) = p_ij + err;
        
        //DLOG(y);

        return y;
    }
    
}
