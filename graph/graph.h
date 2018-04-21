// graph.h
// author:  Joseph Perry
// desc:    Implements a Graph class with nodes and edges with various graph algorithms

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <stack>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

enum class Direction : int { In, Out };

class Graph {
  private:
    // forward reference
    class Node;
    class Edge {
      private:
        Node *node1, *node2;
        float weight;
      public:
        // unweighted edge constructor
        Edge(Node *node1, Node *node2) : node1(node1), node2(node2), weight(1.0) {}

        // weighted edge constructor
        Edge(Node *node1, Node *node2, float weight) : node1(node1), node2(node2), weight(weight) {}

        friend class Node;
        friend class Graph;
    };
    class Node {
      private:
        int id;
        float weight;
        int tree_id;
        int tree_rank;
        float d;
        Node *pi;
        map<int, Edge*> in;
        map<int, Edge*> out;
      public:
        // unweighted node constructor
        Node(int id) : id(id), weight(1.0), tree_id(id), tree_rank(0) {}

        // weighted node constructor
        Node(int id, float weight) : id(id), weight(weight), tree_id(id), tree_rank(0) {}

        // adds unweighted edge to both this node and other node
        Edge* add_edge(Node* other, Direction direction){
          Edge *edge = new Edge(this, other);

          if(direction == Direction::In){
            in[other->id] = edge;
            other->out[id] = edge;
          } else {
            out[other->id] = edge;
            other->in[id] = edge;
          }
        }

        // adds weighted edge to both this node and other node
        Edge* add_edge(Node* other, float weight, Direction direction){
          Edge *edge = new Edge(this, other, weight);

          if(direction == Direction::In){
            in[other->id] = edge;
            other->out[id] = edge;
          } else {
            out[other->id] = edge;
            other->in[id] = edge;
          }
        }

        // removes edge from both nodes
        void remove_edge(Node* node, Direction direction){
          Edge *edge;
          if(direction == Direction::In){
            edge = in[node->id];
            in.erase(node->id);
            node->out.erase(id);
            delete edge;
          } else {
            edge = out[node->id];
            out.erase(node->id);
            node->in.erase(id);
            delete edge;
          }
        }

        // getter for number of edges coming into this node
        size_t inline indegree(){ return in.size(); }

        // getter for number of edges coming out of this node
        size_t inline outdegree(){ return out.size(); }

        friend class Graph;
    };

    map<int, Node*> nodes;
    vector<Edge*> edges;

  public:
    // unweighted graph constructor
    Graph(vector<tuple<int,int>>);

    // weighted graph constructor
    Graph(vector<tuple<int,int,float>>);

    // helper functions - prints the entire graph
    void print_graph();
    bool has_cycle();
    bool cycle_util(int, map<int, bool> &, map<int, bool> &);
    bool reachable(int,int);
    void DFS(int);
    void BFS(int);
    void topological_sort();

    // minimum spanning tree - kruskal's algorithm
    vector<tuple<int,int,float>> mst_kruskal();
    int mst_find_set(Node*);
    void mst_union(Node*, Node*);
    void mst_link(Node*, Node*);

    // getter for number of nodes in this graph
    size_t inline V(){ return nodes.size(); }

    // getter for number of edges in this graph
    size_t inline E(){ return edges.size(); }
};

// unweighted graph constructor
Graph::Graph(vector<tuple<int,int>> edges){
  int id1, id2;
  Node *node1, *node2;

  for(vector<tuple<int,int>>::iterator it=edges.begin(); it!=edges.end(); ++it){
    id1 = get<0>(*it);
    id2 = get<1>(*it);

    if(nodes.count(id1) == 0){
      node1 = new Node(id1);
      nodes[id1] = node1;
    } else {
      node1 = nodes[id1];
    }

    if(nodes.count(id2) == 0){
      node2 = new Node(id2);
      nodes[id2] = node2;
    } else {
      node2 = nodes[id2];
    }

    this->edges.push_back(node1->add_edge(node2, Direction::In));
  }
}

// weighted graph constructor
Graph::Graph(vector<tuple<int,int,float>> edges){
  int id1, id2;
  float weight;
  Node *node1, *node2;

  for(vector<tuple<int,int,float>>::iterator it=edges.begin(); it!=edges.end(); ++it){
    id1 = get<0>(*it);
    id2 = get<1>(*it);
    weight = get<2>(*it);

    if(nodes.count(id1) == 0){
      node1 = new Node(id1);
      nodes[id1] = node1;
    } else {
      node1 = nodes[id1];
    }

    if(nodes.count(id2) == 0){
      node2 = new Node(id2);
      nodes[id2] = node2;
    } else {
      node2 = nodes[id2];
    }

    this->edges.push_back(node1->add_edge(node2, weight, Direction::In));
  }
}

// depth-first search starting at input id
// outputs the id of nodes encountered to cout
void Graph::DFS(int id){
  stack<Node*> st;
  map<int, bool> visited;
  Node *node;

  node = nodes[id];

  st.push(node);

  while(!st.empty()){
    node = st.top();
    st.pop();

    if(visited.count(node->id) == 0){
      visited[node->id] = true;
      for(map<int, Edge*>::reverse_iterator et=node->out.rbegin(); et!=node->out.rend(); ++et){
        st.push(nodes[et->first]);
      }
      cout << node->id << " ";
    }
  }
  cout << endl;
}

