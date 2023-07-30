/* ************************************************************************
> File Name:     skip_list.h
> Author:        ryan
> Mail:          xieg678@gmail.com
> Created Time:  Sun Jul 30 17:05:39 2023
> Description:
 ************************************************************************/

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <mutex>

#define STORE_FILE "store/dump_file"

std::string delimiter = ":";

// Class template to implement node
template <typename K, typename V>
class Node {
 public:
  Node() {}
  Node(K k, V v, int);
  ~Node();

  K key() const;
  V value() const;
  void set_value(V);

  // Linear array to hold pointers to next node of different level
  Node<K, V>** forward;
  int node_level;

 private:
  K key_;
  V value_;
};

template <typename K, typename V>
Node<K, V>::Node(const K k, const V v, int level) {
  this->key_ = k;
  this->value_ = v;
  this->node_level = level;

  // level + 1, because array index is from 0 - level
  this->forward = new Node<K, V>*[level + 1];

  // Fill forward array with 0(nullptr)
  ::memset(this->forward, 0, sizeof(Node<K, V>*) * (level + 1));
}

template <typename K, typename V>
Node<K, V>::~Node() {
  delete[] forward;
}

template <typename K, typename V>
K Node<K, V>::key() const {
  return key_;
}

template <typename K, typename V>
V Node<K, V>::value() const {
  return value_;
}

template <typename K, typename V>
void Node<K, V>::set_value(V value) {
  this->value_ = value;
}

// Class template for Skip list
template <typename K, typename V>
class SkipList {
 public:
  SkipList(int);
  ~SkipList();
  int GetRandomLevel();
  Node<K, V>* CreateNode(K, V, int);
  int InsertElement(K, V);
  void DisplayList();
  bool SearchElement(K);
  void DeleteElement(K);
  void DumpFile();
  void LoadFile();
  int size();

 private:
  void GetKeyValueFromString(const std::string& str, std::string* key,
                                 std::string* value);
  bool IsValidString(const std::string& str);

 private:
  // Maximum level of the skip list
  int max_level_;

  // current level of skip list
  int skip_list_level_;

  // pointer to header node
  Node<K, V>* header_;

  // file operator
  std::ofstream file_writer_;
  std::ifstream file_reader_;

  // skiplist current element count
  int element_count_;
  std::mutex mtx_;
};

// create new node
template <typename K, typename V>
Node<K, V>* SkipList<K, V>::CreateNode(const K k, const V v, int level) {
  Node<K, V>* n = new Node<K, V>(k, v, level);
  return n;
}

// Insert given key and value in skip list
// return 1 means element exists
// return 0 means insert successfully
/*
                           +------------+
                           |  insert 50 |
                           +------------+
level 4     +-->1+                                                      100
                 |
                 |                      insert +----+
level 3         1+-------->10+---------------> | 50 |          70       100
                                               |    |
                                               |    |
level 2         1          10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 1         1    4     10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 0         1    4   9 10         30   40  | 50 |  60      70       100
                                               +----+

*/
template <typename K, typename V>
int SkipList<K, V>::InsertElement(const K key, const V value) {
  std::lock_guard<std::mutex> lock(mtx_);
  Node<K, V>* current = this->header_;

  // create update array and initialize it
  // update is array which put node that the node->forward[i] should be operated
  // later
  Node<K, V>* update[max_level_ + 1];
  ::memset(update, 0, sizeof(Node<K, V>*) * (max_level_ + 1));

  // start form highest level of skip list
  for (int i = skip_list_level_; i >= 0; i--) {
    while (current->forward[i] != nullptr &&
           current->forward[i]->key() < key) {
      current = current->forward[i];
    }
    update[i] = current;
  }

  // reached level 0 and forward pointer to right node, which is desired to
  // insert key.
  current = current->forward[0];

  // if current node have key equal to searched key, we get it
  if (current != nullptr && current->key() == key) {
    std::cout << "key: " << key << ", exists" << std::endl;
    return 1;
  }

  // if current is nullptr that means we have reached to end of the level
  // if current's key is not equal to key that means we have to insert node
  // between update[0] and current node
  if (current == nullptr || current->key() != key) {
    // Generate a random level for node
    int random_level = GetRandomLevel();

    // If random level is greater thar skip list's current level, initialize
    // update value with pointer to header
    if (random_level > skip_list_level_) {
      for (int i = skip_list_level_ + 1; i < random_level + 1; i++) {
        update[i] = header_;
      }
      skip_list_level_ = random_level;
    }

    // create new node with random level generated
    Node<K, V>* inserted_node = CreateNode(key, value, random_level);

    // insert node
    for (int i = 0; i <= random_level; i++) {
      inserted_node->forward[i] = update[i]->forward[i];
      update[i]->forward[i] = inserted_node;
    }
    std::cout << "Successfully inserted key:" << key << ", value:" << value
              << std::endl;
    element_count_++;
  }
  return 0;
}

// Display skip list
template <typename K, typename V>
void SkipList<K, V>::DisplayList() {
  std::cout << "\n*****Skip List*****"
            << "\n";
  for (int i = 0; i <= skip_list_level_; i++) {
    Node<K, V>* node = this->header_->forward[i];
    std::cout << "Level " << i << ": ";
    while (node != nullptr) {
      std::cout << node->key() << ":" << node->value() << ";";
      node = node->forward[i];
    }
    std::cout << std::endl;
  }
}

