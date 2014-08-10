#include "frame.h"
#include <boost/range.hpp>

namespace {
struct get_size : boost::static_visitor<size_t> {
  template <typename T>
  size_t operator()(T &&t) const {
    return t.size();
  }
};

template <typename T>
struct get_data : boost::static_visitor<T> {
  template <typename U>
  T operator()(U &&u) const {
    return u.data();
  }
};
}

namespace pyinstrument {

FrameVector::iterator FrameVector::begin() {
  return boost::apply_visitor(get_data<Frame *>(), storage_);
}
FrameVector::iterator FrameVector::end() { return begin() + size(); }

FrameVector::const_iterator FrameVector::begin() const {
  return boost::apply_visitor(get_data<const Frame *>(), storage_);
}
FrameVector::const_iterator FrameVector::end() const {
  return begin() + size();
}

size_t FrameVector::size() const {
  return boost::apply_visitor(get_size(), storage_);
}

void FrameVector::Append(Frame frame) {
  if (auto *vec = boost::get<StackVector>(&storage_)) {
    try {
      vec->emplace_back(frame);
      return;
    }
    catch (const std::bad_alloc &e) {
      storage_ = HeapVector(vec->begin(), vec->end());
    }
  }

  auto &vec = boost::get<HeapVector>(storage_);
  vec.emplace_back(frame);
}

size_t hash_value(const FrameVector &obj) {
  return boost::hash_range(obj.begin(), obj.end());
}

bool operator==(const FrameVector &a, const FrameVector &b) {
  return boost::range::equal(a, b);
}
}  // namespace pyinstrument
