#ifndef MAP_H
#define MAP_H

template <typename T>
class Map
{
private:
    T *map;
    int width;
    int height;

public:
    Map(int w = 128, int h = 128){
        map = new T[w*h];
        width = w;
        height = h;
        reset();
    }

    ~Map(){
        delete [] map;
    }

    void insert(T obj){
        int index = obj.x + obj.y*width;
        map[index] = obj;
    }

    void insert(int x, int y, T obj){
        int index = x + y*width;
        map[index] = obj;
    }

    void get(int x, int y){
        int index = x + y*width;
        return map[index];
    }

    void reset(){
        for(int i = 0; i < width*height;i++)
            map[i] = 0;
    }
};

#endif // MAP_H
