// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
///////////////////////////////////////////////////////////////////////////////
#ifndef TINK_CORE_TEMPLATE_UTIL_H_
#define TINK_CORE_TEMPLATE_UTIL_H_

#include <tuple>
#include <type_traits>

#include "absl/meta/type_traits.h"

namespace crypto {
namespace tink {

namespace internal {

// A helper class template which decides if the TestType occurs in the Tuple
// For example, OccursInTuple<int, std::tuple<float, float>>::value == false,
// and OccursInTuple<int, std::tuple<float, int>>::value == true. Not intended
// to be used directly.

// First declare the template which always takes two parameters.
template <typename TestType, typename Tuple>
class OccursInList;

// In the special case where the tuple is empty, the result is false.
template <typename TestType>
class OccursInList<TestType, std::tuple<>> : public std::false_type {};

// If the list is not empty, the result is true if TestType equals the first in
// the list, or  TestType occurs in the rest of the list.
template <typename TestType, typename First, typename... List>
class OccursInList<TestType, std::tuple<First, List...>>
    : public absl::disjunction<
          std::is_same<TestType, First>,
          OccursInList<TestType, typename std::tuple<List...>>> {};

// The class HasDuplicates. Defines ::value as true in case the given list has
// a duplicate, false otherwise.
template <typename... Args>
class HasDuplicates;

// Empty list has no duplicates.
template <>
class HasDuplicates<> : public std::false_type {};

// Non-empty list has a duplicate if the first appears in the rest, or if the
// rest has a duplicate.
template <typename First, typename... List>
class HasDuplicates<First, List...>
    : public absl::disjunction<
          OccursInList<First, typename std::tuple<List...>>,
          HasDuplicates<List...>> {};

}  // namespace internal
}  // namespace tink
}  // namespace crypto

#endif  // TINK_CORE_TEMPLATE_UTIL_H_
