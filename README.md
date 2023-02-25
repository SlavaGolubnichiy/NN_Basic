# NN_Basic
Basic Neural Network for experiments

## Description
Implemented with classes that store and calculate data (values and weights between Neurons).

## How to build
1. Download source files

2. Create a project (for Visual Studio, in my case)

3. Add downloaded files to the project

4. Add the directory with downloaded files to "include directories" project parameter. 

	Specifically for VS you need to:

	4.1 Make sure that Configuration = "All configurations" and Platform = "All platforms"
	
	4.2 Navigate to Project -> Properties -> C/C++ -> General

	4.3 In a list of parameters find "Additional include directories" parameter
	
	4.4 Set it to "$(SolutionDir)YOUR_PROJECT_FOLDER_NAME" [for example, $(SolutionDir)MyNeuralNet]
	[note, that in the example, MyNeuralNet folder is expected to contain this repository's files]
