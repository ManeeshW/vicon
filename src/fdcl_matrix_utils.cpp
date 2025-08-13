#ifndef FDCL_MATRIX_UTILS_HPP
#define FDCL_MATRIX_UTILS_HPP

#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <iostream>
#include <cmath>

using Eigen::Matrix3d;
using Eigen::Vector3d;

Matrix3d hat(const Vector3d v)
{
    Matrix3d V;

    V.setZero();
    V(2, 1) = v(0); V(1, 2) = -V(2, 1);
    V(0, 2) = v(1); V(2, 0) = -V(0, 2);
    V(1, 0) = v(2); V(0, 1) = -V(1, 0);

    return V;
}

Vector3d vee(const Matrix3d V)
{
    Vector3d v;
    Matrix3d E;

    v.setZero();
    E = V + V.transpose();

    if (E.norm() > 1.e-6)
    {
        std::cout << "vee WARNING: E.norm() = " << E.norm() << std::endl;
    }

    v(0) = V(2, 1);
    v(1) = V(0, 2);
    v(2) = V(1, 0);

    return v;
}

double sinx_over_x(const double x)
{
    double y;
    double eps = 1.e-6;
    if (std::abs(x) < eps)
    {
        y = -std::pow(x, 10) / 39916800. + std::pow(x, 8) / 362880.
            - std::pow(x, 6) / 5040. + std::pow(x, 4) / 120. - std::pow(x, 2) / 6. + 1.;
    }
    else
    {
        y = std::sin(x) / x;
    }

    return y;
}

Matrix3d expm_SO3(const Vector3d r)
{
    Matrix3d R;
    double theta, y, y2;

    theta = r.norm();
    y = sinx_over_x(theta);
    y2 = sinx_over_x(theta / 2.);

    R.setIdentity();
    R += y * hat(r) + 1. / 2. * std::pow(y2, 2) * hat(r) * hat(r);

    return R;
}

Vector3d logm_SO3(const Matrix3d R)
{
    Vector3d r;
    Matrix3d I;
    double eps = 1.e-6;

    r.setZero();
    I.setIdentity();
    if ((I - R * R.transpose()).norm() > eps || std::abs(R.determinant() - 1) > eps)
    {
        std::cout << "logm_SO3: error: R is not a rotation matrix" << std::endl;
    }
    else
    {
        Eigen::EigenSolver<Matrix3d> eig(R);
        double min_del_lam_1 = 1.0, cos_theta, theta;
        int i, i_min = -1;
        Vector3d v;
        Matrix3d R_new;
        for (i = 0; i < 3; i++)
        {
            if (eig.eigenvectors().col(i).imag().norm() < eps)
            {
                if (std::pow(eig.eigenvalues()[i].real(), 2) - 1. < min_del_lam_1)
                {
                    min_del_lam_1 = std::pow(eig.eigenvalues()[i].real(), 2) - 1.0;
                    i_min = i;
                }
            }
        }
        v = eig.eigenvectors().col(i_min).real();

        cos_theta = (R.trace() - 1.0) / 2.0;
        if (cos_theta > 1.0) cos_theta = 1.0;
        else if (cos_theta < -1.0) cos_theta = -1.0;

        theta = std::acos(cos_theta);
        R_new = expm_SO3(theta * v);

        if ((R - R_new).norm() > (R - R_new.transpose()).norm()) v = -v;

        r = theta * v;

        return r;
    }

    return r;
}

bool assert_SO3(Matrix3d R, const char *R_name)
{
    bool isSO3;
    double errO, errD;
    Matrix3d eye3;
    double eps = 1e-5;

    eye3.setIdentity();
    errO = (R.transpose() * R - eye3).norm();
    errD = std::pow(1 - R.determinant(), 2);

    if (errO > eps || errD > eps)
    {
        isSO3 = false;
        std::cout << "ERROR: " << R_name << ": ||I-R^TR||= " << errO
                  << " det(R)= " << R.determinant()
                  << std::endl;

        std::cout << R << std::endl << std::endl;
    }
    else
    {
        isSO3 = true;
    }

    return isSO3;
}

#endif // FDCL_MATRIX_UTILS_HPP