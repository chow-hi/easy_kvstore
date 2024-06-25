/************************************************************************/
/*                                                                      */
/*    版本更新或帮助文档网址：http://www.kxtry.com                      */
/*                                                                      */
/************************************************************************/
#ifndef _STLPACKET_H_
#define _STLPACKET_H_

#pragma once;

#include <list>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <string>
#include <queue>

#include "inttype.h"

using namespace std;

//! 内部使用
#define UINT16_SWAP_LE_BE_CONSTANT(val) \
	((uint16_t) ( \
	(uint16_t) ((uint16_t) (val) >> 8) |  \
	(uint16_t) ((uint16_t) (val) << 8)))

//! 内部使用
#define UINT32_SWAP_LE_BE_CONSTANT(val) \
	((uint32_t) ( \
	(((uint32_t) (val) & (uint32_t) 0x000000ffU) << 24) | \
	(((uint32_t) (val) & (uint32_t) 0x0000ff00U) <<  8) | \
	(((uint32_t) (val) & (uint32_t) 0x00ff0000U) >>  8) | \
	(((uint32_t) (val) & (uint32_t) 0xff000000U) >> 24)))

//! 内部使用
#define UINT64_SWAP_LE_BE_CONSTANT(val) \
	((uint64_t) ( \
	(((uint64_t) (val) & \
	(uint64_t) 0x00000000000000ffULL) << 56) | \
	(((uint64_t) (val) & \
	(uint64_t) 0x000000000000ff00ULL) << 40) | \
	(((uint64_t) (val) & \
	(uint64_t) 0x0000000000ff0000ULL) << 24) | \
	(((uint64_t) (val) &                         \
	(uint64_t) 0x00000000ff000000ULL) <<  8) | \
	(((uint64_t) (val) & \
	(uint64_t) 0x000000ff00000000ULL) >>  8) | \
	(((uint64_t) (val) & \
	(uint64_t) 0x0000ff0000000000ULL) >> 24) | \
	(((uint64_t) (val) & \
	(uint64_t) 0x00ff000000000000ULL) >> 40) | \
	(((uint64_t) (val) & \
	(uint64_t) 0xff00000000000000ULL) >> 56)))


#define UINT16_TO_LE(val)		UINT16_SWAP_LE_BE_CONSTANT(val)
#define UINT32_TO_LE(val)		UINT32_SWAP_LE_BE_CONSTANT(val)
#define UINT64_TO_LE(val)		UINT64_SWAP_LE_BE_CONSTANT(val)
#define INT16_TO_LE(val)		((int16_t)UINT16_TO_LE(val))
#define INT32_TO_LE(val)		((int32_t)UINT32_TO_LE(val))
#define INT64_TO_LE(val)		((int64_t)UINT64_TO_LE(val))
#define UINT16_TO_BE(val)		UINT16_SWAP_LE_BE_CONSTANT(val)
#define UINT32_TO_BE(val)		UINT32_SWAP_LE_BE_CONSTANT(val)
#define UINT64_TO_BE(val)		UINT64_SWAP_LE_BE_CONSTANT(val)
#define INT16_TO_BE(val)		((int16_t)UINT16_TO_BE(val))
#define INT32_TO_BE(val)		((int32_t)UINT32_TO_BE(val))
#define INT64_TO_BE(val)		((int64_t)UINT64_TO_BE(val))


#define PACK_CHECK_BUFFER_SIZE(s) \
	if(buffer_.size() + (s) > max_capacity_) \
	{ \
		PacketError("buffer_.size() + (s) > max_capacity_");\
		err = -1;	\
		return false; \
	}


#define BEGIN_NAMESPACE(name)	namespace name	{
#define END_NAMESPACE(name)		}
class StlPack;
class StlUnpack;

struct Marshallable
{
	//format:
	enum {uri = (1 << 8 | 15)};

	virtual ~Marshallable() {}
	virtual bool marshal(StlPack& pack) = 0;
	virtual bool unmarshal(StlUnpack& unpack) = 0;
};

