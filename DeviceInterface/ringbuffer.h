#ifndef RINGBUFFER_H
#define RINGBUFFER_H

//! A template class for a cyclic buffer
/*!
    \author Jonas Strubel
*/
template <typename T>
class RingBuffer{

private:
    int size;
    int winSize;
    int start;
    int end;
    T *buffer;

public:
    //! Constructor
    /*!
        \param totalSize Size of the buffer.
        \param windowSize Size of a data window. Allows asking if new data is available. Ensures proper buffer size( multiple of windowSize).
    */
    RingBuffer(int totalSize = 1024, int windowSize = 256){
        int remainder = totalSize%windowSize;       //totalSize should be a multiple of windowSize
        if(remainder == 0)
            size = totalSize;
        else size = totalSize - remainder;
        winSize = windowSize;

        start = 0;
        end = 0;
        buffer = new T[totalSize];
    }

    //! Destructor
    ~RingBuffer(){
        delete [] buffer;
    }

    //! Add a value to the buffer.
    /*!
        \param value A single instance of type T.
    */
    void add(T value){
        buffer[end] = value;
        end++;
        if(end == size)
            end = 0;
    }

    //! Get latest data window.
    /*!
        \return Pointer to new data.
    */
    T *data(){
        T *at = &buffer[start];
        start += winSize;
        start %= size;
        return at;
    }

    /*!
        \return True if new data of at least the windowSize is available.
    */
    bool newData(){
        if(start > end)
            return true;
        else if (end - start >= winSize)
            return true;
        else return false;
    }
};


#endif // RINGBUFFER_H
