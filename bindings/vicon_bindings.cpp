#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include "fdcl/vicon.hpp"

namespace py = pybind11;

PYBIND11_MODULE(fdcl_vicon_module, m) {
    m.doc() = "Python bindings for the fdcl vicon class";
    m.attr("__version__") = "0.1";

    py::class_<fdcl::vicon>(m, "vicon")
        .def(py::init<>())
        .def("open", (void (fdcl::vicon::*)(std::string)) &fdcl::vicon::open, py::arg("object"))
        .def("open", (void (fdcl::vicon::*)()) &fdcl::vicon::open)
        .def("close", &fdcl::vicon::close)
        .def("loop", [](fdcl::vicon& self) {
            auto [x_v, R_vm] = self.loop();
            return std::make_pair(x_v, R_vm);  // Automatically converted to NumPy arrays
        });
}
