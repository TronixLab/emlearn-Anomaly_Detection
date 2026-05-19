
    // !! This file was generated using emlearn

    #include <eml_mixture.h>
    
static const float GaussianMixture_means[4] = { -3.096693f, -0.150822f, -1.407650f, 2.621028f };
static const float GaussianMixture_precisions[8] = { 0.832556f, -0.306410f, 0.000000f, 0.763306f, 0.434932f, -0.421474f, 0.000000f, 0.462609f };
static const float GaussianMixture_log_weights[2] = { -0.204631f, -1.687118f };
static const float GaussianMixture_log_dets[2] = { -0.453351f, -1.603437f };
EmlMixtureModel GaussianMixture_model = { 2, 2, EmlCovarianceFull, GaussianMixture_means, GaussianMixture_precisions, GaussianMixture_log_dets, GaussianMixture_log_weights };


        int32_t
        GaussianMixture_predict_log_proba(const float values[], int32_t values_length, float *out)
        {

            return eml_mixture_predict_log_proba(&GaussianMixture_model,
                                values, values_length,
                                out);
        }


        EmlError
        GaussianMixture_score(const float values[], int32_t values_length, float *probabilities, float *score)
        {

            return eml_mixture_score(&GaussianMixture_model,
                                values, values_length,
                                probabilities, score);
        }

        int32_t
        GaussianMixture_predict_proba(const float values[], int32_t values_length, float *probabilities, float *score, float *resp)
        {

            return eml_mixture_predict_proba(&GaussianMixture_model,
                                values, values_length,
                                probabilities, score, resp);
        }
    