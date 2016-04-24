from distutils.core import setup, Extension

libqth = Extension(
	"libqth",
	sources=["cobra.c", "qth.c"])

setup (
	name = 'libqth',
	version = '1.0',
	description = 'libqth python module',
	url = 'https://github.com/ggramaize/libqth',
	ext_modules = [libqth]
)
