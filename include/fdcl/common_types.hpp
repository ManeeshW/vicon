#ifndef FDCL_COMMON_TYPE_H
#define FDCL_COMMON_TYPE_H

#include "Eigen/Dense"


// common eigen matrix definition used throughout the code
typedef Eigen::Matrix<double, 3, 3> Matrix3;
typedef Eigen::Matrix<double, 3, 1> Vector3;
typedef Eigen::Matrix<double, 2, 1> Vector2;
typedef Eigen::Matrix<double, 4, 1> Vector4;
typedef Eigen::Matrix<double, 6, 6> Matrix6;
typedef Eigen::Matrix<double, 6, 1> Vector6;
typedef Eigen::Matrix<double, 9, 9> Matrix9;
typedef Eigen::Matrix<double, 12, 12> Matrix12;
typedef Eigen::Matrix<double, 15, 15> Matrix15;

#endif
