#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "../../include/fphistogram/fphistogram.hpp"

template <class T>
void print_histogram(
	pybind11::array_t<T, pybind11::array::c_style | pybind11::array::forcecast> array,
	const unsigned num_all_stars = 100
	) {
	const auto info = array.request();
	auto ptr = static_cast<T*>(info.ptr);

	// get size
	unsigned n = 1;
	for (const auto r : info.shape) {
		n *= r;
	}

	// call print func
	mtk::fphistogram::print_histogram(ptr, n, num_all_stars);
}

template <class T>
void print_histogram_pm(
	pybind11::array_t<T, pybind11::array::c_style | pybind11::array::forcecast> array,
	const unsigned num_all_stars = 100
	) {
	const auto info = array.request();
	auto ptr = static_cast<T*>(info.ptr);

	// get size
	unsigned n = 1;
	for (const auto r : info.shape) {
		n *= r;
	}

	// call print func
	mtk::fphistogram::print_histogram_pm(ptr, n, num_all_stars);
}

PYBIND11_MODULE(fphistogram, m) {
    m.doc() = "fp-histogram for Python";

    m.def("print_histogram"   , &print_histogram<double>);
    m.def("print_histogram"    , &print_histogram<float>);
    m.def("print_histogram_pm", &print_histogram_pm<double>);
    m.def("print_histogram_pm" , &print_histogram_pm<float>);
}
