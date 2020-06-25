#include "Jug.h"

Jug::Jug(int cA, int cB, int N, int cfA, int cfB, int ceA, int ceB, int cpAB, int cpBA)
{
	capA = cA;
	capB = cB;
	goal = N;
	costFillA = cfA;
	costFillB = cfB;
	costEmptyA = ceA;
	costEmptyB = ceB;
	costAB = cpAB;
	costBA = cpBA;

	jugs = vector<state>();
	sols = vector<state*>();
}

Jug::~Jug()
{

}

int Jug::solve(string& solution)
{
	solution = "";
	if (!validate())
	{
		return -1;
	}

	buildGraph();

	if (!hasSol())
	{
		return 0;
	}

	dijkstra();

	getSols();

	state* min = sols[sols.size() - 1];

	for (unsigned int i = 0; i < sols.size(); i++)
	{
		if (*sols[i] < *min)
		{
			min = sols[i];
		}
	}

	string actions[6] = { "fill A", "fill B", "empty A", "empty B", "pour A B", "pour B A" };

	stack<string> path;
	state* curr = min;

	path.push("success " + to_string(min->estimatedDistance));

	while (curr->prev != nullptr)
	{
		for (unsigned int i = 0; i < 6; i++)
		{
			int index = get<0>(curr->prev->adj[i]);
			if (&(jugs[index]) == curr)
			{
				path.push(actions[i]);
			}
		}
		curr = curr->prev;
	}

	while (!path.empty())
	{
		if (path.size() == 1)
		{
			solution += path.top();
		}
		else
		{
			solution += path.top() + "\n";
		}
		path.pop();
	}



	return 1;
}





bool Jug::validate()
{
	if (capA <= 0 || capB <= 0 || goal < 0 ||costFillA < 0 || costFillB < 0 || costEmptyA < 0 || costEmptyB < 0 || costAB < 0 || costBA < 0)
	{
		return false;
	}
	if (capA > capB)
	{
		return false;
	}
	if (goal > capA && goal > capB)
	{
		return false;
	}
	if (capA >= 1000 || capB >= 1000)
	{
		return false;
	}

	return true;
}

void Jug::buildGraph()
{
	state start;
	state curr;
	jugs.push_back(start);

	int i;
	int numNodes;

	i = 0;
	numNodes = jugs.size();

	while (i < numNodes)
	{
		fillA(i);
		fillB(i);
		emptyA(i);
		emptyB(i);
		pourAB(i);
		pourBA(i);

		numNodes = jugs.size();
		i++;
	}

}

state Jug::fillA_State(state curr)
{
	state newJug;

	newJug.jugA = capA;
	newJug.jugB = curr.jugB;

	return newJug;
}

state Jug::fillB_State(state curr)
{
	state newJug;

	newJug.jugB = capB;
	newJug.jugA = curr.jugA;

	return newJug;

}

state Jug::emptyA_State(state curr)
{
	state newJug;

	newJug.jugA = 0;
	newJug.jugB = curr.jugB;

	return newJug;

}

state Jug::emptyB_State(state curr)
{
	state newJug;

	newJug.jugB = 0;
	newJug.jugA = curr.jugA;

	return newJug;
}

state Jug::pourAB_State(state curr)
{
	state newJug;

	newJug.jugB = curr.jugA + curr.jugB;
	if (newJug.jugB > capB)
	{
		newJug.jugB = capB;
	}

	newJug.jugA = curr.jugA - (capB - curr.jugB);
	if (newJug.jugA < 0)
	{
		newJug.jugA = 0;
	}

	return newJug;
}

state Jug::pourBA_State(state curr)
{
	state newJug;

	newJug.jugA = curr.jugA + curr.jugB;
	if (newJug.jugA > capA)
	{
		newJug.jugA = capA;
	}

	newJug.jugB = curr.jugB - (capA - curr.jugA);
	if (newJug.jugB < 0)
	{
		newJug.jugB = 0;
	}

	return newJug;
}

