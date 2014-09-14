// TSPGeneticAlgorithm.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <vector>
#include <time.h>
#include <list>
#include <string>
#include <iostream>
#include <algorithm>
#include <random>
#include <math.h>
#include <stdexcept>


const int size = 200;
const int cityNum = 16; //city number is cityNum +1  since 0 counts as an int
const int cityNum_=cityNum-1;
const int maxDis =1000;
const double mutationRate = 0.1*100;
const double elitePercent = size/5;
double hlfsize = cityNum/2;
int halfpt = std::floor(hlfsize);

struct city {
	int x;
	int y;
};

struct individual{
	std::vector<int> chromosome;
	int distance;
	bool elite;
	void calcDistance(std::vector<city> & cities);
	int findNonSwath(individual a,individual b, int pos);
	int checkRow(individual a,individual b,int startPos, int pos, int recursiveCount);
	void PMXCrossover(individual a,individual b);
};

struct sortDis{
	bool operator() (const individual& x, const individual& y) const {return x.distance<y.distance;};
};

std::vector<individual> GenPopulation();
std::vector<city> GenCities();
int cityDis(city city1,city city2);
int getIntLen (int value);
void spawnChild(std::vector<individual> & pop);
void mutation(std::vector<individual> &pop);
individual runAlgorithm();

int main(){
	std::vector<individual> elite;
	elite.resize(10);
	int i=0;
	while (i<10){
		individual candidate = runAlgorithm(); 
		if (candidate.distance < 320){
			elite[i] = candidate;
			i++;
		}
		std::cout << i << std::endl;
	}
	std::sort(elite.begin(),elite.end(),sortDis());
	for (int i=0;i<10;i++){
		std::cout << elite[i].distance << std::endl;
	}
	std::cout << "hi";
	std::cin.ignore(255);
	std::cin.get();
	return 0;
}

individual runAlgorithm()
{
	//sets the random seed to a time(0)
	srand(time(NULL));
	//initializes the vectors of the population and cities

	std::vector<individual> pop = GenPopulation();
	//std::vector<city> cities = GenCities();
	std::vector<city> cities;
	cities.resize(cityNum);
	for (int i=0;i<cityNum;i++){ 
		switch (i){
		case 0:
			cities[i].x=44;
			cities[i].y=79;
			break;
		case 1:
			cities[i].x=56;
			cities[i].y=54;
			break;
		case 2:
			cities[i].x=74;
			cities[i].y=42;
			break;
		case 3:
			cities[i].x=76;
			cities[i].y=80;
			break;
		case 4:
			cities[i].x=53;
			cities[i].y=4;
			break;
		case 5:
			cities[i].x=6;
			cities[i].y=54;
			break;
		case 6:
			cities[i].x=36;
			cities[i].y=99;
			break;
		case 7:
			cities[i].x=88;
			cities[i].y=13;
			break;
		case 8:
			cities[i].x=83;
			cities[i].y=16;
			break;
		case 9:
			cities[i].x=81;
			cities[i].y=23;
			break;
		case 10:
			cities[i].x=1;
			cities[i].y=22;
			break;
		case 11:
			cities[i].x=32;
			cities[i].y=50;
			break;
		case 12:
			cities[i].x=95;
			cities[i].y=40;
			break;
		case 13:
			cities[i].x=38;
			cities[i].y=99;
		case 14:
			cities[i].x=75;
			cities[i].y=35;
			break;
		case 15:
			cities[i].x=72;
			cities[i].y=92;
			break;
		}
	}
	for (int k=0;k<25;k++){
		for (int i=0;i<size;i++){
			pop[i].calcDistance(cities);
		}
		std::sort(pop.begin(),pop.end(),sortDis());


		for (int i=0;i<elitePercent;i++){
			pop[i].elite=true;
		}

		spawnChild(pop);
		mutation(pop);
		std::sort(pop.begin(),pop.end(),sortDis());
	}
	return pop[0];
}

std::vector<individual> GenPopulation(){
	std::vector<individual> pop;
	//sizes the population to the given size
	pop.resize(size); 
	for (int i=0;i<size;i++){
		for (int j=0;j<cityNum;j++){
			pop[i].chromosome.resize(cityNum);
			pop[i].chromosome[j]=j;
		}	
	}

	for (int i=0;i<size;i++){
		//resizes individual list to city number
		//shuffles the list
		std::random_shuffle(pop[i].chromosome.begin(),pop[i].chromosome.end());
	}

	return pop;
}

std::vector<city> GenCities(){
	std::vector<city> cities;
	cities.resize(cityNum);

	 //in the city struct, each city is given a randomized distance from each other
	for (int i=0;i<cityNum;i++){ 
				cities[i].x=rand()%maxDis;
				cities[i].y=rand()%maxDis;

		}
	for (int i=0;i<cityNum;i++){
		for (int j=0;j<cityNum;j++){
			if ((cities[i].x==cities[j].x)&&(cities[i].y==cities[j].y)){	//checks for any identical cities
				cities[j].x=maxDis+i*j*rand()%100;	//increases one point away by maxDis and y
			}
		}
	}
	return cities;
}

