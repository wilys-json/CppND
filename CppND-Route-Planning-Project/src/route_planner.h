#ifndef ROUTE_PLANNER_H
#define ROUTE_PLANNER_H

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include "route_model.h"

struct Compare {
    bool operator() (const RouteModel::Node* a, const RouteModel::Node* b) const {
        float f1 = a->g_value + a->h_value;
        float f2 = b->g_value + b->h_value;
        return f1 > f2;
    }
};

class RoutePlanner {
  public:
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
    std::priority_queue<RouteModel::Node*, std::vector<RouteModel::Node*>, Compare> open_list;
    RouteModel::Node *start_node;
    RouteModel::Node *end_node;

    float distance = 0.0f;
    RouteModel &m_Model;
};

#endif
