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
        std::vector<std::function<Eigen::Vector3cd(Eigen::VectorXcd)>> hl;
        std::vector<int> Rx;
        int size_R = 0;
        bool ready_to_ekf_p1 = false;
        bool ekf_p1_done = false;
        Eigen::VectorXd x;
        Eigen::MatrixXd P;
        Eigen::Vector3d vari_p;
        double vari_co;
        double vari_cb;
        Eigen::VectorXd eita;
        
        Node(int id, std::string name);
        
        void init_x_P(Eigen::VectorXd x, Eigen::MatrixXd P);
        void setAsReference();
        Eigen::Matrix<double, 5, 1> getInitialVar();
        
        void setSizeBuffer(int size_R)
        {
            this->size_R = size_R;
        }
        
        void set_meas(dkf::Meas meas, std::function<Eigen::Vector3cd(Eigen::VectorXcd)> h);
        
        void checkekf_p1();
        void efk_part1();
        void dif_ekf_p1();
        void jaccsd(const std::function<Eigen::Vector3cd(Eigen::VectorXcd)> &fun,
                    const Eigen::VectorXd &x,
                    // returns z and A
                    Eigen::Vector3d &z,
                    Eigen::Matrix<double, 3, Eigen::Dynamic> &A);
    };
    
}

#endif
