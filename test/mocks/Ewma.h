#pragma once

class Ewma {
 public:
  explicit Ewma(float alpha) : alpha_(alpha) {}
  float filter(float value) { value_ = initialized_ ? alpha_ * value + (1.0F - alpha_) * value_ : value; initialized_ = true; return value_; }
 private:
  float alpha_;
  float value_ = 0.0F;
  bool initialized_ = false;
};
