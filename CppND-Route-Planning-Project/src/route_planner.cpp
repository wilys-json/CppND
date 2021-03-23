#include "route_planner.h"
#include <algorithm>

bool RoutePlanner::Compare::operator() (const RouteModel::Node* a, const RouteModel::Node* b) const {
    return a->g_value + a->h_value > b->g_value + b->h_value;  // Compare values
}


RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);
}



float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return end_node->distance(*node);
}


void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    current_node->FindNeighbors();

    for (auto &neighbor : current_node->neighbors) {
            if(!(neighbor->visited)) {
                neighbor->parent = current_node;
                neighbor->h_value = CalculateHValue(neighbor);
                neighbor->g_value = current_node->g_value + current_node->distance(*neighbor);
                neighbor->visited = true;
                open_list.push(neighbor);
            }
    }
}



RouteModel::Node *RoutePlanner::NextNode() {
    RouteModel::Node *next_node = open_list.top();  // Root of Min-heap
    open_list.pop();
    return next_node;
}


std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {

    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.
    std::vector<RouteModel::Node> temp;  // Create a temp vector for reversal

    // Trace until the child of start_node is reached
    while(current_node->parent != nullptr) {
        temp.push_back(*current_node);  // Add current_node to temp vector
        distance += current_node->distance(*current_node->parent);  // Increment distance
        current_node = current_node->parent;  // Update current_node
    }

    temp.push_back(*current_node);  // Add start_node

    // Reverse the vector by having the temp vector behave like a stack
    while(!temp.empty()) {
        path_found.push_back(temp.back());
        temp.pop_back();
    }

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = start_node;  // Initialize `current_node` as `this->start_node`


    // Loop until the end_node is reached
    while(current_node != end_node) {
        current_node->visited = true;  // Set current_node as `visited`
        AddNeighbors(current_node);  // Add neighbors of current_node to open_list
        current_node = NextNode();  // Pop from the priority_queue
    }

    m_Model.path = ConstructFinalPath(current_node);
}
