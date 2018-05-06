// Implementation of the KMeans Algorithm in parallel
// reference: http://mnemstudio.org/clustering-k-means-example-1.htm
//#define TIMER_USES_MICROSECONDS 1
#define TIMER_USES_MICROSECONDS 1
#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include "timer.hh"
#include <fstream>
#include <string>
#include <sstream>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;


using namespace std;

class Point
{
private:
	long int id_point, id_cluster;
	vector<unsigned long long> values;
	long int total_values;
	string name;

public:
	Point(long int id_point, vector<unsigned long long>& values, string name = "")
	{
		this->id_point = id_point;
		total_values = values.size();

		for(int i = 0; i < total_values; i++)
			this->values.push_back(values[i]);

		this->name = name;
		id_cluster = -1;
	}

	long int getID()
	{
		return id_point;
	}

	void setCluster(long int id_cluster)
	{
		this->id_cluster = id_cluster;
	}

	long int getCluster()
	{
		return id_cluster;
	}

	unsigned long long getValue(long int index)
	{
		return values[index];
	}

	long int getTotalValues()
	{
		return total_values;
	}

	void addValue(long double value)
	{
		values.push_back(value);
	}

	string getName()
	{
		return name;
	}
};

class Cluster
{
private:
	long int id_cluster;
	vector<unsigned long long> central_values;
	vector<Point> points;

public:
	Cluster(long int id_cluster, Point point)
	{
		this->id_cluster = id_cluster;

		long int total_values = point.getTotalValues();

		for(int i = 0; i < total_values; i++)
			central_values.push_back(point.getValue(i));

		points.push_back(point);
	}

	void addPoint(Point point)
	{
		points.push_back(point);
	}

	bool removePoint(long int id_point)
	{
		long int total_points = points.size();

		for(int i = 0; i < total_points; i++)
		{
			if(points[i].getID() == id_point)
			{
				points.erase(points.begin() + i);
				return true;
			}
		}
		return false;
	}

	unsigned long long getCentralValue(long int index)
	{
		return central_values[index];
	}

	void setCentralValue(long int index, long double value)
	{
		central_values[index] = value;
	}

	Point getPoint(long int index)
	{
		return points[index];
	}

	long int getTotalPoints()
	{
		return points.size();
	}

	long int getID()
	{
		return id_cluster;
	}
};

class KMeans
{
private:
	long int K; // number of clusters
	long int total_values, total_points, max_iterations;
	vector<Cluster> clusters;

	// return ID of nearest center (uses euclidean distance)
	long int getIDNearestCenter(Point point)
	{
		long double sum = 0.0, min_dist;
		long int id_cluster_center = 0;

		for(int i = 0; i < total_values; i++)
		{
			sum += pow(clusters[0].getCentralValue(i) -
					   point.getValue(i), 2.0);
		}

		min_dist = sqrt(sum);

		for(int i = 1; i < K; i++)
		{
			long double dist;
			sum = 0.0;

			for(int j = 0; j < total_values; j++)
			{
				sum += pow(clusters[i].getCentralValue(j) -
						   point.getValue(j), 2.0);
			}

			dist = sqrt(sum);

			if(dist < min_dist)
			{
				min_dist = dist;
				id_cluster_center = i;
			}
		}

		return id_cluster_center;
	}

public:
	unsigned long long sumaError = 0.0;

	KMeans(long int K, long int total_points, long int total_values, long int max_iterations)
	{
		this->K = K;
		this->total_points = total_points;
		this->total_values = total_values;
		this->max_iterations = max_iterations;
	}

	unsigned long long getSumaError() {
    	return sumaError;
  	}

