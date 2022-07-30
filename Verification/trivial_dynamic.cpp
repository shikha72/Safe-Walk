// TRIVIAL DYNAMIC ALGORITHM
// For incremental verification of safety of walk under edge insertions.
// After each update, it computes the Hydrostructure and then verify for its safety

#include <bits/stdc++.h>
using namespace std;

class Graph
{
public:
    int n;
    list<int> *adj;
    list<int> *revadj;

    vector<bool> cloud;
    vector<bool> sea;
    vector<char> status;

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
// void print_cloud(Graph g)
// {
//     cout << "\nCloud-";
//     for (int i = 1; i <= g.cloud.size(); i++)
//     {
//         if (g.cloud[i] == 1)
//             cout << i << " ";
//     }
// }
// void print_sea(Graph g)
// {
//     cout << "\nSea-";
//     for (int i = 1; i <= g.sea.size(); i++)
//     {
//         if (g.sea[i] == 1)
//             cout << i << " ";
//     }
// }
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

        if (node == d2 && par == d1)
            continue;

        g.sea[node] = 1;
        for (auto it : g.adj[node])
        {
            if (it == d2 && node == d1)
                continue;
            if (!vis[it])
            {
                q.push({it, node});
                vis[it] = 1;
            }
        }
    }
    // print_sea(g);
}
void compute_cloud(Graph &g, int d1, int s1, int s2)
{
    // all nodes reaching END(W) without using W as subwalk
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
            if (it == s1 && node == s2)
                continue;
            if (!vis[it])
            {
                q.push({it, node});
                vis[it] = 1;
            }
        }
    }
    // print_cloud(g);
}
void compute_hydrostructure(Graph &g, int s1, int s2, int d1, int d2)
{
    compute_sea(g, s1, d2, d1);
    // SEA- [R+(W)]- all nodes reachable from start(W) without using W as its subwalk
    compute_cloud(g, d1, s1, s2);
    // CLOUD- [R-(W)]- all nodes reaching END(W) without using W as its subwalk

    for (int i = 1; i <= g.n; i++)
    {
        // CLOUD- [R-(W)]
        if (g.cloud[i] && g.sea[i])
            g.status[i] = 'v';

        // SEA- [R+(W)]
        else if (g.cloud[i])
            g.status[i] = 'c';

        // VAPOR- [R+ ⋂ R-]
        else if (g.sea[i])
            g.status[i] = 's';

        // RIVER- G-[R+(W) ⋃ R-(W)]
        else
            g.status[i] = 'r';
    }
}

//                    2 cases
//                /              \ 
//               /                \   
//          Trivial             Non-trivial
//                             /           \ 
//                            /             \  
//                       bridge               avertible
//                  vapor(W)-open path         vapor(W)-G
//                     (safe)                   (unsafe)
//
// Moreover, for being k-circular safe, river must be non-empty too apart from being vapor(W)-open path

int verify(Graph &g, int u, int v, int s1, int s2, int d1, int d2)
{
    g.addedge(u, v);
    compute_hydrostructure(g, s1, s2, d1, d2);

    // IF RIVER EMPTY AND R+(W) IS G THEN AVERTIBLE CASE -> HYDROSTRUCTURE CAN"T BE COMPUTED
    // VAPOR(W) IS G -> UNSAFE
    bool riv_nonepty = 0; // assuming river empty
    for (int i = 1; i <= g.status.size(); i++)
    {
        if (g.status[i] == 'r')
        {
            riv_nonepty = 1; // yes river is not empty
            break;
        }
    }

    bool open_path = 0;
    for (int i = 1; i <= g.status.size(); i++)
    {
        if (g.status[i] != 'v' && g.status[i] != 'r')
        {
            open_path = 1;
            break;
        }
    }
    return (open_path && riv_nonepty);
}

int main()
{
    // auto start = std::chrono::high_resolution_clock::now();

    freopen("testcase.txt", "r", stdin);
    int n, m;
    cin >> n >> m;
    Graph g(n);

    while (m--)
    {
        int u, v;
        cin >> u >> v;
        g.addedge(u, v);
    }

    int s1, s2, d1, d2;
    cin >> s1 >> s2 >> d1 >> d2;

    auto start = std::chrono::high_resolution_clock::now();

    compute_hydrostructure(g, s1, s2, d1, d2);

    // INCREMENTAL VERIFICATION OF HYDROSTRUCTURE OF WALK 'W'

    // cout<<"\nDo you want to add an edge?";
    // cout<<" [Press 1 to continue & 0 to exit] ";
    
    bool key;
    cin >> key;
    while (key)
    {
        // cout<<"\nEnter node [u,v] - ";
        int u, v;
        cin >> u >> v;

        int check = verify(g, u, v, s1, s2, d1, d2); // O(n+m)

        switch (check)
        {
        case 1:
            cout << "\nWalk 'W' is still safe\n";
            break;
        case 0:
            cout << "\nWalk 'W' is unsafe now\n";
            break;
        default:
            cout << "\nERROR\n";
            // when input node doesn't exist or when adding edge b/w specified node is not possible
            break;
        }

        if (check != 0)
        {
            // cout<<"\nDo you want to add another edge?";
            // cout<<" [Press 1 to continue & 0 to exit] ";
            cin >> key;
        }
        else
        {
            key = 0;
        }

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        cout << duration.count() << " milliseconds" << endl;
    }
    return 0;
}