//==============================================================================
// The MIT License (MIT)
//
// Copyright (c) 2023 Victor Matia <vmatir@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//==============================================================================

#include "celtresoft/cpuid.hpp"

#include <array>

#if defined CELTRESOFT_CPUID_HAS_WIN_INTRIN_H
#  include <intrin.h>
#elif defined CELTRESOFT_CPUID_HAS_GNU_CLANG_CPUID_H
#  include <cpuid.h>
#elif defined CELTRESOFT_CPUID_CAN_HAVE_ASM_CPUID
#  ifndef CELTRESOFT_ARCH_64BIT
#    define __cpuid(level, a, b, c, d)                                         \
      do {                                                                     \
        if (level && level != 1) {                                             \
          asm volatile("cpuid"                                                 \
                       : "=a"(a), "=b"(b), "=c"(c), "=d"(d)                    \
                       : "0"(level));                                          \
        } else {                                                               \
          asm volatile("cpuid"                                                 \
                       : "=a"(a), "=b"(b), "=c"(c), "=d"(d)                    \
                       : "0"(level), "1"(0), "2"(0));                          \
        }                                                                      \
      } while (0)
#  else // CELTRESOFT_ARCH_64BIT
#    define __cpuid(level, a, b, c, d)                                         \
      asm volatile("cpuid" : "=a"(a), "=b"(b), "=c"(c), "=d"(d) : "0"(level))
#  endif // CELTRESOFT_CPUID_64BIT
#else
#  error "__cpuid not supported on this platform"
#endif // CELTRESOFT_CPUID_HAS_WIN_INTRIN_H

namespace celtresoft {
cpuid::cpuid(std::uint32_t info_type) { run_cpuid(info_type); }

[[nodiscard]] auto cpuid::eax() const noexcept -> std::uint32_t const& {
  return _regs[0];
}

[[nodiscard]] auto cpuid::ebx() const noexcept -> std::uint32_t const& {
  return _regs[1];
}

[[nodiscard]] auto cpuid::ecx() const noexcept -> std::uint32_t const& {
  return _regs[2];
}

[[nodiscard]] auto cpuid::edx() const noexcept -> std::uint32_t const& {
  return _regs[3];
}

auto cpuid::operator=(std::uint32_t info_type) -> cpuid& {
  run_cpuid(info_type);
  return *this;
}

auto        cpuid::run_cpuid(std::uint32_t info_type) -> void {
#if defined CELTRESOFT_CPUID_HAS_WIN_INTRIN_H
  __cpuid(_regs.data(), static_cast<int>(info_type));
#else
  __cpuid(static_cast<int>(info_type), _regs[0], _regs[1], _regs[2], _regs[3]);
#endif // CELTRESOFT_CPUID_HAS_WIN_INTRIN_H
}
} // namespace celtresoft