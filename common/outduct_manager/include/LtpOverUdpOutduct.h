#ifndef LTP_OVER_UDP_OUTDUCT_H
#define LTP_OVER_UDP_OUTDUCT_H 1

#include <string>
#include "Outduct.h"
#include "LtpBundleSource.h"
#include <list>

class CLASS_VISIBILITY_OUTDUCT_MANAGER_LIB LtpOverUdpOutduct : public Outduct {
public:
    OUTDUCT_MANAGER_LIB_EXPORT LtpOverUdpOutduct(const outduct_element_config_t & outductConfig, const uint64_t outductUuid);
    OUTDUCT_MANAGER_LIB_EXPORT virtual ~LtpOverUdpOutduct();
    OUTDUCT_MANAGER_LIB_EXPORT virtual std::size_t GetTotalDataSegmentsUnacked();
    OUTDUCT_MANAGER_LIB_EXPORT virtual bool Forward(const uint8_t* bundleData, const std::size_t size);
    OUTDUCT_MANAGER_LIB_EXPORT virtual bool Forward(zmq::message_t & movableDataZmq);
    OUTDUCT_MANAGER_LIB_EXPORT virtual bool Forward(std::vector<uint8_t> & movableDataVec);
    OUTDUCT_MANAGER_LIB_EXPORT virtual void SetOnSuccessfulAckCallback(const OnSuccessfulOutductAckCallback_t & callback);
    OUTDUCT_MANAGER_LIB_EXPORT virtual void Connect();
    OUTDUCT_MANAGER_LIB_EXPORT virtual bool ReadyToForward();
    OUTDUCT_MANAGER_LIB_EXPORT virtual void Stop();
    OUTDUCT_MANAGER_LIB_EXPORT virtual void GetOutductFinalStats(OutductFinalStats & finalStats);

private:
    LtpOverUdpOutduct();


    LtpBundleSource m_ltpBundleSource;
};


#endif // STCP_OUTDUCT_H

