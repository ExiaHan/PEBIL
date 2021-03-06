/* 
 * This file is part of the pebil project.
 * 
 * Copyright (c) 2010, University of California Regents
 * All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _Stack_h_
#define _Stack_h_

#define STACK_GROWTH_FACTOR 2
#define DEFAULT_STACK_SIZE 32

template <class T=uint32_t>
class Stack {
private:
    T* elements;
    uint32_t maxSize;
    int32_t topIndex;

    void growStack(){
        T* newElts = new T[maxSize * STACK_GROWTH_FACTOR];
        memcpy(newElts, elements, maxSize * sizeof(T));
        delete[] elements;
        maxSize *= STACK_GROWTH_FACTOR;
        elements = newElts;
    }

public:
    Stack(){
        maxSize = DEFAULT_STACK_SIZE;
        elements = new T[size];
        topIndex = -1;
    }
    Stack(uint32_t size){
        maxSize = size;
        if (!maxSize){
            maxSize = DEFAULT_STACK_SIZE;
        }
        elements = new T[size];
        topIndex = -1;
    }
    ~Stack(){
        delete[] elements;
    }

    void clear(){
        topIndex = -1;
    }
    void push(T elt){
        if (topIndex + 1 >= maxSize){
            growStack();
        }
        ASSERT((topIndex + 1) < maxSize);
        elements[++topIndex] = elt;
    }
    T pop(){
        ASSERT(topIndex >= 0);
        return elements[topIndex--];
    }
    uint32_t size(){
        return topIndex+1;
    }
    bool empty(){ 
        return (topIndex == -1);
    }
    T top(){
        return elements[topIndex];
    }
    void print(){
        PRINT_INFOR("topIndex:%d\tmaxSize:%d", topIndex, maxSize);
    }
};

#endif
