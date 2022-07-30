//correct

// A C++ Program to generate test cases for
// an unweighted strongly connected directed graph
#include <bits/stdc++.h>
using namespace std;

// Define the number of runs for the test data
// generated
#define RUN 1

// Define the maximum number of vertices of the graph
#define MAX_VERTICES 1000

// Define the maximum number of edges
#define MAX_EDGES 1800

class Graph
{
public:
    int V; // No. of vertices

    // Pointer to an array containing adjacency lists
    // list<int> *adj;
    // list<int> *revadj;
    list<pair<int, int>> *adj;
	list<pair<int, int>> *revadj;

    Graph(int); // Constructor

    // void addEdge(int, int);
    void addedge(int, int, int);

    vector<int> BFS(int, int, int[]);
};
Graph::Graph(int V)
{
    this->V = V;
    adj = new list<pair<int,int>>[V + 1];
    revadj = new list<pair<int,int>>[V + 1];
}
void Graph:: addedge(int u, int v, int wt)
{
	adj[u].push_back({v, wt});
	revadj[v].push_back({u, wt});
}
vector<int> Graph::BFS(int componentNum, int src, int visited[])
{
    // Mark all the vertices as not visited
    // Create a queue for BFS
    queue<int> queue;

    queue.push(src);

    // Assign Component Number
    visited[src] = componentNum;

    // Vector to store all the reachable nodes from 'src'
    vector<int> reachableNodes;

    while (!queue.empty())
    {
        // Dequeue a vertex from queue
        int u = queue.front();
        queue.pop();

        reachableNodes.push_back(u);

        // Get all adjacent vertices of the dequeued
        // vertex u. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        for (auto itr = adj[u].begin();itr != adj[u].end(); itr++)
        {
            if (!visited[(itr->first)])
            {
                // Assign Component Number to all the
                // reachable nodes
                visited[itr->first] = componentNum;
                queue.push(itr->first);
            }
        }
    }
    return reachableNodes;
}
// Display all the Reachable Nodes from a node 'n'
// void displayReachableNodes(int n, unordered_map<int, vector<int>> m)
// {
//     vector<int> temp = m[n];
//     for (int i = 0; i < temp.size(); i++)
//         cout << temp[i] << " ";
//     cout << endl;
// }
// Find all the reachable nodes for every element in the arr
bool findReachableNodes(Graph g, int a, int b)
{
    // Get the number of nodes in the graph
    int V = g.V;
    int flag = 0;

    // Take a integer visited array and initialize
    // all the elements with 0
    int visited[V + 1];
    memset(visited, 0, sizeof(visited));

    // Map to store list of reachable Nodes for a
    // given node.
    unordered_map<int, vector<int>> m;

    // Initialize component Number with 0
    int componentNum = 0;

    // For each node in arr[] find reachable
    // Nodes

    // Visit all the nodes of the component
    if (!visited[b])
    {
        componentNum++;

        // Store the reachable Nodes corresponding to
        // the node 'i'
        m[visited[b]] = g.BFS(componentNum, b, visited);
    }

    // At this point, we have all reachable nodes
    // from b, print them by doing a look up in map m.
    // cout << "Reachable Nodes from " << b << " are\n";
    // displayReachableNodes(visited[b], m);
    // vector<int> temp = m[visited[b]];
    for (int i = 0; i < m[visited[b]].size(); i++)
    {
        if (m[visited[b]][i] == a)
        {
            flag = 1;
        }
    }
    return flag;
}
// utility function to check if current vertex is already present in path
int isNotVisited(int x, vector<int> &path)
{
	int size = path.size();
	for (int i = 0; i < size; i++)
		if (path[i] == x)
			return 0;
	return 1;
}
// utility function for finding paths in graph from source to destination
vector<int> findpaths(Graph &g, int src, int dst)
{
	// create a queue which stores the paths
	queue<vector<int>> q;

	// path vector to store the current path
	vector<int> path;
	path.push_back(src);
	q.push(path);
	while (!q.empty())
	{
		path = q.front();
		q.pop();
		int last = path[path.size() - 1];

		// if last vertex is the desired destination
		// then print the path
		if (last == dst)
		{
			return path;
		}

		// traverse to all the nodes connected to
		// current vertex and push new path to queue

		for (auto it : g.adj[last])
		{
			if (isNotVisited(it.first, path))
			{
				vector<int> newpath(path);
				newpath.push_back(it.first);
				q.push(newpath);
			}
		}
	}
	return {-1};
}
struct Node
{
	int data;
	struct Node *next;
} *head = NULL;
bool search(struct Node **head_ref, int key)
{
	struct Node *curr = *head_ref;
	while (curr != NULL && curr->data != key)
	{
		curr = curr->next;
	}
	if (curr == NULL)
		return 0;

	return 1;
}
void push(struct Node **head_ref, int new_data)
{
	Node *new_node = new Node;
	new_node->data = new_data;
	new_node->next = (*head_ref);
	(*head_ref) = new_node;
}
void deleteNode(struct Node **head_ref, int key)
{
	struct Node *temp = *head_ref, *prev;
	if (temp != NULL && temp->data == key) // head->data==key
	{
		*head_ref = temp->next;
		free(temp);
		return;
	}
	while (temp != NULL && temp->data != key)
	{
		prev = temp;
		temp = temp->next;
	}
	if (temp == NULL)
		return;
	prev->next = temp->next;
	free(temp);
}

