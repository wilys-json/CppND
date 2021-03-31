#include "generic_parser.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using std::string;
using std::vector;

// Function templates

template <typename T>
T GenericParser<T>::getValue(const string& filename, const int& position) {
  string line;
  std::ifstream filestream(filename);
  T value = T();
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < position; i++) linestream >> value;
    return value;
  }
  return value;
}

template <typename T>
T GenericParser<T>::getValue(const string& filename, const string& targetKey) {
  string line, key;
  std::ifstream filestream(filename);
  T value = T();
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == targetKey) return value;
      }
    }
  }
  return value;
}

template <typename T>
T GenericParser<T>::getValue(const std::string& filename,
                             const std::string& targetKey,
                             const std::vector<Replace>& inlineReplace,
                             const std::vector<Replace>& valueReplace) {
  string line, key;
  std::ifstream filestream(filename);
  T value = T();
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (!inlineReplace.empty()) {
        for (auto& lreplace : inlineReplace) {
          std::replace(line.begin(), line.end(), lreplace.from, lreplace.to);
        }
      }
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == targetKey) {
          if (!valueReplace.empty()) {
            for (auto& vreplace : valueReplace) {
              std::replace(line.begin(), line.end(), vreplace.from,
                           vreplace.to);
            }
          }
          return value;
        }
      }
    }
  }
  return value;
}

template <typename T>
vector<T> GenericParser<T>::getValues(const string& filename,
                                      const string& targetKey) {
  string line, key;
  std::ifstream filestream(filename);
  T value;
  vector<T> collector{};
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == targetKey) {
          while (linestream >> value) collector.emplace_back(value);
          return collector;
        }
      }
    }
  }
  return collector;
}

template <typename T>
vector<T> GenericParser<T>::getValues(const string& filename,
                                      const int& range) {
  string line, key;
  std::ifstream filestream(filename);
  T value;
  vector<T> collector{};
  if (filestream.is_open()) {
    for (int i = 0; i < range; i++) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> key >> value;
      collector.emplace_back(value);
    }
    return collector;
  }
  return collector;
}

template <typename T>
vector<T> GenericParser<T>::getValues(const string& filename) {
  string line;
  std::ifstream filestream(filename);
  T value;
  vector<T> collector{};
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value) collector.emplace_back(value);
    return collector;
  }
  return collector;
}
