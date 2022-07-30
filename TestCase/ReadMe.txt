Generate.cpp -> It intends to generate testcase as required by trivial dynamic and dynamic algorithm for incremental verification of safety of walk under edge insertions.

Approach used ->
 
[1] Generate a Strongly connected graph

	a) Generate DAG by adding a random edge 'a->b' such that:
		- a is not reachable from b, 
		- it doesn't already exist,
		- it is not making a self-loop

	b) Compute all sources and Sinks and add edges such that no sources and sink left.

[2] Generate a random safe walk

	a) Compute an edge-covering walk
	b) Apply two pointer algorithm and checks for safety of the walk
	c) Return head and tail nodes of starting and ending edge of the safewalk

[3] Generate random edges that doesn't exist in the graph.
	 (to verify if their addition makes walk unsafe or walk remains safe) 

But, it has some bugs. Some pointers, I can think of-
- In generating safe-walk, a problem is when tail node of last edge is equal to the head node of starting edge, 
	such walk is being reported safe irrespective of being actually unsafe.
- Computation of edge covering walk.
- When combining all codes together.
