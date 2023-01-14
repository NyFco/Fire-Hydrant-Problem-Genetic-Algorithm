// Armita As'hab Yamin, Yazdan Vahidi Sha'rbaaf

#include <iostream>
#include <stdlib.h>
#include <time.h>

#define SIZE 10 //Size of the population
#define R 6 //The amount of chromosomes which are going to participate in crossover (R/SIZE), and ((SIZE - R)/SIZE) for selection
#define M 2 //The mutation rate (M/SIZE)

using namespace std;

int map[10][10] = { // (X, Y)
	{0,4,1,1,7,4,1,4,5,5},
	{6,5,5,4,5,7,7,1,5,2}, //bridge (1, 5)
	{2,9,2,0,8,4,1,2,3,4},
	{8,6,1,6,2,9,6,1,4,8},
	{7,3,2,8,5,9,9,3,4,9},
	{1,9,8,4,2,8,3,8,6,0},
	{2,7,3,0,3,6,1,7,4,3}, // bridge (6, 5)
	{1,6,3,1,9,5,9,8,1,3},
	{5,5,6,2,8,4,6,9,9,8},
	{3,10,2,1,2,2,9,1,1,7}
};

class Chromosome {
	public:
		//Constructors
		Chromosome(int x, int y){
			setX(x);
			setY(y);
		}
		Chromosome(){}
		
		//Setter functions
		void setX(int x){
			x_axis = x;
		}
		
		void setY(int y){
			y_axis = y;
		}
		
		void setFitness(int f){
			fitness = f;
		}
		
		//Getter functions
		int getX(){
			return x_axis;
		}
		
		int getY(){
			return y_axis;
		}
		
		int getFitness(){
			return fitness;
		}
		
		//Operators = and == overloading
		Chromosome operator =(Chromosome c){
    	    x_axis = c.getX();
    	    y_axis = c.getY();
			fitness = c.getFitness();
        	return *this;
    	}
    	
    	bool operator ==(Chromosome c){
    		if (x_axis == c.getX() && y_axis == c.getY()) return true;
    		return false;
		}

	private:
		int x_axis;
		int y_axis;
		int fitness;
};

//Returns minimum of integers a and b
int min(int a, int b){
	if(a > b) return b;
	else return a;
}

//Calculates direct distance between c1 and c2
int directDistance(Chromosome c1, Chromosome c2){
		int x = c1.getX() - c2.getX();
		int y = c1.getY() - c2.getY();
		if(x<0)x = -x;
		if(y<0)y = -y;
		return x+y;
	}

//Calculates total distance between c1 and c2
int distanceCalc(Chromosome c1, Chromosome c2){
	
	Chromosome bridge1(1, 5), bridge2(6, 5); //Bridges
	
	if((c1.getY() < 6 && c2.getY() > 5) || (c1.getY() > 5 && c2.getY() < 6)){ //If the river is between c1 and c2
		//Calculates and returns the minimum distance between c1 and c2 passing through the best bridge
		return min(directDistance(c1, bridge1) + directDistance(bridge1, c2), directDistance(c1, bridge2) + directDistance(bridge2, c2));
	}
	
	return directDistance(c1, c2);
}

//Calculates the fitness of chromosome c using the following formula:
/*
	Fitness: sum of (distance between c and each location L from map / fire rate of location L)
	Note that the smaller fitness is the better
*/
int fitnessCalc(Chromosome c){
	int sum = 0;
	
	for(int i=0; i<10; i++){
		for(int j=0; j<10; j++){
			Chromosome temp(i, j);
			int fitness;
			if(map[i][j] == 0) { //In order not to have a vague fraction ( Z/0 ), if the fire rate equals 0 we will pass 0 as an exception
				fitness = 0;
				continue;
			}
			fitness = distanceCalc(c, temp) / map[i][j];
			sum += fitness;
		}
	}
	
	return sum;
}

