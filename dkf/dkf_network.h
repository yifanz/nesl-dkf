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
        
        Network();
        
        void setneigh_forall() {
            for (int i = 0; i < nodes.size(); i++) {
                nodes[i].setSizeBuffer((int) neighbors.size());
            }
        }
        
        void publishmeas_forneigh(dkf::Meas meas, double h_dt_ref);
    };
}

#endif
