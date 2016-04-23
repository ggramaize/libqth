from distutils.core import setup, Extension

setup(
    ext_modules=[Extension("_libqth", sources=["cobra.c", "qth.c"], extra_compile_args=["-g", "-ggdb3", "-fno-inline"])],
)
