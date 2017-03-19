#ifndef dkf_network_h
#define dkf_network_h

#include <vector>
#include <Eigen/Dense>
#include "dkf_node.h"
#include "dkf_meas.h"

namespace dkf
{
    class Network
    {
    public:
        std::vector<Node> nodes;
        std::vector<std::vector<int>> neighbors;
        
        bool first_meas = true;
        dkf::Meas meas_last;
        
        Network();
        
        void next_meas(dkf::Meas meas);
        
        Eigen::MatrixXd getInitialVar();
        void init_x_P_forall(Eigen::VectorXd x, Eigen::MatrixXd P);
        void setneigh_forall() {
            for (int i = 0; i < nodes.size(); i++) {
                nodes[i].setSizeBuffer((int) nodes.size());
            }
        }
        
        void publishmeas_forneigh(dkf::Meas meas, std::function<Eigen::Vector3cd(Eigen::VectorXcd)> h);
        void checkekf_p1_forall();
        
        Eigen::Vector3cd measurementFcn(Eigen::VectorXcd s, dkf::Meas meas);
        
    };
}

#endif
