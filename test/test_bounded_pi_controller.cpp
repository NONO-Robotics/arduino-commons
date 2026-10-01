#include <cassert>

#include "BoundedPIController.h"

static bool closeEnough(float left, float right) {
  const float difference = left - right;
  return difference < 0.0001f && difference > -0.0001f;
}

int main() {
  BoundedPIController controller(0.5f, 0.0f, 1.0f, 10.0f);
  assert(closeEnough(controller.update(2.0f, 0.0f, 0.0f), 1.0f));
  assert(closeEnough(controller.update(-2.0f, 0.0f, 0.0f), -1.0f));

  BoundedPIController frequent(0.0f, 1.0f, 10.0f, 10.0f);
  BoundedPIController infrequent(0.0f, 1.0f, 10.0f, 10.0f);
  for (int index = 0; index < 10; ++index) {
    frequent.update(1.0f, 0.0f, 0.001f);
  }
  assert(closeEnough(infrequent.update(1.0f, 0.0f, 0.010f),
                     frequent.update(1.0f, 0.0f, 0.0f)));

  BoundedPIController directional(1.0f, 0.0f, 10.0f, 10.0f);
  assert(closeEnough(directional.update(0.1f, 10.0f, 0.0f, -0.1f, 0.9f),
                     -0.1f));
  assert(closeEnough(directional.update(-0.1f, -10.0f, 0.0f, -0.9f, 0.1f),
                     0.1f));

  BoundedPIController saturated(0.0f, 1.0f, 1.0f, 10.0f);
  assert(closeEnough(saturated.update(2.0f, 0.0f, 1.0f, -1.0f, 0.2f),
                     0.2f));
  assert(closeEnough(saturated.update(0.0f, 0.0f, 1.0f), 0.0f));
  saturated.reset();
  assert(closeEnough(saturated.update(1.0f, 0.0f, 0.0f), 0.0f));

  return 0;
}
