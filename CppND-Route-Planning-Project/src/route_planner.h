#ifndef ROUTE_PLANNER_H
#define ROUTE_PLANNER_H

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include "route_model.h"


class RoutePlanner {
  public:
    // Compare object for implemnting min-heap
    class Compare {
      public:
        bool operator() (const RouteModel::Node*, const RouteModel::Node*) const;
    };
    RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y);
    // Add public variables or methods declarations here.
    float GetDistance() const {return distance;}
    void AStarSearch();

    // The following methods have been made public so we can test them individually.
    void AddNeighbors(RouteModel::Node *current_node);
    float CalculateHValue(RouteModel::Node const *node);
    std::vector<RouteModel::Node> ConstructFinalPath(RouteModel::Node *);
    RouteModel::Node *NextNode();

  private:
    // Add private variables or methods declarations here.
    // std::vector<RouteModel::Node*> open_list;
    // Implement A* Search using std::priority_queue with an underlying min-heap
    std::priority_queue<RouteModel::Node*, std::vector<RouteModel::Node*>, Compare> open_list;
    RouteModel::Node *start_node;
    RouteModel::Node *end_node;

    float distance = 0.0f;
    RouteModel &m_Model;
};

#endif