struct PacketError : public std::runtime_error 
{
	PacketError(const std::string & w) : std::runtime_error(w){}
};



inline bool littleEndianOS() 
{
	static union 
	{  
		char c[4];  
		unsigned char l;  
	} endian_test = {{'l','?','?','b'}};  
	return endian_test.l == 'l';
}

inline bool littleEndianOS2() 
{  
	short int x = 0x00ff;  
	char* p = (char*)&x;  
	return (short int)p[0] == -1;
}

class StlPack
{
public:
	//bigendian=true代表打包后的数据序列是BigEndian，否则是LittleEndian
	StlPack(size_t init_capacity = 0, size_t max_capacity = 1024*16, bool bigendian = true) 
	: max_capacity_(max_capacity)
	, bigendian_(bigendian)
	, err(0)
	, littleEndianOS_(littleEndianOS())
	{
		buffer_.reserve(init_capacity);
	}

	bool concatenate(const StlPack &pack)
	{
		PACK_CHECK_BUFFER_SIZE(pack.buffer().size());
		if(pack.buffer().size() > 0)
		{
			buffer_.append(pack.buffer().c_str(), pack.buffer().size());
		}
		return true;
	}

	bool concatenate(const uint8_t *data, size_t size)
	{
		PACK_CHECK_BUFFER_SIZE(size);
		buffer_.append((const char*)data, size);
		return true;
	}

	bool push(const char *data, size_t size)
	{
		return concatenate((const uint8_t*)data, size);
	}
		
	bool push(int8_t val)
	{
		PACK_CHECK_BUFFER_SIZE(sizeof(val));
		buffer_.append((const char*)&val, sizeof(val));
		return true;
	}

	bool push(uint8_t val)
	{
		PACK_CHECK_BUFFER_SIZE(sizeof(val));
		buffer_.append((const char*)&val, sizeof(val));
		return true;
	}

	bool push(int16_t val)
	{
		PACK_CHECK_BUFFER_SIZE(sizeof(val));
		if(littleEndianOS_)
		{
			if(bigendian_)
			{
				val = INT16_TO_BE(val);
			}
		}
		else
		{
			if(!bigendian_)
			{
				val = INT16_TO_LE(val);
			}
		}
		buffer_.append((const char*)&val, sizeof(val));
		return true;
	}

	bool push(uint16_t val)
	{
		PACK_CHECK_BUFFER_SIZE(sizeof(val));
		if(littleEndianOS_)
		{
			if(bigendian_)
			{
				val = UINT16_TO_BE(val);
			}
		}
		else
		{
			if(!bigendian_)
			{
				val = UINT16_TO_LE(val);
			}
		}
		buffer_.append((const char*)&val, sizeof(val));
		return true;
	}

	bool push(int32_t val)
	{
		PACK_CHECK_BUFFER_SIZE(sizeof(val));
		if(littleEndianOS_)
		{
			if(bigendian_)
			{
				val = INT32_TO_BE(val);
			}
		}
		else
		{
			if(!bigendian_)
			{
				val = INT32_TO_LE(val);
			}
		}
		buffer_.append((const char*)&val, sizeof(val));
		return true;
	}

	bool push(uint32_t val)
	{
		PACK_CHECK_BUFFER_SIZE(sizeof(val));
		if(littleEndianOS_)
		{
			if(bigendian_)
			{
				val = UINT32_TO_BE(val);
			}
		}
		else
		{
			if(!bigendian_)
			{
				val = UINT32_TO_LE(val);
			}
		}
		buffer_.append((const char*)&val, sizeof(val));
		return true;
	}

	bool push(int64_t val)
	{
		PACK_CHECK_BUFFER_SIZE(sizeof(val));
		if(littleEndianOS_)
		{
			if(bigendian_)
			{
				val = INT64_TO_BE(val);
			}
		}
		else
		{
			if(!bigendian_)
			{
				val = INT64_TO_LE(val);
			}
		}
		buffer_.append((const char*)&val, sizeof(val));
		return true;
	}

