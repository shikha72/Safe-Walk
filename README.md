# Safe-Walk

This repository depicts my work during summer internship, under the supervision of Dr. Shahbaz Khan, at IIT Roorkee (SPARK Internship programme). I would like to express my gratitude to him for his constant support and guidance during the project.

## Area of Research
Area of Research is application of dynamic graphs in Pan-genomics. 
Pangenome is a term referring to whole set of genes within a species and is used in the field of molecular biology and genetics. (Source: Internet)

When it comes to ‘Bioinformatics’- solving problems of biology using software tools and Information Technology, all these genetic information is represented in the form of graphs. ‘Genome Assembly’ is one such problem of this field, in which many small fragments of DNA sequences are combined together to form original chromosomes. 
In this process a problem is to find out parts that are definitely needed to be present in the solution, called safe. 

## Introduction
#### Problem Statement: 
Our problem is to dynamically verify the safety of a given walk for an edge covering walk of a strongly connected graph under edge insertions.

For incremental verification of safe walks, our intent was to use ‘The Hydrostructure’ model as introduced in – ‘The Hydrostructure: A Universal Framework for Safe and Complete Algorithms for Genome Assembly’. 

#### Hydrostructure Model: [1]
Given a walk ‘W’ of strongly connected graph, hydrostructure model classifies it into four categories- sea, cloud, vapor and river.

R-(W) - all nodes reaching end(W) without using start(W).<br>
R+(W) - all nodes reachable from start(W) without using end(W).<br>

Vapor - all nodes that are present in both R+(W) and R-(W).<br>
Cloud- all nodes in R-(W) that are not in vapor.<br>
Sea - all nodes that are in R+(W) that are not in vapor.<br>
River - all remaining nodes that are not in cloud, sea and vapor.<br>

where,<br>
start(W) - first edge of the walk and<br>
end(W) - last edge of the specified walk

## Work

#### Static Algorithm
It compute ‘the Hydrostructure’ for a given walk in strongly connected graph, using the definitions of Cloud, River, Vapor and Sea as provided in the paper. Computation takes linear time of O(m) where, ‘m’ stands for number of edges in the graph.

#### Trivial Dynamic Algorithm
It incrementally verify safety of the walk under edge insertions. After each update, it computes the Hydrostructure and then verify for its safety by checking whether it is bridge-like case (when Vapor(W) is an open path and river is non-empty) or avertible case (vapor(W) is G), giving an overhead of O(m) time complexity

#### Dynamic Algorithm
It incrementally verify the safety of the specified walk in strongly connected graph using the components of the Hydrostructure, which can be maintained in overall linear time- O(m) where, ‘m’ stands for number of edges in the graph.

## References
[1] [The Hydrostructure: a Universal Framework for Safe and Complete
Algorithms for Genome Assembly](https://arxiv.org/abs/2011.12635)
