#include "dkf_network.h"
#include "dkf_log.h"

#include <vector>

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
            
            publishmeas_forneigh(meas, dt_ref);
            checkekf_p1_forall();
        }
        
    }
    
    void Network::publishmeas_forneigh(dkf::Meas meas,
                                       double h_dt_ref)
    {
        int srcIndex = meas.getSourceId();
        //int destIndex = meas.getDestId();
        
        // Assume everyone is a neighbor to every
        for (int i : neighbors[srcIndex]) {
            nodes[i].set_meas(meas, h_dt_ref);
        }
    }
    
    void Network::checkekf_p1_forall()
    {
        for (int i = 0; i < nodes.size(); i++) {
            nodes[i].checkekf_p1();
        }
    }
    
}
