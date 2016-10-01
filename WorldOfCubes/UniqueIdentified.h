#pragma once

class UniqueIdentified
{
public:
	uint64_t get_uid() const noexcept { return m_uid; }
	virtual ~UniqueIdentified() {};

protected:
	void generate_uid(const type_info& t_info);
	uint64_t m_uid;

	UniqueIdentified() : m_uid(0) {};
};

struct UIDHasher
{
	std::size_t operator() (const UniqueIdentified& uid)
	{
		return (size_t)((uid.get_uid() >> sizeof(uint32_t)) ^ (uid.get_uid() & std::numeric_limits<uint32_t>::max()));
	}
};

inline void UniqueIdentified::generate_uid(const type_info& t_info)
{
	auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
	m_uid = now & std::numeric_limits<uint32_t>::max();
	m_uid |= (std::rand() % std::numeric_limits<uint32_t>::max()) << sizeof(uint32_t);
}

//Component - Component operators
inline bool operator==(const UniqueIdentified& lhs, const UniqueIdentified& rhs) { return lhs.get_uid() == rhs.get_uid(); }
inline bool operator!=(const UniqueIdentified& lhs, const UniqueIdentified& rhs) { return lhs.get_uid() != rhs.get_uid(); }
inline bool operator<(const UniqueIdentified& lhs, const UniqueIdentified& rhs) { return lhs.get_uid() < rhs.get_uid(); }
inline bool operator>(const UniqueIdentified& lhs, const UniqueIdentified& rhs) { return lhs.get_uid() > rhs.get_uid(); }
inline bool operator<=(const UniqueIdentified& lhs, const UniqueIdentified& rhs) { return lhs.get_uid() <= rhs.get_uid(); }
inline bool operator>=(const UniqueIdentified& lhs, const UniqueIdentified& rhs) { return lhs.get_uid() >= rhs.get_uid(); }

//Component - uint64_t operators
inline bool operator==(const UniqueIdentified& lhs, const uint64_t& rhs) { return lhs.get_uid() == rhs; }
inline bool operator!=(const UniqueIdentified& lhs, const uint64_t& rhs) { return lhs.get_uid() != rhs; }
inline bool operator<(const UniqueIdentified& lhs, const uint64_t& rhs) { return lhs.get_uid() < rhs; }
inline bool operator>(const UniqueIdentified& lhs, const uint64_t& rhs) { return lhs.get_uid() > rhs; }
inline bool operator<=(const UniqueIdentified& lhs, const uint64_t& rhs) { return lhs.get_uid() <= rhs; }
inline bool operator>=(const UniqueIdentified& lhs, const uint64_t& rhs) { return lhs.get_uid() >= rhs; }