	bool push(uint64_t val)
	{
		PACK_CHECK_BUFFER_SIZE(sizeof(val));
		if(littleEndianOS_)
		{
			if(bigendian_)
			{
				val = UINT64_TO_BE(val);
			}
		}
		else
		{
			if(!bigendian_)
			{
				val = UINT64_TO_LE(val);
			}
		}
		buffer_.append((const char*)&val, sizeof(val));
		return true;
	}

	bool push(const std::string& val)
	{
		return push<uint16_t>( val.c_str(), val.size() );
	}

	bool push(const Marshallable &m)
	{
		return const_cast<Marshallable&>(m).marshal(*this);
		return true;
	}

	template<typename LENGTH>
	bool push(const char* data , size_t len)
	{
		if (!push((LENGTH)(len)))		
		{
			return false;
		}
		PACK_CHECK_BUFFER_SIZE(len+1);
		buffer_.append(data, len);
		return true;
	}

	template<class T>
	bool pushContainer1(const T &val)
	{
		bool err = push((int32_t)val.size());
		if(!err)
		{
			return err;
		}
		for(typename T::const_iterator citer = val.begin(); citer != val.end(); ++citer)
		{
			if(!(err = push(*citer)))
			{
				return err;
			}
		}
		return true;
	}

	template<class T>
	bool push(const std::vector<T> &val)
	{
		return pushContainer1<std::vector<T> >(val);
	}


	template<class T>
	bool push(const std::list<T> &val)
	{
		return pushContainer1<std::list<T> >(val);
	}

	template<class T>
	bool push(const std::set<T> &val)
	{
		return pushContainer1<std::set<T> >(val);
	}

	template<class T>
	bool pushContainer2(const T &val)
	{
		bool err = push((int32_t)val.size());
		if(!err)
		{
			return err;
		}
		for(typename T::const_iterator citer = val.begin(); citer != val.end(); ++citer)
		{
			if((err = push(citer->first)) != true)
			{
				return err;
			}
			if((err = push(citer->second)) != true)
			{
				return err;
			}
		}
		return true;
	}

	template<class key_T, class value_T>
	bool push(const std::map<key_T, value_T> &val)
	{
		return pushContainer2<std::map<key_T, value_T> >(val);
	}

	template<class key_T, class value_T>
	bool push(const std::unordered_map<key_T, value_T> &val)
	{
		return pushContainer2<std::unordered_map<key_T, value_T> >(val);
	}

	template<class key_T, class value_T>
	bool push(const std::multimap<key_T, value_T> &val)
	{
		return pushContainer2<std::multimap<key_T, value_T> >(val);
	}

	bool bigEndian() const
	{
		return bigendian_;
	}

	const std::string& buffer() const
	{
		return buffer_;
	}

	const char *bufferptr() const
	{
		return buffer_.c_str();
	}

	const int bufferLength() const
	{
		return buffer_.length();
	}

	size_t getMaxCapacity() const
	{
		return max_capacity_;
	}

	void rewind(bool bigendian)
	{
		bigendian_ = bigendian;
		err = 0;
		buffer_.clear();
	}

	void clear()
	{
		err = 0;
		buffer_.clear();
	}

	void swap(std::string& out)
	{
		err = 0;
		buffer_.swap(out);
	}

	int error()
	{
		return err;
	}

private:
	int err;
	size_t max_capacity_;
	std::string buffer_;
	bool bigendian_;
	bool littleEndianOS_;
};

#define UNPACK_CHECK_BUFFER_SIZE(s) \
	if(size_ - offset_ < (s)) \
	{ \
	PacketError("size_ - offset_ < (s)");\
	err = -1;	\
	return false; \
	}

#define _DATA (private_copy_ ? data_ : cdata_)

