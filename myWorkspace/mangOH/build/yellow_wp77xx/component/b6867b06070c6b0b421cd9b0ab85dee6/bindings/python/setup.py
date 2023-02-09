#!/usr/bin/env python
#
# Copyright (C) 2015 Analog Devices, Inc.
# Author: Paul Cercueil <paul.cercueil@analog.com>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

from distutils.core import setup

setup(name='libiio',
		version='0.18',
		package_dir={'': '/home/firstuser/myWorkspace/mangOH/components/libiioComponent/libiio/bindings/python'},
		description='Library to use the Industrial IO devices',
		url='https://github.com/analogdevicesinc/libiio',
		py_modules=['iio'],
		)
