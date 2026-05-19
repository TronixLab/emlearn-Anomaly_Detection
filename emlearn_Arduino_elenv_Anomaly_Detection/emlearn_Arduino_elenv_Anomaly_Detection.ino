#include "EllipticEnvelope.h"
#include "PCA.h"
#include "StandardScaler.h"

// Create scaler instance
StandardScaler scaler;

// Array to hold scaled features
float features_scaled[StandardScaler::n_features];

// Create PCA instance
Eloquent::ML::Port::PCA pca;

// Class labels
String classes[2] = { "Benign", "Malignant" };

// Variable to hold PCA features
float pca_features[2];

// Variable to hold the anomaly score
float scaled_features[30];

// Variable to hold the distance score
float dist = 0.0;

void setup() {
  Serial.begin(9600);
  if (!Serial)
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

    // Do outlier detection
    int pred_class = eml_elliptic_envelope_predict(&EllipticEnvelope_classifier, pca_features, pca_features_size, &dist);

    // Print the prediction and distance score
    Serial.print("Prediction: ");
    Serial.print(classes[pred_class]);
    Serial.print("\t Distance Score: ");
    Serial.println(dist, 4);
  }
  delay(100);
}