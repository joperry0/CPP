// graph.cpp
// author:  Joseph Perry
// desc:    An example of how to use the Graph class defined in graph.h

#include "graph.h"

int main(){
  vector<tuple<int,int,float>> edges = { make_tuple(2, 1, 2.0),
                  make_tuple(3, 1, 7.3),
                  make_tuple(4, 3, 4.2),
                  make_tuple(5, 2, 19.7),
                  make_tuple(6, 1, 33.1),
                  make_tuple(7, 2, 108.9),
                  make_tuple(8, 6, 0.2),
                  make_tuple(9, 8, 8.4),
                  make_tuple(8, 4, 1.0),
                  make_tuple(10, 8, 9.0), //,
                  make_tuple(10, 9, 9.0) //,
                  // make_tuple(2, 5, 3.0) 
                };

  vector<tuple<int,int,float>> result;
  Graph graph(edges);

  graph.print_graph();

  graph.DFS(1);

  graph.BFS(1);

  if(graph.has_cycle())
    cout << "Cycle detected" << endl;
  else
    cout << "No cycle detected" << endl;

  if(graph.reachable(1, 5))
    cout << "1 is reachable to 5" << endl;
  else
    cout << "1 is not reachable to 5" << endl;

  graph.topological_sort();

  cout << "V = " << graph.V() << endl;
  cout << "E = " << graph.E() << endl;

  result = graph.mst_kruskal();

  for(vector<tuple<int,int,float>>::iterator mv=result.begin(); mv!=result.end(); ++mv)
    cout << get<0>(*mv) << " " << get<1>(*mv) << " " << get<2>(*mv) << endl;

  return 0;
}
