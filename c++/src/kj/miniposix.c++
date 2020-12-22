// Copyright (c) 2013-2014 Sandstorm Development Group, Inc. and contributors
// Licensed under the MIT License:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifdef TOTO_TOTO
  #undef TOTO_TOTO
#endif

#define TITI_TITI
#include "miniposix.h"
#undef TITI_TITI
#if _WIN32
#include <io.h>
#include <direct.h>
#include <fcntl.h>  // _O_BINARY
#else
#include <limits.h>
#include <errno.h>
#endif

#if !_WIN32 || __MINGW32__
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#if !_WIN32
#include <limits.h>
#include <sys/uio.h>
#endif

// To get KJ_BEGIN_HEADER/KJ_END_HEADER
#include "common.h"

namespace kj {
namespace miniposix {
#if defined(IOV_MAX)
  #pragma message("c++ IOV_MAX defined")
#else
  #pragma message("c++ IOV_MAX NOT defined")
#endif
#if defined(UIO_MAX_IOV)
  #pragma message("c++ UIO_MAX_IOV defined")
#else
  #pragma message("c++ UIO_MAX_IOV NOT defined")
#endif
#if _WIN32
  #pragma message("c++ _WIN32 defined")
#else
  #pragma message("c++ _WIN32 NOT defined")
#endif

#if !_WIN32 && !defined(IOV_MAX) && !defined(UIO_MAX_IOV)

size_t iovMax() {
  // Thread-safe & lazily initialized only on first use.
  static const size_t KJ_IOV_MAX = [] () -> size_t {
    long iovmax;

    errno = 0;
    iovmax = sysconf(_SC_IOV_MAX);
    if (iovmax == -1) {
      if (errno == 0) {
        // The -1 return value was the actual value, not an error. This means there's no limit.
        return kj::maxValue;
      } else {
        return _XOPEN_IOV_MAX;
      }
    }

    return (size_t) iovmax;
  }();
  return KJ_IOV_MAX;
}
#endif
}  // namespace miniposix
}  // namespace kj
