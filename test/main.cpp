#include <iostream>
#include <random>
#include <fphistogram/fphistogram.hpp>

void test_random_vec() {
	std::printf("# %s\n", __func__);
	std::mt19937 mt(std::random_device{}());
	std::uniform_real_distribution<double> dist(-1., 1.);

	constexpr std::size_t N = 1lu << 16;
	double fp_list[N];
	for (std::size_t i = 0; i < N; i++) {
		fp_list[i] = dist(mt);
	}
	mtk::fphistogram::print_histogram(fp_list, N);
}

void test_random_diff_vec() {
	std::printf("# %s\n", __func__);
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
	mtk::fphistogram::print_histogram(iter, N);
}

void test_all_zero() {
	std::printf("# %s\n", __func__);
	constexpr std::size_t n = 1lu << 16;
	double fp_list[n];
	for (std::size_t i = 0; i < n; i++) {
		fp_list[i] = 0.;
	}
	mtk::fphistogram::print_histogram(fp_list, n);
}

void test_size_zero() {
	std::printf("# %s\n", __func__);
	std::vector<double> vec;
	mtk::fphistogram::print_histogram(vec);
}

int main() {
	test_size_zero();
	test_all_zero();
	test_random_vec();
	test_random_diff_vec();
}
