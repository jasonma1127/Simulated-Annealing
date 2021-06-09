#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <time.h>
#include <random>

using namespace std;
vector<vector<int>> data;
vector<vector<int>> distanceVec;
int capacity = 50;

vector<int> calculatePathSum(vector<vector<int>> _Scurrent){
	vector<vector<int>> Scurrent;
	vector<int> shortestPathVec;
	Scurrent = _Scurrent;
	
	for(int i = 0; i < Scurrent.size(); i ++){
		int pathSum = 0;
		for(int j = 0; j < Scurrent[i].size() - 1; j ++){
			pathSum += distanceVec[Scurrent[i][j] - 1][Scurrent[i][j + 1] - 1];
		}
		shortestPathVec.push_back(pathSum);
	}
	return shortestPathVec;
}

vector<vector<int>> randomCreateSolution(){
	vector<int> randomSolution;
	vector<vector<int>> randomSolutionVec;
	
	//create a initial solution
	for(int i = 2; i <= data.size(); i ++){
        randomSolution.push_back(i);
    }
    
    //shuffle the initial solution
    random_shuffle(randomSolution.begin(), randomSolution.end());
    
    bool flag = true;
	randomSolutionVec.push_back(randomSolution);
    while(flag){
    	int i = 0;
    	while(i <= randomSolutionVec.size()){
    		//if is the end of the randomSolutionVec then stop
    		if(i == randomSolutionVec.size()){
    			flag = false;
    			break;
			} else {
				
				//caculate the sum of the demand
				int sum = 0;
				for(int j = 0; j < randomSolutionVec[i].size(); j++){
					sum += data[randomSolutionVec[i][j] - 1][3];
				}
				
				if(sum > capacity) {
					vector<int> temp;
					randomSolutionVec.push_back(temp);
					
					//split the route in half
					int mid = (randomSolutionVec[i].size()) / 2;
					for(int j = mid; j < randomSolutionVec[i].size(); j++){
						randomSolutionVec[randomSolutionVec.size()-1].push_back(randomSolutionVec[i][j]);
					}
					randomSolutionVec[i].erase(randomSolutionVec[i].begin() + mid, randomSolutionVec[i].end());
					i = i;
					
				} else {
					i ++;
				}
			}
		}
	}
	
	for(int i = 0; i < randomSolutionVec.size(); i ++){
		randomSolutionVec[i].insert(randomSolutionVec[i].begin(), 1);
		randomSolutionVec[i].push_back(1);
	}
	
	return randomSolutionVec;    
}

void simulatedAnnealing(){
	//Setting section
	int Tinitial = 10000;
	int Tfinal = 0;
	float alpha = 0.85;
	vector<vector<int>> Scurrent;
	vector<vector<int>> Sbest;
	vector<int> shortestPathVec;
	int shortestPath = 0;
	int shortestPathOpt;
	int iter_max = 250;
	
	//create a random solution for current solution
	Scurrent = randomCreateSolution();
	Sbest = Scurrent;
	
	cout << "Initial Scurrent======" << endl;
	for(int i = 0; i < Scurrent.size(); i ++){
		for(int j = 0; j < Scurrent[i].size(); j ++){
			cout << Scurrent[i][j] << " ";
		}
		cout << endl;
	}
	
	//caculate the path sum of the current solution
	shortestPathVec = calculatePathSum(Scurrent);
	
	for(int i = 0; i < shortestPathVec.size(); i ++){
		shortestPath += shortestPathVec[i];
	}
	cout << "shortestPath : " << shortestPath << endl;
	cout << endl;
	
	shortestPathOpt = shortestPath;
	
	//Simulated annealig section
	
	while(Tinitial > Tfinal){
		for(int iter = 0; iter < iter_max; iter ++){
			
			//create a random solution for new solution
			vector<vector<int>> Snew = randomCreateSolution();
			
			//caculate the path sum of the new solution
			shortestPathVec = calculatePathSum(Snew);
			
			int newShortestPath = 0;
			for(int i = 0; i < shortestPathVec.size(); i ++){
				newShortestPath += shortestPathVec[i];
			}
			
			//caculation the difference of two solution path sum
			int difference = newShortestPath - shortestPath;
			
			if(difference < 0) {
				Scurrent = Snew;
				shortestPath = newShortestPath;
				
				if(newShortestPath < shortestPathOpt){
					shortestPathOpt = newShortestPath;
					Sbest = Snew;
				}
				
			} else {
				double R = (double) rand() / (RAND_MAX + 1.0);
	            if(R < exp(-(difference) / Tinitial)){
	                Scurrent = Snew;
					shortestPath = newShortestPath;
	            }
			}		
		}
		
		Tinitial = Tinitial * alpha;
		
		cout << "Sbest=================" << endl;
		for(int i = 0; i < Sbest.size(); i ++){
			for(int j = 0; j < Sbest[i].size(); j ++){
				cout << Sbest[i][j] << " ";
			}
			cout << endl;
		}
		cout << "shortestPathOpt => "<< shortestPathOpt << endl;
		cout << endl;
	}
}

int main(){
	ifstream dataFile;
	dataFile.open("Solomon's_instance_1_depot_10_customers.txt");
	
	//read file
	if(dataFile.is_open()){
		cout << "File open successed" << endl;
		
		string line;
		int dataIndex = 0;
		while(getline(dataFile, line)){
			float num;
			istringstream istr(line);
			vector<int> temp;
			data.push_back(temp);
			
			while(istr >> num){
				data[dataIndex].push_back(num);
			}
			dataIndex ++;
		}
	} else {
		cout << "File open failed" << endl;
	}
	
	//caculate distance between the customer
	for(int i = 0; i < data.size(); i ++){
		vector<int> temp;
		distanceVec.push_back(temp);
		for(int j = 0; j < data.size(); j ++){
			distanceVec[i].push_back(sqrt(pow(data[i][1] - data[j][1], 2) + pow(data[i][2] - data[j][2], 2)));
		}
	}
	
	//execute Simulated Annealing
	simulatedAnnealing();

} 
