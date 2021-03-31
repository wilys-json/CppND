#ifndef GENERIC_PARSER_H
#define GENERIC_PARSER_H

#include <string>
#include <vector>

// Helper structure for string replacements
struct Replace {
  Replace(const char& f, const char& t) : from(f), to(t) {}
  char from;
  char to;
};

// Generic Linux File Parser
template <typename T>
struct GenericParser {
  /**
    Function template to get the k-th (position) element from a file.
    @param filename : name of the file
    @param position : target position
    @return : the target value from the file
  */
  static T getValue(const std::string& filename, const int& position);

  /**
    Function template to get the value of the target key from a file.
    @param filename : name of the file
    @param targetKey : target key
    @return : the value of the target key
  */
  static T getValue(const std::string& filename, const std::string& targetKey);

  /**
    Function template to get the value of the target key, with replacements.
    @param filename : name of the file
    @param targetKey : target key
    @param inlineReplace : char(s) to be replaced in each line
    @param valueReplace : char(s) to be replaced in return value
    @return : the value of the target key
  */
  static T getValue(const std::string& filename,
                          const std::string& targetKey,
                          const std::vector<Replace>& inlineReplace,
                          const std::vector<Replace>& valueReplace);

  /**
    Function template to collect all values from a file into a vector.
    @param filename : name of the file
    @return : the vector of all values in the file
  */
  static std::vector<T> getValues(const std::string& filename);

  /**
    Function template to collect values of the target key into a vector.
    @param filename : name of the file
    @param targetKey : target key
    @return : the vecotr of values of the target key
  */
  static std::vector<T> getValues(const std::string& filename,
                                        const std::string& targetKey);

  /**
    Function template to collect values of keys in a defined range.
    @param filename : name of the file
    @param range : number of items to collect
    @return : the vector of values in the defined range
  */
  static std::vector<T> getValues(const std::string& filename,
                                        const int& range);
};

#endif
