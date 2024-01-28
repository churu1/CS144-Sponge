#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : m_buffer(capacity + 1), m_maxCapacity(capacity), m_writeLen(0), 
m_readLen(0), m_head(0), m_tail(m_maxCapacity) {  }

size_t ByteStream::write(const string &data) {
    auto ret = min(data.size(), remaining_capacity());
    for(size_t i = 0; i < ret; i++) {
        m_tail = (m_tail + 1) % (m_maxCapacity + 1);
        m_buffer[m_tail] = data[i];
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
        retVal.push_back(m_buffer[(m_head + i) % (m_maxCapacity + 1)]);
    }

    return retVal;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    // 先比较需要移除的长度和现有数据的长度，取两者中的较小值
    auto real_len = min(len, buffer_size());

    m_head = (m_head + real_len) % (m_maxCapacity + 1);
    m_readLen += real_len;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    auto ret_val = peek_output(len);
    pop_output(len);
    return ret_val;
}

void ByteStream::end_input() {_input_ended_flag = true;}

bool ByteStream::input_ended() const { return _input_ended_flag; }

// 加上 m_maxcapacity + 1 , 是为了防止 m_tail - m_head 出现负数的情况
size_t ByteStream::buffer_size() const { return (m_tail - m_head + 1 + m_maxCapacity + 1) % (m_maxCapacity + 1); }

bool ByteStream::buffer_empty() const { return buffer_size() == 0;}

bool ByteStream::eof() const { return _input_ended_flag && buffer_empty(); }

size_t ByteStream::bytes_written() const { return m_writeLen; }

size_t ByteStream::bytes_read() const { return m_readLen; }

size_t ByteStream::remaining_capacity() const { return m_maxCapacity - buffer_size(); }
