# FP Histogram

This library draws an exponent histogram of a given floating point array.

## Sample code
```cpp
// sample.cpp
// gcc -I/path/to/fphistogram/include -std=c++11 sample.cpp
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

	// std::function<double(const std::int)> iter = [&fp_list](const std::size_t i) {return 3.0 * fp_list[i];};
	// mtk::fphistogram::print_histogram(iter, N);
}
```

Then you can get a histogram like this.
```
[  exp ](   count  ){    ratio   }
[     3](     13099){1.998749e-01}:*******************
[     2](     26306){4.013977e-01}:****************************************
[     1](     13145){2.005768e-01}:********************
[     0](      6446){9.835815e-02}:*********
[    -1](      3296){5.029297e-02}:*****
[    -2](      1599){2.439880e-02}:**
[    -3](       808){1.232910e-02}:*
[    -4](       438){6.683350e-03}:
[    -5](       209){3.189087e-03}:
[    -6](        98){1.495361e-03}:
[    -7](        40){6.103516e-04}:
[    -8](        24){3.662109e-04}:
[    -9](        14){2.136230e-04}:
[   -10](         6){9.155273e-05}:
[   -11](         3){4.577637e-05}:
[   -12](         2){3.051758e-05}:
[   -13](         3){4.577637e-05}:
-----
[ zero ](         0){0.000000e+00}:
```

## Requirements
- C++ >= 11


## License

MIT
