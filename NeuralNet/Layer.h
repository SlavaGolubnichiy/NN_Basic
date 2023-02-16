#pragma once

#include "../Matrix.h"

static double roundWithPrecision(const double value, const unsigned int precision)
{
	double res;
	double k = pow(10, precision);
	res = floor(value * k + 0.5) / k;
	return res;
}

static RandomXorsh random;
static double randomiseDoubleRange01()
{
	uint32_t integer = random.generate32();
	double dfloat = (double)integer;
	dfloat = dfloat / (double)UINT32_MAX;
	dfloat = roundWithPrecision(dfloat, 2);		// floor to hundreds
	return dfloat;
}

static void randomiseArrayMatrix(double** mat, const unsigned int rows, const unsigned int cols)
{
	for (uint32_t i = 0; i < rows; i++)
	{
		for (uint32_t j = 0; j < cols; j++)
		{
			mat[i][j] = randomiseDoubleRange01();
		}
	}
}

class Layer
{
	Matrix* N = nullptr;	// values
	Matrix* w = nullptr;	// weights[thisLayerNeuronIndex][leftLayerNeuronIndex]

	public:
	Layer() {}

	// always set second param to "inputLayer"
	Layer(const unsigned int inputsSize, const char* ignoredInputLayerStr)
	{
		N = new Matrix(inputsSize, 1);
		double** mat = nullptr;
		allocMem(mat, inputsSize, inputsSize);
		w = new Matrix(Matrix::getIdentityMatrix(inputsSize, inputsSize));
	}

	Layer(const unsigned int leftLayerNeuronsNum)
	{
		N = new Matrix(leftLayerNeuronsNum, 1);
		double** mat = nullptr;
		allocMem(mat, leftLayerNeuronsNum, leftLayerNeuronsNum);
		randomiseArrayMatrix(mat, leftLayerNeuronsNum, leftLayerNeuronsNum);
		w = new Matrix((const double**)mat, leftLayerNeuronsNum, leftLayerNeuronsNum);
		freeMem(mat, leftLayerNeuronsNum);
	}

	const Matrix& getValues()
	{
		return *N;
	}

	const Matrix& getWeights()
	{
		return *w;
	}

	void calcMe(const Matrix& leftLayerValues)
	{
		*N = (*w) * leftLayerValues;
	}

};