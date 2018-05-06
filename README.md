<h3>Example that contains data point name (Netlix movies's name): </h3>


105 2 7 100 1 <br />
1488844 3 1 <br />
822109 5 1 <br />
885013 4 2 <br />
30878 4 8 <br />
823519 3 1 <br />
893988 3 14 <br />
124105 4 17 <br />
(...)<br />

"1488844" is an anonymous user encrypted <br />
"3" The integral rating of the  (1 to 5) <br />
"1" The number of the movie <br />


<h2>Quick analysis of the data </h2>

<h3>Sequential</h3>

After compiling the `Kmeans sequential` with the `combined_1.txt` we obtain that putting manual K clusters for the clustering of the data is obtained: <br />

<h3>K = 5 && 100 iterations</h3>

Square error mean: 2333097<br />

Cluster 1 <br />
Numbers of points in the cluster: 6079 <br />
Cluster values: 1191793 3 6 <br />

Cluster 2 <br />
Numbers of points in the cluster: 4124 <br />
Cluster values: 1168937 3 6 <br />

Cluster 3 <br />
Numbers of points in the cluster: 3841
Cluster values: 1708601 3 7 <br />

Cluster 4 <br />
Numbers of points in the cluster: 723 <br />
Cluster values: 1099236 2 4 <br />

Cluster 5 <br />
Numbers of points in the cluster: 233 <br />
Cluster values: 1748914 2 4 <br />

<h3>Parallel</h3>

Compiling the `Kmeans parallel` with the `combined_1.txt` we obtain that putting manual K clusters for the clustering of the data is obtained: 
<br />

<h3>K = 5 && 100 iterations</h3>

Square error mean: 2337811 <br />

Cluster 1 <br />
Numbers of points in the cluster: 5838 <br />
Cluster values: 587162 3 6 <br />

Cluster 2 <br />
Numbers of points in the cluster: 2667 <br />
Cluster values: 1750646 3 5 <br />

Cluster 3 <br />
Numbers of points in the cluster: 2910 <br />
Cluster values: 1448558 3 6 <br />

Cluster 4 <br />
Numbers of points in the cluster: 1346 <br />
Cluster values: 1981024 2 5 <br />

Cluster 5 <br />
Numbers of points in the cluster: 2239 <br />
Cluster values: 2167038 3 7 <br />

Parallel Kmeans 538 s <br />



<h4>#####</h4>


The euclidean distance was used for to calculate the distance of each data point for the centroid of cluster.

The algorithm stops by maximum number of iterations or if no data point exchange cluster.

</p>

## Installing and Running

1. Make sure that you have `g++` installed on your computer.
2. Fork and clone this repo on your computer.
3. `cd` into the root directory and run `g++ -std=c++11 -o skmeans sKmeans.cc`.



This algotihm is based on the next repository and all rights are reserved to him
https://github.com/marcoscastro/kmeans




