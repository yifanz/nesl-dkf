#include "dkf.h"
#include "dkf_log.h"
#include "dkf_network.h"
#include "dkf_meas.h"

static dkf::Network network;

extern "C" void dkf_init()
{
    DLOG("dkf init");
    network.nodes[0].setAsReference();
    Eigen::MatrixXd P = network.getInitialVar();
    Eigen::VectorXd x;
    x.resize(network.nodes.size()*5);
    x.setZero(network.nodes.size()*5);
    network.init_x_P_forall(x, P);
    network.setneigh_forall();
}

extern "C" void dkf_iterate(
                            double walltime,
                            int nodei,
                            int nodej,
                            uint64_t seq,
                            uint64_t t0,
                            uint64_t t1,
                            uint64_t t2,
                            uint64_t t3,
                            uint64_t t4,
                            uint64_t t5
                            )
{
    dkf::Meas meas(
                   walltime,
                   nodei,
                   nodej,
                   seq,
                   t0,
                   t1,
                   t2,
                   t3,
                   t4,
                   t5
                   );
    
    /* Just print out the measurement
     DLOG(
     LOG_PREC(6),
     meas.walltime,", ",
     meas.nodei,", ",
     meas.nodej,", ",
     meas.seq,", ",
     meas.t0,", ",
     meas.t1,", ",
     meas.t2,", ",
     meas.t3,", ",
     meas.t4,", ",
     meas.t5
     );
     */
    
    network.next_meas(meas);
}
