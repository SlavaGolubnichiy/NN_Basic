#pragma once
// no multiple #include protection, bc main.h can be included in main.cpp only

#if 0

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "float64.h"
#include "NeuralNet/NNetwork.h"
#include "matop.h"

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
	for (unsigned int i = 0; i < txtData.content.size(); i++)
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
			// change output matrix from unsigned int to double
			m[i][j] = (double)(txtData.content.at(i).at(j) - '0');	// 0 if '0', 1 if '1'
		}
	}
}

std::string toString(double** matrix, unsigned int matrixRows, unsigned int matrixCols, unsigned int doublePrecision)
{
	std::string res = "";
	for (uint32_t i = 0; i < matrixRows; i++)
	{
		std::string row = "";
		for (uint32_t j = 0; j < matrixCols; j++)
		{
			row += float64::toString(matrix[i][j], 2) + " ";	// std::to_string() for regular
		}
		row += "\n";
		res += row;
	}
	return res;
}



int main(int args)
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
		1, 2, 3
	};
	Matrix x = Matrix(in, true);
	unsigned int layersNum = 2;
	NNetwork net = NNetwork(in.size(), layersNum);

	printf("---- without normalization \n\n");
	Matrix output = net.runOnce(x);
	printf("x = \n%s\n", x.toString(" ").c_str());
	net.printLastRunData();
	matop::normalize(output);
	printf("normalized(output) = \n%s \n\n", output.toString().c_str());
	
	/*
	std::fstream file;
	file.open("one.txt");
	char fileStr[5] = { 0 };
	int err = 0;
	for (int i = 0; i < 5; i++)
	{
		file.getline(fileStr, 11);
	}
	file.close();
	std::cout << fileStr << std::endl;
	printf("\n");
	*/

	system("pause");
	return 0;
}

#endif
