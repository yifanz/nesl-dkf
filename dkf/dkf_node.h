#ifndef dkf_node_h
#define dkf_node_h

#include <vector>
#include <string>
#include <Eigen/Dense>
#include "dkf_meas.h"
#include "dkf_log.h"

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
        std::vector<Eigen::VectorXd> eital;
        bool ready_to_ekf_p2 = false;
        std::vector<int> myneigh;
        Eigen::VectorXd c;
        bool state_ready = false;
        double G = 1;
        Eigen::Vector3d var_p{0,0,0};
        double var_co = 10e-12;
        double var_cb = 1.00;
        
        Node(int id, std::string name);
        
        void init_x_P(Eigen::VectorXd x, Eigen::MatrixXd P);
        void setAsReference();
        Eigen::Matrix<double, 5, 1> getInitialVar();
        Eigen::Matrix<double, 5, 1> getProcessVar();
        
        void setSizeBuffer(int size_R)
        {
            this->size_R = size_R;
        }
        
        void setmyneigh(std::vector<int> neighbors)
        {
            myneigh = neighbors;
            c.resize(neighbors.size());
            c.setZero(neighbors.size());
            for (int i = 0; i < neighbors.size(); i++) {
                c(i) = 1.0/(double)neighbors.size();
            }
//            DLOG(c);
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
        void seteital(Eigen::VectorXd eita);
        void checkekf_p2(std::function<Eigen::VectorXcd(Eigen::VectorXcd)> fstate,
                         Eigen::MatrixXd Q);
        void ekf_part2(std::function<Eigen::VectorXcd(Eigen::VectorXcd)> fstate,
                       Eigen::MatrixXd Q);
        void ekf_part3(std::function<Eigen::VectorXcd(Eigen::VectorXcd)> fstate,
                       Eigen::MatrixXd Q);
        void dif_ekf_p3(std::function<Eigen::VectorXcd(Eigen::VectorXcd)> fstate,
                        Eigen::MatrixXd Q);
        void jaccsd2(const std::function<Eigen::VectorXcd(Eigen::VectorXcd)> &fun,
                     const Eigen::VectorXd &x,
                     // returns z and A
                     Eigen::VectorXd &z,
                     Eigen::MatrixXd &A);
        void reseteital();
    };
    
}

#endif
