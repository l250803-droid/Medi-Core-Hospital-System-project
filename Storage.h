#pragma once

template <typename T>
class Storage {
private:
    T* data;
    int count;
    int capacity;

public:

  
    Storage() {

        capacity = 100;
        count = 0;

        data = new T[capacity];
    }


    
    ~Storage() {

        delete[] data;
        data = nullptr;
    }


   
    bool add(const T& item) {

       
        if (count == capacity) {
            return false;
        }

       
        data[count] = item;

        
        count = count + 1;

        return true;
    }


  
    bool removeByID(int id) {

        int index = -1;

        
        for (int i = 0; i < count; i = i + 1) {

            if (data[i].getID() == id) {
                index = i;
                break;
            }
        }

       
        if (index == -1) {
            return false;
        }

        
        for (int i = index; i < count - 1; i = i + 1) {
            data[i] = data[i + 1];
        }

     
        count = count - 1;

        return true;
    }


   
    T* findByID(int id) {

        for (int i = 0; i < count; i = i + 1) {

            if (data[i].getID() == id) {
                return &data[i];
            }
        }

        return nullptr;
    }


  
    T* getAll() {

        return data;
    }


   
    int getCount() const {

        return count;
    }


    
    bool isFull() const {

        if (count == capacity) {
            return true;
        }
        else {
            return false;
        }
    }


    void clear() {

        count = 0;
    }
};
