Given a strongly connected Graph and a walk, 'hydrostructure.cpp' computes hydrostructure of the specified walk.
It means- categorise all nodes of the graph into four categories on the basis of reachability conditions: Cloud, Vapor, Sea and River.

Definitions of all these are: 

R-(W) - all nodes reaching end(W) without using start(W).
R+(W) - all nodes reachable from start(W) without using end(W).

Vapor - all nodes that are present in both R+(W) and R-(W)
Cloud- all nodes in R-(W) that are not in vapor
Sea - all nodes that are in R+(W) that are not in vapor
River - all remaining nodes that are not in cloud, sea and vapor.

where,
start(W) - first edge of the walk and
end(W) - last edge of the specified walk

hydrostructure.png depicts the model where arrows represent the cases in which edge insertion is safe.

Reference- "The Hydrostructure: a Universal Framework for Safe and Complete
Algorithms for Genome Assembly" Link: https://arxiv.org/abs/2011.12635
