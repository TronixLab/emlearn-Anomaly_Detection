#include "BayesianGaussianMixture.h"
#include "PCA.h"
#include "StandardScaler.h"

// Create scaler instance
StandardScaler scaler;

// Array to hold scaled features
float features_scaled[StandardScaler::n_features];

// Create PCA instance
Eloquent::ML::Port::PCA pca;

// Class labels and anomaly threshold
const char *classes[2] = { "Benign", "Malignant" };
const float bgmm_anomaly_threshold = -4.8423f;

// Variable to hold PCA features
float pca_features[2];

// Variables to hold probabilities
float probabilities[2];

// Variable to hold the anomaly score
float score;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
}

void loop() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    float features[30];
    int featureIndex = 0;
    int lastIndex = 0;
    line.trim();

    while (featureIndex < 30) {
      int commaIndex = line.indexOf(',', lastIndex);
      String value;
      if (commaIndex == -1) {
        value = line.substring(lastIndex);
      } else {
        value = line.substring(lastIndex, commaIndex);
      }
      features[featureIndex] = value.toFloat();
      featureIndex++;
      if (commaIndex == -1)
        break;
      lastIndex = commaIndex + 1;
    }

    // Scale the raw features using StandardScaler
    scaler.transform(features, features_scaled);

    // PCA features transformation
    pca.transform(features_scaled, pca_features);

    // Compute the PCA features size
    int pca_features_size = sizeof(pca_features) / sizeof(pca_features[0]);

    // Compute the anomaly score using the Bayesian Gaussian Mixture Model
    EmlError status = BayesianGaussianMixture_score(pca_features, pca_features_size, probabilities, &score);

    // Check if the score computation was successful
    if (status == EmlOk) {
      // Classify as anomaly (malignant) if the score is below the threshold,
      // otherwise classify as normal (benign)
      int prediction = (score < bgmm_anomaly_threshold) ? 1 : 0;

      Serial.print("Prediction: ");
      Serial.print(classes[prediction]);
      Serial.print("\t Outlier Scores: ");
      Serial.print(score, 7);
      Serial.print("\t Threshold: ");
      Serial.println(bgmm_anomaly_threshold, 7);
    } else {
      Serial.println("Error computing score");
    }
  }
  delay(100);
}