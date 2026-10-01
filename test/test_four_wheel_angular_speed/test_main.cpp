#include <cassert>

#include "FourWheelAngularSpeed.h"

int main() {
  FourWheelAngularSpeed speeds;
  speeds.updateFrom(-2.0F, 6.0F, 4.0F, -8.0F);

  assert(speeds.getAverageLeftWInRad() == 1.0F);
  assert(speeds.getAverageRightWInRad() == -1.0F);

  return 0;
}
