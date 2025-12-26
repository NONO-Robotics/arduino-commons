#pragma once

bool between(float value, float min, float max);

bool between(float value, float limit);

template <typename T> T clamp(T valor, T min, T max);

template <typename T> T minClamp(T a, T b);

template <typename T> T maxClamp(T a, T b);

template <typename T> int sign(T value);