// Dump data in memory to file
template <typename K, typename V>
void SkipList<K, V>::DumpFile() {
  std::cout << "DumpFile-----------------" << std::endl;
  file_writer_.open(STORE_FILE);
  Node<K, V>* node = this->header_->forward[0];

  while (node != nullptr) {
    file_writer_ << node->key() << ":" << node->value() << "\n";
    std::cout << node->key() << ":" << node->value() << ";\n";
    node = node->forward[0];
  }

  file_writer_.flush();
  file_writer_.close();
}

// Load data from disk
template <typename K, typename V>
void SkipList<K, V>::LoadFile() {
  file_reader_.open(STORE_FILE);
  std::cout << "LoadFile-----------------" << std::endl;
  std::string line_str{};
  std::string key_str{};
  std::string value_str{};
  //std::string* key = new std::string();
  //std::string* value = new std::string();
  while (std::getline(file_reader_, line_str)) {
    GetKeyValueFromString(line_str, key_str, value_str);
    if (key_str.empty() || value_str.empty()) {
      continue;
    }
    auto key = static_cast<K>(key_str);
    auto value = static_cast<V>(value_str);
    InsertElement(key, value);
    std::cout << "key:" << key_str << "value:" << value_str << std::endl;
  }
  file_reader_.close();
}

// Get current SkipList size
template <typename K, typename V>
int SkipList<K, V>::size() {
  return element_count_;
}

template <typename K, typename V>
void SkipList<K, V>::GetKeyValueFromString(const std::string& str,
                                               std::string* key,
                                               std::string* value) {
  if (!IsValidString(str)) {
    return;
  }
  *key = str.substr(0, str.find(delimiter));
  *value = str.substr(str.find(delimiter) + 1, str.length());
}

template <typename K, typename V>
bool SkipList<K, V>::IsValidString(const std::string& str) {
  if (str.empty()) {
    return false;
  }
  if (str.find(delimiter) == std::string::npos) {
    return false;
  }
  return true;
}

// Delete element from skip list
template <typename K, typename V>
void SkipList<K, V>::DeleteElement(K key) {
  std::lock_guard<std::mutex> lock(mtx_);
  Node<K, V>* current = this->header_;
  Node<K, V>* update[max_level_ + 1];
  ::memset(update, 0, sizeof(Node<K, V>*) * (max_level_ + 1));

  // start from highest level of skip list
  for (int i = skip_list_level_; i >= 0; i--) {
    while (current->forward[i] != nullptr &&
           current->forward[i]->key() < key) {
      current = current->forward[i];
    }
    update[i] = current;
  }

  current = current->forward[0];
  if (current != nullptr && current->key() == key) {
    // start for lowest level and delete the current node of each level
    for (int i = 0; i <= skip_list_level_; i++) {
      // if at level i, next node is not target node, break the loop.
      if (update[i]->forward[i] != current) break;

      update[i]->forward[i] = current->forward[i];
    }

    // Remove levels which have no elements
    while (skip_list_level_ > 0 && header_->forward[skip_list_level_] == 0) {
      skip_list_level_--;
    }

    std::cout << "Successfully deleted key " << key << std::endl;
    element_count_--;
  }
  }

// Search for element in skip list
/*
                           +------------+
                           |  select 60 |
                           +------------+
level 4     +-->1+                                                      100
                 |
                 |
level 3         1+-------->10+------------------>50+           70       100
                                                   |
                                                   |
level 2         1          10         30         50|           70       100
                                                   |
                                                   |
level 1         1    4     10         30         50|           70       100
                                                   |
                                                   |
level 0         1    4   9 10         30   40    50+-->60      70       100
*/
template <typename K, typename V>
bool SkipList<K, V>::SearchElement(K key) {
  std::cout << "SearchElement-----------------" << std::endl;
  Node<K, V>* current = header_;

  // start from highest level of skip list
  for (int i = skip_list_level_; i >= 0; i--) {
    while (current->forward[i] && current->forward[i]->key() < key) {
      current = current->forward[i];
    }
  }

  // reached level 0 and advance pointer to right node, which we search
  current = current->forward[0];

  // if current node have key equal to searched key, we get it
  if (current and current->key() == key) {
    std::cout << "Found key: " << key << ", value: " << current->value()
              << std::endl;
    return true;
  }

  std::cout << "Not Found Key:" << key << std::endl;
  return false;
}

// construct skip list
template <typename K, typename V>
SkipList<K, V>::SkipList(int max_level) {
  this->max_level_ = max_level;
  this->skip_list_level_ = 0;
  this->element_count_ = 0;

  // create header node and initialize key and value to null
  K k;
  V v;
  this->header_ = new Node<K, V>(k, v, max_level_);
};

template <typename K, typename V>
SkipList<K, V>::~SkipList() {
  if (file_writer_.is_open()) {
    file_writer_.close();
  }
  if (file_reader_.is_open()) {
    file_reader_.close();
  }
  delete header_;
}

template <typename K, typename V>
int SkipList<K, V>::GetRandomLevel() {
  int k = 1;
  while (std::rand() % 2) {
    k++;
  }
  k = (k < max_level_) ? k : max_level_;
  return k;
};
// vim: et tw=100 ts=4 sw=4 cc=120
