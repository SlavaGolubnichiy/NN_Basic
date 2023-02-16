#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <sstream>	// for double to String with precision
#include <iomanip>

#include "Matrix.h"


/// TODO
// priority: HIGH
// find where printed precision comes from (6 numbers after the dot)
// separate Layer::constructor for InputLayer from LayerV2 as possible

// priority: LOW
// FileContent01ToMatrix() - file can contain not a rect. matrix
// LayerV2::LayerV2() {} - try delete it 
// Classes access modificators (Matrix, LayerV2, NetworkV2)
// check performance



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

void FileContent01ToMatrix(const TxtData& txtData, unsigned int** m, const unsigned int mRows, const unsigned int mCols)
{
	char ch;
	for (uint8_t i = 0; i < mRows; i++)
	{
		for (uint8_t j = 0; j < mCols; j++)
		{
			ch = txtData.content.at(i).at(j);
			if ('\n' == ch)
			{
				continue;
			}
			m[i][j] = (double)(txtData.content.at(i).at(j) - '0');	// 0 if '0', 1 if '1'
		}
	}
}



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



#include "NeuralNet/NNetwork.h"



int main()
{
	// file reading
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

	// code of previous Network usage
	/*
	unsigned int inputsNum = 2;
	unsigned int layersNum = 2;
	unsigned int eachLayerValuesNum[2] = {2,2};
	Network net = Network(inputsNum, layersNum, eachLayerValuesNum);	
	net.randomiseWeights();
	//net.setWeightsSequential();
	printf("%s \n", net.weightsToString().c_str());


	double inputs[2] = {1, 7};
	checkCalc(net, inputs, 2);
	
	net.thinkStep1(inputs);
	net.thinkStep2();
	std::vector<double> resDistrib;
	net.thinkStep3TakeOutput(resDistrib);

	printf("%s %s \n", std::to_string(resDistrib[0]).c_str(), std::to_string(resDistrib[1]).c_str());

	*/


	
	std::vector<double> in = 
	{
		1, 2
	};
	Matrix x(in, true);
	unsigned int layersNum = 2;
	NNetwork net = NNetwork(in.size(), layersNum);
	net.runOnce(x);
	








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