class Population {
	public:
		//Initialization of (SIZE) random chromosomes
		void setRandomChromosomes(){
			for(int i=0; i<SIZE; i++){
				chromosomeSet[i].setX(rand()%10);
				chromosomeSet[i].setY(rand()%10);
			}
		}
		
		void setPopulation(Chromosome cSet[]){
			for(int i=0; i<SIZE; i++){
				chromosomeSet[i] = cSet[i];
			}
		}
		
		void totalFitnessCalc(){
			int sum = 0;
			for(int i=0; i<SIZE; i++){
				int fitness = fitnessCalc(chromosomeSet[i]);
				sum += fitness;
				chromosomeSet[i].setFitness(fitness);
			}
			totalFitness = sum;
		}
		
		//Calculates and Returns the index of the fittest chromosome from the chromosomeSet
		int getFittestIdx(){
			int idx;
			for(int i=0; i<SIZE; i++){
				if (chromosomeSet[i].getFitness() < chromosomeSet[idx].getFitness()) idx = i;
			}
			return idx;
		}
		
		void mutation(int idx){
			int axis = rand()%2;//Generating a random number between 0 and 1 to select between x or y axis.(0 for X_Axis and 1 for Y_Axis)
			if(axis == 0) chromosomeSet[idx].setX(rand()%10);//Set a random X axis for the chosen chromosome
			else chromosomeSet[idx].setY(rand()%10);//Set a random Y axis for the chosen chromosome
		}
		
		//Selects and returns a chromosome randomly with probabilty
		//Note that the chromosomes with less fitness have more chance to be chosen
		Chromosome probabilisticallySelection(){
			int differ[SIZE]; //Difference between chromosomes' fitnesses and totalFitness
			for(int i=0; i<SIZE; i++){
				differ[i] = totalFitness - chromosomeSet[i].getFitness();
			}
			int differSum = 0; //Sum of the differences
			for(int i=0; i<SIZE; i++){
				differSum += differ[i];
			}
			int randomNum = rand()%differSum; // A random number from 0 to differSum will be generated
			int idx; // The index of the randomly selected chromosome
			/* The calculated differ for each chromosome will be subtracted from the random number one by one
			and when it's less than 0, that chromosome will be chosen */
			for(int i=0; i<SIZE; i++){
				randomNum -= differ[i];
				if(randomNum < 0){
					idx = i;
					break;
				}
			}
			return chromosomeSet[idx];
		}
		
		//Overloading of operator =
		Population operator =(Population p){
    	    setPopulation(p.chromosomeSet);
        	return *this;
    	}
    	
    	//Prints the chromosomes, their axis and their fitnesses
    	void printChromosomes(int generation){
    		cout << "Generation: " << generation << endl;
    		for(int i=0; i<SIZE; i++){
    			cout << "Chromosome " << i << ". X: " << chromosomeSet[i].getX() << ", Y: " << chromosomeSet[i].getY();
    			cout << ", Fitness: " << chromosomeSet[i].getFitness() << endl;
			}
			cout << "----------------------" << endl;
			int fittest = getFittestIdx();
			cout << "The fittest: Chromosome " << fittest << ". X: " << chromosomeSet[fittest].getX() << ", Y: " << chromosomeSet[fittest].getY();
    		cout << ", Fitness: " << chromosomeSet[fittest].getFitness() << endl;
    		cout << "----------------------" << endl;
		}
		
		//Prints map
		void printMap(){
			for(int i=9; i>=0; i--){
				cout << " | ";
				if(i == 5) { //River
						for(int k=0; k<10; k++){
							if(k == 1 || k == 6) cout << "!"; //Bridges
							else cout << "~";
							cout << " | ";
						}
						cout << endl << "-------------------------------------------" << endl << " | ";
					}
				for(int j=0; j<10; j++){
					int idx = -1;
					for(int k=0; k<SIZE; k++){
						if(chromosomeSet[k].getX() == j && chromosomeSet[k].getY() == i) idx = k; 
					}
					if(idx > -1) cout << "#"; //Shows the chosen Chromosomes
					else cout << map[j][i];
					cout << " | ";
				}
				cout << endl << "-------------------------------------------" << endl;
		}
}
			