void Jug::fillA(int index)
{
	state addState;

	addState = fillA_State(jugs[index]);

	if (!hasState(addState.jugA, addState.jugB))
	{
		//adds new vert
		addState.index = jugs.size();
		jugs.push_back(addState);

		//adds vert to adj
		pair<int, int> neighbor(addState.index, costFillA);
		jugs[index].adj.push_back(neighbor);
	}
	else
	{
		int i;
		i = findState(addState.jugA, addState.jugB);
		pair<int, int> neighbor(i, costFillA);
		jugs[index].adj.push_back(neighbor);
	}
}

void Jug::fillB(int index)
{
	state addState;

	addState = fillB_State(jugs[index]);

	if (!hasState(addState.jugA, addState.jugB))
	{
		//adds new vert
		addState.index = jugs.size();
		jugs.push_back(addState);

		//adds vert to adj
		pair<int, int> neighbor(addState.index, costFillB);
		jugs[index].adj.push_back(neighbor);
	}
	else
	{
		int i;
		i = findState(addState.jugA, addState.jugB);
		pair<int, int> neighbor(i, costFillB);
		jugs[index].adj.push_back(neighbor);
	}
}

void Jug::emptyA(int index)
{
	state addState;

	addState = emptyA_State(jugs[index]);

	if (!hasState(addState.jugA, addState.jugB))
	{
		//adds new vert
		addState.index = jugs.size();
		jugs.push_back(addState);

		//adds vert to adj
		pair<int, int> neighbor(addState.index, costEmptyA);
		jugs[index].adj.push_back(neighbor);
	}
	else
	{
		int i;
		i = findState(addState.jugA, addState.jugB);
		pair<int, int> neighbor(i, costEmptyA);
		jugs[index].adj.push_back(neighbor);
	}
}

void Jug::emptyB(int index)
{
	state addState;

	addState = emptyB_State(jugs[index]);

	if (!hasState(addState.jugA, addState.jugB))
	{
		//adds new vert
		addState.index = jugs.size();
		jugs.push_back(addState);

		//adds vert to adj
		pair<int, int> neighbor(addState.index, costEmptyB);
		jugs[index].adj.push_back(neighbor);
	}
	else
	{
		int i;
		i = findState(addState.jugA, addState.jugB);
		pair<int, int> neighbor(i, costEmptyB);
		jugs[index].adj.push_back(neighbor);
	}
}

void Jug::pourAB(int index)
{
	state addState;

	addState = pourAB_State(jugs[index]);

	if (!hasState(addState.jugA, addState.jugB))
	{
		//adds new vert
		addState.index = jugs.size();
		jugs.push_back(addState);

		//adds vert to adj
		pair<int, int> neighbor(addState.index, costAB);
		jugs[index].adj.push_back(neighbor);
	}
	else
	{
		int i;
		i = findState(addState.jugA, addState.jugB);
		pair<int, int> neighbor(i, costAB);
		jugs[index].adj.push_back(neighbor);
	}
}

void Jug::pourBA(int index)
{
	state addState;

	addState = pourBA_State(jugs[index]);

	if (!hasState(addState.jugA, addState.jugB))
	{
		//adds new vert
		addState.index = jugs.size();
		jugs.push_back(addState);

		//adds vert to adj
		pair<int, int> neighbor(addState.index, costBA);
		jugs[index].adj.push_back(neighbor);
	}
	else
	{
		int i;
		i = findState(addState.jugA, addState.jugB);
		pair<int, int> neighbor(i, costBA);
		jugs[index].adj.push_back(neighbor);
	}
}

int Jug::findState(int A, int B)
{
	int tot = jugs.size();

	for (int i = 0; i < tot; i++)
	{
		if (jugs[i].jugA == A && jugs[i].jugB == B)
		{
			return i;
		}
	}

	return -1;
}

int Jug::find1Sol()
{
	int tot = jugs.size();

	for (int i = 0; i < tot; i++)
	{
		if (jugs[i].jugA == goal && jugs[i].jugB == 0)
		{
			return i;
		}
		if (jugs[i].jugB == goal && jugs[i].jugA == 0)
		{
			return i;
		}
	}

	return -1;
}

bool Jug::hasState(int A, int B)
{
	int pres = findState(A, B);

	if (pres == -1)
	{
		return false;
	}
	
	return true;
}