// breadth-first search starting at input id
// outputs the id of nodes encountered to cout
void Graph::BFS(int id){
  queue<Node*> qu;
  map<int, bool> visited;
  Node *node;

  node = nodes[id];

  qu.push(node);

  while(!qu.empty()){
    node = qu.front();
    qu.pop();

    if(visited.count(node->id) == 0){
      visited[node->id] = true;
      for(map<int, Edge*>::iterator et=node->out.begin(); et!=node->out.end(); ++et){
        qu.push(nodes[et->first]);
      }
      cout << node->id << " ";
    }
  }
  cout << endl;
}

// determines if the graph has a cycle
// outputs true if cycle, false if otherwise
bool Graph::has_cycle(){
  map<int, bool> recurse;
  map<int, bool> visited;
  Node *node;

  for(map<int,Node*>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
    if(visited.count(it->first) == 0)
      if(cycle_util(it->first, visited, recurse))
        return true;

  return false;
}

bool Graph::cycle_util(int nid, map<int, bool> &visited, map<int, bool> &recurse){
  visited[nid] = true;
  recurse[nid] = true;

  for(map<int,Edge*>::iterator it=nodes[nid]->out.begin(); it!=nodes[nid]->out.end(); ++it){
    if(visited.count(it->first) == 0)
      if(cycle_util(it->first, visited, recurse))
        return true;
      else if(recurse[it->first])
        return true;
  }
  recurse[nid] = false;
  return false;
}
// determines if the node with id2 can be reached from the node with id1
// outputs true if a path exists, false if otherwise
bool Graph::reachable(int id1, int id2){
  stack<Node*> st;
  map<int, bool> visited;
  Node *node = nodes[id1];

  st.push(node);

  while(!st.empty()){
    node = st.top();
    st.pop();

    if(node->id == id2){
      return true;
    } else if(visited.count(node->id) == 0){
      visited[node->id] = true;
      for(map<int, Edge*>::reverse_iterator et=node->out.rbegin(); et!=node->out.rend(); ++et){
        st.push(nodes[et->first]);
      }
    }
  }

  return false;
}

// topological sort
// outputs the nodes in topologically sorted order to cout
void Graph::topological_sort(){
  stack<Node*> st;
  stack<int> result;
  map<int, bool> visited;
  Node *node;

  node = nodes.begin()->second;

  st.push(node);

  while(!st.empty()){
    node = st.top();
    st.pop();

    if(visited.count(node->id) == 0){
      visited[node->id] = true;
      for(map<int, Edge*>::reverse_iterator et=node->out.rbegin(); et!=node->out.rend(); ++et){
        st.push(nodes[et->first]);
      }
    }
    result.push(node->id);
  }
  
  while(!result.empty()){
    cout << result.top() << " ";
    result.pop();
  }
  cout << endl;
}

// minimum spanning tree - kruskal's algorithm
// returns a vector of 3tuples containing node id1, node id2 and edge weight of the mst
vector<tuple<int,int,float>> Graph::mst_kruskal(){
  vector<tuple<int,int,float>> result;

  sort(edges.begin(), edges.end(), [=](Edge *a, Edge *b){ return a->weight < b->weight; });

  for(auto edge : edges){
    if(mst_find_set(edge->node1) != mst_find_set(edge->node2)){
      mst_union(edge->node1, edge->node2);
      result.push_back(make_tuple(edge->node1->id, edge->node2->id, edge->weight));
    }
  }
  
  return result;
}

// Helper function for Kruskal's algorithm
int Graph::mst_find_set(Node *node){
  if(node->id != node->tree_id)
    node->tree_id = mst_find_set(nodes[node->tree_id]);

  return node->tree_id;
}

// Helper function for Kruskal's algorithm
void Graph::mst_union(Node *x, Node *y){
  mst_link(nodes[mst_find_set(x)], nodes[mst_find_set(y)]);
}

// Helper function for Kruskal's algorithm
void Graph::mst_link(Node *x, Node *y){
  if(x->tree_rank > y->tree_rank){
    y->tree_id = x->id;
  } else {
    x->tree_id = y->id;
    if(x->tree_rank == y->tree_rank)
      y->tree_rank += 1;
  }
}

// void Graph::initialize_single_soure(Node *source){
//   for(auto node : nodes){
//     node->d = numeric_limits<float>::infinity();
//     node->pi = nullptr;
//   }

//   source->d = 0;
// }

// void Graph::dijkstra_relax(Edge *x){
//   if(x->node1->d > x->node2->d + x->weight){
//     x->node2->d = x->node1->d + x->weight;
//     x->node2->pi = x->node1;
//   }
// }

// void Graph::dijkstra(Node *source, Node *sink){
//   vector<Node*> S;
//   vector<Node*> Q = nodes;
  
//   initialize_single_soure(source);

//   while(!Q.empty()){
    
//   }

// }

// helper function - prints the entire graph
void Graph::print_graph(){
  for(map<int,Node*>::iterator mt=nodes.begin(); mt!=nodes.end(); ++mt){
    cout << "Node " << mt->first << ": ";
    for(map<int,Edge*>::iterator et=mt->second->out.begin();
                   et!=mt->second->out.end();
                   ++et){
      cout << et->first << ", ";
    }
    cout << endl;
  }
}

#endif
