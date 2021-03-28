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
int log_int(const double v);
template <>
int log_int<mode_log2 >(const double v) {return std::log2(std::abs(v));};
template <>
int log_int<mode_log10>(const double v) {return std::log10(std::abs(v));};

// exponent mask and shift
template <class T>
int get_exp(const T v);
template <>
int get_exp<double>(const double v) {return ((*reinterpret_cast<const unsigned long*>(&v)) >> 52) & 0x7ff;}
template <>
int get_exp<float >(const float  v) {return ((*reinterpret_cast<const unsigned*>(&v)) >> 23) & 0xff;}

template <class T>
int get_bias();
template <>
int get_bias<double>() {return 0x3ff;}
template <>
int get_bias<float >() {return 0x7f;}
} // namespace detail
template <class T, class Mode = mode_log2>
void print_histogram(const std::function<T(const std::size_t)> iter, const std::size_t size, const unsigned num_all_stars = 100) {
	if (size == 0) {
		std::printf("Nothing to print: the size of input array is zero\n");
		return;
	}

	int min_exp_value = INT_MAX;
	int max_exp_value = INT_MIN;
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
	for (unsigned i = 0; i < size; i++) {
		const auto v = iter(i);
		const unsigned exp_v = detail::get_exp(v);
		if (exp_v == 0) {
			num_zero++;
			continue;
		}
		const auto exp_v_with_bias = detail::log_int<Mode>(static_cast<double>(v));
		counter[exp_v_with_bias - min_exp_value]++;
	}


	// Draw graph
	if (min_exp_value < INT_MAX) {
		std::printf("[  exp ](   count  ){    ratio   }\n");
		for (unsigned j = 0; j < counter.size(); j++) {
			const unsigned i = counter.size() - j - 1;
			const auto exp_with_bias = min_exp_value + i;
			const auto ratio = static_cast<double>(counter[i]) / size;
			if (exp_with_bias < 0) {
				std::printf("[%5d]", exp_with_bias);
			} else {
				std::printf("[ %5d]", exp_with_bias);
			}
			std::printf("(%10lu){%e}:", counter[i], ratio);
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
void print_histogram(const T* const fp_list, const std::size_t size, const unsigned num_all_stars = 100) {
	std::function<double(const std::size_t)> iter = [&fp_list](const std::size_t i) {return fp_list[i];};
	print_histogram<T, Mode>(iter, size, num_all_stars);
}

template <class T, class Mode = mode_log2>
void print_histogram(const std::vector<T>& fp_list_vec, const unsigned num_all_stars = 100) {
	print_histogram<T, Mode>(fp_list_vec.data(), fp_list_vec.size(), num_all_stars);
}
} // namespace fphistogram
} // namespace mtk
#endif
