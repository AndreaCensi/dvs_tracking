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
        for(int i = 0; i < size;i++)
            buffer[i] = 0;
    }

    //! Destructor
    ~RingBuffer(){
        for(int i = 0; i < size; i++){  //delete buffer entries
            if(buffer[i])
                delete  buffer[i];
        }
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
};


#endif // RINGBUFFER_H
