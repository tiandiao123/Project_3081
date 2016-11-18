#include "PixelBuffer.h"
#include "ColorData.h"

#define MAX_HISTORY_SIZE 50
#define EXTEND_SHRINK_SIZE 10
#define SHRINK_REQ 20

class PixelBufferHistory
{
public:
    PixelBufferHistory();
    ~PixelBufferHistory();

    void push(PixelBuffer *buf);
    PixelBuffer* pop();
    PixelBuffer* unpop();
    void clear();

    int isEmpty();
    int unpopAvailable();
    int popAvailable();

private:
    void extend();
    void shrink();

    PixelBuffer **m_data;
    int m_size;
    int m_pos;
    int m_filledsize;
    int m_lastop;
    static const int UNPOP = 1;
    static const int POP = 2;
    static const int PUSH = 3;
};