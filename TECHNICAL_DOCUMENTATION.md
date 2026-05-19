# Technical Documentation

## 1. Repository Purpose

This repository packages a small anomaly-detection pipeline for embedded deployment. It starts with notebook-based experimentation on the Wisconsin breast cancer dataset and ends with generated C/C++ artifacts that can run on Arduino-class microcontrollers.

The overall goal is to classify incoming feature vectors as normal or anomalous after reproducing the exact preprocessing steps used during training.

## 2. Primary Assets

### 2.1 Notebook

**File:** `/home/runner/work/emlearn-Anomaly_Detection/emlearn-Anomaly_Detection/BreastCancer_AnomalyDetection.ipynb`

Responsibilities:

- dataset ingestion
- exploratory analysis and class-balance visualization
- oversampling to balance benign and malignant classes
- standardization of 30 features
- PCA fitting and projection to 2 dimensions
- anomaly model training
- model evaluation on a held-out test set
- export of preprocessing and model artifacts for embedded use

### 2.2 Dataset

**File:** `/home/runner/work/emlearn-Anomaly_Detection/emlearn-Anomaly_Detection/breast-cancer.csv`

Observed characteristics:

- 569 rows
- 32 columns in raw form
- 1 identifier column (`id`)
- 1 label column (`diagnosis`)
- 30 continuous input features
- class distribution: 357 benign (`B`), 212 malignant (`M`)

### 2.3 Embedded Deployment Directories

Each Arduino directory is a self-contained deployment target:

- `/home/runner/work/emlearn-Anomaly_Detection/emlearn-Anomaly_Detection/emlearn_Arduino_elenv_Anomaly_Detection`
- `/home/runner/work/emlearn-Anomaly_Detection/emlearn-Anomaly_Detection/emlearn_Arduino_gmm_Anomaly_Detection`
- `/home/runner/work/emlearn-Anomaly_Detection/emlearn-Anomaly_Detection/emlearn_Arduino_bgmm_Anomaly_Detection`

Shared file roles:

- `StandardScaler.h` — feature normalization constants and transform helpers
- `PCA.h` — exported two-component PCA projection logic
- model header (`EllipticEnvelope.h`, `GaussianMixture.h`, `BayesianGaussianMixture.h`) — embedded model parameters and scoring helpers
- `.ino` sketch — serial ingestion and inference orchestration

## 3. Training and Export Architecture

## 3.1 Data Preparation

The notebook pulls the breast cancer dataset, drops the identifier column, and uses `diagnosis` as the target. It then applies oversampling to rebalance classes before feature scaling.

Even though the dataset is balanced for analysis and scaling, anomaly modeling is still framed around learning the benign distribution and treating malignant samples as outliers during evaluation.

## 3.2 Feature Scaling

`StandardScaler` is used to compute per-feature mean and standard deviation values. Those learned values are exported into `StandardScaler.h` and reused on the microcontroller.

Why this matters:

- embedded inference must match the training feature space
- PCA and anomaly scores are only meaningful if input normalization is identical
- scaling prevents large-magnitude features from dominating downstream processing

## 3.3 Dimensionality Reduction

PCA reduces the 30 original feature dimensions to 2 principal components.

Practical effects:

- lower compute and memory cost on-device
- easier visualization of normal vs anomalous separation
- reduced complexity for the anomaly models

The exported `PCA.h` contains the learned projection coefficients and performs a deterministic 30-to-2 transform.

## 3.4 Anomaly Models

The notebook trains three distinct anomaly detectors.

### EllipticEnvelope

- models normal data as an ellipse in feature space
- uses a covariance-based distance score
- suitable when normal data is roughly Gaussian in the reduced feature space

### GaussianMixture

- models benign data as a mixture of Gaussian components
- uses sample likelihood as the anomaly score
- lower likelihood indicates more anomalous behavior

### BayesianGaussianMixture

- Bayesian extension of Gaussian mixtures
- can regularize component estimation more strongly than the standard mixture model
- also uses likelihood-based anomaly scoring

## 3.5 Thresholding Strategy

For the mixture-based models, cluster labels are not used for anomaly decisions. Instead, the notebook derives anomaly thresholds from the benign-score distribution using a lower quantile.

At deployment time, the sketches compare the computed model score against a compiled threshold constant to convert a continuous score into a benign/malignant decision.

## 4. Embedded Inference Design

## 4.1 Input Contract

All Arduino sketches expect:

- a single line from serial input
- 30 comma-separated numeric feature values
- feature order matching the notebook training data

Any mismatch in feature order or feature count will invalidate predictions.

## 4.2 Runtime Sequence

The embedded sequence is consistent across all three sketches:

1. parse serial CSV input into a 30-element float array
2. normalize the feature vector with `StandardScaler`
3. reduce to 2 PCA components
4. compute anomaly score with the selected model
5. map the score to `Benign` or `Malignant`
6. print the result and score to serial output

## 4.3 Model-Specific Behavior

### EllipticEnvelope sketch

- invokes `eml_elliptic_envelope_predict`
- prints the predicted class and Mahalanobis-like distance score
- relies on embedded covariance and offset values from `EllipticEnvelope.h`

### GaussianMixture sketch

- invokes `GaussianMixture_score`
- obtains mixture score and probabilities
- converts low likelihood into a malignant prediction via threshold comparison

### BayesianGaussianMixture sketch

- invokes `BayesianGaussianMixture_score`
- follows the same score-threshold decision pattern as the GMM sketch

## 5. Code Assessment

## 5.1 Strengths

- clear separation between training artifacts and embedded runtime use
- preprocessing is embedded, which reduces training/inference drift
- multiple anomaly detector variants allow comparison of deployment tradeoffs
- generated headers are lightweight and easy to include in Arduino projects

## 5.2 Constraints and Risks

- there is no automation for regenerating headers after notebook changes
- there is no environment file listing Python dependencies
- there is no automated build, unit test, or CI configuration in the repository
- the exported artifacts are duplicated across model-specific Arduino directories
- serial parsing assumes valid input and does not perform robust validation

## 5.3 Maintenance Implications

If the notebook is retrained or preprocessing changes, the following artifacts must remain synchronized:

- `StandardScaler.h`
- `PCA.h`
- model header for the selected detector
- any threshold constants embedded in the `.ino` sketch

This notebook-to-header synchronization is the main maintenance dependency in the project.

## 6. Suggested Documentation and Engineering Next Steps

Recommended follow-up improvements:

- add a dependency file for the Python notebook environment
- document the exact feature order expected by the Arduino sketches
- record the model metrics produced by notebook evaluation
- add an artifact regeneration procedure
- add an example serial input row and expected output format
- reduce duplication by sharing common preprocessing headers across Arduino variants

## 7. Summary

This repository is best understood as a notebook-driven TinyML export example. The notebook performs data preparation, dimensionality reduction, anomaly-model training, and artifact generation. The Arduino folders package those learned artifacts into deployable examples that classify breast-cancer feature vectors as benign or malignant through anomaly detection rather than standard supervised classification.
