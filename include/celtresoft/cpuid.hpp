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

#ifndef CELTRESOFT_CPUID_CPUID_IMPL_HPP
#define CELTRESOFT_CPUID_CPUID_IMPL_HPP

#include <array>
#include <cstdint>

#include <celtresoft/preproc/visibility.hpp>

namespace celtresoft::cpuid {
class CELTRESOFT_API_EXPORT cpuid_impl {
public:
  explicit cpuid_impl(std::uint32_t info_type);

  [[nodiscard]] auto eax() const noexcept -> std::uint32_t const&;
  [[nodiscard]] auto ebx() const noexcept -> std::uint32_t const&;
  [[nodiscard]] auto ecx() const noexcept -> std::uint32_t const&;
  [[nodiscard]] auto edx() const noexcept -> std::uint32_t const&;

  auto operator=(std::uint32_t info_type) -> cpuid_impl&;

private:
  std::array<std::uint32_t, 4> _regs{};

  auto run_cpuid(std::uint32_t info_type) -> void;
};
} // namespace celtresoft::cpuid

#endif // CELTRESOFT_CPUID_CPUID_IMPL_HPP