	private:
		Chromosome chromosomeSet[SIZE];
		int totalFitness;
		int fitnessesProduct;
};

void crossoverFunc(Chromosome &c1, Chromosome &c2){
	Chromosome temp = c1;
	c1.setX(c2.getX());
	c2.setX(temp.getX());
}

int main() {
	srand(time(0)); //Set the seed of the rand function.
	
	Population p; //Initialize the population
	
	p.setRandomChromosomes(); //Generate (SIZE) random chromosomes
	p.totalFitnessCalc(); //Compute the fitness of all chromosomes
	
	//Program control
		p.printMap(); // Print the Map
		p.printChromosomes(1); // Print the generation chromosomes
		
		char controlX;
		cout << "To generate an other generation enter (Y), and to finish the program enter (N): ";
		cin >> controlX;
		if(controlX == 'N' || controlX == 'n') return 0;
	
	for(int k=2; k>0; k++){//Infinite loop which will be terminated when the user wants
		system("cls"); //Clear the console
		
		Population ps; //Initialize the next population
		Chromosome select[SIZE - R]; //Initialize an array of chromosomes for selection phase
		Chromosome crossover[R]; //Initialize an array of chromosomes for crossover phase
		
		//1) Selection phase
		for(int i=0; i<SIZE - R; i++){ //Selecting SIZE-R chromosomes for Selection
			bool repetitious = true;
			Chromosome temp;
			while(repetitious) {
				repetitious = false;
				temp = p.probabilisticallySelection();
				for(int j=0; j<i; j++){ //Checking that the chosen chromosome is not repetitious
					if (temp == select[i]) repetitious = true;
				}
			}
			select[i] = temp;
		}
		
		//2) Crossover phase
		for(int i=0; i<R; i++){ //Selecting R chromosomes for Crossover
			bool repetitious = true;
			Chromosome temp;
			while(repetitious) {
				repetitious = false;
				temp = p.probabilisticallySelection();
				for(int j=0; j<i; j++){ //Checking that the chosen chromosome is not repetitious
					if (temp == crossover[i]) repetitious = true;
				}
			}
			crossover[i] = temp;
		}
		for(int i=0; i<R; i+=2){
			crossoverFunc(crossover[i], crossover[i+1]); //Generate the offsprings
		}
		
		//Set the next population: (Selected chromosomes from the Selection phase + Crossover phase)
		Chromosome temp[SIZE];
		int counter = 0;
		for(int i=0; i<SIZE; i++){
			if(i < SIZE - R) temp[i] = select[i];
			else {
				temp[i] = crossover[counter];
				counter++;
			}
		}
		ps.setPopulation(temp);
		
		//3) Mutation phase
		int mutatedIdx = -1;
		for(int i=0; i<M; i++){
			bool repetitious = true;
			while(repetitious){
				repetitious = false;
				int idx = rand()%SIZE; //Generate a random number between 0 and SIZE to select the chromosome for mutation.
				if(idx == mutatedIdx){
					repetitious = true; //Check if the selected chromosome for mutation has already been mutated
					continue;
				}
				ps.mutation(idx);
				mutatedIdx = idx;
			}
		}
		
		//4) Update the generation
		p = ps;
		p.totalFitnessCalc(); //Compute fitness of all chromosomes
		
		//Program control
		p.printMap(); // Print the Map
		p.printChromosomes(k); // Print the generation chromosomes
		
		char control;
		cout << "To generate an other generation enter (Y), and to finish the program enter (N): ";
		cin >> control;
		if(control == 'N' || control == 'n') break;
	}

	return 0;
}