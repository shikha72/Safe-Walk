//Given a walk in strongly connected graph, it computes the hydrostructure

#include <bits/stdc++.h>
using namespace std;

class Graph
{
public:
    int n;//no of nodes in Graph
    list<int> *adj;
    list<int> *revadj;

    vector<bool> cloud;
    vector<bool> sea;
    vector<char> status; // r-river/ s-sea/ c-cloud/ v-vapor

    Graph(int n)
    {
        this->n = n;
        adj = new list<int>[n + 1];
        revadj = new list<int>[n + 1];

        vector<bool> temp(n + 1, 0);
        cloud = temp;
        sea = temp;

        vector<char> temp1(n + 1, 'r');
        status = temp1;
    }

    void addedge(int u, int v)
    {
        adj[u].push_back(v);
        revadj[v].push_back(u);
    }
};
void print_cloud(Graph g)
{
    cout << "\nCloud-";
    for (int i = 1; i <= g.cloud.size(); i++)
    {
        if (g.cloud[i] == 1)
            cout << i << " ";
    }
}
void print_sea(Graph g)
{
    cout << "\nSea-";
    for (int i = 1; i <= g.sea.size(); i++)
    {
        if (g.sea[i] == 1)
            cout << i << " ";
    }
}
void print_vapor(Graph g){
    cout<<"\nVapor-";
    for(int i=1;i<g.status.size();i++){
        if(g.status[i]=='v')
        cout<<i<<" ";
    }
}
void print_river(Graph g){
    cout<<"\nRiver-";
    for(int i=1;i<g.status.size();i++){
        if(g.status[i]=='r')
        cout<<i<<" ";
    }
}
void compute_sea(Graph &g, int s1, int d2, int d1)
{
    // all nodes reachable from start(W) without using W as subwalk
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

        if (node == d2 && par==d1)
        continue;

        g.sea[node] = 1;
        for (auto it : g.adj[node])
        {
            if(it==d2 && node==d1)
            continue;
            if (!vis[it])
            {
                q.push({it, node});
                vis[it] = 1;
            }
        }
    }
    print_sea(g);
}
void compute_cloud(Graph &g, int d1, int s1, int s2)
{
    // all nodes reaching END(W) without using W as subwalk
    vector<bool> vis(g.n + 1, 0);

    int par = -1;
    queue<pair<int, int>> q;
    q.push({d1, par});
    vis[d1] = 1;

    while (!q.empty()){
        int node = q.front().first;
        par = q.front().second;
        q.pop();

        if (node == s1 && par==s2)
        continue;

        g.cloud[node] = 1;
        for (auto it : g.revadj[node]){
            if(it==s1 && node==s2)
            continue;
            if (!vis[it]){
                q.push({it,node});
                vis[it] = 1;
            }
        }
    }
    print_cloud(g);
}

void compute_hydrostructure(Graph &g, int s1, int s2, int d1, int d2)
{
    compute_sea(g, s1, d2, d1);
    // SEA- [R+(W)]- all nodes reachable from start(W) without using W as its subwalk
    compute_cloud(g, d1, s1, s2);
    // CLOUD- [R-(W)]- all nodes reaching END(W) without using W as its subwalk

    for (int i = 1; i <= g.n; i++){
        // VAPOR- [R+ ⋂ R-] 
        if (g.cloud[i] && g.sea[i])
            g.status[i] = 'v';

        // CLOUD- [R-(W)]
        else if (g.cloud[i])
            g.status[i] = 'c';

        // SEA- [R+(W)]
        else if (g.sea[i])
            g.status[i] = 's';

        // RIVER- G-[R+(W) ⋃ R-(W)]
        else
            g.status[i] = 'r';
    }
    print_vapor(g);
    print_river(g);
}

int main()
{
    freopen("testcase.txt", "r", stdin);
    int n,m;
    cin>>n>>m;
    Graph g(n);

    while(m--){
        int u,v;
        cin>>u>>v;
        g.addedge(u,v);
    }

    //s1 and s2 represents head and tail nodes of start(W) edge of the walk
    //d1 and d2 represents head and tail nodes of end(W) edge of the wallk
    int s1, s2, d1, d2;
    cin>>s1>>s2>>d1>>d2;

    compute_hydrostructure(g, s1, s2, d1, d2);

    return 0;
}