#ifndef JUG_H
#define JUG_H

#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <climits>
#include <queue>
#include <functional>
#include <stack>

using namespace std;

enum class color { WHITE, GREY, BLACK };

struct state
{
	//general info
	int jugA = 0;
	int jugB = 0;
	int index = 0;
	//index,cost
	vector<pair<int, int>> adj;

	//all for dijkstras only
	state* prev = nullptr;
	int estimatedDistance = INT_MAX;
	color isExpanded = color::WHITE;


	bool operator < (const state& rhs)
	{
		if (estimatedDistance < rhs.estimatedDistance)
		{
			return true;
		}

		return false;
	}

	bool operator <= (const state& rhs)
	{
		if (estimatedDistance <= rhs.estimatedDistance)
		{
			return true;
		}

		return false;
	}

	bool operator == (const state& rhs)
	{
		if (estimatedDistance == rhs.estimatedDistance)
		{
			return true;
		}

		return false;
	}

	bool operator != (const state& rhs)
	{
		if (estimatedDistance != rhs.estimatedDistance)
		{
			return true;
		}

		return false;
	}

	bool operator >= (const state& rhs)
	{
		if (estimatedDistance >= rhs.estimatedDistance)
		{
			return true;
		}

		return false;
	}

	bool operator > (const state& rhs)
	{
		if (estimatedDistance > rhs.estimatedDistance)
		{
			return true;
		}

		return false;
	}

};

//Modified heap from lab 4
class Heap
{
private:
	vector<state*> min_heap;
	int numItems;  //current number of items in heap

	/*This function is called by dequeue function to move the new root down the heap to the appropriare location.*/
	void trickleDown(int);

	/*This function is called by enqueue function to move the new item up the heap to the appropriare location.*/
	void trickleUp(int);

public:
	/*Initializes an empty Heap*/
	Heap();

	/*Insert a PrintJob to the heap without violating max-heap properties*/
	void enqueue(state*);

	/*Removes the node with highest priority from the heap.Follow the algorithm on priority-queue slides. */
	void dequeue();

	/*Returns the node with highest priority.*/
	state* root();

	/*Prints the PrintJob with highest priority in the following format: Priority: priority, Job Number: jobNum, Number of Pages: numPages (Add a new line at the end.)*/
	void print();

	bool empty();

};

class Jug {
public:
	Jug(int, int, int, int, int, int, int, int, int);
	~Jug();

	//solve is used to check input and find the solution if one exists
	//returns -1 if invalid inputs. solution set to empty string.
	//returns 0 if inputs are valid but a solution does not exist. solution set to empty string.
	//returns 1 if solution is found and stores solution steps in solution string.
	int solve(string& solution);
private:
	//anything else you need

	int capA;
	int capB;
	int goal;
	int costFillA;
	int costFillB;
	int costEmptyA;
	int costEmptyB;
	int costAB;
	int costBA;

	vector<state> jugs;
	vector<state*> sols;
	
	//returns true if everything is hunky dory
	bool validate();
	
	void buildGraph();

	// return state after action is performed
	state fillA_State(state curr);
	state fillB_State(state curr);
	state emptyA_State(state curr);
	state emptyB_State(state curr);
	state pourAB_State(state curr);
	state pourBA_State(state curr);

	//inserts state into graph
	void fillA(int index);
	void fillB(int index);
	void emptyA(int index);
	void emptyB(int index);
	void pourAB(int index);
	void pourBA(int index);

	int findState(int A, int B);
	int find1Sol();
	bool hasState(int A, int B);
	bool hasSol();

	void getSols();
	

	void initialize();
	void relax(state* u, state* v, int cost);

	void dijkstra();






};



#endif // !JUG_H
