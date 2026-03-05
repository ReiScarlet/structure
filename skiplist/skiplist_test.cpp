#include "skiplist.hpp"

#include <assert.h>

#include <algorithm>
#include <chrono>
#include <numeric>
#include <string>
#include <vector>

void testPerformance(int count) {
  SkipList<int, int> sl(18);  // 对于百万级数据，建议 max_level 设为 16-20
  std::vector<int> data(count);
  std::iota(data.begin(), data.end(), 0);  // 生成 0 到 count-1 的序列

  // 打乱数据以模拟随机插入场景
  std::random_shuffle(data.begin(), data.end());

  std::cout << "\n===== 性能测试 (数据量: " << count << ") =====" << std::endl;

  // 1. 测试插入效率
  auto start = std::chrono::high_resolution_clock::now();
  for (int i : data) {
    sl.insert(i, i);
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> insert_dur = end - start;
  std::cout << "插入耗时: " << insert_dur.count() << " ms" << std::endl;

  sl.printList();

  // 2. 测试查询效率
  std::random_shuffle(data.begin(), data.end());  // 随机化查询顺序
  start = std::chrono::high_resolution_clock::now();
  for (int i : data) {
    sl.search(i);
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> search_dur = end - start;
  std::cout << "查询耗时: " << search_dur.count() << " ms" << std::endl;

  // 3. 测试删除效率
  start = std::chrono::high_resolution_clock::now();
  for (int i : data) {
    sl.remove(i);
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> remove_dur = end - start;
  std::cout << "删除耗时: " << remove_dur.count() << " ms" << std::endl;

  sl.printList();
}

void testSkipList() {
  std::cout << "========== 开始跳表功能测试 ==========" << std::endl;

  // 初始化一个最大高度为 6 的跳表
  SkipList<int, std::string> sl(6);

  // 1. 测试插入
  std::cout << "\n[测试 1] 正在插入数据..." << std::endl;
  sl.insert(1, "Value_1");
  sl.insert(3, "Value_3");
  sl.insert(7, "Value_7");
  sl.insert(8, "Value_8");
  sl.insert(19, "Value_19");
  sl.insert(5, "Value_5");

  sl.printList();

  // 2. 测试查找
  std::cout << "\n[测试 2] 测试查找功能:" << std::endl;
  int searchKeys[] = {7, 10, 1};
  for (int k : searchKeys) {
    std::string* val = sl.search(k);
    if (val) {
      std::cout << "找到 Key " << k << " -> Value: " << *val << std::endl;
    } else {
      std::cout << "未找到 Key " << k << " (符合预期)" << std::endl;
    }
  }

  // 3. 测试更新 (Update)
  std::cout << "\n[测试 3] 更新 Key 3 的值为 'New_Value_3'..." << std::endl;
  sl.insert(3, "New_Value_3");
  std::string* updatedVal = sl.search(3);
  if (updatedVal) std::cout << "更新后的值: " << *updatedVal << std::endl;

  // 4. 测试删除
  std::cout << "\n[测试 4] 删除 Key 7 和 Key 1..." << std::endl;
  sl.remove(7);
  sl.remove(1);
  sl.printList();

  // 5. 边界查找
  std::cout << "\n[测试 5] 再次查找已删除的 Key 1..." << std::endl;
  if (sl.search(1) == nullptr) {
    std::cout << "Key 1 已成功移除。" << std::endl;
  }

  std::cout << "\n========== 测试完成 ==========" << std::endl;
}

int main() {
  // 设置随机种子
  std::srand(std::time(0));

  testSkipList();
  testPerformance(20);
  return 0;
}