//Generate testcases in following manner
//N M
// edges
// safe-walk
//edges to be added

//I THINK CORRECT but it needs debugging

#include <bits/stdc++.h>
using namespace std;

#define RUN 1
#define MAX_VERTICES 1000
#define MAX_EDGES 1800

class Graph
{
public:
    int V; // No. of vertices

    list<pair<int, int>> *adj;
    list<pair<int, int>> *revadj;

    vector<bool> cloud;
    vector<bool> sea;
    vector<int> status; // 2-vapor, 0-river, 3-cloud, 1-sea

    Graph(int); // Constructor
    void renew();
    void addedge(int, int, int);
    vector<int> BFS(int, int, int[]);

};

//CLASS INSTANCES
Graph::Graph(int V)
{
    this->V = V;
    adj = new list<pair<int, int>>[V + 1];
    revadj = new list<pair<int, int>>[V + 1];
}
void Graph::renew(){
    delete adj;
    delete revadj;
    adj = new list<pair<int, int>>[V + 1];
    revadj = new list<pair<int, int>>[V + 1];
}
void Graph::addedge(int u, int v, int wt)
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
        for (auto itr = adj[u].begin(); itr != adj[u].end(); itr++)
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

//To GENERATE SCC GRAPH
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
set<pair<int, int>> Generate(Graph &g, int n, int m)
{
    //renew the Graph
    // for(auto it=*g.adj->begin();it!=*g.adj->end();){
    //     it=*g.adj->erase(it);
    // }
    g.renew();

    set<pair<int, int>> container;
    vector<bool> sources(n, true);
    vector<bool> sink(n, true);

    srand(time(NULL));

    int wt = 0;
    for (int j = 1; j <= m; j++)
    {
        int a = 1 + rand() % (n - 2);
        int b = 1 + rand() % (n - 2);
        pair<int, int> p = make_pair(a, b);
        //    cout << a << " " << b << endl;

        // Search for a random "new" edge everytime
        // Note - In a tree the edge (a, b) is same
        // as the edge (b, a)
        while (container.find(p) != container.end() || findReachableNodes(g, a, b) || a == b)
        {
            a = 1 + rand() % (n - 2);
            b = 1 + rand() % (n - 2);
            p = make_pair(a, b);
        }

        container.insert(p);
        g.addedge(a, b, wt++);
    }
    // now check for all sinks and all sources
    for (auto it = container.begin(); it != container.end(); it++)
    {
        sources[it->second] = false;
        sink[it->first] = false;
    }

    // To make graph SCC

    // cout << "\nSink\n";
    for (int i = 1; i <= n; i++)
    {
        if (sink[i])
        {
            int x = 1 + rand() % (n - 2);
            pair<int, int> p = make_pair(i, x);

            while (container.find(p) != container.end() || x == i)
            {
                x = 1 + rand() % (n - 2);
                p = make_pair(i, x);
            }

            g.addedge(i, x, wt++);
            container.insert(p);

            sources[x] = 0;
            sink[i] = 0;
        }
    }

    // cout << "\nSources\n";
    for (int i = 1; i <= n; i++)
    {
        if (sources[i])
        {
            int x = 1 + rand() % (n - 2);
            pair<int, int> p = make_pair(x, i);

            while (container.find(p) != container.end() || x == i)
            {
                x = 1 + rand() % (n - 2);
                p = make_pair(x, i);
            }

            g.addedge(x, i, wt++);
            container.insert(p);

            sink[x] = 0;
            sources[i] = 0;
        }
    }

    return container;
}

// For EDGE COVERING WALK
int isNotVisited(int x, vector<int> &path)
{
	int size = path.size();
	for (int i = 0; i < size; i++)
		if (path[i] == x)
			return 0;
	return 1;
}
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

