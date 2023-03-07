#pragma once
#ifndef LAYER_H_
#define LAYER_H_

#include "matop.h"	// tanh(Mat), normalize(Mat) functions
#include "Matrix.h"
#include "float64.h"

#include "RandXorsh.h"

// !!!
// somehow separate Layer class from InpuLayer class
// InputLayer: 
//		data - Matrix* N;
//		methods - constructor, getValues(), calcMe(Mat inputs)
// Layer:
//		data - Matrix* N, w;
//		methods - constructor, getValues(), getWeights(), setWeights(), calcMe(), private randomiseWeights()
class Layer
{
	protected:
	Matrix* N = nullptr;	// values
	Matrix* w = nullptr;	// weights[thisLayerNeuronIndex][leftLayerNeuronIndex]
	bool isInputLayer = false;

	public:
	Layer() {}

	Layer(const unsigned int leftLayerNeuronsNum, bool isInputLayer = false)
		:isInputLayer(isInputLayer)
	{
		if (!isInputLayer)
		{
			N = new Matrix(leftLayerNeuronsNum, 1);
			w = new Matrix(leftLayerNeuronsNum, leftLayerNeuronsNum);
			randomiseMat01Floor(*w);
		}
		else
		{
			unsigned int inputsSize = leftLayerNeuronsNum;
			N = new Matrix(inputsSize, 1);
			w = new Matrix(Matrix::getIdentityMatrix(inputsSize, inputsSize));
		}

	}

	const Matrix getValues() const
	{
		return *N;
	}

	const Matrix getWeights() const
	{
		return *w;
	}

	void setWeightsValues(const Matrix& newWeightsValues)
	{
		if (isInputLayer)
		{
			throw std::runtime_error("setWeights() is not allowed for InputLayer");
		}
		if (w->rows() != newWeightsValues.rows() || w->cols() != newWeightsValues.cols())
		{
			throw std::logic_error("new weights matrix must have same dimensions");
		}
		if (areWeightsValid(newWeightsValues))
		{
			throw std::invalid_argument("One or more weights' values are invalid.");
		}

		*(this->w) = newWeightsValues;
	}

	void calcMe(const Matrix& leftLayerValues)
	{
		if (w->cols() != leftLayerValues.rows())
		{
			throw std::logic_error("leftLayerValues matrix is not [NeuronsNum x 1]. Unable to calculate neurons.");
		}
		*N = (*w) * leftLayerValues;
	}

	void calcMeNormalized(const Matrix& leftLayerValues)
	{
		if (w->cols() != leftLayerValues.rows())
		{
			throw std::logic_error("leftLayerValues matrix is not [NeuronsNum x 1]. Unable to calculate neurons.");
		}
		*N = (*w) * leftLayerValues;
		matop::normalize(*N);
	}

	/// <summary>
	/// Calculates layer's neurons' values and normalizes them using tanh(val, steepness) function.
	/// </summary>
	/// <param name="steepness">
	/// Steepness defines how steep (how fast) y goes from -1 to +1 considering x goes from -inf to +inf.
	/// </param>
	void calcMeTanh(const Matrix& leftLayerValues, double steepness)
	{
		if (w->cols() != leftLayerValues.rows())
		{
			throw std::logic_error("leftLayerValues matrix is not [NeuronsNum x 1]. Unable to calculate neurons.");
		}
		*N = (*w) * leftLayerValues;
		matop::tanhCustom(*N, steepness);
	}

	private:
	// Floor function call is temporary, for degugging simplicity
	void randomiseMat01Floor(Matrix& mat)
	{
		for (unsigned int i = 0; i < mat.rows(); i++)
		{
			for (unsigned int j = 0; j < mat.cols(); j++)
			{
				mat.set(i, j, float64::floor(xorsh::generateDoubleRange01(), 2));
			}
		}
	}

	bool areWeightsValid(const Matrix& weights) const
	{
		for (unsigned int i = 0; i < weights.rows(); i++)
		{
			for (unsigned int j = 0; j < weights.cols(); j++)
			{
				double weight = weights.get(i, j);
				if (weight < 0 || 1.0 <= weight)
				{
					return false;
				}
			}
		}
		return true;
	}

};



#endif	// LAYER_H_
