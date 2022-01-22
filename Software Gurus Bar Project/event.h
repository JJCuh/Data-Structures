#include <iostream>
#include "MinPQ.h"
using namespace std;

/*
 * event.h file
 * CSCI 61
 * Project 2
 * Justin Li
 * June 1, 2019
 *
 * Purpose and usage of this application:
 * This file contains class Event, which stores a time
 * Also contains eventComparison and SimulationFramework classes
 */

 #ifndef event_
 #define event_

class Event {
  public:
  // constructor requires time of event
  Event(unsigned int t) : time(t) {}

  // time is a public data field
  unsigned int time;

  // execute event by invoking this method
  virtual void processEvent() = 0;
};

class eventComparison {
  public:
  bool operator() (Event* l, Event* r) {
    return (l->time > r->time);
  }
};

class SimulationFramework {
  public:
  SimulationFramework() : eventQueue(), currentTime(0) {}
  void scheduleEvent(Event* newEvent)
  {
    // insert newEvent into eventQueue (Priority Queue)
    // Priority Queue is based on MinHeap using newEvent’s time
    eventQueue.insert(newEvent);
  }

  void run();

  unsigned int currentTime;

  protected:
  MinPQ <Event*, eventComparison> eventQueue;
};

void SimulationFramework::run()
{
  // execute events until event queue becomes empty
  while (!eventQueue.isEmpty()) {
    // copy & remove min-priority element (min time) from eventQueue
    Event* nextEvent = eventQueue.delMin();

    // update simulation’s current time
    currentTime = nextEvent->time;

    // process nextEvent
    nextEvent->processEvent(); // Late binding/Polymorphism at work

    // cleanup nextEvent object
    delete nextEvent;
  }
}
 #endif
