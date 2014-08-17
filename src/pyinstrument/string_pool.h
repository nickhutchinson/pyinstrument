#pragma once

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/unordered_set.hpp>
#include <boost/utility/string_ref.hpp>

namespace pyinstrument {

class InternedStringPool : boost::noncopyable {
 public:
  using string = std::string;
  using string_ref = boost::string_ref;

  const string *Get(string_ref s) {
    auto it = data_.find(s, Hash(), EqualTo());
    if (it != data_.end()) {
      return &*it;
    } else {
      auto result = data_.emplace(s.data(), s.size());
      return &*result.first;
    }
  }

 private:
  struct EqualTo {
    template <typename T, typename U>
    bool operator()(T &&t, U &&u) const {
      return t == u;
    }
  };
  struct Hash {
    template <typename T>
    size_t operator()(T &&s) const {
      return boost::hash_range(s.begin(), s.end());
    }
  };

  boost::unordered_set<string, Hash, EqualTo> data_;
};

}
