#pragma once

#include <string>
#include <vector>

#include <boost/variant.hpp>
#include <boost/noncopyable.hpp>
#include <boost/utility/swap.hpp>
#include <boost/container/static_vector.hpp>

namespace pyinstrument {

struct Frame {
  using string = std::string;

  // Strings are interned; compare by identity, not value.
  const string *function_name;
  int line_number;
  const string *file_path;

  friend bool operator==(const Frame &a, const Frame &b) {
    return a.function_name == b.function_name && a.file_path == b.file_path &&
           a.line_number == b.line_number;
  }

  friend size_t hash_value(const Frame &frame) {
    size_t hash = 0;
    boost::hash_combine(hash, frame.function_name);
    boost::hash_combine(hash, frame.file_path);
    boost::hash_combine(hash, frame.line_number);
    return hash;
  }
};


class FrameVector : boost::noncopyable {
 public:
  FrameVector() = default;

  FrameVector(FrameVector &&other) : FrameVector() {
    boost::swap(*this, other);
  }

  FrameVector &operator=(FrameVector other) {
    boost::swap(*this, other);
    return *this;
  }

  using iterator = Frame *;
  using const_iterator = const Frame*;

  iterator begin();
  iterator end();

  const_iterator begin() const;
  const_iterator end() const;

  size_t size() const;

  void Append(Frame frame);

  friend void swap(FrameVector &a, FrameVector &b) {
    boost::swap(a.storage_, b.storage_);
  }

  friend size_t hash_value(const FrameVector& obj);
  friend bool operator==(const FrameVector& a, const FrameVector& b);

 private:
  using StackVector = boost::container::static_vector<Frame, 20>;
  using HeapVector = std::vector<Frame>;
  boost::variant<StackVector, HeapVector> storage_;
};

}
