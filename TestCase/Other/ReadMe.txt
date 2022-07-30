Since, Generation of Testcase using 'Generate.cpp' needs debugging, I tried generating testcases using same approach in following manner:

gen_graph.cpp -> It generates strongly connected graph of 1000 nodes.

gen_safeWalk.cpp -> It generates a safewalk using same approach as in Generate.cpp but, it may or maynot be safe so needs to be verified separately.

verify_safety.cpp -> It verifies if walk generated is safe.

gen_incedge.cpp -> It generates random edges that doesn't exist in the graph for incremental verification of walk.
