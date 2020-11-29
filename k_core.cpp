/*
Change the dataset dir and default parameter before use.
Input data required the nodes are numbered from 1 to n.
*/  

#include <iostream>
#include <cstdio>
#include <ctime>
using namespace std;

//const char* dir = "C:\\Working\\graph\\dataset\\Email-Enron.txt";
//const int max_node = 37e3;
//const int max_edge = .2e6;

const char* dir = "C:\\Working\\graph\\dataset\\graph500-scale18-ef16_adj.edges";
const int max_node = 175e3;
const int max_edge = 8e6;


clock_t startTime, restartTime, endTime;
FILE *dataset;	

int eq[max_edge*2];
int eq_next[max_edge*2];
int eq_head[max_node*2];
int eq_tail[max_node*2];
int edge_count = 0;

int d[max_node];
int node_count = 0;
int node_maxid = 0; 
int dq_head[max_node];
int dq_tail[max_node];
int dq_front[max_node];
int dq_next[max_node];

int c[max_node];
int kcore = 0;

void init();
void read_data();
void show_graph(); 
void init_dq();
void calculation_kcore();

void init()
{
	for (int i = 0; i<max_node; i++)
	{
		c[i] = -1;
		d[i] = 0;
		eq_head[i] = -1;
		eq_tail[i] = -1;
		dq_head[i] = -1;
		dq_tail[i] = -1;
		dq_front[i] = -1;
		dq_next[i] = -1; 
	}
	
	for (int i = 0; i<max_edge; i++)
	{
		eq[i] = -1;
		eq_next[i] = -1;
	}
	
	endTime = clock();
	cout << "Initialization       uses " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << 's' << endl;
	restartTime = endTime;
}

void read_data()
{
	dataset = fopen(dir, "r");
	
	int o, p, l;
	while (fscanf(dataset, "%d %d", &o, &p) != EOF)
	{
		node_maxid = max(node_maxid, max(o,p));
		o = o-1;
		p = p-1;
		if (d[o] == 0)
		{
		 	node_count = node_count + 1;
		}
		d[o] = d[o]+1;
		
		
		l = edge_count;
		eq[l] = p;
		if (eq_head[o] == -1)
		{
			eq_head[o] = l;
			eq_tail[o] = l;
		}
		else
		{
			eq_next[eq_tail[o]] = l;
			eq_tail[o] = l;
		}
		edge_count = edge_count + 1;
		
//		l = edge_count;
//		eq[l] = o;
//		if (eq_head[p] == -1)
//		{
//			eq_head[p] = l;
//			eq_tail[p] = l;
//		}
//		else
//		{
//			eq_next[eq_tail[p]] = l;
//			eq_tail[p] = l;
//		}
//		edge_count = edge_count + 1;
				
	}
	
//	cout << "nodes count = " << node_count << endl;
//	cout << "edges count = " << edge_count << endl; 
	
	if (node_maxid > node_count)
		cout << endl << "****Mistake may occur because node_maxid bigger to node_count!****" << endl << endl;
		
			
	endTime = clock();
	cout << "Read data            uses " <<(double)(endTime - restartTime) / CLOCKS_PER_SEC << 's' << endl;
	restartTime = endTime;
}

void show_graph()
{
	int l;
	
//	for (int i=0; i<node_count; i++)
//		printf("(%d %d) ", i, d[i]);
//	cout << endl;
//	
//	for (int i=0; i<edge_count; i++)
//		printf("%3d  %3d %3d %3d %3d\n", i, eq[i], eq_next[i], eq_head[i], eq_tail[i]);
//
//	for (int i=0; i<node_count; i++)
//		printf("%d (%d %d %d %d)\n", i, dq_head[i], dq_tail[i], dq_front[i], dq_next[i]);
//	cout << endl;
	
	for (int i=0; i<node_count; i++)
	{
		printf("node %d  degree = %d(%d)  ", i, d[i], c[i]);
		l = eq_head[i];
		while (l != -1)
		{
			printf("%d ", eq[l]);
			l = eq_next[l];
		}
		cout << endl;
	}
	cout << endl;
	
	for (int i=0; i<node_count; i++)
	{
		printf("queue %d(degree = %d)  ", i, i);
		l = dq_head[i];
		while (l != -1)
		{
			printf("%d ", l);
			l = dq_next[l];
		}
		cout << endl;
	}
	cout << endl;
}

void init_dq()
{
	int l;
	for (int i=0; i<node_count; i++)
	{
		l = d[i];
		if (dq_head[l] == -1)
		{
			dq_head[l] = i;
			dq_tail[l] = i;
		}
		else
		{
			dq_next[dq_tail[l]] = i;
			dq_front[i] = dq_tail[l];
			dq_tail[l] = i;
		}
	}
	
	
	endTime = clock();
	cout << "Initialization queue uses " <<(double)(endTime - restartTime) / CLOCKS_PER_SEC << 's' << endl;
	restartTime = endTime;
}

void calculation_kcore()
{
	int i, l, l_id, dl, v, l_front, l_next;
	for (int n=0; n<node_count; n++)
	{
		
		i = 0;
		while (dq_head[i] == -1) i++;
		kcore = max(kcore, i);
		
		v = dq_head[i];
		c[v] = kcore;
		if (dq_tail[i] != v)
		{
			dq_head[i] = dq_next[v];
			dq_front[dq_head[i]] = -1;
		}
		else
		{
			dq_head[i] = -1;
			dq_tail[i] = -1;
		}
		dq_next[v] = -1;
		dq_front[v] = -1;
		
		
		l_id = eq_head[v];
		while (l_id != -1)
		{
			l = eq[l_id];
			dl = d[l];
			if(c[l] == -1)
			{
				if (dq_front[l] == -1)
				{
					if (dq_next[l] == -1)	
					{
						dq_head[dl] = -1;
						dq_tail[dl] = -1;
					}
					else
					{
						l_next = dq_next[l];
						dq_head[dl] = l_next;
						dq_front[l_next] = -1;
					}
				}
				else
				{
					if (dq_next[l] == -1)
					{
						l_front = dq_front[l];
						dq_tail[dl] = l_front;
						dq_next[l_front] = -1;
					}
					else
					{
						l_front = dq_front[l];
						l_next = dq_next[l];
						dq_next[l_front] = l_next;
						dq_front[l_next] = l_front;
					}
					
				}
				
				d[l] = d[l]-1;
				dl = dl - 1;
				if (dq_head[dl] == -1)
				{
					dq_head[dl] = l;
					dq_tail[dl] = l;
					dq_front[l] = -1;
					dq_next[l] = -1;
				}
				else
				{
					dq_front[l] = dq_tail[dl];
					dq_next[dq_tail[dl]] = l;
					dq_next[l] = -1;
					dq_tail[dl] = l;
				}
		
			}
			
			l_id = eq_next[l_id];
		}
	}
	
	endTime = clock();
	cout << "Calculation k-core   uses " << (double)(endTime - restartTime) / CLOCKS_PER_SEC << 's' << endl;
	restartTime = endTime;
}

int main()
{
//	freopen("output.txt", "w", stdout);
	startTime = clock();
	init();
	read_data();
	init_dq();
		
	calculation_kcore();
	
//	show_graph();
	
	endTime = clock();
	cout << "Total time                " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << 's' << endl;
	restartTime = endTime;
		
//	for (int i=0; i<node_count; i++)
//		cout << c[i] << ' ';
	cout << endl << "kcore = " << kcore << endl;
	
}
