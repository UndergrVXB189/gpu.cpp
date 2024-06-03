#ifndef ARRAY_UTILS_H
#define ARRAY_UTILS_H

#include <algorithm> // std::max_element
#include <array>
#include <cmath>
#include <random>
#include <string>
#include <utility>

namespace gpu {

static constexpr int kShowMaxRows = 14;
static constexpr int kShowMaxCols = 8;

template <typename numtype, size_t rows, size_t cols>
std::string show(std::array<numtype, rows * cols> a, std::string name) {
  std::string output = "\n";
  if (name != "") {
    output += name + " : \n";
  }
  // spacing as log10 of max value
  int spacing = 1;
  numtype max = *std::max_element(a.begin(), a.end());
  if constexpr (std::is_same<numtype, int>::value) {
    spacing = std::max(0, (int)log10(max + .01)) + 2;
  } else if constexpr (std::is_same<numtype, float>::value) {
    // spacing = std::max(0, (int)log10(max + .01)) + 1;
    spacing = 8; // scientific notation
  } else {
    throw std::runtime_error("Unsupported number type for show()");
  }
  // print to stdout line break for each row
  for (size_t i = 0; i < rows; i++) {
    if (i == 7 && rows > 14) {
      output += "...\n";
      i = rows - 7;
    }
    for (size_t j = 0; j < cols; j++) {
      if (j == kShowMaxCols / 2 && cols > kShowMaxCols) {
        output += " .. ";
        j = cols - kShowMaxCols/2;
      }
      char buffer[50];
      if constexpr (std::is_same<numtype, int>::value) {
        sprintf(buffer, "%*d", spacing, a[i * cols + j]);
      } else if constexpr (std::is_same<numtype, float>::value) {
        if (std::abs(a[i * cols + j]) < 1000 && std::abs(a[i * cols + j]) > 0.01 || a[i * cols + j] == 0.0) {
          sprintf(buffer, "%10.2f", a[i * cols + j]);
        } else
        sprintf(buffer, "%10.2e", a[i * cols + j]);
      } else {
        throw std::runtime_error("Unsupported number type for show()");
      }
      output += buffer;
    }
    output += "\n";
  }
  output += "\n";
  return output;
}


// For testing only, not optimized
void transpose(float* input, float* output, size_t M, size_t N) {
  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < N; j++) {
      output[j * M + i] = input[i * N + j];
    }
  }
}


template <size_t N> void range(std::array<float, N> &input) {
  for (size_t i = 0; i < N; i++) {
    input[i] = static_cast<float>(i);
  }
}


template <typename numtype, size_t size>
void randint(std::array<numtype, size> &a, std::mt19937 &gen, int min,
             int max) {
  std::uniform_int_distribution<> dist(min, max);
  for (int i = 0; i < size; i++) {
    a[i] = static_cast<numtype>(dist(gen));
  }
}

} // namespace gpu

#endif 