bool Jug::hasSol()
{
	int pres = find1Sol();

	if (pres == -1)
	{
		return false;
	}

	return true;
}

void Jug::getSols()
{
	for (unsigned int i = 0; i < jugs.size(); i++)
	{
		if (jugs[i].jugA == goal && jugs[i].jugB == 0)
		{
			sols.push_back(&(jugs[i]));
		}
		if (jugs[i].jugB == goal && jugs[i].jugA == 0)
		{
			sols.push_back(&(jugs[i]));
		}
	}


}



void Jug::initialize()
{
	for (unsigned int i = 1; i < jugs.size(); i++)
	{
		jugs[i].prev = nullptr;
		jugs[i].estimatedDistance = INT_MAX;
		jugs[i].isExpanded = color::WHITE;
	}

	jugs[0].prev = nullptr;
	jugs[0].estimatedDistance = 0;
	jugs[0].isExpanded = color::GREY;

}

void Jug::relax(state* u, state* v, int cost)
{
	if (v->estimatedDistance > u->estimatedDistance + cost)
	{
		v->estimatedDistance = u->estimatedDistance + cost;
		v->prev = u;
	}
}

void Jug::dijkstra()
{
	initialize();

	Heap Q;

	vector<state*> cloud;

	state* u;
	state* v;

	int index;
	int weight;

	Q.enqueue(&(jugs[0]));

	while (!Q.empty())
	{
		u = Q.root();
		u->isExpanded = color::BLACK;

		for (int i = 0; i < 6; i++)
		{
			index = get<0>(u->adj[i]);
			weight = get<1>(u->adj[i]);

			v = &(jugs[index]);

			if (v->isExpanded != color::BLACK)
			{
				relax(u, v, weight);
				Q.enqueue(v);
				v->isExpanded = color::GREY;
			}
		}

		
		cloud.push_back(u);

		Q.dequeue();
		
	}

	
}











Heap::Heap()
{
	min_heap = vector <state*>();
	numItems = 0;
}

void Heap::trickleDown(int index)
{
	int childIndex;
	int value;
	int minValue;
	int minIndex;
	state* temp;

	childIndex = 2 * index + 1;
	value = min_heap[index]->estimatedDistance;

	while (childIndex < numItems)
	{
		minValue = value;
		minIndex = -1;
		for (int i = 0; i < 2 && i + childIndex < numItems; i++)
		{
			if (min_heap[i + childIndex]->estimatedDistance < minValue)
			{
				minValue = min_heap[i + childIndex]->estimatedDistance;
				minIndex = i + childIndex;
			}
		}

		if (minValue == value)
		{
			return;
		}
		else
		{
			temp = min_heap[index];
			min_heap[index] = min_heap[minIndex];
			min_heap[minIndex] = temp;
			index = minIndex;
			childIndex = 2 * index + 1;
		}
	}

}

void Heap::trickleUp(int index)
{
	int parentIndex;
	state* temp;

	while (index > 0)
	{
		parentIndex = (index - 1) / 2;
		if (min_heap[index]->estimatedDistance >= min_heap[parentIndex]->estimatedDistance)
		{
			return;
		}

		else
		{
			temp = min_heap[index];
			min_heap[index] = min_heap[parentIndex];
			min_heap[parentIndex] = temp;
			index = parentIndex;
		}
	}
}

void Heap::enqueue(state* item)
{
	min_heap.push_back(item);
	trickleUp(numItems);
	numItems++;

}

void Heap::dequeue()
{
	if (numItems != 0)
	{
		//delete arr[0];
		min_heap[0] = min_heap[numItems - 1];

		numItems--;

		trickleDown(0);

		min_heap.pop_back();

	}
	else
	{
		cout << "Heap is empty" << endl;
	}

}

state* Heap::root()
{
	return min_heap[0];
}

void Heap::print()
{
	cout << "distance: " << min_heap[0]->estimatedDistance << ", A: " << min_heap[0]->jugA << ", B: " << min_heap[0]->jugB << endl;
}

bool Heap::empty()
{
	return min_heap.empty();
}
