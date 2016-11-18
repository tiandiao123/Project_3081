#include "PixelBufferHistory.h"
#include <iostream>

PixelBufferHistory::PixelBufferHistory() {
    m_size = 0;
    m_filledsize = 0;
    m_pos = -1;
    m_data = 0;
    extend();
}

PixelBufferHistory::~PixelBufferHistory() {
    clear();
    delete [] m_data;
}

void PixelBufferHistory::clear() {
    for (int i = 0; i < m_filledsize; ++i)
        if (m_data[i] != 0) delete m_data[i];
    m_size = 0;
    m_filledsize = 0;
    m_pos = -1;
}

void PixelBufferHistory::push(PixelBuffer *buf) {
    m_pos += 1;
    // if      (m_lastop == PUSH)  m_pos += 1;
    // else if (m_lastop == POP)   m_pos += 1;
    // else if (m_lastop == UNPOP) m_pos += 1;
    // else                        m_pos += 1;
    if (m_pos >= m_size) extend();
    std::cout << "Pushing buffer pos=" << m_pos << " size=" << m_size << " filled=" << m_filledsize << std::endl;
    PixelBuffer *cbuf = new PixelBuffer(buf->getWidth(), buf->getHeight(), buf->getBackgroundColor());
    PixelBuffer::copyPixelBuffer(buf, cbuf);
    m_data[m_pos] = cbuf;
    m_filledsize = m_pos;
    m_lastop = PUSH;
}

PixelBuffer* PixelBufferHistory::pop() {
    if (m_filledsize + EXTEND_SHRINK_SIZE < m_size
            || m_pos + SHRINK_REQ < m_size) shrink();
    if (m_pos <= 0) return 0;
    std::cout << "Popping buffer pos=" << m_pos - 1 << " size=" << m_size << " filled=" << m_filledsize << std::endl;
    m_lastop = POP;
    return m_data[--m_pos];
}

PixelBuffer* PixelBufferHistory::unpop() {
    if (m_lastop == PUSH) return 0;
    if (m_pos >= m_filledsize) return 0;
    std::cout << "Unpopping buffer to pos=" << m_pos + 1 << " size=" << m_size << " filled=" << m_filledsize << std::endl;
    m_lastop = UNPOP;
    return m_data[++m_pos];
}

void PixelBufferHistory::extend() {
    std::cout << "Extending history stack..." << std::endl;
    int n_size = m_size + EXTEND_SHRINK_SIZE;
    int n_off = 0;
    // Check for max size and setup rollover
    if (n_size > MAX_HISTORY_SIZE) {
        std::cout << "Reached maximum size, dropping beginning items..." << std::endl;
        n_size = MAX_HISTORY_SIZE;
        n_off = EXTEND_SHRINK_SIZE;
        m_filledsize -= EXTEND_SHRINK_SIZE;
        m_pos -= EXTEND_SHRINK_SIZE;
    }
    // Allocate and copy
    PixelBuffer **n_data = new PixelBuffer*[n_size];
    for (int i, j = 0; i < n_size && j < m_size; j = ++i + n_off)
        n_data[i] = m_data[j];
    // Cleanup old
    if (m_data) delete [] m_data;
    // Update pointer and size
    m_data = n_data;
    m_size = n_size;
}

void PixelBufferHistory::shrink() {
    std::cout << "Shrinking history stack..." << std::endl;
    int n_size = m_size - EXTEND_SHRINK_SIZE;
    // Sanity check
    if (n_size < m_pos || n_size < EXTEND_SHRINK_SIZE || n_size < EXTEND_SHRINK_SIZE) {
        std::cout << "Stack unready to shrink" << std::endl;
        return;
    }
    // Allocate and copy
    PixelBuffer **n_data = new PixelBuffer*[n_size];
    for (int i = 0; i < n_size; ++i)
        n_data[i] = m_data[i];
    // Cleanup old
    for (int i = n_size; i <= m_filledsize; ++i)
        if (m_data[i]) delete m_data[i];
    delete [] m_data;
    // Update pointer and sizes
    m_data = n_data;
    m_size = n_size;
    m_filledsize = m_size - 1;
}

int PixelBufferHistory::isEmpty() {
    if (m_filledsize == m_pos) return 1;
    else return 0;
}

int PixelBufferHistory::popAvailable() {
    if (m_pos > 0) return true;
    else return false;
}

int PixelBufferHistory::unpopAvailable() {
    if (m_pos < m_filledsize) return true;
    else return false;
}