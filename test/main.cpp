#include <iostream>
#include <random>
#include <fphistogram/fphistogram.hpp>

int main() {
	std::mt19937 mt(std::random_device{}());
	std::uniform_real_distribution<double> dist(-1., 1.);

	constexpr std::size_t N = 1lu << 16;
	double fp_list[N];
	for (std::size_t i = 0; i < N; i++) {
		fp_list[i] = dist(mt);
	}
	mtk::fphistogram::print_histogram(fp_list, N);
}

