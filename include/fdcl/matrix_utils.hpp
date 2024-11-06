#ifndef MISC_MATRIX_FUNC_HPP
#define MISC_MATRIX_FUNC_HPP

#include <iostream>

#include "Eigen/Dense"
#include "Eigen/Eigenvalues"

#include "fdcl/common_types.hpp"


Matrix3 hat(const Vector3 v);
Vector3 vee(const Matrix3 V);
double sinx_over_x(const double x);
Matrix3 expm_SO3(const Vector3 r);
Vector3 logm_SO3(const Matrix3 R);
bool assert_SO3(Matrix3 R,const char *R_name);
void sat(Vector3&, double, double );
void sat(Vector4&, double, double );

#endif
