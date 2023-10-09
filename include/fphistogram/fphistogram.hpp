#ifndef __MTK_FP_HISTOGRAM_HPP__
#define __MTK_FP_HISTOGRAM_HPP__
#include <iostream>
#include <climits>
#include <vector>
#include <functional>
#include <cmath>

namespace mtk {
namespace fphistogram {
// mode
struct mode_log10;
struct mode_log2;

namespace detail {

// log ratio
template <class Mode>
inline int log_int(const double v);
template <>
inline int log_int<mode_log2 >(const double v) {return std::log2(std::abs(v));};
template <>
inline int log_int<mode_log10>(const double v) {return std::log10(std::abs(v));};

// exponent mask and shift
template <class T>
inline int get_exp(const T v);
template <>
inline int get_exp<double>(const double v) {return ((*reinterpret_cast<const unsigned long*>(&v)) >> 52) & 0x7ff;}
template <>
inline int get_exp<float >(const float  v) {return ((*reinterpret_cast<const unsigned*>(&v)) >> 23) & 0xff;}

template <class T>
inline int get_bias();
template <>
inline int get_bias<double>() {return 0x3ff;}
template <>
inline int get_bias<float >() {return 0x7f;}
} // namespace detail

template <class T, class Mode = mode_log2>
inline void print_histogram(const std::function<T(const std::size_t)> iter, const std::size_t size, const unsigned num_all_stars = 100) {
  if (size == 0) {
    std::printf("Nothing to print: the size of input array is zero\n");
    return;
  }

  int min_exp_value = INT_MAX;
  int max_exp_value = INT_MIN;
#pragma omp parallel for reduction(min: min_exp_value) reduction(max:max_exp_value)
  for (std::size_t i = 0; i < size; i++) {
    const auto v = iter(i);
    const unsigned exp_v = detail::get_exp(v);
    if (exp_v == 0) continue;
    const auto exp_v_with_bias = detail::log_int<Mode>(static_cast<double>(v));
    min_exp_value = std::min(min_exp_value, exp_v_with_bias);
    max_exp_value = std::max(max_exp_value, exp_v_with_bias);
  }

  std::vector<std::size_t> counter(max_exp_value - min_exp_value + 1);
  for (auto &v : counter) v = 0u;

  std::size_t num_zero = 0u;
#pragma omp parallel for reduction(+: num_zero)
  for (unsigned i = 0; i < size; i++) {
    const auto v = iter(i);
    const unsigned exp_v = detail::get_exp(v);
    if (exp_v == 0) {
      num_zero++;
      continue;
    }
    const auto exp_v_with_bias = detail::log_int<Mode>(static_cast<double>(v));
#pragma omp critical
    {counter[exp_v_with_bias - min_exp_value]++;}
  }

  // Draw graph
  if (min_exp_value < INT_MAX) {
    if (std::is_same<Mode, mode_log2>::value) {
      std::printf("[ exp2]");
    } else if (std::is_same<Mode, mode_log10>::value) {
      std::printf("[exp10]");
    }
    std::printf("(   count  ){    ratio   }{  acc }\n");
    std::size_t acc = 0;
    for (unsigned j = 0; j < counter.size(); j++) {
      const int i = counter.size() - j - 1;
      acc += counter[i];
      const auto exp_with_bias = min_exp_value + i;
      const auto ratio = static_cast<double>(counter[i]) / size;
      const auto ratio_accumulation = static_cast<double>(acc) / size;
      std::printf("[%+5d]", exp_with_bias);
      std::printf("(%10lu){%e}{%6.2f}:", counter[i], ratio, ratio_accumulation);
      for (unsigned i = 0; i < static_cast<unsigned>(ratio * num_all_stars); i++) {
        std::printf("*");
      }
      std::printf("\n");
    }
    std::printf("-----\n");
  }
  const auto ratio = static_cast<double>(num_zero) / size;
  std::printf("[ zero ]");
  std::printf("(%10lu){%e}:", num_zero, ratio);
  for (unsigned i = 0; i < static_cast<unsigned>(ratio * num_all_stars); i++) {
    std::printf("*");
  }
  std::printf("\n");
}

template <class T, class Mode = mode_log2>
inline void print_histogram(const T* const fp_list, const std::size_t size, const unsigned num_all_stars = 100) {
  std::function<double(const std::size_t)> iter = [&fp_list](const std::size_t i) {return fp_list[i];};
  print_histogram<T, Mode>(iter, size, num_all_stars);
}

template <class T, class Mode = mode_log2>
inline void print_histogram(const std::vector<T>& fp_list_vec, const unsigned num_all_stars = 100) {
  print_histogram<T, Mode>(fp_list_vec.data(), fp_list_vec.size(), num_all_stars);
}

template <class T, class Mode = mode_log2>
inline void print_histogram_pm(const std::function<T(const std::size_t)> iter, const std::size_t size, const unsigned num_all_stars = 100) {
  if (size == 0) {
    std::printf("Nothing to print: the size of input array is zero\n");
    return;
  }

  int min_exp_value = INT_MAX;
  int max_exp_value = INT_MIN;
#pragma omp parallel for reduction(min: min_exp_value) reduction(max:max_exp_value)
  for (std::size_t i = 0; i < size; i++) {
    const auto v = iter(i);
    const unsigned exp_v = detail::get_exp(v);
    if (exp_v == 0) continue;
    const auto exp_v_with_bias = detail::log_int<Mode>(static_cast<double>(v));
    min_exp_value = std::min(min_exp_value, exp_v_with_bias);
    max_exp_value = std::max(max_exp_value, exp_v_with_bias);
  }

  std::vector<std::size_t> counter_p(max_exp_value - min_exp_value + 1);
  std::vector<std::size_t> counter_m(max_exp_value - min_exp_value + 1);
  for (auto &v : counter_p) v = 0u;
  for (auto &v : counter_m) v = 0u;

  std::size_t num_zero = 0u;
#pragma omp parallel for reduction(+: num_zero)
  for (unsigned i = 0; i < size; i++) {
    const auto v = iter(i);
    const unsigned exp_v = detail::get_exp(v);
    if (exp_v == 0) {
      num_zero++;
      continue;
    }
    const auto exp_v_with_bias = detail::log_int<Mode>(static_cast<double>(v));
    if (v < 0.) {
#pragma omp critical
      {counter_m[exp_v_with_bias - min_exp_value]++;}
    } else {
#pragma omp critical
      {counter_p[exp_v_with_bias - min_exp_value]++;}
    }
  }

  std::size_t max_counter_m = 0;
  for (const auto &v : counter_m) max_counter_m = std::max(v, max_counter_m);
  const auto max_ratio_m = static_cast<double>(max_counter_m) / size;

  // Draw graph
  std::size_t acc_m = 0, acc_p = 0;
  if (min_exp_value < INT_MAX) {
    for (unsigned i = 0; i < static_cast<unsigned>(max_ratio_m * num_all_stars) + 1; i++) {
      std::printf(" ");
    }
    std::printf(" (  -count  ){   -ratio   }{ -acc }");
    if (std::is_same<Mode, mode_log2>::value) {
      std::printf("[ exp2]");
    } else if (std::is_same<Mode, mode_log10>::value) {
      std::printf("[exp10]");
    }
    std::printf("(  +count  ){   +ratio   }{ +acc }\n");
    for (unsigned j = 0; j < counter_p.size(); j++) {
      const int i = counter_p.size() - j - 1;
      const auto exp_with_bias = min_exp_value + i;
      const auto ratio_m = static_cast<double>(counter_m[i]) / size;
      const auto ratio_p = static_cast<double>(counter_p[i]) / size;
      acc_m += counter_m[i];
      acc_p += counter_p[i];
      const auto acc_ratio_m = static_cast<double>(acc_m) / size;
      const auto acc_ratio_p = static_cast<double>(acc_p) / size;

      // minus
      for (unsigned i = 0; i < static_cast<unsigned>(max_ratio_m * num_all_stars) - static_cast<unsigned>(ratio_m * num_all_stars) + 1; i++) {
        std::printf(" ");
      }
      for (unsigned i = 0; i < static_cast<unsigned>(ratio_m * num_all_stars); i++) {
        std::printf("*");
      }
      std::printf(":(%10lu){%e}{%6.2f}", counter_m[i], ratio_m, acc_ratio_m);
      // plus
      std::printf("[%+5d]", exp_with_bias);
      std::printf("(%10lu){%e}{%6.2f}:", counter_p[i], ratio_p, acc_ratio_p);
      for (unsigned i = 0; i < static_cast<unsigned>(ratio_p * num_all_stars); i++) {
        std::printf("*");
      }
      std::printf("\n");
    }
    std::printf("-----\n");
  }
  const auto ratio = static_cast<double>(num_zero) / size;
  std::printf("[ zero ]");
  std::printf("(%10lu){%e}:", num_zero, ratio);
  for (unsigned i = 0; i < static_cast<unsigned>(ratio * num_all_stars); i++) {
    std::printf("*");
  }
  std::printf("\n");
}

template <class T, class Mode = mode_log2>
inline void print_histogram_pm(const T* const fp_list, const std::size_t size, const unsigned num_all_stars = 100) {
  std::function<double(const std::size_t)> iter = [&fp_list](const std::size_t i) {return fp_list[i];};
  print_histogram_pm<T, Mode>(iter, size, num_all_stars);
}

template <class T, class Mode = mode_log2>
inline void print_histogram_pm(const std::vector<T>& fp_list_vec, const unsigned num_all_stars = 100) {
  print_histogram_pm<T, Mode>(fp_list_vec.data(), fp_list_vec.size(), num_all_stars);
}
} // namespace fphistogram
} // namespace mtk
#endif
