#include <Python.h>

int wrapper_getbuffer(PyObject* self, Py_buffer* view, int flags) {
    PyObject* data = PyObject_GetAttrString(self, "_data");

    if (data == NULL) {
        return 0;
    }

    int result = PyObject_GetBuffer(data, view, flags);

    // To offset the reference from PyObject_GetAttrString
    Py_DECREF(data);  
    // To make sure the wrapper's release function gets called first
    view->obj = self; 
    // To make sure the wrapper lives long enough for the release function to get called
    Py_INCREF(self); 
    return result;
}


void wrapper_releasebuffer(PyObject* self, Py_buffer* view) {
    PyObject* data = PyObject_GetAttrString(self, "_data");
    if (data == NULL) {
        return;
    }
    view->obj = data;
    PyBuffer_Release(view);
    // To offset the reference from PyObject_GetAttrString
    Py_DECREF(data); 
    // Set it back to self, PyBuffer_Release will handle the decrement
    view->obj = self; 
}

// Placed outside of functions because of lifetime reasons
PyBufferProcs buffer_procs = {
        wrapper_getbuffer,
        wrapper_releasebuffer,
    };

static PyObject * expose_buffer(PyObject* module, PyObject* cls) {
    // get wrapper class type, note if we use Py_TYPE we would be getting the type of the
    // wrapper class which should be type unless it inherits from something
    PyTypeObject* type = (PyTypeObject*) cls;

    type->tp_as_buffer = &buffer_procs;
    PyType_Modified(type);
    // In place modification so, per python convention we return None.
    Py_RETURN_NONE;
}

static PyMethodDef buffer_exposer_functions[] = {
    {"expose_buffer", expose_buffer, METH_O, PyDoc_STR("Exposes the buffer interface for a wrapper class which stored a buffer interfaced class in the _data attribute.")},
    {NULL, NULL, 0, NULL}  // Sentinel
};

static struct PyModuleDef buffer_exposermodule = {
    PyModuleDef_HEAD_INIT,
    "buffer_exposer",
    PyDoc_STR("Module to provide expose buffer method."), // module documentation
    -1,
    buffer_exposer_functions
};

PyMODINIT_FUNC PyInit_buffer_exposer(void) {
    return PyModule_Create(&buffer_exposermodule);
}

