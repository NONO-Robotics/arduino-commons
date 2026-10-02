#pragma once

#include <Arduino.h>
#include <deque>
#include <map>
#include <vector>

class TwoWire {
 public:
  void begin() {}
  void beginTransmission(uint8_t address) { address_ = address; written_.clear(); }
  size_t write(uint8_t value) { written_.push_back(value); return 1; }
  uint8_t endTransmission(bool = true) {
    transmissions_.push_back({address_, written_});
    if (end_statuses_.empty()) return end_status_;
    const uint8_t status = end_statuses_.front();
    end_statuses_.pop_front();
    return status;
  }
  uint8_t requestFrom(uint8_t address, uint8_t count) {
    requests_.push_back({address, count});
    if (received_by_address_.count(address)) {
      received_ = received_by_address_[address];
      received_by_address_.erase(address);
    }
    if (request_statuses_.empty()) {
      return std::min<uint8_t>(count, static_cast<uint8_t>(received_.size()));
    }
    const uint8_t status = request_statuses_.front();
    request_statuses_.pop_front();
    return status;
  }
  int available() const { return static_cast<int>(received_.size()); }
  int read() {
    if (received_.empty()) return -1;
    const auto value = received_.front();
    received_.pop_front();
    return value;
  }
  void queue(std::initializer_list<uint8_t> values) { received_.insert(received_.end(), values); }
  void queue(uint8_t address, std::initializer_list<uint8_t> values) {
    received_by_address_[address].insert(received_by_address_[address].end(), values);
  }
  void scriptEndTransmission(uint8_t status) { end_statuses_.push_back(status); }
  void scriptRequestFrom(uint8_t returned) { request_statuses_.push_back(returned); }
  void reset() {
    received_.clear();
    received_by_address_.clear();
    transmissions_.clear();
    requests_.clear();
    end_statuses_.clear();
    request_statuses_.clear();
    end_status_ = 0;
  }

  struct Transmission { uint8_t address; std::vector<uint8_t> bytes; };
  struct Request { uint8_t address; uint8_t count; };
  std::vector<Transmission> transmissions_;
  std::vector<Request> requests_;
  uint8_t end_status_ = 0;

 private:
  uint8_t address_ = 0;
  std::vector<uint8_t> written_;
  std::deque<uint8_t> received_;
  std::map<uint8_t, std::deque<uint8_t>> received_by_address_;
  std::deque<uint8_t> end_statuses_;
  std::deque<uint8_t> request_statuses_;
};

inline TwoWire Wire;
