#!/usr/bin/python
"""setup.py for PyFLX."""

from setuptools import setup
from setuptools.dist import Distribution
# To use a consistent encoding
from codecs import open
from os import path
import platform

here = path.abspath(path.dirname(__file__))

if platform.system() == 'Windows':
    packagefile = 'pyflx.pyd'
else:
    packagefile = 'pyflx.so'


class BinaryDistribution(Distribution):
    """For binary distribution."""

    def is_pure(self):
        """is_pure function."""
        return False


with open(path.join(here, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

setup(
    name='pyflx',
    version='1.2.0',
    description='Provides Python market data API access to FlexTrade SYM',
    long_description=long_description,
    long_description_content_type='text/markdown',
    url='https://github.com/devcartel/pyflx',
    author='DevCartel',
    author_email='support@devcartel.com',
    license='MIT',
    classifiers=[
        'Development Status :: 5 - Production/Stable',
        'Intended Audience :: Developers',
        'Topic :: Software Development :: Build Tools',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 2.7',
    ],
    keywords='flextrade application sym symproxy api market data feed',
    packages=['pyflx'],
    package_data={
        'pyflx': [packagefile],
    },
    include_package_data=True,
    distclass=BinaryDistribution,
)
