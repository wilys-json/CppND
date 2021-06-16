#include <iostream>
#include <random>
#include <future>
#include "TrafficLight.h"

 
template <typename T>
T MessageQueue<T>::receive()
{

  std::unique_lock<std::mutex> Lock(_mutex);
  _condition.wait(Lock, [this]{return !_queue.empty();});
  T receivedMessage = std::move(_queue.back());
  _queue.clear();
  return receivedMessage;
}

template <typename T>
void MessageQueue<T>::send(T&& msg)
{
  std::lock_guard<std::mutex> Lock(_mutex);
  _queue.push_back(std::move(msg));
  _condition.notify_one();
}


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{

    while(true) {
      TrafficLightPhase signal = _messageQueue.receive();
      if (signal == TrafficLightPhase::green) return;
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}


// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{

  
  // generate random value of 4 to 6 seconds
  std::random_device rd;  
  std::mt19937 generator(rd()); 
  std::uniform_int_distribution<> distribution(4000, 6000);
  int duration = distribution(generator);
  
  // create checkpoint
  auto lastCheckPoint = std::chrono::system_clock::now();
  
  while (true) {
    
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastCheckPoint).count();
    if (elapsed >= duration) {
      _currentPhase = (_currentPhase == TrafficLightPhase::red) ? TrafficLightPhase::green : TrafficLightPhase::red;
      auto signal = _currentPhase;
      _messageQueue.send(std::move(signal));
      duration = distribution(generator);
      lastCheckPoint = std::chrono::system_clock::now();
    }
    std::this_thread::sleep_for(SLEEP_FOR);
  }
}
