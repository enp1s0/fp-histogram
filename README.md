# FP Histogram

This library draws an exponent histogram of floating point array.

## Sample code
```cpp
// sample.cpp
// gcc -I/path/to/fp-histogram/include -std=c++11 sample.cpp
#include <random>
#include <fphistogram/fphistogram.hpp>

int main() {
	std::mt19937 mt(std::random_device{}());
	std::uniform_real_distribution<double> dist(-10., 10.);

	constexpr std::size_t N = 1lu << 16;
	double fp_list[N];
	for (std::size_t i = 0; i < N; i++) {
		fp_list[i] = dist(mt);
	}
	mtk::fphistogram::print_histogram(fp_list, N);

	// std::vector<float> fp_list_vec;
	// mtk::fphistogram::print_histogram(fp_list_vec);
}
```

Then you can get a histogram like this.
```
[ 003](     12999){1.983490e-01}:*******************
[ 002](     26328){4.017334e-01}:****************************************
[ 001](     12975){1.979828e-01}:*******************
[ 000](      6712){1.024170e-01}:**********
[ -01](      3252){4.962158e-02}:****
[ -02](      1619){2.470398e-02}:**
[ -03](       825){1.258850e-02}:*
[ -04](       411){6.271362e-03}:
[ -05](       204){3.112793e-03}:
[ -06](       111){1.693726e-03}:
[ -07](        60){9.155273e-04}:
[ -08](        18){2.746582e-04}:
[ -09](        11){1.678467e-04}:
[ -10](         4){6.103516e-05}:
[ -11](         6){9.155273e-05}:
[ -12](         0){0.000000e+00}:
[ -13](         1){1.525879e-05}:
-----
[zero](         0){0.000000e+00}:
```

## Requirements
- C++ >= 11


## License

MIT
