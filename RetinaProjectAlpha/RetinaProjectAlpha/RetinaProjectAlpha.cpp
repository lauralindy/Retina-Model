// Demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>

enum  ConeType {
	// created as a basic enum to specify all the possibly RGB values
	RED,
	GREEN,
	BLUE,
};


class Neuron {
protected:
	double cellPotential;
	double cellMin;
	double cellMax;
	bool isGraded;
	std::vector<Neuron*> inputCenterConnectedCells;
	std::vector<Neuron*> inputSurroundConnectedCells;
	std::vector<Neuron*> outputConnectedCells;

public:
	virtual const char getType() { return 'N'; } // Allows for quick type checking of an object
	Neuron() {
		cellMin = -40;
		cellMax = -65;
		cellPotential = cellMin;
		isGraded = false;
	}

	Neuron(double potentialMin, double potentialMax) {
		cellMin = potentialMin;
		cellMax = potentialMax;
		cellPotential = cellMin;
		isGraded = false;
	}

	Neuron(double potentialMin, double potentialMax, bool graded) {
		cellMin = potentialMin;
		cellMax = potentialMax;
		cellPotential = cellMin;
		isGraded = graded;
	}

	void addInputCellCenter(Neuron* n) {
		inputCenterConnectedCells.resize(inputCenterConnectedCells.size() + 1);
		inputCenterConnectedCells[inputCenterConnectedCells.size() - 1] = n;
	}

	void addInputCellSurround(Neuron* n) {
		inputSurroundConnectedCells.resize(inputSurroundConnectedCells.size() + 1);
		inputSurroundConnectedCells[inputSurroundConnectedCells.size() - 1] = n;
	}

	void resetConnections() {
		inputCenterConnectedCells = std::vector<Neuron*> {};
		inputSurroundConnectedCells = std::vector<Neuron*>{};
	}

	std::vector<std::vector<double>> acquireInputs() {
		std::vector<double> centerPotentials = std::vector<double>{};
		for (int i = 0; i < inputCenterConnectedCells.size(); ++i) {
			centerPotentials.resize(i + 1);
			centerPotentials[i] = inputCenterConnectedCells[i]->getPotential();
		}
		std::vector<double> surroundPotentials = std::vector<double>{};
		for (int i = 0; i < inputSurroundConnectedCells.size(); ++i) {
			centerPotentials.resize(i + 1);
			centerPotentials[i] = inputSurroundConnectedCells[i]->getPotential();
		}

		return std::vector<std::vector<double>> {centerPotentials, surroundPotentials};
	}

	void update(int ticks) {
		double averageInput = 0;
		std::vector<std::vector<double>> inputs = acquireInputs();
		for (int i = 0; i < inputs.size(); i++) {
			for (int j = 0; j < inputs[i].size(); j++) {
				averageInput += inputs[i][j];
			}
		}
		averageInput /= inputs[0].size()+inputs[1].size();

		cellPotential = averageInput;
	}

	double getPotential() {
		return cellPotential;
	}
};

class Photoreceptor : public Neuron {
protected:
	//double eccentricity;
	//double locationAngle; //Relative to straight up
	int photonsHit;
	int photonFrequency;

public:
	virtual const char getType() override { return 'P'; } // returns the type 
	virtual const char getRGB() { return '0'; } // creates a virtual method which is overriden by rod's and cones
	Photoreceptor() : Neuron(-40,-65, true) {}

	void setInputs(int photons, int frequency, int lightAngle) {
		photonsHit = photons;
		photonFrequency = frequency;
		//TODO: light angle/Stiles-Crawford Effect
	}

	virtual void update(int ticks) {
		//double num = 1.0f / photonsHit;
		//num = 1.0f - num;
		//cellPotential = num*(cellMax - cellMin) + cellMin;
	}
};

class Cone : public Photoreceptor {
private:
	ConeType coneType;
public:
	static constexpr double PEAK_DENSITY() { return 199.2f; }
	static constexpr double PERIPHERY_DENSITY() { return 3.6f; }
	static constexpr double MEAN_DENSITY_CONSTANT() { return 15; } //Ignore these for now

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
	//void setInputs(int lightIntensity, int lightFrequency, int lightAngle) {
		//TODO: Actually do something with this
	//}

	virtual void update(int ticks) override{
		double num = 1.0 / ((photonsHit/200.0)+1.0); //Arbitrary Numbers; needs real data
		num = 1.0 - num;
		cellPotential = num*(cellMax - cellMin) + cellMin;
	}

	virtual const char getType() override {
		// ALLOWS for visual display of whether it is a rod or a cone being generated
		return 'C';
	}
};
class Rod : public Photoreceptor {
public:
	virtual const char getType() override {
		// ALLOWS for visual display of whether it is a rod or a cone being generated
		return 'R';
	}

