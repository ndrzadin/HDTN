#ifndef CUSTODY_TRANSFER_MANAGER_H
#define CUSTODY_TRANSFER_MANAGER_H 1


#include "codec/bpv6.h"
#include "codec/CustodyTransferEnhancementBlock.h"
#include "codec/BundleViewV6.h"
#include "codec/AggregateCustodySignal.h"
#include <string>
#include <cstdint>
#include <vector>

enum class BPV6_ACS_STATUS_REASON_INDICES : uint8_t {
    SUCCESS__NO_ADDITIONAL_INFORMATION = 0,
    FAIL__REDUNDANT_RECEPTION,
    FAIL__DEPLETED_STORAGE,
    FAIL__DESTINATION_ENDPOINT_ID_UNINTELLIGIBLE,
    FAIL__NO_KNOWN_ROUTE_TO_DESTINATION_FROM_HERE,
    FAIL__NO_TIMELY_CONTACT_WITH_NEXT_NODE_ON_ROUTE,
    FAIL__BLOCK_UNINTELLIGIBLE,

    NUM_INDICES
};
static constexpr unsigned int NUM_ACS_STATUS_INDICES = static_cast<unsigned int>(BPV6_ACS_STATUS_REASON_INDICES::NUM_INDICES);

class CustodyTransferManager {
private:
    CustodyTransferManager();
public:
    
    CustodyTransferManager(const bool isAcsAware, const uint64_t myCustodianNodeId, const uint64_t myCustodianServiceId);
    ~CustodyTransferManager();

    bool ProcessCustodyOfBundle(BundleViewV6 & bv, bool acceptCustody,
        const BPV6_ACS_STATUS_REASON_INDICES statusReasonIndex, std::vector<uint8_t> & custodySignalRfc5050SerializedBundle);
    void Reset();
    bool GenerateCustodySignalBundle(std::vector<uint8_t> & serializedBundle, const bpv6_primary_block & primaryFromSender, const BPV6_ACS_STATUS_REASON_INDICES statusReasonIndex) const;
    bool GenerateAcsBundle(std::vector<uint8_t> & serializedBundle, const bpv6_primary_block & primaryFromSender, const BPV6_ACS_STATUS_REASON_INDICES statusReasonIndex) const;
    const AggregateCustodySignal & GetAcsConstRef(const BPV6_ACS_STATUS_REASON_INDICES statusReasonIndex);
private:
    const bool m_isAcsAware;
    const uint64_t m_myCustodianNodeId;
    const uint64_t m_myCustodianServiceId;
    const std::string m_myCtebCreatorCustodianEidString;
    uint64_t m_myNextCustodyIdForNextHopCtebToSend;

    AggregateCustodySignal m_acsArray[NUM_ACS_STATUS_INDICES];
};

#endif // CUSTODY_TRANSFER_MANAGER_H

