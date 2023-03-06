#include <cstdlib>

//#include "NeuralNet/NNetwork.h"
//#include "matop.h"

/// TODO
// priority: HIGH
// implement logging of each NNetwork run to .json or/and .txt file
// 

// priority: LOW
// improve code style, methods' names etc.
// check performance
// separate Layer::constructor for InputLayer as possible
// FileContent01ToMatrix() - file can contain not a rect. matrix
// parallelize some matrix's class methords (+ , -, /, *num, toString, *mat, etc)

#include "RandXorsh.h"

int main(int args)
{
	xorsh::test_generate32(2000);


	/*
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
	
	printf("\n\n");
	printf("---- normalized \n\n");
	Matrix outputNorm = net.runOnceNormalized(x);
	printf("x = \n%s\n", x.toString(" ").c_str());
	net.printLastRunData();
	matop::normalize(outputNorm);
	printf("normalized(outputNorm) = \n%s \n\n", outputNorm.toString().c_str());

	printf("\n\n");
	printf("---- hyperbolically tangented\n\n");
	Matrix outputTanh = net.runOnceTanh(x, 0.2);
	printf("x = \n%s\n", x.toString(" ").c_str());
	net.printLastRunData();
	matop::normalize(outputTanh);
	printf("normalized(outputTanh) = \n%s \n\n", outputTanh.toString().c_str());
	*/

	
	system("pause");
	return 0;
}
