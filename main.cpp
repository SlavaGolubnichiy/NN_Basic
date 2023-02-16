#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <sstream>	// for double to String with precisoon
#include <iomanip>

#include "Matrix.h"
#include "RandomXorsh.h"


// Matrix::operator*=
// Matrix::operator/= etc.
// FileContent01ToMatrix() - file can contain not a rect. matrix
// Matrix:: puclic access violation
// NN classes access violations


class ReaderTxt
{
	std::string path;
	std::fstream file;
	std::vector<std::string> lines;

	ReaderTxt(const char* path)
		:
		path(path)
	{
	}

	void tryToBinMatrix(char colDelim = ' ', char rowDelim = '\n')
	{
		file.open(path.c_str());
		
		// file's single line length ?
		char fileStr[11] = { 0 };

		// file's number of lines ?
		for (uint8_t i = 0; i < UINT8_MAX; i++)
		{
			file.getline(fileStr, 11);
			lines.push_back(std::string(fileStr));
		}
		
		file.close();
	}



};

struct TxtData
{
	std::vector<std::string> content;
};

class Reader
{
	FILE* file;
	std::string content;

	public:
	// last line (if empty) is ignored
	void read(const char* path, std::vector<std::string>& contentDest)
	{
		fopen_s(&file, path, "r");
		if (file == 0)
		{
			throw std::exception("file read error");
			__debugbreak();
		}
		
		std::string line;
		char ch;
		while (EOF != (ch = getc(file)))
		{
			line.append(1, ch);
			if ('\n' == ch)
			{
				contentDest.push_back(line);
				line.clear();
			}
		}

		if (line.size() != 0)
		{
			contentDest.push_back(line); // add last line with EOF, if not empty
		}
		
	}


};

void printFileContent(const TxtData& txtData)
{
	for (int i = 0; i < txtData.content.size(); i++)
	{
		printf("%s", txtData.content.at(i).c_str());
	}
}

void FileContent01ToMatrix(const TxtData& txtData, Matrix& m)
{
	char ch;
	for (uint8_t i = 0; i < m.rows_; i++)
	{
		for (uint8_t j = 0; j < m.cols_; j++)
		{
			ch = txtData.content.at(i).at(j);
			if ('\n' == ch)
			{
				continue;
			}
			m.p[i][j] = txtData.content.at(i).at(j) - '0';	// 0 if '0', 1 if '1'
		}
	}
}



/// train
// teacher gives prepared inputs to NN
// NN gets inputs and creates output
// teacher checks the output =>
	// if not ok => ask NN to readjust weights
// give next exercise to NN

/// test
// NN gets inputs and creates output
// human evaluates if NN did a good job or not






class Neuron
{
	public:
	double value = 0;
	// bias = 0.2;		// shouldnt it be in Layer class?

	// x1 and x2 will be arrays of Xs and Ws
	void calcValue(const double x1, const double w1, const double x2, const double w2)
	{
		value = x1 * w1 + x2 * w2;
		//return value;	// return to Layer for check if normaliation is needed
	}

	double getValue()
	{
		return value;
	}

};



static RandomXorsh random;

double randomiseDoubleRange01()
{
	uint32_t integer = random.generate32();
	//printf("int = %u \n", integer);
	double dfloat = (double)integer;
	dfloat = dfloat / (double)UINT32_MAX;
	// floor to hundreds
	dfloat = floor(dfloat * 100 + 0.5) / 100;
	return dfloat;
}

// weights[i][j]: i = index of this layer's neuron, j = index of leftLayer's neuron
// neurons[i] = lNeurons[0].value * weights[i][0] + ... + lNeurons[N-1].value * weights[i][N-1];
class Layer
{
	public:
	unsigned int neuronsNum;
	Neuron* neurons;
	double** weights;	// i = index of this layer's neuron, j = index of leftLayer's neuron
	unsigned int leftLayerNeuronsNum;

	Layer(const unsigned int leftLayerNeuronsNum, unsigned int neuronsNum)
		:
		neuronsNum(neuronsNum),
		neurons(new Neuron[neuronsNum]),
		weights(new double*[neuronsNum]),
		leftLayerNeuronsNum(leftLayerNeuronsNum)
	{
		for (uint8_t i = 0; i < neuronsNum; i++)
		{
			weights[i] = new double[leftLayerNeuronsNum];
		}
	}
	// implement destructor

	// for InputLayer only
	Layer(const unsigned int& inputsSize)
		:
		neuronsNum(inputsSize),
		neurons(new Neuron[inputsSize]),
		weights(new double*[inputsSize]),
		leftLayerNeuronsNum(1)	// value = x1*(w1=1); only
	{
		// initialize weights
		for (uint8_t i = 0; i < neuronsNum; i++)
		{
			weights[i] = new double[this->leftLayerNeuronsNum];
			weights[i][0] = 1;
		}

		for (uint8_t i = 0; i < neuronsNum; i++)
		{
			neurons[i] = Neuron();
		}
	}
	// for InputLayer only
	void receiveInput(const double* inputs, const unsigned int& inputsSize)
	{
		// init neurons
		for (uint8_t i = 0; i < neuronsNum; i++)
		{
			neurons[i].calcValue(inputs[i], 1, 0, 0);				// value = input1*(w1=1) + 0*0; only
			neurons[i].calcValue(inputs[i], weights[i][0], 0, 0);	// more correct version
		}
	}

	void randomiseWeights()
	{
		for (uint32_t i = 0; i < neuronsNum; i++)
		{
			for (uint32_t j = 0; j < leftLayerNeuronsNum; j++)
			{
				weights[i][j] = randomiseDoubleRange01();
			}
		}	
	}

