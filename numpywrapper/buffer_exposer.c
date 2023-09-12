#include <Python.h>
#include <stdio.h>

int wrapper_getbuffer(PyObject* self, Py_buffer* view, int flags) {
    printf("entering getbuffer!\n");
    PyObject* data = PyObject_GetAttrString(self, "data");

    if (data == NULL) {
        printf("data is null!\n");
        return 0;
    }
    int result = PyObject_GetBuffer(data, view, flags);
    Py_DECREF(data);  // to offset the reference from GetAttrString
    view->obj = self; // to make sure the wrapper's release function gets called first
    Py_INCREF(self);  // to make sure the wrapper lives long enough for the release function to get called
    return result;
}


void wrapper_releasebuffer(PyObject* self, Py_buffer* view) {
    printf("entering release!\n");
    PyObject* data = PyObject_GetAttrString(self, "data");
    if (data == NULL) {
        printf("data is null!\n");
        return;
    }
    view->obj = data;
    PyBuffer_Release(view);
    Py_DECREF(data); // to offset the reference from GetAttrString
    view->obj = self; // set it back to self, PyBuffer_Release will handle the decrement
}

PyBufferProcs buffer_procs = {
        wrapper_getbuffer,
        wrapper_releasebuffer,
    };

static PyObject * expose_buffer(PyObject* cls, PyObject* args) {
    printf("entered exposing buffer!\n");

    PyTypeObject* type = (PyTypeObject*) cls;

    type->tp_as_buffer = &buffer_procs;
    PyType_Modified(type);
    printf("done!\n");

    // in place modification so per python convention return None.
    Py_RETURN_NONE;
}

static PyMethodDef buffer_exposer_module[] = {
    {"expose_buffer", expose_buffer, METH_VARARGS, "Exposes the buffer interface for a wrapper class."},
    {NULL, NULL, 0, NULL}  /* Sentinel */
};

static struct PyModuleDef buffer_exposermodule = {
    PyModuleDef_HEAD_INIT,
    "buffer_exposer",
    NULL, // module documentation, may be NULL
    -1,
    buffer_exposer_module
};

PyMODINIT_FUNC PyInit_buffer_exposer(void) {
    return PyModule_Create(&buffer_exposermodule);
}

