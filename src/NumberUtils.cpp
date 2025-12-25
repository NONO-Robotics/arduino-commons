#include "NumberUtils.h"

bool between(float value, float min, float max) {
  return value >= min && value <= max;
}

bool between(float value, float limit) { return between(value, -limit, limit); }

template <typename T> T clamp(T valor, T min, T max) {
  if (valor < min)
    return min;
  if (valor > max)
    return max;
  return valor;
}

// Para obtener el mínimo: comparamos 'a' contra un techo 'b'
template <typename T> T minClamp(T a, T b) {
  // El límite inferior se pone igual al superior (o menor) para que no
  // interfiera
  return clamp(a, a < b ? a : b, b);
}

// Para obtener el máximo: comparamos 'a' contra un piso 'b'
template <typename T> T maxClamp(T a, T b) {
  // El límite superior se pone igual al inferior (o mayor) para que no
  // interfiera
  return clamp(a, b, a > b ? a : b);
}

template <typename T> int sign(T value) { return value > 0 ? 1 : -1; }