//Trying to find a safe walk for a given graph

//ALL CORRECT

#include <bits/stdc++.h>
using namespace std;

class Graph
{
public:
	int n;
	list<pair<int, int>> *adj;
	list<pair<int, int>> *revadj;

	vector<bool> cloud;
	vector<bool> sea;
	vector<int> status;//2-vapor, 0-river, 3-cloud, 1-sea

	Graph(int n)
	{
		this->n = n;
		adj = new list<pair<int, int>>[n + 1];
		revadj = new list<pair<int, int>>[n + 1];
	}

	void addedge(int u, int v, int wt)
	{
		adj[u].push_back({v, wt});
		revadj[v].push_back({u, wt});
	}
};

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

void compute_Rminus(Graph &g, int s1, int d2, int d1)
{
	// all nodes reachable from start(W) without using W as subwalk
	g.sea.resize(g.n + 1, 0);
	vector<bool> vis(g.n + 1, 0);

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
	g.cloud.resize(g.n + 1, 0);
	
//cout<<"\ncomputing cloud- size of cloud vector="<<g.cloud.size();	
// print_cloud(g);
	vector<bool> vis(g.n + 1, 0);

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
	g.status.resize(g.n + 1, 0);
	for (int i = 1; i <= g.n; i++)
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
            if(edge_cover[left]==edge_cover[right])
                right=right-1;
			if(left+1!=right ){
			    allwalks.push_back({left, right});
            }
		}
		left = left+1;
	}
    //when right reaches end of the walk

	return allwalks;
}

int main()
{
	freopen("Testcase.txt", "r", stdin);

	int n, m;
	cin >> n >> m;
	Graph g(n);

	int wt = 0, t = m;
	while (t--)
	{
		int u, v;
		cin >> u >> v;
		g.addedge(u, v, wt);
		wt++;
	}
	fclose(stdin);

	cout << "\nEdge cover walk- ";
	vector<int> wlk = edge_covering_walk(g, m);
	// for (auto it : wlk)
	// cout << it << " ";

	cout<<"\nEdgecover.size="<<wlk.size();

	cout<<"\nAll maximal Safewalks are- ";
	vector<pair<int,int>> allwalks=all_maximal_safewalk(g, wlk);

	cout<<"\nAllwalk.size="<<allwalks.size()<<"\n";
	for(auto it: allwalks){
	    cout<<"{"<<it.first<<"->"<<it.second<<"}\n";
		cout<<"\ns1-"<<wlk[it.first]<<" s2-"<<wlk[it.first+1]<<"\nd1-"<<wlk[it.second-1]<<" d2-"<<wlk[it.second];
	}

    cout<<"\nend";
	return 0;
}