#ifndef OUTDUCT_H
#define OUTDUCT_H 1

#include <string>
#include <boost/integer.hpp>
#include <boost/function.hpp>
#include "OutductsConfig.h"
#include <list>
#include <zmq.hpp>

struct OutductFinalStats {
    std::string m_convergenceLayer;
    std::size_t m_totalDataSegmentsOrPacketsSent;
    std::size_t m_totalDataSegmentsOrPacketsAcked;

    OutductFinalStats() : m_convergenceLayer(""), m_totalDataSegmentsOrPacketsSent(0), m_totalDataSegmentsOrPacketsAcked(0) {}
};

class Outduct {
private:
    Outduct();
public:
    typedef boost::function<void()> OnSuccessfulOutductAckCallback_t;

    Outduct(const outduct_element_config_t & outductConfig, const uint64_t outductUuid);
    virtual ~Outduct();
    virtual std::size_t GetTotalDataSegmentsUnacked() = 0;
    virtual bool Forward(const uint8_t* bundleData, const std::size_t size) = 0;
    virtual bool Forward(zmq::message_t & movableDataZmq) = 0;
    virtual bool Forward(std::vector<uint8_t> & movableDataVec) = 0;
    virtual void SetOnSuccessfulAckCallback(const OnSuccessfulOutductAckCallback_t & callback) = 0;
    virtual void Connect() = 0;
    virtual bool ReadyToForward() = 0;
    virtual void Stop() = 0;
    virtual void GetOutductFinalStats(OutductFinalStats & finalStats) = 0;

    uint64_t GetOutductUuid() const;
    uint64_t GetOutductMaxBundlesInPipeline() const;
    std::string GetConvergenceLayerName() const;

protected:
    const outduct_element_config_t m_outductConfig;
    const uint64_t m_outductUuid;
};

#endif // OUTDUCT_H