	void recalculateNeurons(const Layer& lLayer)	// recalc these neurons based on changed values from lLayer
	{
		for (uint32_t i = 0; i < neuronsNum; i++)	// iterate through neurons on this layer
		{
			this->neurons[i].calcValue
			(
				lLayer.neurons[0].value, this->weights[i][0],	// this->weights[i]
				lLayer.neurons[1].value, this->weights[i][1]
			);
		}
	}

};


std::string toStringWithPrecision(double value, unsigned int precision)
{
	
	std::ostringstream streamObj3;		// create an output string stream
	streamObj3 << std::fixed;			// set fixed-point Notation
	streamObj3 << std::setprecision(precision);
	streamObj3 << value;				// add double to stream
	return streamObj3.str();
}

std::string toString(double** matrix, unsigned int matrixRows, unsigned int matrixCols, unsigned int doublePrecision)
{
	std::string res = "";
	for (uint32_t i = 0; i < matrixRows; i++)
	{
		std::string row = "";
		for (uint32_t j = 0; j < matrixCols; j++)
		{
			row += toStringWithPrecision(matrix[i][j], 2) + " ";	// std::to_string() for regular
		}
		row += "\n";
		res += row;
	}
	return res;
}

// implement value compression !!!
class Network
{
	public:
	Layer inputLayer = Layer(2);	// make editable on run-time
	Layer layers[2] = {Layer(inputLayer.neuronsNum, 2), Layer(layers[0].neuronsNum, 2)};	// = ;
	unsigned int layersNum;

	// layersNum = 2, eachLayerNeuronsNum = {2,2} 
	Network(unsigned int inputsNum, unsigned int layersNum, unsigned int* eachLayerNeuronsNum)
		:
		layersNum(layersNum)
	{
		// inputLayer = Layer(inputsNum);
		//layers[0] = Layer(eachLayerNeuronsNum[0], inputLayer.neuronsNum);	// bind to inputsLayer
		//for (uint32_t i = 2; i < layersNum; i++)	// i=2 bc already inputLayer + layers[0]
		//{
		//	layers[i] = Layer(eachLayerNeuronsNum[i], layers[i-1].neuronsNum);	// bound to leftLayers
		//}
	}

	void randomiseWeights()
	{
		for (uint32_t i = 0; i < 2; i++)
		{
			layers[i].randomiseWeights();
		}
	}

	std::string weightsToString()
	{	
		std::string res = "x[N] : layerInput \n";
		res += toString(inputLayer.weights, inputLayer.neuronsNum, 1, 2);
		res += "\n";
		res += "layerInput : layer0 \n";
		res += toString(layers[0].weights, layers[0].neuronsNum, inputLayer.neuronsNum, 2);
		res += "\n";

		for (uint8_t i = 1; i < layersNum; i++)
		{
			res += "layer" + std::to_string(i) + " : layer" + std::to_string(i+1) + "\n";
			res += toString(layers[i].weights, layers[i].neuronsNum, layers[i-1].neuronsNum, 2);
			res += "\n";
		}
		return res;
	}

	void thinkStep1(const double* inputs, const unsigned int inputsSize = 2)
	{
		inputLayer.receiveInput(inputs, inputsSize);
	}

	void thinkStep2()
	{
		// test-purpose version
		layers[0].recalculateNeurons(inputLayer);
		layers[1].recalculateNeurons(layers[0]);
		/*
		layers[0].recalculateNeurons(inputLayer);
		for (uint32_t i = 1; i < layersNum; i++)
		{
			// each layer is calculated depending on left layer output
			layers[i].recalculateNeurons(layers[i-1]);
		}
		*/
	}

	void thinkStep3TakeOutput(std::vector<double>& resultsDest)
	{
		uint8_t lastLayerIndex = layersNum - 1;
		uint8_t lastLayerNeuronsNum = layers[lastLayerIndex].neuronsNum;
		for (uint8_t i = 0; i < lastLayerNeuronsNum; i++)
		{
			resultsDest.push_back(this->layers[lastLayerIndex].neurons[i].getValue());
		}
	}


};








int main()
{
	/*
	TxtData oneTxtData = {};
	int linesNum = 0;

	Reader reader;
	reader.read("one.txt", oneTxtData.content);
	linesNum = oneTxtData.content.size();
	printf("file content: \n\n");
	printFileContent(oneTxtData);
	printf("number of lines = %u \n", linesNum);

	Matrix m(oneTxtData.content.size(), oneTxtData.content.at(0).size());
	FileContent01ToMatrix(oneTxtData, m);
	printf("\n\nmatrix: \n");
	printf("%s", m.toString("").c_str());
	
	*/

	unsigned int inputsNum = 2;
	unsigned int layersNum = 2;
	unsigned int eachLayerValuesNum[2] = {2,2};
	Network net = Network(inputsNum, layersNum, eachLayerValuesNum);	
	net.randomiseWeights();
	printf("%s \n", net.weightsToString().c_str());

	double inputs[2] = {1, 7};
	net.thinkStep1(inputs);
	net.thinkStep2();
	std::vector<double> resDistrib;
	net.thinkStep3TakeOutput(resDistrib);

	printf("%s %s \n", std::to_string(resDistrib[0]).c_str(), std::to_string(resDistrib[1]).c_str());

	
	
	#if 0

	std::fstream file;	
	file.open("one.txt");
	char fileStr[5] = {0};
	int err = 0;
	for (int i = 0; i < 5; i++)
	{
		file.getline(fileStr, 11);
	}
	file.close();
	std::cout << fileStr << std::endl;
	printf("\n");

	#endif
	
	system("pause");
	return 0;
}
