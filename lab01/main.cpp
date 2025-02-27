#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <random>
#include <unordered_map>

using clk = std::chrono::steady_clock;
using duration = std::chrono::duration<double, std::milli>;

template <typename T>
void print(T x) {
  std::cout << x << std::endl;
}

// ported from Rust
// prevents compiler optimisations
template <typename T>
T black_box(T x) {
  asm volatile("" : "+r"(x));
  return x;
}

template <typename T, size_t N>
auto linear_search(std::span<T, N> s, T needle) {
  size_t n = s.size();
  for (size_t i = 0; i < n; i++) {
    if (s[i] == needle) {
      return true;
    }
  }
  return false;
}

template <typename T, size_t N>
auto binary_search(std::span<T, N> s, T needle) {
  size_t n = s.size();
  size_t l = 0, r = n - 1;
  while (l < r) {
    auto m = l + (r - l) / 2;

    if (s[m] == needle) return true;

    if (s[m] < needle)
      l = m + 1;
    else
      r = m;
  }
  return false;
}

template <typename T, size_t N>
auto sum_search(std::span<T, N> s, T sum) {
  size_t n = s.size();
  for (size_t i = 0; i < n; i++) {
    for (size_t j = i + 1; j < n; j++) {
      if (sum - s[i] == s[j]) {
        return true;
      }
    }
  }
  return false;
}

template <typename T, size_t N>
auto sum_hashmap_search(std::span<T, N> s, T sum) {
  size_t n = s.size();
  std::unordered_map<T, size_t> reverse_map;
  for (size_t i = 0; i < n; i++) {
    reverse_map.insert(std::make_pair(s[i], i));
  }

  for (size_t i = 0; i < n; i++) {
    auto j = reverse_map.find(sum - s[i]);
    if (j != reverse_map.end()) {
      return true;
    }
  }

  return false;
}

template <typename T, size_t N>
auto sum_linear_search(std::span<T, N> s, T sum) {
  size_t n = s.size();
  size_t i = 0, j = n - 1;

  while (i < j) {
    int ss = sum - s[i];
    if (ss == 0) {
      return true;
    } else if (ss < s[j]) {
      i++;
    } else {
      j--;
    }
  }

  return false;
}

template <typename T>
void swap(T& x, T& y) {
  T tmp = x;
  x = y;
  y = tmp;
}

template <typename T, size_t N>
auto findA(std::span<T, N> s, T needle) {
  size_t n = s.size();
  for (size_t i = 0; i < n; i++) {
    if (s[i] == needle) {
      swap(s[0], s[i]);
      return true;
    }
  }
  return false;
}

template <typename T, size_t N>
auto findB(std::span<T, N> s, T needle) {
  size_t n = s.size();
  for (size_t i = 0; i < n; i++) {
    if (s[i] == needle) {
      if (i != 0) {
        swap(s[i - 1], s[i]);
      }
      return true;
    }
  }
  return false;
}

std::vector<int32_t> cache(1000000); // filled with zeros on every bench

template <typename T, size_t N>
auto findC(std::span<T, N> s, T needle) {
  size_t n = s.size();
  for (size_t i = 0; i < n; i++) {
    if (s[i] == needle) {
      cache[i]++;
      if (i != 0 && cache[i - 1] < cache[i]) {
        swap(s[i - 1], s[i]);
        swap(cache[i - 1], cache[i]);
      }
      return true;
    }
  }
  return false;
}

template <typename T, size_t N, typename F>
void bench_with(std::span<T, N> s, size_t i, F f) {
  std::mt19937 mt{};
  std::uniform_int_distribution<int32_t> dist(0, ((int)i - 1) * 2);
  auto t0 = clk::now();
  int benchN = 1000;
  for (int j = 0; j < benchN; j++) {
    f(s.subspan(0, i), dist(mt));
  }
  duration dur = clk::now() - t0;
  std::cout << i << "\t" << dur.count() / (double)benchN << std::endl;
}

template <typename T, size_t N, typename F>
void bench_with_fixed(std::span<T, N> s, size_t i, F f, int32_t needle) {
  auto t0 = clk::now();
  int benchN = 1000;
  for (int j = 0; j < benchN; j++) {
    f(s.subspan(0, i), needle);
  }
  duration dur = clk::now() - t0;
  std::cout << i << "\t" << dur.count() / (double)benchN << std::endl;
}

