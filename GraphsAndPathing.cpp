#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

class Attraction {
public:
    string name;
    map<Attraction*, int> neighbors;

    Attraction(string n) : name(n) {}

    void addNeighbor(Attraction* neighbor, int time) {
        neighbors[neighbor] = time;
    }

    ~Attraction() {} // Destructor to release memory
};


pair<vector<Attraction*>, int> findShortestPathDijkstra(Attraction* start, Attraction* end) {
    vector<Attraction*> path;
    int total_time = 0;

    map<Attraction*, int> distance;
    map<Attraction*, Attraction*> parent;
    priority_queue<pair<int, Attraction*>, vector<pair<int, Attraction*>>, greater<pair<int, Attraction*>>> pq;

    for (auto& neighbor : start->neighbors) {
        distance[neighbor.first] = neighbor.second;
        pq.push({neighbor.second, neighbor.first});
        parent[neighbor.first] = start;
    }

  while (!pq.empty()) {
      pair<int, Attraction*> top_pair = pq.top();
      int time = top_pair.first;
      Attraction* current = top_pair.second;
      pq.pop();

      if (current == end) {
          total_time = time;
          while (current != start) {
              path.push_back(current);
              current = parent[current];
          }
          path.push_back(start);
          reverse(path.begin(), path.end());
          break;
      }

      for (auto& neighbor : current->neighbors) {
          int new_time = time + neighbor.second;
          if (new_time < distance[neighbor.first]) {
              distance[neighbor.first] = new_time;
              pq.push({new_time, neighbor.first});
              parent[neighbor.first] = current;
          }
      }
  }

    return {path, total_time};
}


#define INF 99999
vector<vector<int>> map1 = {
    {0, 10, INF, 30, 100},
    {10, 0, 50, INF, INF},
    {INF, 50, 0, 20, 10},
    {30, INF, 20, 0, 60},
    {100, INF, 10, 60, 0}
};


vector<Attraction*> createAttractions(vector<vector<int>>& adjacency_matrix) {
    vector<Attraction*> attractions(adjacency_matrix.size());
    for (int i = 0; i < adjacency_matrix.size(); ++i) {
        attractions[i] = new Attraction(to_string(i));
    }
    for (int i = 0; i < adjacency_matrix.size(); ++i) {
        for (int j = 0; j < adjacency_matrix[i].size(); ++j) {
            if (adjacency_matrix[i][j] != INF) {
                attractions[i]->addNeighbor(attractions[j], adjacency_matrix[i][j]);
            }
        }
    }
    return attractions;
}

int main() {
    vector<Attraction*> attractions = createAttractions(map1);

    // Testing shortest path
    Attraction* start = attractions[0];
    Attraction* end = attractions[4];
    auto result = findShortestPathDijkstra(start, end);

    cout << "Shortest path from " << start->name << " to " << end->name << ": ";
    for (auto* attr : result.first) {
        cout << attr->name << " -> ";
    }
    cout << "Total time: " << result.second << " minutes" << endl;

    // Memory cleanup
    for (auto* attr : attractions) {
        delete attr;
    }

    return 0;
}
