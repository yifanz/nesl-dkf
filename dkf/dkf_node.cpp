#include "dkf_node.h"
#include "dkf_log.h"
#include <complex>
#include "dkf_helpers.hpp"

#define MATLAB_EPS 2.220446049250313e-016

namespace dkf
{
    Node::Node(int id, std::string name):
    id(id), name(name)
    {
        vari_p << 5., 5., 5.;
        vari_co = 1e-3;
        vari_cb = 1.;
    }
    
    void Node::init_x_P(Eigen::VectorXd x, Eigen::MatrixXd P)
    {
        this->x = x;
        this->P = P;
    }
    
    void Node::setAsReference()
    {
        vari_p << 1e-14, 1e-14, 1e-14;
        vari_co = 1e-14;
        vari_cb = 1e-14;
    }
    
    Eigen::Matrix<double, 5, 1> Node::getInitialVar()
    {
        Eigen::Matrix<double, 5, 1> q;
        q << vari_p(0), vari_p(1), vari_p(2), vari_co, vari_cb;
        
        return q;
    }
    
    Eigen::Matrix<double, 5, 1> Node::getProcessVar()
    {
        Eigen::Matrix<double, 5, 1> q;
        q << var_p(0), var_p(1), var_p(2), var_co, var_cb;
        
        return q;
    }
    
    void Node::set_meas(dkf::Meas meas, std::function<Eigen::Vector3cd(Eigen::VectorXcd)> h)
    {
        yl.push_back(meas.vectorize());
        Rl.push_back(meas.getCovariance());
        
        hl.push_back(h);
        Rx.push_back(1);
        
        if (Rx.size() == size_R) {
            ready_to_ekf_p1 = true;
        }
    }
    
    void Node::checkekf_p1()
    {
        if (ready_to_ekf_p1 && !ekf_p1_done) {
            efk_part1();
        }
    }
    
    void Node::efk_part1()
    {
        // [obj.eita,obj.P]=dif_ekf_p1(obj.x,obj.P,obj.hl,obj.Rl,obj.yl);
        dif_ekf_p1();
    }
    
    void Node::dif_ekf_p1()
    {
        Eigen::Vector3d h;
        Eigen::Matrix<double, 3, Eigen::Dynamic> H_cap;
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> sum;
        Eigen::MatrixXd P_next;
        
        /*
         sum=0;
         for i=1:length( Rl)
         [ h, H_cap]= jaccsd(hmeas{i},x);
         sum = sum + conj(H_cap)'*( Rl{i}^-1)*H_cap;
         end
         sum = sum + (P)^-1 ;
         P_next=(sum)^-1;
         */
        sum.resize(x.rows(), x.rows());
        sum.setZero(x.rows(), x.rows());
        for (int k = 0; k < Rl.size(); k++) {
            jaccsd(hl[k], x, h, H_cap);
            sum = sum + H_cap.conjugate().transpose()*(Rl[k].inverse())*H_cap;
//            DLOG(sum(0,0), " ", sum(0,1), " ", sum(0,2), " ", sum(0,3), " ", sum(0,4));
//            DLOG(sum(1,0), " ", sum(1,1), " ", sum(1,2), " ", sum(1,3), " ", sum(1,4));
//            DLOG(sum(3,0), " ", sum(3,1), " ", sum(3,2), " ", sum(3,3), " ", sum(3,4));
        }
//        DLOG(sum(0,0), " ", sum(0,1), " ", sum(0,2), " ", sum(0,3), " ", sum(0,4));
//        DLOG(sum(1,0), " ", sum(1,1), " ", sum(1,2), " ", sum(1,3), " ", sum(1,4));
//        DLOG(sum(3,0), " ", sum(3,1), " ", sum(3,2), " ", sum(3,3), " ", sum(3,4));
        sum = sum + P.inverse();
        P_next = sum.inverse();
//        DLOG(P_next(0,0), " ", P_next(0,1), " ", P_next(0,2), " ", P_next(0,3), " ", P_next(0,4));
//        DLOG(P_next(1,0), " ", P_next(1,1), " ", P_next(1,2), " ", P_next(1,3), " ", P_next(1,4));
//        DLOG(P_next(3,0), " ", P_next(3,1), " ", P_next(3,2), " ", P_next(3,3), " ", P_next(3,4));
        /*
         sum=0;
         for i=1:length( Rl)
         [ h, H_cap]= jaccsd(hmeas{i},x);
         sum = sum + conj(H_cap)'*Rl{i}^(-1)*( yl{i} - h);
         end
         eita = P_next*sum + x;
         */
        sum.resize(x.rows(), 1);
        sum.setZero(x.rows(), 1);
        eita.resize(x.rows(), 1);
        eita.setZero(x.rows(), 1);
        for (int k = 0; k < Rl.size(); k++) {
            jaccsd(hl[k], x, h, H_cap);
            sum = sum + H_cap.conjugate().transpose()*(Rl[k].inverse())*(yl[k]-h);
        }
        eita = P_next * sum + x;
//        DLOG(eita(0), " ", eita(1), " ", eita(2), " ", eita(3), " ", eita(4), " ", eita(5));
//        DLOG(eita);
    }
    
