#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <cassert>

using std::vector, std::unique_ptr, std::lower_bound;

template<class T>
class AppendSet {
 protected:
  vector<T> items;

 private:
  typename vector<T>::const_iterator binarySearch(const T &data) const;

 public:
  AppendSet() = default;
  explicit AppendSet(const AppendSet &source);
  void insert(const T &item);

  size_t size() const;
  bool empty() const;
  typename vector<T>::const_iterator begin() const;
  typename vector<T>::const_iterator end() const;
  typename vector<T>::const_iterator find(const T &item) const;
};

template<class T>
using AppendOnlySetPtr = unique_ptr<AppendSet<T>>;

template<class T>
AppendSet<T>::AppendSet(const AppendSet<T> &source):
    items(source.items) {}

template<class T>
typename vector<T>::const_iterator AppendSet<T>::begin() const {
  return items.begin();
}

template<class T>
typename vector<T>::const_iterator AppendSet<T>::end() const {
  return items.end();
}

template<class T>
size_t AppendSet<T>::size() const {
  return items.size();
}

template<class T>
bool AppendSet<T>::empty() const {
  return items.empty();
}

template<class T>
void AppendSet<T>::insert(const T &item) {
  items.push_back(item);
  size_t newSize = items.size();
  size_t i = 1;
  for (; item < items[newSize - 1 - i] && i < newSize; i++) {
    items[newSize - i] = items[newSize - 1 - i];
  }
  items[newSize - i] = item;
}

template<class T>
typename vector<T>::const_iterator AppendSet<T>::find(const T &item) const {
  return binarySearch(item);
}

template<class T>
typename vector<T>::const_iterator AppendSet<T>::binarySearch(
    const T &data) const {
  auto it = lower_bound(items.begin(), items.end(), data);
  if (it == items.end() || *it != data) {
    return items.end();
  }
  return it;
}
