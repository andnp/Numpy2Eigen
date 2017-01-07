#pragma once
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
#include <boost/python.hpp>
#include <Eigen/Dense>
namespace py = boost::python;

namespace Numpy2Eigen {
    typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> RowMajor;

    Eigen::MatrixXd getEigenMatrix(PyObject* np_matrix);
    py::object getNumpyMatrix(Eigen::MatrixXf &X);  // NOLINT
    py::object getNumpyMatrix(RowMajor &X);  // NOLINT
    void init();
};
