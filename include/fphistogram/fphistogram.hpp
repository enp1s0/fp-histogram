#ifndef __MTK_FP_HISTOGRAM_HPP__
#define __MTK_FP_HISTOGRAM_HPP__
#include <iostream>
#include <climits>
#include <vector>
#include <functional>

namespace mtk {
namespace fphistogram {
// mode
struct mode_log10;
struct mode_log2;

namespace detail {

// log ratio
template <class Mode>
int update_exponent(const int e);
template <>
int update_exponent<mode_log2 >(const int e_log2) {return e_log2;};
template <>
int update_exponent<mode_log10>(const int e_log2) {return 0.30102999566 * e_log2;};

// exponent mask and shift
template <class T>
unsigned get_exp(const T v);
template <>
unsigned get_exp<double>(const double v) {return ((*reinterpret_cast<const unsigned long*>(&v)) >> 52) & 0x7ff;}
template <>
unsigned get_exp<float >(const float  v) {return ((*reinterpret_cast<const unsigned*>(&v)) >> 23) & 0xff;}

template <class T>
unsigned get_bias();
template <>
unsigned get_bias<double>() {return 0x3ff;}
template <>
unsigned get_bias<float >() {return 0x7f;}
} // namespace detail
template <class T, class Mode = mode_log2>
void print_histogram(const std::function<T(const std::size_t)> iter, const std::size_t size, const unsigned num_all_stars = 100) {
	if (size == 0) {
		std::printf("Nothing to print: the size of input array is zero\n");
		return;
	}

	unsigned min_exp_value = UINT_MAX;
	unsigned max_exp_value = 0;
	for (std::size_t i = 0; i < size; i++) {
		const unsigned exp_v = detail::update_exponent<Mode>(detail::get_exp(iter(i)));
		if (exp_v == 0) continue;
		min_exp_value = std::min(min_exp_value, exp_v);
		max_exp_value = std::max(max_exp_value, exp_v);
	}

	std::vector<std::size_t> counter(max_exp_value - min_exp_value + 1);
	for (auto &v : counter) v = 0u;

	std::size_t num_zero = 0u;
	for (unsigned i = 0; i < size; i++) {
		const auto exp_v = detail::update_exponent<Mode>(detail::get_exp(iter(i)));
		if (exp_v == 0) {
			num_zero++;
			continue;
		}
		counter[exp_v - min_exp_value]++;
	}

	// Draw graph
	if (min_exp_value < UINT_MAX) {
		std::printf("[  exp ](   count  ){    ratio   }\n");
		for (unsigned j = 0; j < counter.size(); j++) {
			const unsigned i = counter.size() - j - 1;
			const auto exp_with_bias = static_cast<int>(min_exp_value) - detail::update_exponent<Mode>(detail::get_bias<T>()) + i;
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
	print_histogram(iter, size, num_all_stars);
}

template <class T, class Mode = mode_log2>
void print_histogram(const std::vector<T>& fp_list_vec, const unsigned num_all_stars = 100) {
	print_histogram(fp_list_vec.data(), fp_list_vec.size(), num_all_stars);
}
} // namespace fphistogram
} // namespace mtk
#endif
