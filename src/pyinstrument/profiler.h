#pragma once
#include <memory>

namespace pyinstrument {
class Profiler {
 public:
  Profiler();

  void Start();
  void Stop();

 private:
  class Impl;
  std::shared_ptr<Impl> impl_;
};
}

