#include <array>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <new>
#include <vector>

struct Vec {
  int* arr = nullptr;
  int len = 0;
  int cap = 0;

  void push(int x) {
    if (arr == nullptr) {
      arr = new int[1];
      arr[0] = x;
      len = 1;
      cap = 1;
      return;
    }
    if (len == cap) {
      int nextcap = cap * 2;
      auto newarr = new int[nextcap];
      for (int i = 0; i < cap; ++i) {
        newarr[i] = arr[i];
      }
      delete[] arr;
      newarr[cap] = x;
      cap = nextcap;
      len++;
      arr = newarr;
    } else {
      arr[len] = x;
      len++;
    }
  }

  int get(int i) { return arr[i]; }

  void print() {
    if (arr != nullptr) {
      std::cout << arr[0];
    }
    for (int i = 1; i < cap; ++i) {
      std::cout << ' ' << arr[i];
    }
    std::cout << std::endl;
  }

  void clear() {
    delete[] arr;
    arr = nullptr;
    cap = 0;
    len = 0;
  }
};

struct Queue {
  int* arr = nullptr;
  int cap = 0;
  int poll_idx = 0;
  int push_idx = INT32_MIN;  // empty

  bool poll(int& x) {
    if (arr == nullptr || push_idx == INT32_MIN) return false;
    x = arr[poll_idx];
    arr[poll_idx] = -1;
    poll_idx = (poll_idx + 1) % cap;
    // if empty
    if (poll_idx == push_idx) {
      poll_idx = 0;
      push_idx = INT32_MIN;
    }
    return true;
  }
  bool push(int x) {
    // if uninit
    if (arr == nullptr) {
      arr = new (std::nothrow) int[1];
      if (arr == nullptr) return false;
      cap = 1;
      arr[0] = x;
      poll_idx = 0;
      push_idx = 0;
      return true;
    }

    // if empty
    if (push_idx == INT32_MIN) {
      arr[0] = x;
      poll_idx = 0;
      push_idx = 1 % cap;
      return true;
    }

    // if full
    if (push_idx == poll_idx) {
      int newcap = cap * 2;
      auto newarr = new (std::nothrow) int[newcap];
      if (newarr == nullptr) return false;

      int j = 0;
      for (int i = poll_idx; i < cap; ++i) {
        newarr[j] = arr[i];
        j++;
      }
      for (int i = 0; i < push_idx; ++i) {
        newarr[j] = arr[i];
        j++;
      }

      delete[] arr;
      newarr[cap] = x;
      push_idx = (cap + 1) % newcap;
      cap = newcap;
      poll_idx = 0;
      arr = newarr;

      return true;
    }

    // or else
    arr[push_idx] = x;
    push_idx = (push_idx + 1) % cap;
    return true;
  }

  void print() {
    std::cout << "cap: " << cap << " push_idx: " << push_idx << " | ";
    if (arr != nullptr) {
      std::cout << arr[0];
    }
    for (int i = 1; i < cap; ++i) {
      std::cout << ' ' << arr[i];
    }
    std::cout << std::endl;
  }

  void clear() {
    delete[] arr;
    arr = nullptr;
    cap = 0;
    poll_idx = 0;
    push_idx = 0;
  }
};

// euclid modulo
int mod(int a, int b) {
  assert(b != 0);
  if (b == -1) return 0;
  int m = a % b;
  if (m < 0) {
    m = (b < 0) ? m - b : m + b;
  }
  return m;
}

#define DBG

template <typename T>
struct Ring {
  T* arr = nullptr;
  int cap = 0;
  int poll_idx = 0;
  int push_idx = INT32_MIN;  // empty

  bool poll_front(T& x) {
    if (arr == nullptr || push_idx == INT32_MIN) return false;
    x = arr[poll_idx];
#ifdef DBG
    arr[poll_idx] = -1;
#endif
    poll_idx = (poll_idx + 1) % cap;
    // if empty
    if (poll_idx == push_idx) {
      poll_idx = 0;
      push_idx = INT32_MIN;
    }
    return true;
  }
  bool poll_back(T& x) {
    if (arr == nullptr || push_idx == INT32_MIN) return false;
    push_idx = mod(push_idx - 1, cap);
    x = arr[push_idx];
#ifdef DBG
    arr[push_idx] = -1;
#endif
    // if empty
    if (poll_idx == push_idx) {
      poll_idx = 0;
      push_idx = INT32_MIN;
    }
    return true;
  }