    void Node::jaccsd(const std::function<Eigen::Vector3cd(Eigen::VectorXcd)> &fun,
                      const Eigen::VectorXd &x,
                      // returns z and A
                      Eigen::Vector3d &z,
                      Eigen::Matrix<double, 3, Eigen::Dynamic> &A)
    {
        /*
         z=fun(x);
         n=numel(x);%Number of elements in an array or subscripted array expression.
         m=numel(z);
         A=zeros(m,n);
         h=n*eps;
         for k=1:n
         x1=x;
         x1(k)=x1(k)+h*i;
         A(:,k)=imag(fun(x1))/h;
         end
         */
        
        Eigen::Vector3cd comp_z = fun(x);
        z(0) = comp_z(0).real();
        z(1) = comp_z(1).real();
        z(2) = comp_z(2).real();
        
        long n = x.rows();
        long m = z.rows();
        A.resize(m, n);
        A.setZero(m, n);
        double h = n * MATLAB_EPS; // __DBL_EPSILON__
        
        for (int k = 0; k < n; k++) {
            Eigen::VectorXcd x1(n);
            x1.setZero(n);
            x1 = x;
            std::complex<double> xi(x1(k).real(),h);
            x1(k) = xi;
            Eigen::Vector3cd fun_x = fun(x1);
            A(0,k) = fun_x(0).imag() / h;
            A(1,k) = fun_x(1).imag() / h;
            A(2,k) = fun_x(2).imag() / h;
            //DLOG(A(0,k), " " , A(1,k), " ", A(2,k));
        }
    }
    
    void Node::seteital(Eigen::VectorXd eita)
    {
        eital.push_back(eita);
        if (eital.size() == Rx.size()) {
            ready_to_ekf_p2 = true;
        }
    }
    
    void Node::checkekf_p2(std::function<Eigen::VectorXcd(Eigen::VectorXcd)> fstate,
                           Eigen::MatrixXd Q)
    {
        if (ready_to_ekf_p2) {
            ekf_part2(fstate,Q);
        }
    }
    
    void Node::ekf_part2(std::function<Eigen::VectorXcd(Eigen::VectorXcd)> fstate,
                         Eigen::MatrixXd Q)
    {
        x.resize(myneigh.size() * 5);
        x.setZero(myneigh.size() * 5);
        for (int i = 0; i < c.rows(); i++) {
            x = x + eital[i] * c(i);
        }
        ekf_part3(fstate,Q);
    }
    
    void Node::ekf_part3(std::function<Eigen::VectorXcd(Eigen::VectorXcd)> fstate,
                         Eigen::MatrixXd Q)
    {
        dif_ekf_p3(fstate, Q);
        state_ready = false;
        x(3) = 0;
        x(4) = 0;
        reseteital();
    }
    
    void Node::dif_ekf_p3(std::function<Eigen::VectorXcd(Eigen::VectorXcd)> fstate,
                    Eigen::MatrixXd Q)
    {
        /*
         [f, F_bar]= jaccsd(fstate,x);
         u = f - F_bar*x;
         x_next = F_bar*x + u;
         P_next = F_bar*P*transpose(F_bar) + G*Q*transpose(G);
         */
        Eigen::MatrixXd F_bar;
        Eigen::VectorXd f;
        Eigen::VectorXd u;
        
        F_bar.resize(x.size(), x.size());
        F_bar.setZero(x.size(), x.size());
        f.resize(x.size());
        f.setZero(x.size());
        u.resize(x.size());
        u.setZero(x.size());
        
        jaccsd2(fstate, x, f, F_bar);
        u = f - F_bar*x;
        x = F_bar*x + u;
        P = F_bar*P*F_bar.transpose() + Q;
    }
    
    void Node::jaccsd2(const std::function<Eigen::VectorXcd(Eigen::VectorXcd)> &fun,
                      const Eigen::VectorXd &x,
                      // returns z and A
                      Eigen::VectorXd &z,
                      Eigen::MatrixXd &A)
    {
        /*
         z=fun(x);
         n=numel(x);%Number of elements in an array or subscripted array expression.
         m=numel(z);
         A=zeros(m,n);
         h=n*eps;
         for k=1:n
         x1=x;
         x1(k)=x1(k)+h*i;
         A(:,k)=imag(fun(x1))/h;
         end
         */
        
        Eigen::VectorXcd comp_z = fun(x);
        for (int i = 0; i < comp_z.rows(); i++) {
            z(i) = comp_z(i).real();
        }
        
        long n = x.rows();
        long m = z.rows();
        A.resize(m, n);
        A.setZero(m, n);
        double h = n * MATLAB_EPS; // __DBL_EPSILON__
        
        for (int k = 0; k < n; k++) {
            Eigen::VectorXcd x1(n);
            x1.setZero(n);
            x1 = x;
            std::complex<double> xi(x1(k).real(),h);
            x1(k) = xi;
            Eigen::VectorXcd fun_x = fun(x1);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    A(j, i) = fun_x(j).imag() / h;
                }
            }
            //DLOG(A(0,k), " " , A(1,k), " ", A(2,k));
        }
    }
    
    void Node::reseteital()
    {
        eital.clear();
        yl.clear();
        Rl.clear();
        Rx.clear();
        ready_to_ekf_p1 = false;
        ready_to_ekf_p2 = false;
        hl.clear();
        ekf_p1_done = false;
    }
}
