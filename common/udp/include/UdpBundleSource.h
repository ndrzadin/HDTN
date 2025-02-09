/**
 * @file UdpBundleSource.h
 * @author  Brian Tomko <brian.j.tomko@nasa.gov>
 *
 * @copyright Copyright � 2021 United States Government as represented by
 * the National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S.Code.
 * All Other Rights Reserved.
 *
 * @section LICENSE
 * Released under the NASA Open Source Agreement (NOSA)
 * See LICENSE.md in the source root directory for more information.
 *
 * @section DESCRIPTION
 *
 * This UdpBundleSource class encapsulates the appropriate UDP functionality
 * to send a pipeline of bundles (or any other user defined data) over a UDP socket
 * and calls the user defined function OnSuccessfulAckCallback_t when the session closes, meaning
 * a bundle has been delivered to this OS UDP network layer.
 * This class assumes an entire bundle is small enough to fit entirely in one UDP datagram.
 */

#ifndef _UDP_BUNDLE_SOURCE_H
#define _UDP_BUNDLE_SOURCE_H 1

#include <string>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <map>
#include <queue>
#include "CircularIndexBufferSingleProducerSingleConsumerConfigurable.h"
#include "TokenRateLimiter.h"
#include <zmq.hpp>
#include "udp_lib_export.h"

class UdpBundleSource {
private:
    UdpBundleSource();
public:
    typedef boost::function<void()> OnSuccessfulAckCallback_t;
    UDP_LIB_EXPORT UdpBundleSource(const uint64_t rateBps, const unsigned int maxUnacked); //const uint64_t rateBps = 50, const unsigned int maxUnacked = 100

    UDP_LIB_EXPORT ~UdpBundleSource();
    UDP_LIB_EXPORT void Stop();
    UDP_LIB_EXPORT bool Forward(const uint8_t* bundleData, const std::size_t size);
    UDP_LIB_EXPORT bool Forward(zmq::message_t & dataZmq);
    UDP_LIB_EXPORT bool Forward(std::vector<uint8_t> & dataVec);
    UDP_LIB_EXPORT std::size_t GetTotalUdpPacketsAcked();
    UDP_LIB_EXPORT std::size_t GetTotalUdpPacketsSent();
    UDP_LIB_EXPORT std::size_t GetTotalUdpPacketsUnacked();
    UDP_LIB_EXPORT std::size_t GetTotalBundleBytesAcked();
    UDP_LIB_EXPORT std::size_t GetTotalBundleBytesSent();
    UDP_LIB_EXPORT std::size_t GetTotalBundleBytesUnacked();
    UDP_LIB_EXPORT void UpdateRate(uint64_t rateBitsPerSec);
    UDP_LIB_EXPORT void Connect(const std::string & hostname, const std::string & port);
    UDP_LIB_EXPORT bool ReadyToForward() const;
    UDP_LIB_EXPORT void SetOnSuccessfulAckCallback(const OnSuccessfulAckCallback_t & callback);
private:
    UDP_LIB_NO_EXPORT void OnResolve(const boost::system::error_code & ec, boost::asio::ip::udp::resolver::results_type results);
    UDP_LIB_NO_EXPORT void OnConnect(const boost::system::error_code & ec);
    UDP_LIB_NO_EXPORT void HandlePostForUdpSendVecMessage(boost::shared_ptr<std::vector<boost::uint8_t> > & vecDataToSendPtr);
    UDP_LIB_NO_EXPORT void HandlePostForUdpSendZmqMessage(boost::shared_ptr<zmq::message_t> & zmqDataToSendPtr);
    UDP_LIB_NO_EXPORT void HandleUdpSendVecMessage(boost::shared_ptr<std::vector<boost::uint8_t> > & dataSentPtr, const boost::system::error_code& error, std::size_t bytes_transferred);
    UDP_LIB_NO_EXPORT void HandleUdpSendZmqMessage(boost::shared_ptr<zmq::message_t> & dataZmqSentPtr, const boost::system::error_code& error, std::size_t bytes_transferred);
    UDP_LIB_NO_EXPORT bool ProcessPacketSent(std::size_t bytes_transferred);

    UDP_LIB_NO_EXPORT void TryRestartTokenRefreshTimer();
    UDP_LIB_NO_EXPORT void TryRestartTokenRefreshTimer(const boost::posix_time::ptime & nowPtime);
    UDP_LIB_NO_EXPORT void OnTokenRefresh_TimerExpired(const boost::system::error_code& e);

    UDP_LIB_NO_EXPORT void DoUdpShutdown();
    UDP_LIB_NO_EXPORT void DoHandleSocketShutdown();
    



    
    boost::asio::io_service m_ioService;
    boost::asio::io_service::work m_work;
    boost::asio::ip::udp::resolver m_resolver;
    TokenRateLimiter m_tokenRateLimiter;
    boost::asio::deadline_timer m_tokenRefreshTimer;
    boost::posix_time::ptime m_lastTimeTokensWereRefreshed;
    std::queue<boost::shared_ptr<std::vector<boost::uint8_t> > > m_queueVecDataToSendPtrs;
    std::queue<boost::shared_ptr<zmq::message_t> > m_queueZmqDataToSendPtrs;
    boost::asio::ip::udp::socket m_udpSocket;
    boost::asio::ip::udp::endpoint m_udpDestinationEndpoint;
    std::unique_ptr<boost::thread> m_ioServiceThreadPtr;
    boost::condition_variable m_localConditionVariableAckReceived;
    const uint64_t m_maxPacketsBeingSent;
    CircularIndexBufferSingleProducerSingleConsumerConfigurable m_bytesToAckBySentCallbackCb;
    std::vector<std::size_t> m_bytesToAckBySentCallbackCbVec;

    OnSuccessfulAckCallback_t m_onSuccessfulAckCallback;
    volatile bool m_readyToForward;
    volatile bool m_useLocalConditionVariableAckReceived;
    bool m_tokenRefreshTimerIsRunning;

public:
    std::size_t m_totalPacketsSentBySentCallback;
    std::size_t m_totalBytesSentBySentCallback;
    std::size_t m_totalPacketsDequeuedForSend;
    std::size_t m_totalBytesDequeuedForSend;
    std::size_t m_totalPacketsLimitedByRate;
};



#endif //_UDP_BUNDLE_SOURCE_H
