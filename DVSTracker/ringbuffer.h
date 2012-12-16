#ifndef RINGBUFFER_H
#define RINGBUFFER_H

//! A template class for a cyclic buffer
/*!
    \author Jonas Strubel
*/
template <typename T>
class RingBuffer{
private:
    int length;
    int start;
    int end;

public:
    T *buffer;


    //! Constructor
    /*!
        \param totallength length of the buffer.
    */
    RingBuffer(int totalLength = 8192){
        length = totalLength;
        start = 0;
        end = 0;
        buffer = new T[totalLength];
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
        if(end == length)
            end = 0;
    }

    T at(int i){
        return buffer[i];
    }

    T* ref(int i){
        return &buffer[i];
    }

    void set(int i, T value){
        buffer[i] = value;
    }

    /*!
        \return length of new data available.
    */
    int available(){
        if (end > start)
            return end - start;
        else if(start > end)
            return length - start + end;
        else return 0;
    }

    int latestIndex(){
        int i = end - 1;
        if(end - 1 < 0)
            i = length - 1;
        return i;
    }

    T* latest(){
        int i = end - 1;
        if(end - 1 < 0)
            i = length - 1;
        return &buffer[i];
    }

    T* getNext(){
        if(start != end){
            T *at = &buffer[start];
            start++;
            if(start == length)
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
        start %= length;
        return at;
    }

    int size(){
        return length;
    }
};


#endif // RINGBUFFER_H
