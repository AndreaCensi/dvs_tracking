#ifndef RINGBUFFER_H
#define RINGBUFFER_H

//! A template class for a cyclic buffer
/*!
    \author Jonas Strubel
*/
template <typename T>
class RingBuffer{
public:
    T *buffer;
    int size;
    int latest;

    //! Constructor
    /*!
        \param totalSize Size of the buffer.
    */
    RingBuffer(int totalSize = 512){
        latest = 0;
        size = totalSize;
        buffer = new T[totalSize];
    }

    //! Destructor
    ~RingBuffer(){
        delete [] buffer;
    }

    //! Add a value to the buffer.
    /*!
        \param value A pointer to an instance of type T.
    */
    void add(T value){
        latest++;
        if(latest == size)
            latest = 0;
        buffer[latest] = value;
    }

    T at(int i){
        return buffer[i];
    }

    void set(int i, T value){
        buffer[i] = value;
    }
};


#endif // RINGBUFFER_H