  bool push_front(T x) {
    // if uninit
    if (arr == nullptr) {
      arr = new (std::nothrow) T[1];
      if (arr == nullptr) return false;
      cap = 1;
      arr[0] = x;
      poll_idx = 0;
      push_idx = 0;
      return true;
    }

    // if empty
    if (push_idx == INT32_MIN) {
      arr[0] = x;
      poll_idx = 0;
      push_idx = 1 % cap;
      return true;
    }

    // if full
    if (push_idx == poll_idx) {
      int newcap = cap * 2;
      auto newarr = new (std::nothrow) T[newcap];
      if (newarr == nullptr) return false;

      int j = 0;
      for (int i = poll_idx; i < cap; ++i) {
        newarr[j] = arr[i];
        j++;
      }
      for (int i = 0; i < push_idx; ++i) {
        newarr[j] = arr[i];
        j++;
      }

      delete[] arr;
      newarr[cap] = x;
      push_idx = (cap + 1) % newcap;
      cap = newcap;
      poll_idx = 0;
      arr = newarr;

      return true;
    }

    // or else
    arr[push_idx] = x;
    push_idx = (push_idx + 1) % cap;
    return true;
  }

  bool push_back(T x) {
    // if uninit
    if (arr == nullptr) {
      arr = new (std::nothrow) T[1];
      if (arr == nullptr) return false;
      cap = 1;
      arr[0] = x;
      poll_idx = 0;
      push_idx = 0;
      return true;
    }

    // if empty
    if (push_idx == INT32_MIN) {
      arr[0] = x;
      poll_idx = 0;
      push_idx = 1 % cap;
      return true;
    }

    // if full
    if (push_idx == poll_idx) {
      int newcap = cap * 2;
      auto newarr = new (std::nothrow) T[newcap];
      if (newarr == nullptr) return false;

      int j = 1;
      for (int i = poll_idx; i < cap; ++i) {
        newarr[j] = arr[i];
        j++;
      }
      for (int i = 0; i < push_idx; ++i) {
        newarr[j] = arr[i];
        j++;
      }

      delete[] arr;
      newarr[0] = x;
      push_idx = (cap + 1) % newcap;
      cap = newcap;
      poll_idx = 0;
      arr = newarr;

      return true;
    }

    // or else
    poll_idx = mod(poll_idx - 1, cap);
    arr[poll_idx] = x;
    return true;
  }

#ifdef DBG
  void print() {
    std::cout << "cap: " << cap << " push_idx: " << push_idx << " | ";
    if (arr != nullptr) {
      std::cout << arr[0];
    }
    for (int i = 1; i < cap; ++i) {
      std::cout << ' ' << arr[i];
    }
    std::cout << std::endl;
  }
#endif

  void clear() {
    delete[] arr;
    arr = nullptr;
    cap = 0;
    poll_idx = 0;
    push_idx = 0;
  }
};

void bench() {
  using clk = std::chrono::steady_clock;
  using duration = std::chrono::duration<double, std::milli>;

  std::array<double, 600> resN;
  std::array<double, 600> resT;

  int benchN = 50;
  int benchTimes = 10;

  for (int i = 0; i < (int)resN.size(); ++i) {
    resN[i] = i * benchN;
  }

  for (int n = 0; n < benchTimes; ++n) {
    std::cerr << n << std::endl;
    auto v = Vec();
    // auto v = std::vector<int>();
    // auto v = Queue();
    // auto v = Ring<int>();

    auto t0 = clk::now();
    for (int i = 0; i < (int)resT.size(); ++i) {
      for (int j = 0; j < benchN; ++j) {
        // v.push(j);
        v.push(j);
      }
      duration dur = clk::now() - t0;
      resT[i] += dur.count();
      // std::cout << i * benchN << "\t" << dur.count() << std::endl;
    }

    v.clear();
  }

  for (int i = 0; i < (int)resT.size(); ++i) {
    std::cout << resN[i] << "\t" << resT[i] / (double)benchTimes << std::endl;
  }
}

int main() {
  // std::cout << (-1) % 0 << std::endl;

  bench();

  return 0;

  // auto v = Ring<std::array<unsigned char, 2>>();
  auto v = Ring<int>();
  // std::array

  std::string in = "";
  int i = 1;

  while (in != "q") {
    std::cin >> in;
    if (in == "pf") {
      v.push_front(i);
      i++;
    } else if (in == "pb") {
      v.push_back(i);
      i++;
    } else if (in == "gf") {
      int x = -1;
      v.poll_front(x);
      std::cout << x << std::endl;
    } else if (in == "gb") {
      int x = -1;
      v.poll_back(x);
      std::cout << x << std::endl;
    }
    v.print();
  }

  // v.print();
  // v.push(1);
  // // v.print();
  // v.push(2);
  // // v.print();
  // v.push(3);
  // v.push(4);
  // v.print();
  // int x;
  // v.poll(x);
  // v.poll(x);
  // v.print();
  // v.push(5);
  // v.print();
  // v.push(6);
  // v.print();
  // v.poll(x);
  // v.poll(x);
  // v.poll(x);
  // v.poll(x);
  // v.poll(x);
  // v.poll(x);
  // v.print();
  // v.push(1);
  // v.print();
  // v.push(2);
  // v.push(3);
  // v.push(4);
  // v.print();
  // v.push(5);
  // v.push(6);
  // v.print();
  v.clear();
}
