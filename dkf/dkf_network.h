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
        Eigen::MatrixXd Q;
        
        bool first_meas = true;
        dkf::Meas meas_last;
        
        Network();
        
        void next_meas(dkf::Meas meas);
        
        Eigen::MatrixXd getProcessVar();
        Eigen::MatrixXd getInitialVar();
        void init_x_P_forall(Eigen::VectorXd x, Eigen::MatrixXd P);
        void setneigh_forall() {
            for (int i = 0; i < nodes.size(); i++) {
                nodes[i].setmyneigh(neighbors[i]);
                nodes[i].setSizeBuffer((int) nodes.size());
            }
        }
        
        void publishmeas_forneigh(dkf::Meas meas, std::function<Eigen::Vector3cd(Eigen::VectorXcd)> h);
        void checkekf_p1_forall();
        void publisheita_forneigh();
        void checkekf_p2_forall(std::function<Eigen::VectorXcd(Eigen::VectorXcd)> fstate,
                                Eigen::MatrixXd Q);
        
        Eigen::VectorXcd processFcn(Eigen::VectorXcd s, double dt);
        Eigen::Vector3cd measurementFcn(Eigen::VectorXcd s, dkf::Meas meas);
        
    };
}

#endif
