/*
A library to do linear algebra for neural network
evaluation.

Tao Chen, 07/05/2018
*/

#include <math.h>
#include <stdlib.h>

struct m_matrix {
	int input_size;		// height
	int output_size;	// width
	float **data;
};


// construct a matrix from an 1D array
inline struct m_matrix make_matrix_from_1D(int input_size, int output_size, float *data)
{
	struct m_matrix matrix;
	matrix.input_size = input_size;
	matrix.output_size = output_size;

	matrix.data = (float**)malloc(sizeof(float*)*input_size);
	for (int i = 0; i < input_size; i++) {
		matrix.data[i] = (float*)malloc(sizeof(float)*output_size);
		for (int j = 0; j < output_size; j++) {
			matrix.data[i][j] = data[i*input_size+j];
		}
	}

	return matrix;
}

// construct a matrix from a 2D array
inline struct m_matrix make_matrix_from_2D(int input_size, int output_size, float **data) 
{
	struct m_matrix matrix;
	matrix.input_size = input_size;
	matrix.output_size = output_size;

	matrix.data = (float**)malloc(sizeof(float*)*input_size);
	for (int i = 0; i < input_size; i++) {
		matrix.data[i] = (float*)malloc(sizeof(float)*output_size);
		for (int j = 0; j < output_size; j++) {
			matrix.data[i][j] = data[i][j];
		}
	}

	return matrix;
}

// construct a vector from an 1D array
inline struct m_matrix make_vector(int size, float *data) 
{
	struct m_matrix vector;
	vector.output_size = size;
	vector.input_size = 1;

	vector.data = (float**)malloc(sizeof(float*));
	vector.data[0] = (float*)malloc(sizeof(float)*size);
	for (int i = 0; i < size; i++) {
		vector.data[0][i] = data[i];
	}

	return vector;
}

// construct a vector from 3 floats 
inline struct m_matrix make_vector_3(float x, float y, float z)
{
	struct m_matrix vector;
	vector.output_size = 3;
	vector.input_size = 1;

	vector.data = (float**)malloc(sizeof(float*));
	vector.data[0] = (float*)malloc(sizeof(float)*3);
	vector.data[0][0] = x;
	vector.data[0][1] = y;
	vector.data[0][2] = z;

	return vector;
}

// construct a vector of all 0s
inline struct m_matrix make_vector_zeros(int size) 
{
	struct m_matrix vector;
	vector.output_size = size;
	vector.input_size = 1;

	vector.data = (float**)malloc(sizeof(float*));
	vector.data[0] = (float*)malloc(sizeof(float)*size);
	for (int i = 0; i < size; i++) {
		vector.data[0][i] = 0;
	}

	return vector;
}

// =============================
// arithmetics
// =============================

// negate
struct m_matrix vector_neg(const struct m_matrix *a)
{
	struct m_matrix vector;
	vector.output_size = a->output_size;
	vector.input_size = 1;
	vector.data = (float**)malloc(sizeof(float*));
	vector.data[0] = (float*)malloc(sizeof(float)*a->output_size);

	for (int i = 0; i < a->output_size; i++) {
		vector.data[0][i] = a->data[0][i];
	}

	return vector;
}

// a-b
struct m_matrix vector_sub(const struct m_matrix *a, const struct m_matrix *b)
{
	// vectors must be of the same length
	// return a vector the same size of a and contains only 0s
	if (a->output_size != b->output_size) {
		return make_vector_zeros(a->output_size);
	}

	struct m_matrix vector;
	vector.output_size = a->output_size;
	vector.input_size = 1;
	vector.data = (float**)malloc(sizeof(float*));
	vector.data[0] = (float*)malloc(sizeof(float)*a->output_size);

	for (int i = 0; i < a->output_size; i++) {
		vector.data[0][i] = a->data[0][i] - b->data[0][i];
	}

	return vector;
}

// a+b
struct m_matrix vector_add(const struct m_matrix *a, const struct m_matrix *b)
{
	struct m_matrix b_neg = vector_neg(b);
	return vector_sub(a, &b_neg);
}

// dot product, a dot b
float vector_dot(const struct m_matrix *a, const struct m_matrix *b)
{
	if (a->output_size != b->output_size) {
		return 0;
	}

	float dot = 0;
	for (int i = 0; i < a->output_size; i++) {
		dot += a->data[0][i] * b->data[0][i];
	}

	return dot;
}

// return a column as a vector of from a matrix specify by the index
struct m_matrix get_col(const struct m_matrix *a, int col) {
	struct m_matrix vector;
	vector.output_size = a->input_size;
	vector.input_size = 1;

	vector.data = (float**)malloc(sizeof(float*));
	vector.data[0] = (float*)malloc(sizeof(float)*a->input_size);

	for (int i = 0; i < a->input_size; i++) {
		vector.data[0][i] = a->data[i][col];
	}

	return vector;
}

// return a row as a vector of from a matrix specify by the index
struct m_matrix get_row(const struct m_matrix *a, int row) {
	struct m_matrix vector;
	vector.output_size = a->output_size;
	vector.input_size = 1;

	vector.data = (float**)malloc(sizeof(float*));
	vector.data[0] = (float*)malloc(sizeof(float)*a->output_size);

	for (int i = 0; i < a->output_size; i++) {
		vector.data[0][i] = a->data[row][i];
	}

	return vector;
}

// multipltcation, a*b 
struct m_matrix mul(const struct m_matrix *a, const struct m_matrix *b)
{
	if (a->output_size != b->input_size) {
		return make_vector_zeros(a->output_size);
	}

	float **result;
	result = (float**)malloc(sizeof(float*)*a->input_size);
	for (int i = 0; i < a->input_size; i++) {
		result[i] = (float*)malloc(sizeof(float)*a->output_size);
	}

	struct m_matrix row_vec;
	struct m_matrix col_vec;

	for (int row = 0; row < a->input_size; row++) {
		row_vec = get_row(a, row);
		for (int col = 0; col < b->output_size; col++) {
			col_vec = get_col(b, col);
			result[row][col] = vector_dot(&row_vec, &col_vec);
		}
	}

	return make_matrix_from_2D(a->input_size, b->output_size, result);
}





