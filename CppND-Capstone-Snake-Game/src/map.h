#ifndef MAP_H
#define MAP_H

#include <vector>
#include <memory>

class GameObject;

// Helper class with underlying vector implementation

class Map : public std::enable_shared_from_this<Map> {
  public:
    Map(int Y, int X) : pointers(Y, std::vector<std::shared_ptr<GameObject>>(X)) {};
    std::vector<std::shared_ptr<GameObject>>& operator[](int index) { return pointers[index]; }
    std::vector<std::vector<std::shared_ptr<GameObject>>>::iterator begin() { return pointers.begin();}
    std::vector<std::vector<std::shared_ptr<GameObject>>>::iterator end() { return pointers.end();}

  private:
    std::vector<std::vector<std::shared_ptr<GameObject>>> pointers;
};

#endif
