#ifndef MAP_H
#define MAP_H

#include <vector>
#include <memory>
#include <iostream>

// Helper class with underlying 2D vector implementation
template <class T>
class Map : public std::enable_shared_from_this<Map<T>> {
  public:
    Map() {};
    Map(int Y, int X) :
      row(Y),
      col(X),
      pointers(Y, std::vector<std::shared_ptr<T>>(X)) {};

    // Mimic the behavior of vector
    std::vector<std::shared_ptr<T>>& operator[](const int& index) {
      return pointers[index];
    }
    typename std::vector<std::vector<std::shared_ptr<T>>>::iterator begin() {
      return pointers.begin();
    }
    typename std::vector<std::vector<std::shared_ptr<T>>>::iterator end() {
      return pointers.end();
    }
    std::vector<std::shared_ptr<T>>& at(const int& rowIndex) {
      return pointers[rowIndex];
    }
    std::shared_ptr<T>& at(const int& rowIndex, const int& colIndex) {
      return pointers[rowIndex][colIndex];
    }


    // for debugging
    void print() {
      for (auto row : pointers) {
        for (auto pointer : row) {
          if (pointer == nullptr) {
            std::cout << "-";
          } else {
            std::cout << "*";
          }
        }
        std::cout << "\n";
      }
      std::cout << "\n\n" << std::endl;
    };

    // for updating during game loop
    void clear() {
      for (int i = 0; i < pointers.size(); ++i) {
        for (int j = 0; j < pointers[i].size(); ++j) {
          pointers[i][j] = nullptr;
        }
      }
    }

    // destructor
    ~Map() {
      clear();
      pointers.clear();
    }

  private:
    std::vector<std::vector<std::shared_ptr<T>>> pointers;
    int row;
    int col;
};

#endif
