#ifndef dkf_node_h
#define dkf_node_h

#include <vector>
#include <string>
#include <Eigen/Dense>
#include "dkf_meas.h"

namespace dkf
{
    class Node
    {
    public:
        int id;
        std::string name;
        std::vector<Eigen::Vector3d> yl;
        std::vector<Eigen::Matrix3d> Rl;
        std::vector<double> hl;
        std::vector<int> Rx;
        int size_R = 0;
        bool ready_to_ekf_p1 = false;
        
        Node(int id, std::string name);
        
        void init_x_P();
        
        void setSizeBuffer(int size_R)
        {
            size_R = size_R;
        }
        
        void set_meas(dkf::Meas meas, double h_dt_ref);
    };
}

#endif
