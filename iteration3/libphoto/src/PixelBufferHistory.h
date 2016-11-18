#include "PixelBuffer.h"
#include "ColorData.h"

#define MAX_HISTORY_SIZE 50
#define EXTEND_SHRINK_SIZE 10
#define SHRINK_REQ 20

/// A modified stack to store undo/redo history of PixelBuffers.
/// Features:
/// - Dynamically resizing memory with shrinking and extension
/// - A max size at which pushing new items overwrites the oldest items
/// - Basic push and pop operations and an unpop operation (for redo)
class PixelBufferHistory
{
public:
    PixelBufferHistory(); ///< Initializes the history array with a single extend operation
    ~PixelBufferHistory(); ///< Frees memory of each stored PixelBuffer and m_data

    void push(PixelBuffer *buf); ///< Adds an item to the current position in the stack, extends if neccessary
    PixelBuffer* pop(); ///< Removes an item from the current position of the stack
    PixelBuffer* unpop(); ///< Returns an item above the current position of the stack
    void clear(); ///< Resets the history and frees memory of items

    int isEmpty();
    int unpopAvailable();
    int popAvailable();

private:
    void extend(); ///< Increases the size of the array, only dropping items if max sized is reached
    void shrink(); ///< Decreases the size of the array, dropping items above the current position

    PixelBuffer **m_data;
    int m_size;
    int m_pos;
    int m_filledsize;
    int m_lastop;
    static const int UNPOP = 1;
    static const int POP = 2;
    static const int PUSH = 3;
};