from buffer_exposer import expose_buffer

class NumpyWrapper:
    def __init__(self, data) -> None:
        self.data = data 

expose_buffer(NumpyWrapper)
