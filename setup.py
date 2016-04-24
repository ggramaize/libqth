#!/usr/bin/env python
"""
 Copyright (c) 2016, dashie <dashie@sigpipe.me>
 
 This file is part of libqth
 
 libqth is free software; you can redistribute it and/or modify it
 under the terms of the 3-clause BSD License as published by the
 Free Software Foundation: http://directory.fsf.org/wiki/License:BSD_3Clause
 
 libqth is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 
 You should have received a copy of the 3-clause BSD License along with GA;
 if not, write to the Free Software Foundation, Inc.,
 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

"""

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
