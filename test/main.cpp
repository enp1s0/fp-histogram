#include <iostream>
#include <random>
#include <fphistogram/fphistogram.hpp>

template <class Mode>
const char* get_mode_string();
template <> const char* get_mode_string<mtk::fphistogram::mode_log10>() {return "mode_log10";}
template <> const char* get_mode_string<mtk::fphistogram::mode_log2 >() {return "mode_log2" ;}

template <class Mode>
void test_random_vec() {
	std::printf("# %s\n", __func__);
	std::printf("Mode : %s\n", get_mode_string<Mode>());
	std::mt19937 mt(std::random_device{}());
	std::uniform_real_distribution<double> dist(-1., 1.);

	constexpr std::size_t N = 1lu << 16;
	double fp_list[N];
	for (std::size_t i = 0; i < N; i++) {
		fp_list[i] = dist(mt);
	}
	mtk::fphistogram::print_histogram<double, Mode>(fp_list, N);
}

template <class Mode>
void test_random_diff_vec() {
	std::printf("# %s\n", __func__);
	std::printf("Mode : %s\n", get_mode_string<Mode>());
	std::mt19937 mt(std::random_device{}());
	std::uniform_real_distribution<double> dist(-1., 1.);

	constexpr std::size_t N = 1lu << 16;
	double fp_list_a[N];
	double fp_list_b[N];
	for (std::size_t i = 0; i < N; i++) {
		fp_list_a[i] = dist(mt);
		fp_list_b[i] = dist(mt);
	}
	std::function<double(const std::size_t)> iter = [&fp_list_a, &fp_list_b](const std::size_t i) {return fp_list_a[i] - fp_list_b[i];};
	mtk::fphistogram::print_histogram<double, Mode>(iter, N);
}

template <class Mode>
void test_all_zero() {
	std::printf("# %s\n", __func__);
	std::printf("Mode : %s\n", get_mode_string<Mode>());
	constexpr std::size_t n = 1lu << 16;
	double fp_list[n];
	for (std::size_t i = 0; i < n; i++) {
		fp_list[i] = 0.;
	}
	mtk::fphistogram::print_histogram<double, Mode>(fp_list, n);
}

template <class Mode>
void test_size_zero() {
	std::printf("# %s\n", __func__);
	std::printf("Mode : %s\n", get_mode_string<Mode>());
	std::vector<double> vec;
	mtk::fphistogram::print_histogram<double, Mode>(vec);
}

template <class Mode>
void test_half_half() {
	std::printf("# %s\n", __func__);
	std::printf("Mode : %s\n", get_mode_string<Mode>());
	constexpr std::size_t n = 1lu << 16;
	double fp_list[n];
	std::size_t i = 0;
	for (; i < n / 2; i++) {
		fp_list[i] = 10;
	}
	for (; i < n; i++) {
		fp_list[i] = 5.;
	}
	mtk::fphistogram::print_histogram<double, Mode>(fp_list, n);
}

int main() {
	test_size_zero<mtk::fphistogram::mode_log10>();
	test_size_zero<mtk::fphistogram::mode_log2 >();
	test_all_zero<mtk::fphistogram::mode_log10>();
	test_all_zero<mtk::fphistogram::mode_log2 >();
	test_random_vec<mtk::fphistogram::mode_log10>();
	test_random_vec<mtk::fphistogram::mode_log2 >();
	test_random_diff_vec<mtk::fphistogram::mode_log10>();
	test_random_diff_vec<mtk::fphistogram::mode_log2 >();
	test_half_half<mtk::fphistogram::mode_log10>();
	test_half_half<mtk::fphistogram::mode_log2 >();
}
