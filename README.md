# Travelling-Salesman-Problem-Asatr-search

•	States: (<city id 1>, …), list of cities traveled so far <br>
•	Start state: (<city id 1>) only visited the first city <br>
•	Goal state: (<city id 1>, <city id 2>, …, <city id 1>) visited all cities and reached the start city again <br>
•	Successors function: generates all cities that have not yet visited <br>
•	Cost: the Euclidean distance between cities, use it to calculate g(n) <br>
• Admissible heuristic function: h(n) = distance to the nearest unvisited city from the current city (h1) + distance to travel all the unvisited cities (using MST) (h2) + nearest distance from an unvisited city to the start city (h3). <br>