template <typename T, size_t N, typename F>
void bench_with_cache(std::span<T, N> s, size_t i, F f) {
  std::mt19937 mt{};
  std::uniform_int_distribution<int32_t> dist(0, ((int)i - 1) * 2);
  std::iota(std::begin(s), std::end(s), 0);
  std::fill(std::begin(cache), std::end(cache), 0);
  auto t0 = clk::now();
  int benchN = 1000;
  for (int j = 0; j < benchN; j++) {
    f(s.subspan(0, i), dist(mt));
  }
  duration dur = clk::now() - t0;
  std::cout << i << "\t" << dur.count() / (double)benchN << std::endl;
}

template <typename T, size_t N, typename F>
void bench_with_cache_the_same(std::span<T, N> s, size_t i, F f) {
  std::mt19937 mt{};
  std::uniform_int_distribution<int32_t> dist(0, ((int32_t)i - 1) * 2);
  std::iota(std::begin(s), std::end(s), 0);
  std::fill(std::begin(cache), std::end(cache), 0);
  auto t0 = clk::now();
  int32_t benchN = 1000;
  auto needle = dist(mt);
  for (int32_t j = 0; j < benchN; j++) {
    f(s.subspan(0, i), needle);
  }
  duration dur = clk::now() - t0;
  std::cout << i << "\t" << dur.count() / (double)benchN << std::endl;
}

template <typename T, size_t N, typename F>
void bench_with_cache_square(std::span<T, N> s, size_t i, F f) {
  std::mt19937 mt{};
  std::uniform_int_distribution<int32_t> dist(
      0, (int32_t)std::sqrt(((int32_t)i - 1)));
  std::iota(std::begin(s), std::end(s), 0);
  std::fill(std::begin(cache), std::end(cache), 0);
  auto t0 = clk::now();
  int32_t benchN = 1000;
  for (int32_t j = 0; j < benchN; j++) {
    auto needle = dist(mt);
    f(s.subspan(0, i), needle * needle);
  }
  duration dur = clk::now() - t0;
  std::cout << i << "\t" << dur.count() / (double)benchN << std::endl;
}

int main() {
  const size_t N = 1000000;
  std::array<int32_t, N> arr;
  std::iota(std::begin(arr), std::end(arr), 0);  // 0 1 2 3 4 5 ... N-1
  auto s = std::span(arr);                       // to take subspans

  auto lin = [](auto s, auto x) {
    black_box(linear_search(black_box(s), black_box(x)));
  };

  auto bin = [](auto s, auto x) {
    black_box(binary_search(black_box(s), black_box(x)));
  };

  auto sum1 = [](auto s, auto x) {
    black_box(sum_search(black_box(s), black_box(x)));
  };

  auto sum2 = [](auto s, auto x) {
    black_box(sum_hashmap_search(black_box(s), black_box(x)));
  };

  auto sum3 = [](auto s, auto x) {
    black_box(sum_linear_search(black_box(s), black_box(x)));
  };

  auto finda = [](auto s, auto x) {
    black_box(findA(black_box(s), black_box(x)));
  };

  auto findb = [](auto s, auto x) {
    black_box(findB(black_box(s), black_box(x)));
  };

  auto findc = [](auto s, auto x) {
    black_box(findC(black_box(s), black_box(x)));
  };

  auto bench_by = [&](auto f, auto step, auto n) {
    bench_with(s, 100, f);
    for (size_t i = step; i <= n * step; i += step) {
      bench_with(s, i, f);
    };
  };
  auto bench_by_fixed = [&](auto f, auto step, auto n) {
    bench_with_fixed(s, 100, f, N + 100);
    for (size_t i = step; i <= n * step; i += step) {
      bench_with_fixed(s, i, f, N + 100);
    };
  };
  auto bench_by_cache = [&](auto f, auto step, auto n) {
    bench_with_cache(s, 100, f);
    for (size_t i = step; i <= n * step; i += step) {
      bench_with_cache(s, i, f);
    };
  };
  auto bench_by_cache_the_same = [&](auto f, auto step, auto n) {
    bench_with_cache_the_same(s, 100, f);
    for (size_t i = step; i <= n * step; i += step) {
      bench_with_cache_the_same(s, i, f);
    };
  };
  auto bench_by_cache_square = [&](auto f, auto step, auto n) {
    bench_with_cache_square(s, 100, f);
    for (size_t i = step; i <= n * step; i += step) {
      bench_with_cache_square(s, i, f);
    };
  };

  // bench_by(sum1, 1000, 10);
  // bench_by(bin, 1000, 1000);

  // bench_by_cache(findc, 100000, 10);
  // bench_by_cache_the_same(findc, 100000, 10);
  // bench_by_cache_square(findc, 100000, 10);

  bench_by(lin, 10000, 100);

  // bench_by(sum3, 100000, 10);
}
