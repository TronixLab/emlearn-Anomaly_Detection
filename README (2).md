# emlearn Anomaly Detection

TinyML anomaly detection examples for the Wisconsin Breast Cancer dataset using a Python training notebook and Arduino-ready model exports.

## Overview

This repository demonstrates an end-to-end workflow for:

- loading and exploring the breast cancer dataset
- preprocessing 30 numeric features with `StandardScaler`
- reducing dimensionality to 2 principal components with PCA
- training anomaly detection models on benign samples
- exporting trained models to C/C++ headers for embedded inference
- running inference on Arduino sketches over serial input

The project combines:

- **Python / scikit-learn** for data preparation, training, evaluation, and export
- **emlearn** for converting anomaly detection models to embedded C/C++
- **micromlgen** for exporting PCA logic
- **Arduino sketches** for on-device inference

## Repository Structure

```text
/home/runner/work/emlearn-Anomaly_Detection/emlearn-Anomaly_Detection
├── BreastCancer_AnomalyDetection.ipynb
├── breast-cancer.csv
├── emlearn_Arduino_elenv_Anomaly_Detection/
│   ├── EllipticEnvelope.h
│   ├── PCA.h
│   ├── StandardScaler.h
│   └── emlearn_Arduino_elenv_Anomaly_Detection.ino
├── emlearn_Arduino_gmm_Anomaly_Detection/
│   ├── GaussianMixture.h
│   ├── PCA.h
│   ├── StandardScaler.h
│   └── emlearn_Arduino_gmm_Anomaly_Detection.ino
└── emlearn_Arduino_bgmm_Anomaly_Detection/
    ├── BayesianGaussianMixture.h
    ├── PCA.h
    ├── StandardScaler.h
    └── emlearn_Arduino_bgmm_Anomaly_Detection.ino
```

## Dataset

The workflow is based on the Wisconsin Diagnostic Breast Cancer dataset.

- local CSV: `breast-cancer.csv`
- rows: **569**
- columns: **32** total in the raw CSV
- label distribution: **357 benign**, **212 malignant**

The notebook removes the `id` column and uses `diagnosis` as the target label.

## Modeling Pipeline

The notebook implements the following pipeline:

1. load the dataset
2. balance the class distribution with oversampling
3. scale the 30 feature columns with `StandardScaler`
4. project the scaled features to 2 dimensions with PCA
5. treat **benign** samples as the normal class
6. train three anomaly detectors:
   - `EllipticEnvelope`
   - `GaussianMixture`
   - `BayesianGaussianMixture`
7. evaluate the models on a held-out test split
8. export preprocessing and model parameters to Arduino-friendly headers

## Arduino Inference Flow

Each Arduino sketch follows the same runtime sequence:

1. read one comma-separated line with 30 feature values from serial
2. scale features with `StandardScaler.h`
3. transform scaled features with `PCA.h`
4. run the selected anomaly detection model
5. print a benign/malignant decision and score to serial output

### Available Embedded Variants

- `emlearn_Arduino_elenv_Anomaly_Detection` — Elliptic Envelope distance-based detector
- `emlearn_Arduino_gmm_Anomaly_Detection` — Gaussian Mixture likelihood-based detector
- `emlearn_Arduino_bgmm_Anomaly_Detection` — Bayesian Gaussian Mixture likelihood-based detector

## How to Use

### Reproduce the training workflow

Open and run:

- `/home/runner/work/emlearn-Anomaly_Detection/emlearn-Anomaly_Detection/BreastCancer_AnomalyDetection.ipynb`

The notebook uses common Python data-science packages plus:

- `pandas`
- `numpy`
- `matplotlib`
- `seaborn`
- `scikit-learn`
- `emlearn`
- `micromlgen`

### Run on Arduino

1. Open one of the `.ino` sketches in the Arduino IDE.
2. Make sure the required `emlearn` runtime headers are available in the Arduino environment.
3. Flash the sketch to a supported board.
4. Send a single CSV row with 30 numeric features over the serial monitor.
5. Read the predicted class and score from serial output.

## Technical Notes

- `StandardScaler.h` stores the learned feature means and standard deviations.
- `PCA.h` contains an exported 30-to-2 projection.
- Model headers are generated artifacts from Python training.
- The three Arduino projects reuse the same preprocessing artifacts and differ mainly in the anomaly detector.
- Mixture-model sketches classify anomalies by comparing model scores against a fixed threshold compiled into the sketch.

## Limitations

- The repository currently has no dependency manifest or automated test/build workflow.
- The notebook is the source of truth for training and export steps.
- Generated headers must be regenerated manually after retraining.

## Additional Documentation

See `/home/runner/work/emlearn-Anomaly_Detection/emlearn-Anomaly_Detection/TECHNICAL_DOCUMENTATION.md` for a more detailed technical analysis of the repository.
