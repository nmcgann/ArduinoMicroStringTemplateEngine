#pragma once

template <typename T, size_t N>
void testCallbacks(Print& pr, T* const (&i_values)[N])
{
  uint8_t idx1 = 0;

  if((idx1)<N) i_values[idx1](pr);

}

template <typename T, size_t N>
void testCallbacks1(Print& pr, T* const (&i_values)[N])
{
  uint8_t idx1 = 1;
  T* ptr = (T*) pgm_read_ptr(&i_values[idx1]);
  //if((idx1)<N) ptr(pr);
  if((idx1)<N) ((T*) pgm_read_ptr(&i_values[idx1]))(pr);
}
