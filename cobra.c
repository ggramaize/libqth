#include <Python.h>
#include "qth.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char module_docstring[] = "This module provides an interface to the libqth using C.";
static char qth_to_coords_docstring[] = "Converts a C string QTH locator to floating point coordinates\r\nqth_to_coords(qth, precision)";
static char coords_to_qth_docstring[] = "Converts floating point coordinates to a QTH locator.\r\ncoords_to_qth(lat, lon, precision)";
static char is_valid_qth_docstring[] = "Validates that a C string is a valid QTH\r\ncoords_to_qth(qth, precision)";

static PyObject *qth_to_coords_libqth(PyObject *self, PyObject *args);
static PyObject *coords_to_qth_libqth(PyObject *self, PyObject *args);
static PyObject *is_valid_qth_libqth(PyObject *self, PyObject *args);

static PyMethodDef module_methods[] = {
    {"qth_to_coords", qth_to_coords_libqth, METH_VARARGS, qth_to_coords_docstring},
    {"coords_to_qth", coords_to_qth_libqth, METH_VARARGS, coords_to_qth_docstring},
    {"is_valid_qth", is_valid_qth_libqth, METH_VARARGS, is_valid_qth_docstring},
    {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit__libqth(void) {
    PyObject *module;
    static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "_libqth",
        module_docstring,
        -1,
        module_methods,
        NULL,
        NULL,
        NULL,
        NULL
    };
    module = PyModule_Create(&moduledef);
    if (!module) return NULL;

    return module;
}
#else
PyMODINIT_FUNC init_libqth(void)
{
    PyObject *m = Py_InitModule3("_libqth", module_methods, module_docstring);
    if (m == NULL)
        return;
}
#endif

// int qth_to_coords( const char *qth, size_t qthStringSize, double *latitude, double *longitude, size_t *precision);
static PyObject *qth_to_coords_libqth(PyObject *self, PyObject *args) {
	char *qth;
    
    double latitude = -300, longitude = -300;
    size_t precision = 0;

    PyObject *coordsDict;

    /* Parse the input tuple */
    if (!PyArg_ParseTuple(args, "sn", &qth, &precision))
        return NULL;

    size_t qthStringSize = strlen(qth);

    /* Call the external C function to compute the coordinates. */
    int retcode = qth_to_coords(qth, qthStringSize, &latitude, &longitude, &precision);

    if (retcode != 0) {
    	Py_RETURN_FALSE;
    }

    /* Build the output dict */
    coordsDict = Py_BuildValue("{s:s, s:I, s:d, s:d}",
    	"qth", qth,
    	"precision", precision,
    	"latitude", latitude,
    	"longitude", longitude);

    return coordsDict;
}

// int coords_to_qth( double latitude, double longitude, size_t precision, char *qth);
static PyObject *coords_to_qth_libqth(PyObject *self, PyObject *args) {
	double latitude = -300, longitude = -300;
	size_t precision = 0;
	
	char qth[13];
	memset( qth, 0, 13);

	PyObject *qthDict;

    /* Parse the input tuple */
    if (!PyArg_ParseTuple(args, "ddn", &latitude, &longitude, &precision))
        return NULL;

    /* Call the external C function to compute the QTH locator. */
    int retcode = coords_to_qth(latitude, longitude, precision, qth);

    if (retcode == EDOM) {
    	Py_RETURN_FALSE;
    }

    /* Build the output dict */
    qthDict = Py_BuildValue("{s:s, s:I, s:d, s:d}",
    	"qth", qth,
    	"precision", precision,
    	"latitude", latitude,
    	"longitude", longitude);

    return qthDict;
}

// int is_valid_qth( const char *qthToTest, size_t qthStringSize, size_t *precision);
static PyObject *is_valid_qth_libqth(PyObject *self, PyObject *args) {
	char *qth;
    
    size_t precision = 0;

    /* Parse the input tuple */
    if (!PyArg_ParseTuple(args, "sn", &qth, &precision))
        return NULL;

    size_t qthStringSize = strlen(qth);

    /* Call the external C function to check the QTH locator validity. */
    int retcode = is_valid_qth(qth, qthStringSize, &precision);

    if (retcode == EDOM)
    	Py_RETURN_FALSE;
    else
    	Py_RETURN_TRUE;
}
