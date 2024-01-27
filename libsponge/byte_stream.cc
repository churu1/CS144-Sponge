#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : m_maxCapacity(capacity), m_writeLen(0), 
m_readLen(0) {  }

size_t ByteStream::write(const string &data) {
    auto ret = min(data.size(), remaining_capacity());
    for(size_t i = 0; i < ret; i++) {
        container.push_back(data[i]);
    }
    m_writeLen += ret;
    return ret;    
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    // 先比较需要拷贝的长度和现有数据的长度，去两者中的较小值
    string retVal;
    auto real_len = min(len, buffer_size());
    
    // 将 real_len 长度的字符合并成字符串并返回
    for(size_t i = 0; i < real_len; i++) {
        retVal += container[i];
    }

    return retVal;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    // 先比较需要移除的长度和现有数据的长度，取两者中的较小值
    auto real_len = min(len, buffer_size());

    // 移除 real_len 次数据
    for(size_t i = 0; i < real_len; i++) {
        container.pop_front();
    }
    m_readLen += real_len;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string ret_val;
    //先比较需要读取的数据的长度和现有的数据的长度，取两者中的较小值
    auto real_len = min(len, buffer_size());
    for(size_t i = 0; i < real_len; i++) {
        ret_val += container.front();
        container.pop_front();
    }
    m_readLen += real_len;
    return ret_val;
}

void ByteStream::end_input() {_input_ended_flag = true;}

bool ByteStream::input_ended() const { return _input_ended_flag; }

size_t ByteStream::buffer_size() const { return container.size(); }

bool ByteStream::buffer_empty() const { return container.empty();}

bool ByteStream::eof() const { return _input_ended_flag && buffer_empty(); }

size_t ByteStream::bytes_written() const { return m_writeLen; }

size_t ByteStream::bytes_read() const { return m_readLen; }

size_t ByteStream::remaining_capacity() const { return m_maxCapacity - buffer_size(); }
