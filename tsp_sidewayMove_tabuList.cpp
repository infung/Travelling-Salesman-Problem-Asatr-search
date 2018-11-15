/*
    Please use the instances from instances.zip to test, since I pass an index as 
    the first input to my program instead of the number of citie.
*/

#include <iostream>
#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <string>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <climits>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <chrono>
using namespace std;

float cities36 = 464;

/*float cities14[] = {322.33, 316.67, 324.31, 336.99, 319.52, 351.22, 311.11, 271.63, 358.91, 273.09};
float cities15[] = {326.73, 315.08, 316.14, 280.76, 326.24, 377.93, 291.90, 350.01, 343.40, 355.11};
float cities16[] = {330.51, 404.46, 354.56, 360.20, 347.44, 357.18, 346.52, 373.30, 356.73, 243.98};
*/

float cities14[] = {322.326, 316.677, 324.308, 336.995, 319.528, 351.219, 311.107, 271.633, 358.914, 273.093};
float cities15[] = {326.729, 315.076, 316.137, 280.760, 326.244, 377.928, 291.896, 350.009, 343.399, 355.105};
float cities16[] = {330.505, 404.458, 354.556, 360.202, 347.436, 357.177, 346.519, 373.299, 356.725, 243.975};

//returns the Euclidean distance between two cities
float calculateDistance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}

//tsp info
class Tsp {
public:
    
    int numCities;
    std::vector<std::string> cities;
    std::vector<vector<int> > nodes;
    std::vector<vector<float> > graph;

    std::vector<vector<int>> tabulist;

    Tsp(int num): numCities(num), cities(num), nodes(num, std::vector<int>(2)),
    graph(num, std::vector<float>(num)) {}

    bool isNeighbourInTabulist(std::vector<int> Neighbour) {
        if(tabulist.size() == 0) return false;
        bool exist = false;
        for(int i = 0; i < tabulist.size(); i++) {
            exist = false;
            for(int j = 0; j < Neighbour.size(); j++) {
                if(tabulist[i][j] == Neighbour[j]) {
                    exist = true;
                    if(j == Neighbour.size() - 1) break;
                } else {
                    exist = false;
                    break;
                }
            }
        }
        return exist;
    }
    
    void addToTabulist(std::vector<int> newPath) {
        int newSize = tabulist.size() + 1;
        tabulist.resize(newSize);
        for(int i = 0; i < newPath.size(); i++) {
            tabulist[newSize - 1].push_back(newPath[i]);
        }
    }

    void removeOldestFromTabulist() {
        if(tabulist.size() == 0) return;
        tabulist[0].clear();
        tabulist.erase(tabulist.begin());
    }
    
    void resetTabulist() {
        //reset tabulist
        for(int k =0; k < tabulist.size(); k++) {
            tabulist[k].clear();
        }
        tabulist.clear();
    }

    void readData() {
        string cityID;
        int x, y;
        
        for(int i = 0; i < numCities; i++) {
            cin >> cityID >> x >> y;
            cities[i] = cityID;
            nodes[i][0] = x;
            nodes[i][1] = y;
        }
        
        for(int k = 0; k < numCities; k++) {
            graph[k][k] = 0;
            for(int j = k + 1; j < numCities; j++) {
                graph[k][j] = calculateDistance(nodes[k][0], nodes[k][1], nodes[j][0], nodes[j][1]);
                graph[j][k] = graph[k][j];
            }
        }
    }
    
    vector<int> randomRoute() {
        vector<int> path;
        
        for(int i = 0; i < numCities; i++) {
            path.push_back(i);
        }
        auto rng = std::default_random_engine {};
        rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
        std::shuffle(path.begin() + 1, path.end(), rng);
        path.push_back(0);
        return path;
    }
    
    float calTotalCost(vector<int> &path) {
        if(path.empty()) return 0;
        int start = path[0];
        float cost = graph[start][path.back()];
        int pathsize = path.size();
        for (int i = 1; i < pathsize; i++) {
            //cout << start << "->" << path[i] << ": " << graph[start][path[i]] << " total cost: " << cost << endl;
            cost += graph[start][path[i]];
            start = path[i];
        }
        return cost;
    }
    
    //Neighbour Relation
    vector<int> swapTwoCitiesReconnect(vector<int> &path, int c1, int c2) {
        vector<int> newPath;
        
        for(int i = 0; i < c1; i++) {
            newPath.push_back(path[i]);
        }
        for(int j = c1, k = c2; j <= c2; j++, k--) {
            newPath.push_back(path[k]);
        }
        for(int l = c2+1; l <= numCities; l++) {
            newPath.push_back(path[l]);
        }
        
        return newPath;
    }
    
    vector<int> getBestNeighbour(vector<int> &path) {
        vector<int> sofarPath;
        vector<int> bestPath;
        bestPath = swapTwoCitiesReconnect(path, 1, 2);
        float sofarCost;
        float bestCost = calTotalCost(bestPath);
        
        for(int i = 1; i < numCities; i++) {
            for(int j = 1; j < numCities; j++) {
                if(i >= j || (i== 1 && j == numCities -1)) continue;
                sofarPath = swapTwoCitiesReconnect(path, i, j);
                sofarCost = calTotalCost(sofarPath);
                
                if(sofarCost < bestCost) {
                    bestPath = sofarPath;
                    bestCost = sofarCost;
                }
            }
        }
        
        return bestPath;
    }
    
