#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

const std::chrono::duration<int, std::milli> SLEEP_FOR{1};

// enum class
enum TrafficLightPhase
{
  red,
  green
};

// forward declarations to avoid include cycle
class Vehicle;


template <class T>
class MessageQueue
{
public:
    void send(T&& msg);
    T receive();

private:
    std::deque<T> _queue;
    std::condition_variable _condition;
    std::mutex _mutex;
};


class TrafficLight : public TrafficObject
{
public:
    // constructor / desctructor
    TrafficLight ();
    
    // getters / setters
    TrafficLightPhase getCurrentPhase();
    
    // typical behaviour methods
    void waitForGreen();
    void simulate() override;
    
private:
    // typical behaviour methods
    void cycleThroughPhases();
    std::condition_variable _condition;
    std::mutex _mutex;
    TrafficLightPhase _currentPhase;
    MessageQueue<TrafficLightPhase> _messageQueue;
};

#endif