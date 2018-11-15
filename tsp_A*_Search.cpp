#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <stdio.h>
#include <queue>
#include <climits>
#include <time.h>

using namespace std;

class Node {
    public:
        int key;
        int parentKey;
        float keyValue;
        bool visited;

        Node() {};
        Node(int k, int pk, float kv, bool v) : key(k), parentKey(pk), keyValue(kv), visited(v) {}
};

class MST {
    public:
        vector<int> visitedNodes;
        vector<int> unvisitedNodes;
        vector<vector<float> > graph;
    MST(vector<int> v, vector<int> unv, vector<vector<float> > g) : visitedNodes(v), unvisitedNodes(unv),
                                                                    graph(g) {}
    float MST_cost(int cur, int nearestCity) {
        std::vector<Node> mySet;

        int size = unvisitedNodes.size();
        for(int i = 0; i < size; i++) {
            if(unvisitedNodes[i] == cur) continue;
            if(unvisitedNodes[i] == nearestCity) {
                Node node(unvisitedNodes[i], -1, 0, true);
                mySet.push_back(node);
            } else {
                Node node(unvisitedNodes[i], nearestCity, graph[nearestCity][unvisitedNodes[i]], false);
                mySet.push_back(node);
            }
        }
        
        int size_mySet = mySet.size();
        while (true) {
            Node n;
            bool any = false;
            int index = 0;
            for(int j = 0; j < size_mySet; j++) {
                if(!mySet[j].visited) {
                    n = mySet[j];
                    index = j;
                    any = true;
                    break;
                }
            }
            if(!any) break;
            
            for(int k = 0; k < size_mySet; k++) {
                if(!mySet[k].visited && mySet[k].keyValue < n.keyValue) {
                    n = mySet[k];
                    index = k;
                }
            }

            n.visited = true;
            mySet[index].visited = true;

            for(int l = 0; l < size_mySet; l++) {
                if(!mySet[l].visited && graph[n.key][mySet[l].key] < mySet[l].keyValue) {
                    mySet[l].parentKey = n.key;
                    mySet[l].keyValue = graph[n.key][mySet[l].key];
                }
            }
        }

        int mst_total_cost = 0;
        for(int m = 0; m < size_mySet; m++) {
            mst_total_cost += mySet[m].keyValue;
        }

        return mst_total_cost;
    }
};

//returns the Euclidean distance between two cities
int calculateDistance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}

//info of one city 
class City {
    public:
    int order;
    string cityID;
    int x;
    int y;
    int parentOrder;
    bool visited;
    
    City() {};
    City(int order, string cityID, int x, int y, int parentOrder, bool visited) :
        order(order), cityID(cityID), x(x), y(y), parentOrder(parentOrder), visited(visited) {}
};

//state info of current city
class cityState {
    public:
        int order;
        std::vector<City> citiesRecord;
        float g_cost;
        float h_cost;
        float f_cost;
        int size_record;
        
        cityState() {};
        cityState(int o, vector<City> CR, float g, float h) : order(o), 
                 citiesRecord(std::move(CR)), g_cost(g), h_cost(h) {
                     f_cost = g_cost + h_cost;
                     size_record = citiesRecord.size();
        }
        
        bool operator < (cityState other) const {
            return this->f_cost > other.f_cost;
        }
        
        bool all_visited() {
            for (int i = 0; i < this->size_record; i++) {
                if(!citiesRecord[i].visited) return false;
            }
            return true;
        }
        
        vector<int> visitedCities() {
            vector<int> ret;
            for (int i = 0; i < this->size_record; i++) {
                if(this->citiesRecord[i].visited) {
                    ret.push_back(this->citiesRecord[i].order);
                }
            }
            return ret;
        }
        
        vector<int> unvisitedCities() {
            vector<int> ret;
            for (int i = 0; i < this->size_record; i++) {
                if(!this->citiesRecord[i].visited) {
                    ret.push_back(this->citiesRecord[i].order);
                }
            }
            return ret;
        }
        
        vector<int> getPath() {
            vector<int> Path;
            City current;
            current = this->citiesRecord[this->order];
            while (true) {
                Path.insert(Path.begin(), current.order);
                if(current.parentOrder == -1) {
                    break;
                } else {
                    current = this->citiesRecord[current.parentOrder];
                }
            }
            Path.push_back(current.order);
            return Path;
        }
        
        float calTotalCost(vector<int> &path, vector<vector<float> > &graph) {
            if (path.empty()) return 0;
            int start = path[0];
            int cost = graph[start][path.back()];
            int pathsize = path.size();
            for (int i = 1; i < pathsize; i++) {
                cost += graph[start][path[i]];
                start = path[i];
            }
            return cost;
        }
};

//tsp info
class Tsp {
    public:
    
    int numCities;
    std::vector<std::string> cities;
    std::vector<vector<int> > nodes;
    std::vector<vector<float> > graph;
    Tsp(int num): numCities(num), cities(num), nodes(num, std::vector<int>(2)),
                  graph(num, std::vector<float>(num)) {}
                  
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
    