vector<int> edge_covering_walk(Graph g, int m)
{
	vector<int> walk;
	vector<bool> vis_edge(m, 0);

	int strt = 1, nxt_node, edge;
	do
	{
		bool key = 1;
		walk.push_back(strt);
		// cout << "\nStart- " << strt << " ";
		while (key)
		{
			// cout << "\n\n\nStart- " << strt << " ";
			for (auto it : g.adj[strt])
			{
				if (!vis_edge[it.second])
				{
					nxt_node = it.first;
					walk.push_back(nxt_node);
					// cout << "\nNext- " << nxt_node << " ";

					edge = it.second;
					vis_edge[edge] = 1;
					// cout << "\nvisited- " << edge;
					break;
				}
			}
			bool flag = 1;
			for (auto it : g.adj[strt])
			{
				// cout << "\nadj[" << strt << "] -> " << it.second << " visited=" << vis_edge[it.second];
				if (vis_edge[it.second] == 0)
				{
					flag = 0;
					break;
				}
			}
			// flag 0 means node 'strt' has outgoing edges left untreversed
			// cout << "\nFlag- " << flag;
			// if no untraversed edges left- remove from linked list
			if (flag == 1)
			{ // && find(temp.begin(),temp.end(),strt)!=temp.end())
				deleteNode(&head, strt);
				// cout << "\ndelete " << strt << " ";
			}

			// else push node to linked list (if not present)
			if (flag == 0 && (!search(&head, strt)))
			{ // && (find(temp.begin(),temp.end(),strt)==temp.end()))
				push(&head, strt);
				// cout << "\npush " << strt << " ";
			}

			// cout << "\nChanging start pointer and checking if stuck or not";

			strt = nxt_node;
			key = 0;
			for (auto it : g.adj[strt])
			{
				// cout << "\nadj[" << strt << "] -> " << it.second << " visited=" << vis_edge[it.second];
				if (!vis_edge[it.second])
				{
					key = 1;
				}
			}
		}
		if (head != NULL)
		{
			// cout << "\ncalling djk from " << strt << " to " << head->data << "\n";
			// cout << "\nstrt- " << strt << "head->data " << head->data;

			vector<int> mid_wlk = findpaths(g, strt, head->data);
			for (int i = 1; i < mid_wlk.size() - 1; i++)
				walk.push_back(mid_wlk[i]);

			strt = head->data;
			deleteNode(&head, head->data);
		}
	} while (head != NULL);
	return walk;
}

int main()
{
    freopen("Testcase.txt", "r", stdin);

    srand(time(NULL));

	int NUM, m;
	cin >> NUM >> m;
	Graph g(NUM);

    set<pair<int, int>> container;
    set<pair<int, int>>::iterator it;

	int wt = 0, t = m;
	while (t--)
	{
		int u, v;
		cin >> u >> v;
		g.addedge(u, v, wt);
        pair<int, int> p = make_pair(u, v);
        container.insert(p);
		wt++;
	}
	fclose(stdin);
    
    // std::ofstream ofs;
    // ofs.open("Testcase.txt", std::ofstream::out | std::ofstream::trunc);
    // ofs.close();

    // // Uncomment the below line to store
    // // the test data in a file
    // freopen("Testcase.txt", "w", stdout);

    

    // input for incremental verification
    bool key = 1;
    printf("\n%d", key);
    while (key)
    {
        int u = 1 + rand() % NUM;
        int v = 1 + rand() % NUM;

        pair<int, int> p = make_pair(u, v);
        while (u == v || container.find(p) != container.end())
        {
            int v = 1 + rand() % NUM;
        }

        printf("\n%d %d", u, v);

        key = rand() % 2;
        printf("\n%d", key);
    }

    // Uncomment the below line to store
    // the test data in a file
    container.clear();
    fclose(stdout);

    return (0);
}
