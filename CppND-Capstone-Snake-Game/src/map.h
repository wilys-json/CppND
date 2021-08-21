#ifndef MAP_H
#define MAP_H

#include <vector>
#include <memory>
#include <iostream>

class GameObject;

// Helper class with underlying vector implementation

class Map : public std::enable_shared_from_this<Map> {
  public:
    Map(int Y, int X) :
      row(Y),
      col(X),
      pointers(Y, std::vector<std::shared_ptr<GameObject>>(X)) {};
    std::vector<std::shared_ptr<GameObject>>& operator[](int index) { return pointers[index]; }
    std::vector<std::vector<std::shared_ptr<GameObject>>>::iterator begin() { return pointers.begin();}
    std::vector<std::vector<std::shared_ptr<GameObject>>>::iterator end() { return pointers.end();}

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

    void clear() {
      for (int i = 0; i < pointers.size(); ++i) {
        for (int j = 0; j < pointers[i].size(); ++j) {
          pointers[i][j] = nullptr;
        }
      }
    }
    ~Map() {
      clear();
      pointers.clear();
    }
  private:
    std::vector<std::vector<std::shared_ptr<GameObject>>> pointers;
    int row;
    int col;
};

#endif
