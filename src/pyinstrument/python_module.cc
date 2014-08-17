#include <boost/python.hpp>
#include "pyinstrument/profiler.h"

namespace pyinstrument {

BOOST_PYTHON_MODULE(_pyinstrument_cmodule) {
  using namespace boost::python;

  class_<Profiler, boost::noncopyable>("Profiler")
      .def("start", &Profiler::Start)
      .def("stop", &Profiler::Stop);

  class_<std::shared_ptr<void>>("OpaqueType", no_init);
}
}
