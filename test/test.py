import numpywrapper
import numpy as np

x = np.array([1.1, 2.3, 4.1], dtype=np.float32)
print("creating wrapper")
wrapper = numpywrapper.NumpyWrapper(x)
view = memoryview(wrapper)
print(view.tobytes())
"""
entered exposing buffer!
done!
creating wrapper
Traceback (most recent call last):
  File "/tmp/test.py", line 7, in <module>
    view = memoryview(wrapper)
TypeError: memoryview: a bytes-like object is required, not 'NumpyWrapper'
"""
