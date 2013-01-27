#ifndef MAP_H
#define MAP_H

#include <stdio.h>

//! A template class for a 2D-array
/*!
    \author Jonas Strubel
*/
template <typename T>
class Map{
public:
    T *map;
    int width;
    int height;

    Map(int w = 128, int h = 128){
        width = w;
        height = h;
        map = new T[width*height];
    }

    ~Map(){
        delete [] map;
    }

    void insert(int x, int y, T obj){
        int index = x + y*width;
        map[index] = obj;
    }

    void set(int i, T obj){
        map[i] = obj;
    }

    T get(int x, int y){
        int index = x + y*width;
        return map[index];
    }

    int size(){
        return width*height;
    }
};

#endif // MAP_H
