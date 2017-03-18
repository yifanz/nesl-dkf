#ifndef dkf_network_h
#define dkf_network_h

#include <vector>
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
        
        void setneigh_forall() {
            for (int i = 0; i < nodes.size(); i++) {
                nodes[i].setSizeBuffer((int) neighbors.size());
            }
        }
        
        void publishmeas_forneigh(dkf::Meas meas, double h_dt_ref);
        void checkekf_p1_forall();
    };
}

#endif
