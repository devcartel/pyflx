# Copyright (C) DevCartel Co.,Ltd.
# Email: support@devcartel.com
import sys
__version__ = '1.3.0'

if sys.version_info >= (3, 0):
    from pyflx.pyflx import *
else:
    from pyflx import *
