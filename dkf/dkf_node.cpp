#include "dkf_node.h"
#include "dkf_log.h"

namespace dkf
{
    Node::Node(int id, std::string name):
    id(id), name(name)
    {
    }
    
    void Node::set_meas(dkf::Meas meas, double h_dt_ref)
    {
        yl.push_back(meas.vectorize());
        Rl.push_back(meas.getCovariance());
        
        hl.push_back(h_dt_ref);
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
        
    }
}