class StlUnpack
{
	StlUnpack(const StlUnpack&);
	StlUnpack& operator = (const StlUnpack&);
public:
	//bigendian=true代表打包后的数据序列是BigEndian，否则是LittleEndian
	StlUnpack(const char *data, size_t size, bool bigendian = true, bool private_copy = false)
		: data_(NULL), cdata_(NULL), size_(size), err(0), bigendian_(bigendian), private_copy_(private_copy), offset_(0), littleEndianOS_(littleEndianOS())

	{
		if(private_copy_)
		{
			data_ = new uint8_t[size];
			memcpy(data_, data, size);
		}
		else
		{
			cdata_ = (const uint8_t*)data;
		}
	}

	StlUnpack(const string& d, bool bigendian = true, bool private_copy = false)
		: data_(NULL), cdata_(NULL), size_(d.length()), bigendian_(bigendian), private_copy_(private_copy), offset_(0), littleEndianOS_(littleEndianOS())
	{
		uint8_t *data = (uint8_t*)d.data();
		size_t size = d.length();
		if(private_copy_)
		{
			data_ = new uint8_t[size];
			memcpy(data_, data, size);
		}
		else
		{
			cdata_ = data;
		}
	}

	~StlUnpack()
	{
		if(private_copy_)
		{
			delete [] data_;
		}
	}

	uint8_t* bufferptr() const
	{
		return data_;
	}

	uint32_t bufferLength() const
	{
		return size_;
	}

	bool pop(int8_t &val)
	{
		UNPACK_CHECK_BUFFER_SIZE(sizeof(int8_t));
		val = *((int8_t*)(_DATA + offset_));
		offset_ += sizeof(int8_t);
		return true;
	}

	bool pop(uint8_t &val)
	{
		UNPACK_CHECK_BUFFER_SIZE(sizeof(uint8_t));
		val = *((uint8_t*)(_DATA + offset_));
		offset_ += sizeof(uint8_t);
		return true;
	}

	bool pop(int16_t &val)
	{
		UNPACK_CHECK_BUFFER_SIZE(sizeof(int16_t));
		int16_t *p = (int16_t*)(_DATA + offset_);
		if(littleEndianOS_)
		{
			if(bigendian_)
			{
				val = INT16_TO_LE(*p);
			}
			else
			{
				val = *p;
			}
		}
		else
		{
			if(!bigendian_)
			{
				val = INT16_TO_BE(*p);
			}
			else
			{
				val = *p;
			}
		}
		offset_ += sizeof(int16_t);
		return true;
	}

	bool pop(uint16_t &val)
	{
		UNPACK_CHECK_BUFFER_SIZE(sizeof(uint16_t));
		uint16_t *p = (uint16_t*)(_DATA + offset_);
		if(littleEndianOS_)
		{
			if(bigendian_)
			{
				val = UINT16_TO_LE(*p);
			}
			else
			{
				val = *p;
			}
		}
		else
		{
			if(!bigendian_)
			{
				val = UINT16_TO_BE(*p);
			}
			else
			{
				val = *p;
			}
		}
		offset_ += sizeof(uint16_t);
		return true;
	}

	bool pop(int32_t &val)
	{
		UNPACK_CHECK_BUFFER_SIZE(sizeof(int32_t));
		int32_t *p = (int32_t*)(_DATA + offset_);
		if(littleEndianOS_)
		{
			if(bigendian_)
			{
				val = INT32_TO_LE(*p);
			}
			else
			{
				val = *p;
			}
		}
		else
		{
			if(!bigendian_)
			{
				val = INT32_TO_BE(*p);
			}
			else
			{
				val = *p;
			}
		}
		offset_ += sizeof(int32_t);
		return true;
	}

	bool pop(uint32_t &val)
	{
		UNPACK_CHECK_BUFFER_SIZE(sizeof(uint32_t));
		uint32_t *p = (uint32_t*)(_DATA + offset_);
		if(littleEndianOS_)
		{
			if(bigendian_)
			{
				val = UINT32_TO_LE(*p);
			}
			else
			{
				val = *p;
			}
		}
		else
		{
			if(!bigendian_)
			{
				val = UINT32_TO_BE(*p);
			}
			else
			{
				val = *p;
			}
		}
		offset_ += sizeof(uint32_t);
		return true;
	}

