
/*******************************************************************************
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/

#include "kb_pipeline.h"

#include "tree_ensemble.h"

#include "sml_profile_utils.h"
static uint32_t profile_cycle_count = 0;
static float profile_total_time = 0.0f;
static float profile_avg_iter_time = 0.0f;

#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-variable"

// Operations
#define FLT_MAX (FLOAT)0x7f7fffff
#define FLT_MIN -(FLT_MAX)
// FILL_MAX_NUMBER_COLUMNS
// FILL_MAX_NUMBER_PARAMS
#ifndef NUMBER_OF_COLUMNS
#define NUMBER_OF_COLUMNS 10
#endif

#ifndef MAX_NUMBER_PARAMS
#define MAX_NUMBER_PARAMS 10
#endif

int16_t columns[NUMBER_OF_COLUMNS];
static float params[MAX_NUMBER_PARAMS];

static float_data_t input_params = {
    .data = params,
    .size = MAX_NUMBER_PARAMS};

static int16_data_t input_columns = {
    .data = columns,
    .size = NUMBER_OF_COLUMNS};


int32_t data_streaming_0(void* model, int16_t *pSample)
{
	int32_t FrameIdx = 0;
	kb_model_t * kb_model = (kb_model_t*)model;

	input_columns.data[0] = AX_S_PIPELINE_2_RANK_1;
	input_columns.data[1] = AY_S_PIPELINE_2_RANK_1;
	input_columns.data[2] = SVTOTAL_S_PIPELINE_2_RANK_1;
	input_columns.size = 3;
	FrameIdx += tr_sensor_sensors(pSample, &input_columns, &kb_model->input_data[FrameIdx]);	
saveSensorData(kb_model->pdata_buffer->data, kb_model->input_data, kb_model->pdata_buffer->size);
	return 1;
}



int32_t data_segmentation_0(void* model, int32_t model_index)
{
	int32_t FrameIdx = 0;
	kb_model_t * kb_model = (kb_model_t*)model+model_index;
	int32_t new_samples = rb_num_items(kb_model->pdata_buffer->data, kb_model->last_read_idx);
	int32_t i;
	for (i=0; i<new_samples; i++){
			kb_model->last_read_idx += 1;
		input_columns.size = 0;

	if(sg_windowing(kb_model, &input_columns, kb_model->psegParams)){
			return 1;
		}
	}
	// Colect more samples
	return -1;
}



int32_t feature_gen_0(void * model, int32_t *nfeats)
{
	int32_t column = 0;
	int32_t num_params = 1;
	kb_model_t * kb_model = (kb_model_t*)model;
	int32_t nframes = kb_model->sg_length;
	int32_t CompIdx = (kb_model->feature_bank_index) * (kb_model->pfeature_bank->bank_size);
	
input_params.data[0] = 23;	
input_params.size = 1;	
input_columns.data[0] = AX_D_PIPELINE_2_RANK_1;	
input_columns.size=1;	
*nfeats = fg_frequency_mfe(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.data[0] = 100;	
input_params.data[1] = 0;	
input_params.data[2] = 500;	
input_params.data[3] = 0.2;	
input_params.data[4] = 2;	
input_params.data[5] = 0;	
input_params.size = 6;	
input_columns.data[0] = AY_D_PIPELINE_2_RANK_1;	
input_columns.size=1;	
*nfeats = fg_frequency_peak_frequencies(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.data[0] = 100;	
input_params.data[1] = 0;	
input_params.data[2] = 500;	
input_params.data[3] = 0.2;	
input_params.data[4] = 2;	
input_params.data[5] = 0;	
input_params.size = 6;	
input_columns.data[0] = SVTOTAL_D_PIPELINE_2_RANK_1;	
input_columns.size=1;	
*nfeats = fg_frequency_peak_frequencies(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.data[0] = 5;	
input_params.size = 1;	
input_columns.data[0] = AY_D_PIPELINE_2_RANK_1;	
input_columns.size=1;	
*nfeats = fg_frequency_peak_harmonic_product_spectrum(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.data[0] = 5;	
input_params.size = 1;	
input_columns.data[0] = SVTOTAL_D_PIPELINE_2_RANK_1;	
input_columns.size=1;	
*nfeats = fg_frequency_peak_harmonic_product_spectrum(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.data[0] = 16;	
input_params.data[1] = 0;	
input_params.size = 2;	
input_columns.data[0] = AY_D_PIPELINE_2_RANK_1;	
input_columns.size=1;	
*nfeats = fg_frequency_power_spectrum(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = AX_D_PIPELINE_2_RANK_1;	
input_columns.size=1;	
*nfeats = fg_frequency_spectral_entropy(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
input_params.size = 0;	
input_columns.data[0] = AY_D_PIPELINE_2_RANK_1;	
input_columns.size=1;	
*nfeats = fg_frequency_spectral_entropy(kb_model, &input_columns, &input_params, &kb_model->pfeature_bank->pFeatures[CompIdx]);	
CompIdx += *nfeats;	
return 1;
}



int32_t feature_transform_0(void * model)
{
	kb_model_t * kb_model = (kb_model_t*)model;
	
	static struct minmax aminmax[24] = {
	
		{3, 49125.0f, 64858.0f},	
		{8, 49125.0f, 66999.0f},	
		{9, 49125.0f, 67440.0f},	
		{11, 49125.0f, 73814.0f},	
		{12, 49125.0f, 72137.0f},	
		{14, 49125.0f, 73375.0f},	
		{15, 49125.0f, 75842.0f},	
		{17, 49125.0f, 75431.0f},	
		{18, 49125.0f, 71283.0f},	
		{25, 0.0f, 450.0f},	
		{26, 0.0f, 490.0f},	
		{31, 0.0f, 480.0f},	
		{27, 0.0f, 35.0f},	
		{28, 0.0f, 2116295936.0f},	
		{29, 0.0f, 50.0f},	
		{30, 0.0f, 2142936704.0f},	
		{32, 0.0f, 22.627412796020508f},	
		{35, 0.0f, 34.54102325439453f},	
		{37, 0.0f, 35.77709197998047f},	
		{38, 0.0f, 49.43416213989258f},	
		{42, 0.0f, 30.0f},	
		{43, 0.0f, 35.77709197998047f},	
		{47, 0.0f, 2.383645534515381f},	
		{48, 0.0f, 2.40824031829834f},	
	};
	
	int32_t start = (kb_model->feature_bank_index+1) * (kb_model->pfeature_bank->bank_size);
	
	int32_t total_features = (kb_model->pfeature_bank->bank_size) * (kb_model->pfeature_bank->num_banks);
	
	min_max_scale(kb_model->pfeature_bank->pFeatures, kb_model->pfeature_vector, kb_model->pfeature_vector->size, start, total_features, 0, 255, aminmax);
	return 1;

}



int32_t recognize_vector_0(void * model)
{
	int32_t ret;
	kb_model_t * kb_model = (kb_model_t*)model;
	ret = tree_ensemble_simple_submit(kb_model->classifier_id, kb_model->pfeature_vector, kb_model->pmodel_results);
	return ret;

}
