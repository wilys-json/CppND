#ifndef ROUTE_PLANNER_H
#define ROUTE_PLANNER_H

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include "route_model.h"


class RoutePlanner {
  public:
    // Compare object for implementing min-heap
    class Compare {
      public:
        bool operator() (const RouteModel::Node*, const RouteModel::Node*) const;
    };
    RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y);
    float GetDistance() const {return distance;}
    void AStarSearch();
    void AddNeighbors(RouteModel::Node *current_node);
    float CalculateHValue(RouteModel::Node const *node);
    std::vector<RouteModel::Node> ConstructFinalPath(RouteModel::Node *);
    RouteModel::Node *NextNode();

  private:
    /*
    Instead of storing nodes in a std::vector, implemnting the open_list as a
    std::priority_queue with an underlying min-heap avoids sorting the open_list
    every time when we need to pop from the open list.
    */
    std::priority_queue<RouteModel::Node*, std::vector<RouteModel::Node*>, Compare> open_list;
    RouteModel::Node *start_node;
    RouteModel::Node *end_node;

    float distance = 0.0f;
    RouteModel &m_Model;
};

#endif
