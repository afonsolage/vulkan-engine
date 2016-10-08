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