	void run(vector<Point> & points)
	{
		if(K > total_points)
			return;

		vector<long int> prohibited_indexes;
		sumaError = 0.0;
		// choose K distinct values for the centers of the clusters
		for(int i = 0; i < K; i++)
		{
			while(true)
			{
				long int index_point = rand() % total_points;

				if(find(prohibited_indexes.begin(), prohibited_indexes.end(),
						index_point) == prohibited_indexes.end())
				{
					prohibited_indexes.push_back(index_point);
					points[index_point].setCluster(i);
					Cluster cluster(i, points[index_point]);
					clusters.push_back(cluster);
					break;
				}
			}
		}

		long int iter = 1;

		while(true)
		{
			bool done = true;
			// associates each point to the nearest center
			for(int i = 0; i < total_points; i++)
			{
				long int id_old_cluster = points[i].getCluster();
				long int id_nearest_center = getIDNearestCenter(points[i]);

				if(id_old_cluster != id_nearest_center)
				{
					if(id_old_cluster != -1)
						clusters[id_old_cluster].removePoint(points[i].getID());

					points[i].setCluster(id_nearest_center);
					clusters[id_nearest_center].addPoint(points[i]);
					done = false;
				}
			}

			// recalculating the center of each cluster
			
			for(int i = 0; i < K; i++)
			{	


				for(int j = 0; j < total_values; j++)
				{
					long int total_points_cluster = clusters[i].getTotalPoints();
					unsigned long long sum = 0.0;

					if(total_points_cluster > 0)
					{
						//Se paraleliza la forma de calcular para cada cluster la media
						#pragma omp for schedule(static,2)
						for(int p = 0; p < total_points_cluster; p++){
							sum += clusters[i].getPoint(p).getValue(j); 
						}

						
						
						for(int p = 0; p < total_points_cluster; p++)
						{
							sumaError += clusters[i].getPoint(p).getValue(j) - (sum / total_points_cluster);
						}

							
						clusters[i].setCentralValue(j, sum / total_points_cluster);
					}
				}
			}

			if(done == true || iter >= max_iterations)
			{
				cout << "Break in iteration " << iter << "\n\n";
				cout << "\n";

				cout << "Square error mean: "<< sumaError << "\n\n";
				break;
			}

			iter++;
		}

		// shows elements of clusters
		//for(int i = 0; i < K; i++)
		//{
		//	long int total_points_cluster =  clusters[i].getTotalPoints();

		//	cout << "Cluster " << clusters[i].getID() + 1 << endl;
		//	for(int j = 0; j < total_points_cluster; j++)
		//	{
		//		cout << "Point " << clusters[i].getPoint(j).getID() + 1 << ": ";
		//		for(int p = 0; p < total_values; p++)
		//			cout << clusters[i].getPoint(j).getValue(p) << " ";
//
//				string point_name = clusters[i].getPoint(j).getName();

//				if(point_name != "")
//					cout << "- " << point_name;

//				cout << endl;
//			}

//			cout << "Cluster values: ";

//			for(int j = 0; j < total_values; j++)
//				cout << clusters[i].getCentralValue(j) << " ";

//			cout << "\n\n";
//		}
	}
};

int main(int argc, char *argv[]) {
	const string endpoint = "tcp://*:4242";

	// initialize the 0MQ context
	context context;

	// generate a pull socket
	socket_type type = socket_type::rep;
	socket socket (context, type);

	// bind to the socket
	cout << "Binding to " << endpoint << "..." << endl;
	socket.bind(endpoint);

	// receive the message
	cout << "Receiving message..." << endl;
	message params;
	// decompose the message 
	socket.receive(params);

	int K, has_name;
	params >> K >> has_name;

	cout << "Received K:" << K << " and has_name: " << has_name << endl;

	long int total_points, total_values, max_iterations, another, another_has;
	ifstream filein("Netflix.txt");
	string line;
	getline(filein, line);
	//cout<<line<<endl;		
	istringstream iss(line);
	string subs;
	for(int i=0;i<5;i++)
	{
	    if(i==0) iss >> total_points;
		if(i==1) iss >> total_values;
		if(i==2) iss >> K;
		if(i==3) iss >> max_iterations;
		if(i==4) iss >> has_name;
	}	
	//cin >> total_points >> total_values >> K >> max_iterations >> has_name;
	vector<Point> points;
	string point_name;
	//Se paraleliza la creación de cada punto con sus atributos en el plano
	#pragma omp for schedule(static,2)
	for(int i = 0; i < total_points; i++)
	{
		vector<unsigned long long> values;
		getline(filein, line);
		//cout<<line<<endl;
		istringstream iss(line);
		for(int j = 0; j < total_values; j++)
		{
			long double value;
			iss >> value;
			//cin >> value;
			values.push_back(value);
		}

		if(has_name)
		{
			iss >> point_name;
			//cin >> point_name;
			Point p(i, values, point_name);
			points.push_back(p);
		}
		else
		{
			Point p(i, values);
			points.push_back(p);
		}
	}

	cout << "Starting Kmeans..." <<endl;
	srand (time(NULL));
	Timer t1;
	KMeans kmeans(K, total_points, total_values, max_iterations);
	kmeans.run(points);

	long long t1time = t1.elapsed();
	cout << "Parallel Kmeans "<< t1time<<" ms"<<endl;

	cout << "Finished." << endl;

	int error = kmeans.getSumaError();
	cout << error << endl;
	message answer;
	answer << error << int(t1time);
	socket.send(answer);

	return 0;
}

