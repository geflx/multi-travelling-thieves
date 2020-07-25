

# Multiple Traveling Thieves Problem

This problem is a variation of the _Travelling Thief Problem (TTP)_ with **multiple agents**. The _TTP_ is a combination of Knapsack Problem with Traveling Salesman Problem, which is proved to be **NP-Hard**. In a nutshell, a group of thieves rented a truck and must do their job in a set of cities, focusing on have the optimal profit from the robbery.  

The following procedures were implemented in this work:
- Greedy Randomized Adaptive Search Procedure (GRASP);
- Iterated Local Search (ILS);
- Variable Neighborhood Descent (VND).

The main objective here is to obtain near-optimal solutions for the problem in a reasonable time.

## How to run? :running:
```bash
$ g++ main.cpp -O3 -o exec
$ ./exec
```
## Problem description :ledger:

A set of _N_ cities are disposed in a state, with distance _d[i][j]_ between any two cities. Each item _k_ positioned in a city _i_ has value _p[i][k]_ and weight _w[i][k]_.  
The global knapsack capacity limits the amount of items and a tax _R_ is paid for each time unit. The velocity _v_ of a thief depends on its carried items. 

The **multi-objective function** is defined as follows:

<p align="center">
  <img src="assets/obj_function.png" align=center width=620 height=85/>
</p>


## Conclusions :mag_right:

  This problem was one of the most challenging that the team encountered, due to its **difficult** multi-objetive function, considerably large instances and amount of information. It was very fun to discuss methods to solve this problem, and even funnier to implement them.  
Considering the set of instances, results shown that the obtained ones overcome the literature results.

## Team :octocat:

* Gabriel Felix 
* [Natan Garcias](https://github.com/NatanGarcias) 

## References :books:
[1] https://cs.adelaide.edu.au/~optlog/research/ttp/2016gecco-mttp.pdf


