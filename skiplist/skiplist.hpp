#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>

template <typename K, typename V>
class SkipList {
 private:
  struct Node {
    K key;
    V value;
    // forward[i] 表示该节点在第 i 层的下一个节点
    std::vector<Node*> forward;

    Node(K k, V v, int level) : key(k), value(v), forward(level, nullptr) {}
  };

  Node* head;
  int max_level;
  int current_level;
  const float P = 0.5;  // 晋升概率

  // 随机决定节点高度
  int randomLevel() {
    int lvl = 1;
    while ((float)std::rand() / RAND_MAX < P && lvl < max_level) lvl++;
    return lvl;
  }

 public:
  SkipList(int max_lvl) : max_level(max_lvl), current_level(1) {
    // 初始化头节点，值为空
    head = new Node(K(), V(), max_level);
    std::srand(std::time(0));
  }

  V* search(K key) {
    Node* curr = head;
    // 从最高层开始向下寻找
    for (int i = current_level - 1; i >= 0; i--) {
      while (curr->forward[i] && key > curr->forward[i]->key) {
        curr = curr->forward[i];
      }
    }
    curr = curr->forward[0];
    if (curr && curr->key == key) return &(curr->value);
    return nullptr;
  }

  // 插入操作
  void insert(K key, V value) {
    std::vector<Node*> update(max_level, nullptr);
    Node* curr = head;

    // 找到每一层待插入位置的前驱节点
    for (int i = current_level - 1; i >= 0; i--) {
      while (curr->forward[i] && key > curr->forward[i]->key) {
        curr = curr->forward[i];
      }
      update[i] = curr;
    }

    curr = curr->forward[0];

    // 如果 key 已存在，更新 value
    if (curr && curr->key == key) {
      curr->value = value;
      return;
    }

    // 随机生成层数
    int lvl = randomLevel();
    if (lvl > current_level) {
      for (int i = current_level; i < lvl; i++) update[i] = head;
      current_level = lvl;
    }

    // 创建新节点并更新指针
    Node* newNode = new Node(key, value, lvl);
    for (int i = 0; i < lvl; i++) {
      newNode->forward[i] = update[i]->forward[i]; // 指向待插入节点的下一个节点
      update[i]->forward[i] = newNode; // 待插入节点更新下下一个节点
    }
  }

  // 删除操作
  void remove(K key) {
    std::vector<Node*> update(max_level, nullptr);
    Node* curr = head;

    for (int i = current_level - 1; i >= 0; i--) {
      while (curr->forward[i] && curr->forward[i]->key < key) {
        curr = curr->forward[i];
      }
      update[i] = curr;
    }

    curr = curr->forward[0];
    if (curr && curr->key == key) {
      for (int i = 0; i < current_level; i++) {
        if (update[i]->forward[i] != curr) break;
        update[i]->forward[i] = curr->forward[i];
      }
      delete curr;
      // 更新当前最大层数
      while (current_level > 1 && head->forward[current_level - 1] == nullptr) {
        current_level--;
      }
    }
  }

  void printList() {
    // 1. 获取所有 Key (Level 0) 并计算最大显示宽度
    std::vector<K> all_keys;
    int max_w = 1;
    Node* curr = head->forward[0];
    while (curr) {
      all_keys.push_back(curr->key);
      max_w = std::max(max_w, (int)std::to_string(curr->key).length());
      curr = curr->forward[0];
    }

    std::cout << "\n[SkipList Structure]" << std::endl;

    // 2. 从高层向低层打印
    for (int i = current_level - 1; i >= 0; i--) {
      std::cout << "L" << i << ": ";
      Node* p = head->forward[i];

      for (const auto& k : all_keys) {
        if (p && p->key == k) {
          // 打印 Key，左右留一格空格
          std::cout << " " << std::setw(max_w) << p->key << " ";
          p = p->forward[i];
        } else {
          // 打印占位符，保持宽度一致
          std::cout << std::string(max_w + 2, '-');
        }
      }
      std::cout << " NIL" << std::endl;
    }
  }
};
