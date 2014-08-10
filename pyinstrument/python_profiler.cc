#include <boost/predef.h>
#include <Python.h>
#include <Python/frameobject.h>

#if BOOST_OS_MACOS
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#include <stdexcept>
#include <string>

#include <boost/exception/exception.hpp>
#include <boost/noncopyable.hpp>
#include <boost/python.hpp>
#include <boost/thread/once.hpp>
#include <boost/unordered_map.hpp>
#include <boost/utility/string_ref.hpp>

#include "frame.h"
#include "string_pool.h"

namespace pyinstrument {
namespace python = boost::python;

using Timestamp = uint64_t;
using Nanoseconds = uint64_t;

using string = std::string;
using string_ref = boost::string_ref;

static constexpr Nanoseconds kNsecPerSec = 1000000000ull;

class Error : virtual std::runtime_error, virtual boost::exception {
  using std::runtime_error::runtime_error;
};


class Profiler : boost::noncopyable {
 private:
  using SelfType = Profiler;

  bool is_profiling_{false};
  Timestamp due_{0};
  uint64_t frequency_{97};  // in Hz
  uint64_t interval_{IntervalForFrequency(frequency_)};
  InternedStringPool string_data_;
  boost::unordered_map<FrameVector, int> samples_;

 public:
  void Start() {
    if (is_profiling_) throw Error("Profiler already running");

    Py_tracefunc callback = [](PyObject * obj, PyFrameObject * frame, int what,
                               PyObject * arg)->int {
      auto self = static_cast<SelfType *>(PyLong_AsVoidPtr(obj));
      self->RecordEvent(*frame, what, python::object(python::borrowed(arg)));
      return 0;
    };
    PyObject *self = PyLong_FromVoidPtr(this);
    PyEval_SetProfile(callback, self);
    Py_DECREF(self);
  }

  void Stop() {
    if (!is_profiling_) throw Error("Profiler ain't running");
    PyEval_SetProfile(nullptr, nullptr);
  }

 private:
  const string *GetInternedString(PyObject *obj) {
    char *value;
    ssize_t length;

    if (PyString_AsStringAndSize(obj, &value, &length))
      throw Error("Object not a Python string");

    return string_data_.Get({value, static_cast<size_t>(length)});
  }

  bool Tick() {
    Timestamp now = Now();
    if (now < due_) return false;
    Timestamp next_due_date = now + 1;
    int remainder = next_due_date % interval_;
    if (remainder != 0) next_due_date += interval_ - remainder;

    due_ = next_due_date;
    return true;
  }

  void RecordEvent(const PyFrameObject &frame, int what, python::object arg) {
    (void)arg;
    if (!(what == PyTrace_CALL || what == PyTrace_C_CALL)) return;
    if (!Tick()) return;

    FrameVector frames;
    const PyFrameObject *current_frame = &frame;
    while (current_frame) {
      const string *function_name =
          GetInternedString(current_frame->f_code->co_name);
      const string *file_name =
          GetInternedString(current_frame->f_code->co_filename);
      int line_number = current_frame->f_code->co_firstlineno;

      frames.Append({function_name, line_number, file_name});
      current_frame = current_frame->f_back;
    }

    std::reverse(frames.begin(), frames.end());

    auto it = samples_.emplace(std::move(frames), 0).first;
    ++it->second;
  }

  static uint64_t IntervalForFrequency(uint64_t frequency) {
#if BOOST_OS_MACOS
    mach_timebase_info_data_t timebase_info;
    mach_timebase_info(&timebase_info);

    double conversion_factor =
        (double)timebase_info.denom / timebase_info.numer;
    return conversion_factor * kNsecPerSec / frequency;
#else
#error "Unsupported platform"
#endif
  }

  static Timestamp Now() {
#if BOOST_OS_MACOS
    return mach_absolute_time();
#else
#error "Unsupported platform"
#endif
  }
};

}  // namespace pyinstrument

BOOST_PYTHON_MODULE(_pyinstrument_cmodule) {
  using namespace boost::python;

  class_<pyinstrument::Profiler, boost::noncopyable>("Profiler")
      .def("start", &pyinstrument::Profiler::Start)
      .def("stop", &pyinstrument::Profiler::Stop);
}