	virtual const char getRGB() override {
		return ' ';
	}
	
    //void setInputs(int lightIntensity, int lightFrequency, int lightAngle) {
		//TODO: Actually do something with this
	//}

	virtual void update(int ticks) override{
		double num = 1.0 / ((photonsHit/25.0) + 1.0); //Arbitrary Numbers; needs real data
		num = 1.0 - num;
		cellPotential = num*(cellMax - cellMin) + cellMin;
	}
};

class Bipolar : public Neuron {

public:
	Bipolar() : Neuron(-40 /*Not sure what this number is for the bipolar cells*/, -65, true) {  }

};

class Amacrine : public Neuron {

public:
	Amacrine() {}
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
	srand(std::time(0));
	rand();

	const int rows = 12;
	const int cols = 12;
	const int bipolarRange = 3; // I've arbitrarily made the receptive fields a 3x3 square for the purposes of this test


	//std::vector<Photoreceptor> receptors; // will be implemented in the future
	//std::vector<Bipolar> bipolars; 
	//std::vector<Ganglion> ganglia;


	Photoreceptor* x[rows][cols] = {};// call a method which returns a list filled in with all the inputs;
	Bipolar* y[rows / bipolarRange][cols / bipolarRange] = {}; 

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			x[i][j] = builder(i + j);
		}
	}

	std::cout << "Display of Rod and Cone Generator : \n\n";
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			std::cout << x[i][j]->getType();
		}
		std::cout << '\n';
	}

	std::cout << "\n\nDisplay the type of Cone that is created : \n\n";

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			std::cout << x[i][j]->getRGB();
		}
		std::cout << '\n';
	}

	//Low Light
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			x[i][j]->setInputs(25,600,50);
			x[i][j]->update(0);
		}
	}

	for (int i = 0; i < rows / bipolarRange; i++) {
		for (int j = 0; j < cols / bipolarRange; j++) {
			y[i][j] = new Bipolar();
			for (int k = -(bipolarRange / 2); k < (bipolarRange / 2) + 1; k++) {
				for (int m = -(bipolarRange / 2); m < (int)(bipolarRange / 2) + 1; m++) {
					if (m != 0 || k != 0)
						y[i][j]->addInputCellSurround(x[i*bipolarRange + (bipolarRange / 2) - k][j*bipolarRange + (bipolarRange / 2) - m]);
					else
						y[i][j]->addInputCellCenter(x[i*bipolarRange + (bipolarRange / 2) - k][j*bipolarRange + (bipolarRange / 2) - m]);
				}
			}
		}
	}

	std::cout << "\n\nLow Light Light:\n\n";
	std::cout << "\nRod Potentials: \n\n";
	for (int i = 0; i < rows; ++i) {
		for (int j = i%2; j < cols; j+=2) {
			std::cout << "[" << x[i][j]->getPotential() << "] ";
		}
		std::cout << '\n';
	}

	std::cout << "\nCone Potentials: \n\n";
	for (int i = 0; i < rows; ++i) {
		for (int j = (i+1)%2; j < cols; j+=2) {
			std::cout << "[" << x[i][j]->getPotential() << "] ";
		}
		std::cout << '\n';
	}
	
	std::cout << "\nBipolar Potentials: \n\n";
	for (int i = 0; i < rows / bipolarRange; i++) {
		for (int j = 0; j < cols / bipolarRange; j++) {
			y[i][j]->update(0);
			std::cout << "[" << y[i][j]->getPotential() << "] ";
		}
		std::cout << '\n';
	}

	std::cout << "\n\nHeavy Light:\n\n";
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			x[i][j]->setInputs(400, 600, 50);
			x[i][j]->update(0);
		}
	}

	std::cout << "\nRod Potentials: \n\n";
	for (int i = 0; i < rows; ++i) {
		for (int j = i % 2; j < cols; j += 2) {
			std::cout << "[" << x[i][j]->getPotential() << "] ";
		}
		std::cout << '\n';
	}

	std::cout << "\nCone Potentials: \n\n";
	for (int i = 0; i < rows; ++i) {
		for (int j = (i + 1) % 2; j < cols; j += 2) {
			std::cout << "[" << x[i][j]->getPotential() << "] ";
		}
		std::cout << '\n';
	}

	std::cout << "\nBipolar Potentials: \n\n";
	for (int i = 0; i < rows / bipolarRange; i++) {
		for (int j = 0; j < cols / bipolarRange; j++) {
			y[i][j]->update(0);
			std::cout << "[" << y[i][j]->getPotential() << "] ";
		}
		std::cout << '\n';
	}

	int xy;
	std::cin >> xy;

	return 0;
}
