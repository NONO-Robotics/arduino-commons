#pragma once

bool between(float value, float min, float max) {
  return value >= min && value <= max;
}

int intClamp(int valor, int min, int max) {
  if (valor < min)
    return min;
  if (valor > max)
    return max;
  return valor;
}

int floatSign(float value) { return value > 0 ? 1 : -1; }