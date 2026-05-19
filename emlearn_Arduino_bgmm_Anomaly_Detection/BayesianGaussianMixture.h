
    // !! This file was generated using emlearn

    #include <eml_mixture.h>
    
static const float BayesianGaussianMixture_means[4] = { -3.022053f, -0.019245f, -0.472582f, 4.067253f };
static const float BayesianGaussianMixture_precisions[8] = { 0.784568f, -0.327009f, 0.000000f, 0.718572f, 0.415463f, -0.420271f, 0.000000f, 0.454021f };
static const float BayesianGaussianMixture_log_weights[2] = { -0.104392f, -2.488471f };
static const float BayesianGaussianMixture_log_dets[2] = { -0.573111f, -1.667975f };
EmlMixtureModel BayesianGaussianMixture_model = { 2, 2, EmlCovarianceFull, BayesianGaussianMixture_means, BayesianGaussianMixture_precisions, BayesianGaussianMixture_log_dets, BayesianGaussianMixture_log_weights };


        int32_t
        BayesianGaussianMixture_predict_log_proba(const float values[], int32_t values_length, float *out)
        {

            return eml_mixture_predict_log_proba(&BayesianGaussianMixture_model,
                                values, values_length,
                                out);
        }


        EmlError
        BayesianGaussianMixture_score(const float values[], int32_t values_length, float *probabilities, float *score)
        {

            return eml_mixture_score(&BayesianGaussianMixture_model,
                                values, values_length,
                                probabilities, score);
        }

        int32_t
        BayesianGaussianMixture_predict_proba(const float values[], int32_t values_length, float *probabilities, float *score, float *resp)
        {

            return eml_mixture_predict_proba(&BayesianGaussianMixture_model,
                                values, values_length,
                                probabilities, score, resp);
        }
    