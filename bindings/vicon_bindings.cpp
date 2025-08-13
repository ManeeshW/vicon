#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include "vicon.hpp"

namespace py = pybind11;

PYBIND11_MODULE(vicon_tracker, m) {
    m.doc() = "Python bindings for the vicon class";
    m.attr("__version__") = "1.0";

    py::class_<vicon>(m, "vicon")
        .def(py::init<>())
        .def("open", (void (vicon::*)(std::string)) &vicon::open, py::arg("object"))
        .def("open", (void (vicon::*)()) &vicon::open)
        .def("close", &vicon::close)
        .def("loop", [](vicon& self) {
            auto [x_v, R_vm] = self.loop();
            return std::make_pair(x_v, R_vm);  // Automatically converted to NumPy arrays
        });
}
