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
[ -14](         1){1.525879e-05}:
[ -13](         2){3.051758e-05}:
[ -12](         2){3.051758e-05}:
[ -11](         1){1.525879e-05}:
[ -10](         4){6.103516e-05}:
[ -09](        12){1.831055e-04}:
[ -08](        20){3.051758e-04}:
[ -07](        60){9.155273e-04}:
[ -06](       102){1.556396e-03}:
[ -05](       205){3.128052e-03}:
[ -04](       439){6.698608e-03}:
[ -03](       827){1.261902e-02}:*
[ -02](      1681){2.565002e-02}:**
[ -01](      3212){4.901123e-02}:****
[ 000](      6525){9.956360e-02}:*********
[ 001](     13253){2.022247e-01}:********************
[ 002](     26146){3.989563e-01}:***************************************
[ 003](     13044){1.990356e-01}:*******************
[zero](         0){0.000000e+00}:
```

## Requirements
- C++ >= 11


## License

MIT
