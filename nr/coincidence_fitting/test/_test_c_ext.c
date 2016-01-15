#include <Python.h>
#include <numpy/arrayobject.h>
#include "test_c_ext.h"

static char module_docstring[] =
    "This module provides an interface for calculating chi-squared using C.";
static char test_c_ext_docstring[] =
    "Calculate the chi-squared of some data given a model.";


static PyObject *test_c_ext_test_c_ext(PyObject *self, PyObject *args);

static PyMethodDef module_methods[] = {
    {"test_c_ext", test_c_ext_test_c_ext, METH_VARARGS, test_c_ext_docstring},
    {NULL, NULL, 0, NULL}
};


PyMODINIT_FUNC init_test_c_ext(void)
{
    PyObject *m = Py_InitModule3("_test_c_ext", module_methods, module_docstring);
    if (m == NULL)
        return;

    /* Load `numpy` functionality. */
    import_array();
}


static PyObject *chi2_chi2(PyObject *self, PyObject *args)
{
	double numTrials;
	PyObject *arr_obj, *hist_obj;
	
	if (!PyArg_ParseTuple(args, "iOO", &numTrials, &arr_obj, &hist_obj))
        return NULL;

	PyObject *energy_arr = PyArray_FROM_OTF(arr_obj, NPY_DOUBLE, NPY_IN_ARRAY);
	
	
	
	
}


