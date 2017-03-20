// Demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

enum  ConeType {
	// created as a basic enum to specify all the possibly RGB values
	RED,
	GREEN,
	BLUE,
};


class Neuron {
private:
	float cellPotential;
	bool isGraded;
	std::vector<Neuron> inputConnectedCells;
	std::vector<Neuron> outputConnectedCells;

public:
	virtual const char getType() { return 'N'; } // Allows for quick type checking of an object
	Neuron() {
		cellPotential = -40;
		isGraded = false;
	}

	Neuron(float potential) {
		cellPotential = potential;
		isGraded = false;
	}

	Neuron(float potential, bool graded) {
		cellPotential = potential;
		isGraded = graded;
	}

	std::vector<float> acquireInputs() {
		std::vector<float> potentials = std::vector<float>{};
		for (int i = 0; i < inputConnectedCells.size(); ++i) {
			potentials.resize(i + 1);
			potentials[i] = inputConnectedCells[i].getPotential();
		}

		return potentials;
	}

	void update(int ticks) {
		acquireInputs();
		//Do whatever calculation is neccesary to obtain a cell potential.
		//cellPotential = evaluated value;
	}

	float getPotential() {
		return cellPotential;
	}
};

class Photoreceptor : public Neuron {
private:
	//float eccentricity;
	//float locationAngle; //Relative to straight up
public:
	virtual const char getType() override { return 'P'; } // returns the type 
	virtual const char getRGB() { return '0'; } // creates a virtual method which is overriden by rod's and cones
	Photoreceptor() : Neuron(-40, true) {}

	virtual void setInputs(int lightIntensity, int lightFrequency, int lightAngle) {

	}


	float getPotential() {
		// for the case of displaying something
		return 0.0;
	}
};

class Cone : public Photoreceptor {
private:
	ConeType coneType;
public:
	static constexpr float PEAK_DENSITY() { return 199.2f; }
	static constexpr float PERIPHERY_DENSITY() { return 3.6f; }
	static constexpr float MEAN_DENSITY_CONSTANT() { return 15; } //Ignore these for now

	Cone(ConeType t) {
		coneType = t; // stores RGB value which is unique to the cone

	}
	virtual const char getRGB() override {
		if (coneType == RED) {
			return 'R';
		}
		else if (coneType == BLUE) {
			return 'B';
		}
		return 'G';
	}
	void setInputs(int lightIntensity, int lightFrequency, int lightAngle) {
		//TODO: Actually do something with this
	}

public:
	virtual const char getType() override {
		// ALLOWS for visual display of weather it is a rod or a con being generated
		return 'C';
	}
};
class Rod : public Photoreceptor {
	void setInputs(int lightIntensity, int lightFrequency, int lightAngle) {
		//TODO: Actually so something with this too
	}

public:
	virtual const char getType() override {
		// ALLOWS for visual display of weather it is a rod or a con being generated
		return 'R';
	}

	virtual const char getRGB() override {
		return ' ';
	}
};

class Bipolar : public Neuron {


public:
	Bipolar() : Neuron(-40 /*Not sure what this number is for the bipolar cells*/, true) {  }
};

class Ganglion : public Neuron {
public:
	Ganglion() {}
};



Photoreceptor* builder(int num) {
	// Method creates sequncial Rod's and Cones depending position in the matrix
	if (num % 2 == 0) {
		Photoreceptor* p = new Rod(); // creates a new rod which is then returned
		return p;
	}
	else {
		static int count = 1; // allows for quick alteration of RGB cones being created 
		if (count % 3 == 1) {
			Photoreceptor* p = new Cone(RED);
			count++;
			return p;
		}
		else if (count % 3 == 2) {
			Photoreceptor* p = new Cone(BLUE);
			count++;
			return p;
		}
		else {
			Photoreceptor* p = new Cone(GREEN);
			count++;
			return p;
		}


	}



}




int main()
{
	const int rows = 10;
	const int col = 10;


	//std::vector<Photoreceptor> receptors; // will be implemented in the future
	//std::vector<Bipolar> bipolars; 
	//std::vector<Ganglion> ganglia;


	Photoreceptor* x[rows][col] = {};// call a method which returns a list filled in with all the inputs;

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < col; ++j) {
			x[i][j] = builder(i + j);

		}

	}
	std::cout << "Display of Rod and Cone Generator : \n\n";
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < col; ++j) {

			std::cout << x[i][j]->getType();
		}
		std::cout << '\n';
	}

	std::cout << "\n\nDisplay the type of Cone that is created : \n\n";

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < col; ++j) {

			if (x[i][j]->getType() == 'c') {
				std::cout << x[i][j]->getRGB();
			}
			else {
				std::cout << x[i][j]->getRGB();
			}
		}
		std::cout << '\n';
	}
	int xy;
	std::cin >> xy;

	return 0;


}


