#pragma once

#include <iostream>
#include <stdexcept>
#include <cassert>

#include <cstdint>
using int32  = int32_t;
using uint32 = uint32_t;
using int64 = int64_t;
using uint64 = uint64_t;

#include <memory>
template<class T>
using unique = std::unique_ptr<T>;

template<class T>
using shared = std::shared_ptr<T>;

template <class T, class = std::enable_if_t<std::is_pointer<T>::value>>
using owner = T;

#include <string>
using string = std::string;

#include <queue>
template<class T>
using queue = std::queue<T>;

#include <deque>
template<class T>
using deque = std::deque<T>;

#include <vector>
template<class T>
using vector = std::vector<T>;

#include <functional>
#include <algorithm>
#include <tuple>
template<class T, class U>
using pair = std::pair<T, U>;