void spawnChild(std::vector<individual> &pop){
	bool addChild= true;
	for (int i=elitePercent;i<size;i++){
		pop[i].chromosome.resize(0);		//resets all non-elite chromosome
	}
	int child = std::floor(elitePercent);
	while (addChild){
		//chooses 2 random parents 
		int i=rand()%child;
		int j=rand()%child;
		if (i!=j){ 
			//runs the PMX crossover algorithm
			pop[child].PMXCrossover(pop[i],pop[j]);
			if (child!=size-1){
				pop[child+1].PMXCrossover(pop[j],pop[i]);
				child+=1;
			}
			child+=1;
		}

		//for (int k=0;k<cityNum;k++){
		//		std::cout<< pop[child].chromosome[k];
		//}
		std::cout<<std::endl;
		if (child == size){
			addChild=false;
		}
	}


}

void individual::calcDistance(std::vector<city> & cities){
		for (int i=0;i<size;i++){
			int totalDistance=0;
			for (int j=0;j<cityNum_;j++){
				int x=chromosome[j];
				int y=chromosome[j+1];
				totalDistance= totalDistance + cityDis(cities[x],cities[y]);
				if (j==cityNum-2){
					distance = totalDistance;
					elite=false;
				}
			}
		}
}

void individual::PMXCrossover(individual p1,individual p2){
	//resizes child to size
	chromosome.resize(cityNum);
	//PMX crossover
		//selects a random pos up to the point where adding halfpt would not cause an subscript error
		int  randPos = rand()%((cityNum-halfpt)+1);
		//sets child to parent i from random swatch
		for (int i=0;i<chromosome.size();i++){
			if ((i>=randPos)&&(i<randPos+halfpt)){
				//sets child to values in random swath in parent i
				chromosome[i]=p1.chromosome[i];
			}
			else{
				//sets everything else as unknown
				chromosome[i]=-1;
			}
		}
		//finds numbers in p2 in the random swatch that are not in p1
		findNonSwath(p1,p2, randPos);

		for (int i=0;i<cityNum;i++){
			//finds flagged unkown numbers and inserts p2 numbers at that point
			if (chromosome[i]==-1){
				chromosome[i]=p2.chromosome[i];
			}
		}
}
int individual::findNonSwath(individual p1, individual p2 , int pos){
	//creates an iterator to find the position of values
	std::vector<int>::iterator it_pos;
	std::vector<int>::iterator swathStart= p1.chromosome.begin()+pos;
	std::vector<int>::iterator swathEnd =  p1.chromosome.begin()+pos+halfpt-1;
	//creates start and end points of the random swath


	for (int i=pos;i<halfpt+pos;i++){
		//runs through p1's random swath to see if any matches to p2 swath exist
		it_pos= std::find(swathStart,swathEnd,p2.chromosome[i]);
		//with no matches, it inserts p2[i] into p1[i] to find p1[i]
		//as it cycles through it recursively finds the position of the value p1[i] in p2, until a position is not in the swath and then returns it
		if ((it_pos==swathEnd)&&(*it_pos!=p2.chromosome[i])){
			int x =checkRow(p1,p2,pos,i,0);
			chromosome[x]=p2.chromosome[i];
		}
	}
	return 0;
}

int individual::checkRow(individual p1,individual p2,int startPos, int pos,int recursiveCount){
	int a = p1.chromosome[pos];
	int pos1;
	//creates an iterator where p1.chromosome begins
	std::vector<int>::iterator x=p2.chromosome.begin();	

	recursiveCount +=1;
	//marks start and end of random swath in p2
	std::vector<int>::iterator swathStart= p2.chromosome.begin()+startPos;
	std::vector<int>::iterator swathEnd=p2.chromosome.begin()+startPos+halfpt-1;

	x= std::find(swathStart,swathEnd,a);
	//recursively tries to find a non-swath value
	int y=x-p2.chromosome.begin();
	//checks to see if the pos is in the random swath
	if ((x==swathEnd)&&(a!=*swathEnd)){
		x= std::find(p2.chromosome.begin(),p2.chromosome.end(),a);
		y=x-p2.chromosome.begin();
		if (chromosome[y]==-1){
			return y;
		}
		else{
			pos1 = checkRow(p1,p2,startPos,y,recursiveCount);
			return pos1;
		}
	}
	else{
		//runs through p2's random swath to see if any matches to p1 swath exist
		pos1 = checkRow(p1,p2,startPos,y,recursiveCount);
		return pos1;
	}
};
int cityDis(city city1,city city2){
	double xDis=std::abs(city1.x-city2.x);
	double yDis=std::abs(city1.y-city2.y);
	int cityDis = std::floor(std::sqrt((xDis*xDis) +(yDis*yDis)));
	return cityDis;
}




void mutation(std::vector<individual> &pop){
	for (int i=10;i<size;i++){
		double mutChance = rand()%100;
		if (mutationRate>=mutChance){
			int a = rand()%cityNum;
			int b = rand()%cityNum;
			int x= pop[i].chromosome[a];
			pop[i].chromosome[a] = pop[i].chromosome[b];
			pop[i].chromosome[b] =x;
		}
	}
}