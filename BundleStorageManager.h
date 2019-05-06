#ifndef _BUNDLE_STORAGE_MANAGER_H
#define _BUNDLE_STORAGE_MANAGER_H

#include <boost/integer.hpp>
#include <stdint.h>
#include <map>
#include <vector>
#include <string>
#include <boost/iostreams/device/mapped_file.hpp>

//#define USE_VECTOR_CIRCULAR_BUFFER 1
#ifdef USE_VECTOR_CIRCULAR_BUFFER
typedef boost::uint32_t segment_id_t;
typedef std::vector<segment_id_t> segment_id_vec_t;
typedef std::vector<segment_id_vec_t> expiration_circular_buf_t;
typedef std::vector<expiration_circular_buf_t> priority_vec_t;
typedef std::map<std::string, priority_vec_t> destination_map_t; //results in about 12MB per link
#else
typedef boost::uint32_t segment_id_t;
typedef boost::uint64_t abs_expiration_t;
typedef std::vector<segment_id_t> segment_id_vec_t;
typedef std::map<abs_expiration_t, segment_id_vec_t> expiration_map_t;
typedef std::vector<expiration_map_t> priority_vec_t;
typedef std::map<std::string, priority_vec_t> destination_map_t;
#endif

//two days
#define NUMBER_OF_EXPIRATIONS (86400*2)
#define NUMBER_OF_PRIORITIES (3)

class BundleStorageManager {
public:
	void OpenFile();
	void CloseFile();
	void AddLink(const std::string & linkName);
	void StoreBundle(const std::string & linkName, const unsigned int priorityIndex, const abs_expiration_t absExpiration, const segment_id_t segmentId, const char * const data, std::size_t dataSize);
	segment_id_t GetBundle(const std::vector<std::string> & availableDestLinks, std::size_t & retLinkIndex, unsigned int & retPriorityIndex, abs_expiration_t & retAbsExpiration, char * const data, std::size_t dataSize);
	
	static bool UnitTest();

private:
	
private:
	destination_map_t m_destMap;
	boost::iostreams::mapped_file  m_mappedFile;
};


#endif //_BUNDLE_STORAGE_MANAGER_H
