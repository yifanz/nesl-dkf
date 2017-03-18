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
    
}