    vector<int> getBestNeighbour_tabulist(vector<int> &path) {
        vector<int> sofarPath;
        vector<int> bestPath;
        bestPath = swapTwoCitiesReconnect(path, 1, 2);
        float sofarCost;
        float bestCost = calTotalCost(bestPath);
        bool update = false;
        
        for(int i = 1; i < numCities; i++) {
            for(int j = 1; j < numCities; j++) {
                if(i >= j /*|| (i== 1 && j == numCities -1)*/) continue;
                sofarPath = swapTwoCitiesReconnect(path, i, j);
                sofarCost = calTotalCost(sofarPath);
                
                if(sofarCost < bestCost) {
                    if(isNeighbourInTabulist(sofarPath)) {
                        continue;
                    } else {
                        bestPath = sofarPath;
                        bestCost = sofarCost;
                        update = true;
                    }
                }
            }
        }

        if(tabulist.size() <= 100) {
            addToTabulist(path);
        } else {
            removeOldestFromTabulist();
            addToTabulist(path);
        }

        if(update) {
            return bestPath;
        }  
        return path;
    }
    
    float hillClimbing_sidewayMove_tabulist() {
        vector<int> currentPath;
        vector<int> nextPath;
        float currentCost;
        float nextCost;
        
        currentPath = randomRoute();
        currentCost = calTotalCost(currentPath);
        
        int step = 0;//number of neighbours
        int moves = 0; 
        int count = 0;//count total number of sideway moves
        while(true) {
            nextPath = getBestNeighbour_tabulist(currentPath);
            nextCost = calTotalCost(nextPath);
            if(currentCost < nextCost) {
                break;
            } else if(currentCost == nextCost) {
                if(moves < 100) {
                    step++;
                    currentPath = nextPath;
                    currentCost = nextCost;
                    moves++;
                    count++;
                } else {
                    //reset tabulist
                    resetTabulist();
                    break;
                }
            }else {
                step++;
                currentPath = nextPath;
                currentCost = nextCost;
                moves = 0;
            }
            
        }
    }

    float hillClimbing_tabulist() {
        vector<int> currentPath;
        vector<int> nextPath;
        float currentCost;
        float nextCost;
        
        //reset tabulist
        resetTabulist();

        currentPath = randomRoute();
        currentCost = calTotalCost(currentPath);
        
        int step = 0;//number of neighbours
        while(true) {
            nextPath = getBestNeighbour_tabulist(currentPath);
            nextCost = calTotalCost(nextPath);
            if(step > 100) {
                //reset tabulist
                resetTabulist();
            }
            if(currentCost < nextCost || isNeighbourInTabulist(nextPath)) {
                //reset tabulist
                resetTabulist();
                break;
            } else {
                step++;
                currentPath = nextPath;
                currentCost = nextCost;
            }
            
        }
        //cout << "The total cost: " << currentCost << endl;
        //return step;
        return currentCost;
    }
    
    float hillClimbing_sidewayMove() {
        vector<int> currentPath;
        vector<int> nextPath;
        float currentCost;
        float nextCost;
        
        currentPath = randomRoute();
        currentCost = calTotalCost(currentPath);
        
        int step = 0;//number of neighbours
        int moves = 0; 
        int count = 0;//count total number of sideway moves
        while(true) {
            nextPath = getBestNeighbour(currentPath);
            nextCost = calTotalCost(nextPath);
            if(currentCost < nextCost) {
                break;
            } else if(currentCost == nextCost) {
                if(moves < 100) {
                    step++;
                    currentPath = nextPath;
                    currentCost = nextCost;
                    moves++;
                    count++;
                } else {
                    break;
                }
            } else {
                step++;
                currentPath = nextPath;
                currentCost = nextCost;
                moves = 0;
            }
            
        }
        //cout << "The total cost: " << currentCost << endl;
        //return step;
        return currentCost;
        //return count;
    }
};

int main()
{
    clock_t start;
    clock_t finish;
    start = clock();
    int index;
    cin >> index;
    int num;
    cin >> num;
    // cout << "Testing:" << endl;
    Tsp tsp(num);
    tsp.readData();
    /*int total_steps = 0;
    int total = 0;
    float total_cost = 0;
    for(int i = 0; i < 100; i++) {
        double ccost = tsp.hillClimbing_tabulist();
        float quality = 0;
        //total_steps += tsp.hillClimbing_sidewayMove();
        //int cost = (int)(tsp.hillClimbing_sidewayMove() + 0.5);
        //int ans = 0;
    if(num == 14) {
        //ans = (int)(cities14[index] + 0.5);
        quality = ccost/cities14[index];
    }else if(num == 15) {
        //ans = (int)(cities15[index] + 0.5);
        quality = ccost/cities15[index];
    } else {
        //ans = (int)(cities16[index] + 0.5);
        quality = ccost/cities16[index];
    }
        total_cost += quality;
        /*if(cost == ans) {
            total++;
        }
    }*/
    /////////print!!!/////////
   // cout << tsp.hillClimbing_tabulist() << endl;
    //cout << (float)total_cost/100 << endl;
    //cout << total << endl;
    //cout << (float)total_steps/100 << endl;
    /*cout << "The total cost: " << bestSoln << endl;*/
    float best = INT_MAX;
    while(true) {
        float cur = tsp.hillClimbing_sidewayMove();
        if(cur < best) best = cur;
        if(best <= 465) break;
        if(((clock() - start)/CLOCKS_PER_SEC) > 300) break;
    }
    cout << (float)best <<endl;
     finish = clock();
     cout << "The total execution time: " << (double)(finish - start)/CLOCKS_PER_SEC << endl;
    return 0;
}