    cityState Astar() {
        int startOrder = 0;
        vector<City> record;
        for(int i = 0; i < this->numCities; i++) {
            if(i == startOrder) {
                City currentCity(i, cities[i], nodes[i][0], nodes[i][1], -1, true);
                record.push_back(currentCity);
            } else {
                City currentCity(i, cities[i], nodes[i][0], nodes[i][1], -1, false);
                record.push_back(currentCity);
            }
        }
        
        int hCost = 0;
        int gCost = this->cal_g_cost(record);
        cityState startCity(startOrder, record, gCost, hCost);
        
        //cout << "Testing: city " << cities[startOrder] << ": g(n) = " << startCity.g_cost << ", h(n) = " << startCity.h_cost << ", f(n) = " << startCity.f_cost << endl;

        priority_queue<cityState> astar;
        astar.push(startCity);
        cityState currentState;
        
        int countNode = 1;
        
        while(!astar.empty()) {
            currentState = astar.top();
            astar.pop();
            
            //cout << "Testing: current city is " << cities[currentState.order]  << " with f(n) = " << currentState.f_cost << endl;
            
            if (currentState.all_visited()) break;
            
            vector<int> visited = currentState.visitedCities();
            vector<int> unvisited = currentState.unvisitedCities();
            
            int unvisitedSize = unvisited.size();
            for(int j = 0; j < unvisitedSize; j++) {
                vector<City> new_record(currentState.citiesRecord);
                new_record[unvisited[j]].parentOrder = currentState.order;
                new_record[unvisited[j]].visited = true;

                gCost = currentState.g_cost + graph[currentState.order][unvisited[j]];
                hCost = this->cal_h_cost(startOrder, unvisited[j], visited, unvisited);
                cityState nextState(unvisited[j], new_record, gCost, hCost);
                
                astar.push(nextState);
                countNode++;
            }
        }
        cout << "Total number of Nodes: " << countNode << endl;
        //cout << countNode << endl;
	return currentState;
    }
    
    float cal_g_cost(vector<City> &record) {
        int g_cost = 0;
        int size = record.size();
        for(int i = 0; i < size; i++) {
            if(record[i].parentOrder > -1) {
                g_cost += this->graph[record[i].order][record[i].parentOrder];
            }
        }
        return g_cost;
    }
    
    float nearest_unvisited_city(int curOrder, int city, vector<int> unvisited) {
        if(unvisited.empty()) return 0;
        float nearest_cost = this->graph[city][unvisited[0]];

        int size = unvisited.size();
        for(int i = 1; i < size; i++) {
            if(unvisited[i] == curOrder) continue;
            if(graph[city][unvisited[i]] < nearest_cost) {
                nearest_cost = this->graph[city][unvisited[i]];
            }
        }
        return nearest_cost;
    }

    int nearest_unvisited_city_id(int curOrder, int city, vector<int> unvisited) {
        if(unvisited.empty()) return 0;
        float nearest_cost = this->graph[city][unvisited[0]];
        int curId = unvisited[0];

        int size = unvisited.size();
        for(int i = 1; i < size; i++) {
            if(unvisited[i] == curOrder) continue;
            if(graph[city][unvisited[i]] < nearest_cost) {
                nearest_cost = this->graph[city][unvisited[i]];
                curId = unvisited[i];
            }
        }
        return curId;
    }
    
    float cal_h_cost(int start, int cur, vector<int> visited, vector<int> unvisited) {
        float nearest_cost_from_cur= this->nearest_unvisited_city(cur, cur, unvisited);
        float nearest_cost_to_start = this->nearest_unvisited_city(cur, start, unvisited);
        int nearest_city = this->nearest_unvisited_city_id(cur, cur, unvisited);

        MST mst(visited, unvisited, this->graph);
        float mst_cost = mst.MST_cost(cur, nearest_city);

        return nearest_cost_from_cur + mst_cost + nearest_cost_to_start;
    }
};


int main()
{
    clock_t start;
    clock_t finish;
    start = clock();
    int num;
    cin >> num;
   // cout << "Testing:" << endl;
    Tsp tsp(num);
    tsp.readData();
    cityState finalCityState = tsp.Astar();
    vector<int> path = finalCityState.getPath();
    float totalDistance = finalCityState.calTotalCost(path, tsp.graph);
    /////////print!!!/////////
   cout << "Final Path: " << endl;
    int size = path.size();
    for(int i = 0; i < size - 1; i++) {
        cout << tsp.cities[path[i]] << " -> "; 
    }
    cout << tsp.cities[path[size-1]] << endl;
    cout << "Total distance is " << totalDistance << endl;
    finish = clock();
    cout << "The total execution time: " << (double)(finish - start)/CLOCKS_PER_SEC << endl;
    return 0;
}




