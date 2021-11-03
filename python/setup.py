from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

__version__ = "0.0.1"

ext_modules = [
    Pybind11Extension(
        "fphistogram",
        sorted(glob("src/*.cpp")),
    ),
]

setup(
    name="fphistogram",
    version=__version__,
    author="enp1s0",
    author_email="mutsuki@momo86.net",
    url="https://github.com/enp1s0/fphistogram",
    description="Print exponent histgram",
    long_description="",
    ext_modules=ext_modules,
    zip_safe=False,
)

