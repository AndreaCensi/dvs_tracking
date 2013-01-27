#ifndef BUFFER_H
#define BUFFER_H

//! A template class for
/*!
    \author Jonas Strubel
*/
template <typename T>
class Buffer
{
private:
    int length;
    T** buffer;
    int currentBuffer;
    int end;

public:
    Buffer(int numBuffers = 5, int totalLength = 4096){
        length = totalLength;
        buffer = new T[length];
        for(int i = 0; i < numBuffers; i++){
            buffer[i] = new T[length];
        }
    }

    ~Buffer(){
        buffer = new T[length];
        for(int i = 0; i < numBuffers; i++){
            delete [] buffer[i];
        }
        delete [] buffer;
    }

    bool available(){

    }

    void add(T t){
        buffer[currentBuffer][end] = t;
    }

    void getData(){

    }

};

#endif // BUFFER_H
