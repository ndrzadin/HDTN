#ifndef LTP_RANDOM_NUMBER_GENERATOR_H
#define LTP_RANDOM_NUMBER_GENERATOR_H 1

#include <boost/integer.hpp>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <set>

class LtpRandomNumberGenerator {
public:
    LtpRandomNumberGenerator();
    uint64_t GetRandom(const uint64_t additionalRandomness = 0);
    uint64_t GetRandom(boost::random_device & randomDevice);
private:
    //std::set<uint64_t> m_alreadyUsedRandomNumbers;
    uint16_t m_birthdayParadoxPreventer_incrementalPart_U16;
    //boost::random_device m_randomDevice;
    
};

#endif // LTP_RANDOM_NUMBER_GENERATOR_H

