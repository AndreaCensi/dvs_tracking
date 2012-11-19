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
    int newInsert;

    //! Constructor
    /*!
        \param totalSize Size of the buffer.
    */
    RingBuffer(int totalSize = 512){
        latest = newInsert = 0;
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
        buffer[newInsert] = value;
        newInsert++;
        if(newInsert == size)
            newInsert = 0;
        latest = newInsert-1;
        if(latest == -1)
            latest = size - 1;
    }

    T at(int i){
        return buffer[i];
    }

    void set(int i, T value){
        buffer[i] = value;
    }
};


#endif // RINGBUFFER_H
