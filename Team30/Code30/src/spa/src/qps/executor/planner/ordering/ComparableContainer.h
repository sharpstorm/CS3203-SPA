#pragma once

template<class MetricType, class DataType>
class ComparableContainer {
 private:
  MetricType metric;
  DataType data;

 public:
  ComparableContainer(const MetricType &metric,
                      const DataType &data) :
      metric(metric), data(data) {}

  DataType getData() const {
    return data;
  }

  MetricType getMetric() const {
    return metric;
  }

  bool operator<(const ComparableContainer<MetricType, DataType> &other) const {
    return metric > other.metric;
  }

  bool operator>(const ComparableContainer<MetricType, DataType> &other) const {
    return metric < other.metric;
  }
};