//For SAFE WALK
void compute_Rminus(Graph &g, int s1, int d2, int d1)
{
	// all nodes reachable from start(W) without using W as subwalk
	g.sea.resize(g.V + 1, 0);
	vector<bool> vis(g.V + 1, 0);

	int par = -1;
	queue<pair<int, int>> q;
	q.push({s1, par});
	vis[s1] = 1;

	while (!q.empty())
	{
		int node = q.front().first;
		par = q.front().second;

		q.pop();

		if (node == d2 && par == d1)
			continue;

		g.sea[node] = 1;
		for (auto it : g.adj[node])
		{
			if (it.first == d2 && node == d1)
				continue;
			if (!vis[it.first])
			{
				q.push({it.first, node});
				vis[it.first] = 1;
			}
		}
	}
	// print_sea(g);
}
void compute_Rplus(Graph &g, int d1, int s1, int s2)
{
	// all nodes reaching END(W) without using W as subwalk
	g.cloud.resize(g.V + 1, 0);
	
//cout<<"\ncomputing cloud- size of cloud vector="<<g.cloud.size();	
// print_cloud(g);
	vector<bool> vis(g.V + 1, 0);

	int par = -1;
	queue<pair<int, int>> q;
	q.push({d1, par});
	vis[d1] = 1;

	while (!q.empty())
	{
		int node = q.front().first;
		par = q.front().second;
		q.pop();

		if (node == s1 && par == s2)
			continue;

		g.cloud[node] = 1;
		for (auto it : g.revadj[node])
		{
			if (it.first == s1 && node == s2)
				continue;
			if (!vis[it.first])
			{
				q.push({it.first, node});
				vis[it.first] = 1;
			}
		}
	}
	// print_cloud(g);
}
int extendR(Graph &g, int d1, int d2, int s1, int s2, vector<int> edge_cover)
{
	// d1 d2 represent locations in edge cover walk
	d1 = d2;
	d2 = d2 + 1; // d2=d2+1 i.e. unmarked

//  cout<<"\n\nInside extendR\n\nd1= "<<d1<<" d2= "<<d2;

	compute_Rplus(g, edge_cover[d1], edge_cover[s1], edge_cover[s2]); // values at source & destination location passed
	compute_Rminus(g, edge_cover[s1], edge_cover[d2], edge_cover[d1]);
	
//  cout<<"\nR+ and R- computed";

	// computing vapor, river, cloud and sea
	g.status.resize(g.V + 1, 0);
	for (int i = 1; i <= g.V; i++)
	{ 
		if (g.cloud[i] && g.sea[i])
			g.status[i] = 2;
		
		else if(g.cloud[i])
			g.status[i]=3;
		
		else if(g.sea[i])
			g.status[i]=1;
		
	}

//  cout<<"\nVapor & River computed";
	
	//IF RIVER EMPTY AND R+(W) IS G THEN AVERTIBLE CASE -> HYDROSTRUCTURE CAN"T BE COMPUTED
    //VAPOR(W) IS G -> UNSAFE
    bool riv_nonepty=0;//assuming river empty
    for(int i=1;i<=g.status.size();i++){
        if(g.status[i]==0){
            riv_nonepty=1;//yes river is not empty
            break;
        }
    }

    bool open_path=0;
    for(int i=1;i<=g.status.size();i++){
        if(g.status[i]!=2 && g.status[i]!=0){
            open_path=1;
            break;
        }
    }

// cout<<"\nSafety condn checked";

	if (open_path && riv_nonepty)
		{
			// cout<<"\nlooping-";
			return extendR(g, d1, d2, s1, s2, edge_cover);
		}

	return d1;
}
vector<pair<int, int>> all_maximal_safewalk(Graph g, vector<int> edge_cover)
{
	vector<pair<int, int>> allwalks;

	int left = 0, right = left+1, prev_right;
	while (right < edge_cover.size()-1)
	{ 
// cout<<"\nhere0";

		int s1 = left, s2 = left + 1, d1 = right-1, d2 = right;

		//in case target edge comes before starting edge
		if(s1>d1){
			d1=s1;
			d2=s2;
		}

//   cout<<"\ns1- "<<s1<<" s2- "<<s2<<" d1- "<<d1<<" d2- "<<d2;

        prev_right=right;
		right = extendR(g, d1, d2, s1, s2, edge_cover);

//   cout<<"\n\nout of extendR";
//   cout<<"\nRight-"<<right;
//   cout<<"\nprev_right= "<<prev_right;

        //report only if right!=prev_right
        if(right!=prev_right){
			// cout<<"\npush {"<<left<<","<<right<<"}";
			if(left+1!=right)
			allwalks.push_back({left, right});
		}
		left = left+1;
	}
    //when right reaches end of the walk

	return allwalks;
}


int main(){
    std::ofstream ofs;
    ofs.open("Testcase.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    freopen("Testcase.txt", "w", stdout);

    srand(time(NULL));

    int NUM = MAX_VERTICES;  // Number of Vertices
    int NUMEDGE = MAX_EDGES; // Number of Edges

    Graph g(NUM);    
    set<pair<int, int>> container=Generate(g, NUM, NUMEDGE);

    // Generating Safewalk
    int s1, s2, d1, d2;

    vector<int> wlk = edge_covering_walk(g, container.size());
    vector<pair<int, int>> allwalks = all_maximal_safewalk(g, wlk);

    while (allwalks.size() == 0){
        // generate another testcase

        container=Generate(g, NUM, NUMEDGE);

        wlk = edge_covering_walk(g, container.size());
        allwalks = all_maximal_safewalk(g, wlk);
    }

    s1 = wlk[allwalks[0].first];
    s2 = wlk[allwalks[0].first + 1];
    d1 = wlk[allwalks[0].second - 1];
    d2 = wlk[allwalks[0].second];

    // PRINTING
    printf("%d %d", NUM, container.size());// no of nodes and no of edges
    for (auto it = container.begin(); it != container.end(); ++it)
    {
        printf("\n%d %d", it->first, it->second);
    }

    // input - start(w) and end(w)
    printf("\n%d %d %d %d", s1, s2, d1, d2);

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

    container.clear();
    fclose(stdout);

    return 0;
}