#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
#include <boost/python.hpp>
#include <Eigen/Dense>
#include "Numpy2Eigen.hpp"
namespace py = boost::python;

namespace Numpy2Eigen {
    typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> RowMajor;
    Eigen::MatrixXd getEigenMatrix(PyObject* raw_np) {
        PyObject* np_matrix =  PyArray_ContiguousFromAny(raw_np, NPY_DOUBLE, 2, 2);
        PyArrayObject* data_arr = reinterpret_cast<PyArrayObject*>(np_matrix);
        const int num_data_dims = PyArray_NDIM(data_arr);
        if (num_data_dims != 2) {
            throw std::invalid_argument("Number of dimensions should be 2");
        }
        const npy_intp* data_dims = PyArray_DIMS(data_arr);
        double* data_raw = reinterpret_cast<double*>(PyArray_DATA(data_arr));
        Eigen::Map<RowMajor> eigen_data(data_raw, data_dims[0], data_dims[1]);
        return eigen_data;
    }

    double* cp(double* a, int size) {
        double* r = static_cast<double*>(malloc(sizeof(double) * size));
        for (int i = 0; i < size; ++i) {
            r[i] = a[i];
        }
        return r;
    }

    py::object getNumpyMatrix(Eigen::MatrixXf &Xf) {  // NOLINT
        RowMajor X = Xf.cast<double>();
        npy_intp np_dims[] = {X.rows(), X.cols()};
        double* dat = cp(X.data(), X.size());  // Unmanaged data
        PyObject* npX = PyArray_SimpleNewFromData(2, np_dims, NPY_DOUBLE, dat);
        py::object o((py::handle<>(py::borrowed(npX))));
        return o;
    }

    py::object getNumpyMatrix(RowMajor &X) {  // NOLINT
        npy_intp np_dims[] = {X.rows(), X.cols()};
        double* dat = cp(X.data(), X.size());  // Unmanaged data
        PyObject* npX = PyArray_SimpleNewFromData(2, np_dims, NPY_DOUBLE, dat);
        py::object o((py::handle<>(py::borrowed(npX))));
        return o;
    }

    void init() {
        if (PyArray_API == NULL) {
            import_array();
        }
    }
};  // namespace Numpy2Eigen