	bool pop(int64_t &val)
	{
		UNPACK_CHECK_BUFFER_SIZE(sizeof(int64_t));
		int64_t *p = (int64_t*)(_DATA + offset_);
		if(littleEndianOS_)
		{
			if(bigendian_)
			{
				val = INT64_TO_LE(*p);
			}
			else
			{
				val = *p;
			}
		}
		else
		{
			if(!bigendian_)
			{
				val = INT64_TO_BE(*p);
			}
			else
			{
				val = *p;
			}
		}
		offset_ += sizeof(int64_t);
		return true;
	}

	bool pop(uint64_t &val)
	{
		UNPACK_CHECK_BUFFER_SIZE(sizeof(uint64_t));
		uint64_t *p = (uint64_t*)(_DATA + offset_);
		if(littleEndianOS_)
		{
			if(bigendian_)
			{
				val = UINT64_TO_LE(*p);
			}
			else
			{
				val = *p;
			}
		}
		else
		{
			if(!bigendian_)
			{
				val = UINT64_TO_BE(*p);
			}
			else
			{
				val = *p;
			}
		}
		offset_ += sizeof(uint64_t);
		return true;
	}

	bool pop(Marshallable &m)
	{
		return m.unmarshal(*this);
	}

	bool pop(std::string& val)
	{		
		return pop<uint16_t>(val);
	}

	template<typename LENGTH>
	bool pop(std::string& val)
	{			
		LENGTH size = 0;		
		if (!pop(size))
		{
			return false;
		}
		UNPACK_CHECK_BUFFER_SIZE(size);
		val.append((const char*)(_DATA + offset_), size);
		offset_ += size;
		return true;
	}

	template<class T>
	bool popContainer1(T &val)
	{        
		uint32_t size = 0;
		bool err = pop(size);
		if(!err)
		{
			return err;
		}        
		for(uint32_t i = 0; i < size; ++i)
		{
			typename T::value_type tmpval;
			if(!(err = pop(tmpval)))
			{
				return err;
			}
			val.push_back(tmpval);
		}
		return true;
	}

	template<class T>
	bool pop(std::vector<T>& val)
	{
		return popContainer1<std::vector<T> >(val);
	}

	template<class T>
	bool pop(std::list<T>& val)
	{
		return popContainer1<std::list<T> >(val);
	}


    template<class T>
	bool popContainer3(T &val)
	{        
		uint32_t size = 0;
		bool err = pop(size);
		if(!err)
		{
			return err;
		}        
		for(uint32_t i = 0; i < size; ++i)
		{
			typename T::value_type tmpval;
			if(!(err = pop(tmpval)))
			{
				return err;
			}
			val.insert(tmpval);
		}
		return true;
	}

	template<class T>
	bool pop(std::set<T>& val)
	{
		uint32_t size = 0;
		bool err = pop(size);
		if(!err)
		{
			return err;
		}

		std::pair<typename std::set<T>::iterator, bool> insret;
		for(uint32_t i = 0; i < size; ++i)
		{
			T tmpval;
			if(!(err = pop(tmpval)))
			{
				return err;
			}
			insret = val.insert(tmpval);
			if(!insret.second)
			{
				return false;
			}
		}
		return true;
	}

	template<class T>
	bool popContainer2(T& val)
	{        
		uint32_t size = 0;
		bool err = pop(size);
		if(!err)
		{
			return err;
		}
		std::pair<typename T::iterator,bool> insret;
		for(uint32_t i = 0; i < size; ++i)
		{
			typename T::key_type keyval;
			typename T::value_type::second_type tmpval;
			if(!(err = pop(keyval)))
			{
				return err;
			}
			if(!(err = pop(tmpval)))
			{
				return err;
			}
			insret = val.insert(typename T::value_type(keyval, tmpval));
			if(!insret.second)
			{
				return false;
			}
		}
		return true;
	}

