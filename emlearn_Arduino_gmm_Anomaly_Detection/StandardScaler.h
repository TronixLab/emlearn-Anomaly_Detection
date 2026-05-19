// StandardScaler for Arduino - matches scikit-learn StandardScaler transform
#ifndef STANDARD_SCALER_H
#define STANDARD_SCALER_H

class StandardScaler
{
public:
  static constexpr int n_features = 30;

  const float scaler_means[30] = {14.836763f, 19.815840f, 96.968613f, 726.498319f, 0.097303f,
                                  0.112317f, 0.103089f, 0.056617f, 0.182656f, 0.062602f, 0.463917f, 1.220715f, 3.298612f, 49.548218f,
                                  0.007110f, 0.027071f, 0.034044f, 0.012651f, 0.020478f, 0.003845f, 17.329522f, 26.363039f,
                                  114.761863f, 1005.942017f, 0.134326f, 0.273978f, 0.300454f, 0.127194f, 0.293304f, 0.084576f};

  const float scaler_stds[30] = {3.781972f, 4.263198f, 26.147458f, 396.320694f, 0.013475f, 0.053978f,
                                 0.084075f, 0.041293f, 0.025918f, 0.006870f, 0.335309f, 0.523180f, 2.453988f, 59.254841f, 0.003013f,
                                 0.018057f, 0.028901f, 0.006250f, 0.008033f, 0.002520f, 5.315191f, 6.078518f, 37.003105f,
                                 664.523275f, 0.021860f, 0.157084f, 0.205174f, 0.066771f, 0.062195f, 0.017270f};

  // Scale input array in-place
  void transform(float *X, float *X_scaled) const
  {
    for (int i = 0; i < n_features; i++)
    {
      X_scaled[i] = (X[i] - scaler_means[i]) / scaler_stds[i];
    }
  }

  // Inverse scale input array in-place
  void inverse_transform(float *X_scaled, float *X) const
  {
    for (int i = 0; i < n_features; i++)
    {
      X[i] = X_scaled[i] * scaler_stds[i] + scaler_means[i];
    }
  }
};

#endif // STANDARD_SCALER_H