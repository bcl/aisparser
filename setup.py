from setuptools import setup, Extension
import platform

SYSTEM = platform.system().lower()

MAJOR_VERSION = 1
MINOR_VERSION = 11
PATCH_VERSION = 0

aisparser_module = Extension(
    name='_aisparser',

    # If some macros are needed
    define_macros=[
        ('MAJOR_VERSION', MAJOR_VERSION),
        ('MINOR_VERSION', MINOR_VERSION),
        ('PATCH_VERSION', PATCH_VERSION)
    ],

    sources=[
        'c/src/access.c',
        'c/src/imo.c',
        'c/src/nmea.c',
        'c/src/seaway.c',
        'c/src/sixbit.c',
        'c/src/vdm_parse.c',
        'python/{}/aisparser.i'.format(SYSTEM),
    ]
)


setup(
    name = 'python-aisparser',
    description = 'AIS Parsing library',
    url = 'https://github.com/bcl/aisparser',
    author = "Brian C. Lane",
    author_email = "bcl@brianlane.com",
    license = "BSD-3-Clause",
    version = '{}.{}.{}'.format(MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION),
    py_modules=['aisparser'],
    package_dir = {'': 'python/{}'.format(SYSTEM)},
    ext_modules = [aisparser_module],
)
