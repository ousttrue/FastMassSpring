#pragma once

// System parameters
struct SystemParam {
  const int n = 33;     // must be odd, n * n = n_vertices | 61
  const float w = 2.0f; // width | 2.0f
  const float h =
      0.008f; // time step, smaller for better results | 0.008f = 0.016f/2
  const float r = w / (n - 1) * 1.05f; // spring rest legnth
  const float k = 1.0f;                // spring stiffness | 1.0f;
  const float m = 0.25f / (n * n);     // point mass | 0.25f
  const float a = 0.993f;              // damping, close to 1.0 | 0.993f
  const float g = 9.8f * m;            // gravitational force | 9.8f
};
