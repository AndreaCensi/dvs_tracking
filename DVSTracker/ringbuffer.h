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
    int start;
    int end;

    //! Constructor
    /*!
        \param totalSize Size of the buffer.
    */
    RingBuffer(int totalSize = 8192){
        size = totalSize;
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

    T at(int i){
        return buffer[i];
    }

    void set(int i, T value){
        buffer[i] = value;
    }

    /*!
        \return Size of new data available.
    */
    int available(){
        if (end > start)
            return end - start;
        else if(start > end)
            return size - start + end;
        else return 0;
    }

    int latestIndex(){
        int i = end - 1;
        if(end - 1 < 0)
            i = size - 1;
        return i;
    }

    T* latest(){
        int i = end - 1;
        if(end - 1 < 0)
            i = size - 1;
        return &buffer[i];
    }

    T* getNext(){
        if(start != end){
            T *at = &buffer[start];
            start++;
            if(start == size)
                start = 0;
            return at;
        }
        else return 0;
    }

    //! Get latest data window. Available should be called first to know how much new data there is.
    /*!
        \return Pointer to new data.
    */
    T* newData(){
        T *at = &buffer[start];
        int length = newData();
        start += length;
        start %= size;
        return at;
    }
};


#endif // RINGBUFFER_H