	template<class key_T, class value_T>
	bool pop(std::map<key_T, value_T>& val)
	{
		return popContainer2<std::map<key_T, value_T> >(val);
	}

	template<class key_T, class value_T>
	bool pop(std::unordered_map<key_T, value_T>& val)
	{
		return popContainer2<std::unordered_map<key_T, value_T> >(val);
	}

	template<class key_T, class value_T>
	bool pop(std::multimap<key_T, value_T>& val)
	{
		return popContainer2<std::multimap<key_T, value_T> >(val);
	}

	bool bigEndian() const
	{
		return bigendian_;
	}

	int error()
	{
		return err;
	}

private:
	int err;
	uint8_t *data_;
	const uint8_t *cdata_;
	size_t size_;
	bool bigendian_;
	bool private_copy_;
	size_t offset_;
	bool littleEndianOS_;
};


//pack
inline StlPack & operator << (StlPack & p, bool sign)
{
	p.push((uint8_t)(sign ? 1 : 0));
	return p;
}


inline StlPack & operator << (StlPack & p, uint8_t  u8)
{
	p.push((uint8_t)u8);
	return p;
}

inline StlPack & operator << (StlPack & p, int8_t  i8)
{
	p.push((int8_t)i8);
	return p;
}

inline StlPack & operator << (StlPack & p, uint16_t  u16)
{
	p.push((uint16_t)u16);
	return p;
}

inline StlPack & operator << (StlPack & p, int16_t  i16)
{
	p.push((int16_t)i16);
	return p;
}

inline StlPack & operator << (StlPack & p, uint32_t  u32)
{
	p.push((uint32_t)u32);
	return p;
}
inline StlPack & operator << (StlPack & p, uint64_t  u64)
{
	p.push((uint64_t)u64);
	return p;
}

inline StlPack & operator << (StlPack & p, int64_t  i64)
{
	p.push((int64_t)i64);
	return p;
}

inline StlPack & operator << (StlPack & p, int32_t  i32)
{
	p.push((int32_t)i32);
	return p;
}

inline StlPack & operator << (StlPack & p, const std::string & str)
{
	p.push(str);
	return p;
}

inline StlPack & operator << (StlPack & p, Marshallable& m)
{
	m.marshal(p);
	return p;
}

inline StlPack & operator << (StlPack & p, const Marshallable& m)
{
	const_cast<Marshallable&>(m).marshal(p);
	return p;
}

template<class T>
inline StlPack & operator << (StlPack & p, const std::vector<T> & val)
{
	p.pushContainer1<std::vector<T> >(val);
	return p;
}

template<class T>
inline StlPack & operator << (StlPack & p, const std::list<T> & val)
{
	p.pushContainer1<std::list<T> >(val);
	return p;
}

template<class T>
inline StlPack & operator << (StlPack & p, const std::set<T> & val)
{
	p.pushContainer1<std::set<T> >(val);
	return p;
}

template<class key_T, class value_T>
inline StlPack & operator << (StlPack & p, const std::map<key_T, value_T> & val)
{
	p.pushContainer2<std::map<key_T, value_T> >(val);
	return p;
}

template<class key_T, class value_T>
inline StlPack & operator << (StlPack & p, const std::unordered_map<key_T, value_T> & val)
{
	p.pushContainer2<std::unordered_map<key_T, value_T> >(val);
	return p;
}

template<class key_T, class value_T>
inline StlPack & operator << (StlPack & p, const std::multimap<key_T, value_T> & val)
{
	p.pushContainer2<std::multimap<key_T, value_T> >(val);
	return p;
}

//StlUnpack

inline StlUnpack & operator >> (StlUnpack & p, uint32_t & u32)
{
	p.pop((uint32_t&)u32);
	return p;
}

inline StlUnpack & operator >> (StlUnpack & p, uint64_t & u64)
{
	p.pop((uint64_t&)u64);
	return p;
}

inline StlUnpack & operator >> (StlUnpack & p, int64_t & i64)
{
	p.pop((int64_t&)i64);
	return p;
}

