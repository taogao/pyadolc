import sys, os
sys.path.append(os.path.dirname(__file__))
import _adolc
from _adolc import *
from .wrapped_functions import *
from . import cgraph
from . import interpolation

try:
    from . import sparse
except Exception as e:
    print('adolc Notice: sparse drivers not available')
    print(e)
try:
    from . import colpack

except Exception as e:
    print('adolc Notice: colpack drivers not available')
    print(e)

# testing
from numpy.testing import Tester
test = Tester().test

__doc__ = """
Adolc: Algorithmic Differentiation Software
    see http://www.math.tu-dresden.de/~adol-c/ for documentation of Adolc
    http://github.com/b45ch1/pyadolc/tree/master for more information and documentation of this Python extension

    return values are always numpy arrays!

    Example Session:
    from numpy import *
    from adolc import *
    def vector_f(x):
    \tV=vander(x)
    \treturn dot(v,x)

    x = arange(5,dtype=float)
    ax = adouble(x)

    trace_on(0)
    independent(ax)
    ay = vector_f(ax)
    dependent(ay)
    trace_off()

    x = array([1.,4.,0.,0.,0.])
    y = function(0,x)
    J = jacobian(0,x)
"""