inline StlUnpack & operator >> (StlUnpack & p, int32_t & i32)
{
	p.pop((int32_t&)i32);
	return p;
}

inline StlUnpack & operator >> (StlUnpack & p, std::string & str)
{
	p.pop((std::string&)str);
	return p;
}

inline StlUnpack & operator >> (StlUnpack & p, uint8_t & u8)
{
	p.pop((uint8_t&)u8);
	return p;
}

inline StlUnpack & operator >> (StlUnpack & p, int8_t & i8)
{
	p.pop((int8_t&)i8);
	return p;
}

inline StlUnpack & operator >> (StlUnpack & p, uint16_t & u16)
{
	p.pop((uint16_t&)u16);
	return p;
}

inline StlUnpack & operator >> (StlUnpack & p, int16_t & i16)
{
	p.pop((int16_t&)i16);
	return p;
}

inline StlUnpack & operator >> (StlUnpack & p, bool & sign)
{
	uint8_t bval;
	p.pop((uint8_t&)bval);
	sign = bval == 0 ? false : true;
	return p;
}

inline StlUnpack & operator >> (StlUnpack & p, Marshallable& m)
{
	m.unmarshal(p);
	return p;
}

template<class T>
inline StlUnpack & operator >> (StlUnpack & p, std::vector<T> & val)
{
	p.popContainer1<std::vector<T> >(val);
	return p;
}

template<class T>
inline StlUnpack & operator >> (StlUnpack & p, std::list<T> & val)
{
	p.popContainer1<std::list<T> >(val);
	return p;
}

template<class T>
inline StlUnpack & operator >> (StlUnpack & p, std::set<T> & val)
{
	p.popContainer3<std::set<T> >(val);
	return p;
}

template<class key_T, class value_T>
inline StlUnpack & operator >> (StlUnpack & p, std::map<key_T, value_T> & val)
{
	p.popContainer2<std::map<key_T, value_T> >(val);
	return p;
}

template<class key_T, class value_T>
inline StlUnpack & operator >> (StlUnpack & p, std::unordered_map<key_T, value_T> & val)
{
	p.popContainer2<std::unordered_map<key_T, value_T> >(val);
	return p;
}

template<class key_T, class value_T>
inline StlUnpack & operator >> (StlUnpack & p, std::multimap<key_T, value_T> & val)
{
	p.popContainer2<std::multimap<key_T, value_T> >(val);
	return p;
}

namespace packtest
{

struct MyTest
{
	int i, j, k;
	bool bval;
	std::string txt;
	std::vector<short> v;
	std::list<std::vector<int>> c;
	std::map<std::string, std::vector<int>> m;
	std::map<string, std::map<std::string, std::vector<int>> > s;

	void toString(string& str) const
	{
		StlPack p;
		p << i << j << k << bval << txt << v << c << m << s;
		str = p.buffer();
	}

	void unString(const string& str)
	{
		StlUnpack up(str);
		up >> i >> j >> k >> bval >> txt >> v >> c >> m >> s;
	}
};

template<typename T>
int Test()
{
	MyTest t1, t2;
	t1.bval = true;
	t1.i = 2;
	t1.j = 3;
	t1.k = 4;
	t1.txt = "abc";
	t1.v.push_back(5);
	t1.v.push_back(6);
	t1.v.push_back(7);
	std::vector<int> ca1, ca2;
	ca1.push_back(8);
	ca1.push_back(9);
	ca2.push_back(10);
	ca2.push_back(11);
	t1.c.push_back(ca1);
	t1.c.push_back(ca2);
	t1.m.insert(std::map<std::string,std::vector<int>>::value_type("xyz", ca1));
	t1.m.insert(std::map<std::string,std::vector<int>>::value_type("ijk", ca2));
	t1.s.insert(std::map<std::string,std::map<std::string, std::vector<int>>>::value_type("opq", t1.m));
	string txt;
	t1.toString(txt);

	t2.unString(txt);
	return 0;
}

}


#endif