#include <iostream>
#include "event.h"
using namespace std;

/*
 * SoftwareGurusBar.cpp file
 * CSCI 61
 * Project 2
 * Justin Li
 * June 1, 2019
 *
 * Purpose and usage of this application:
 * This file contains class randomInteger, which creates a random integer
 * Also contains SoftwareGurusBar class, which is a representation of a bar selling beers, which is the primary object in the simulation.
 * This class estimates # customers arriving, length of time taken to order, and length of time staying after having been served.
 * ArriveEvent, OrderEvent, and LeaveEvent classes are also implemented, representing different types of events in the simulation.
 */

class randomInteger {
  public:
  unsigned int operator() (unsigned int);
} randomizer;

unsigned int randomInteger::operator() (unsigned int max)
{
  // rand return random integer
  // convert to unsigned to make positive
  // take remainder to put in range
  unsigned int rval = rand();
  return rval % (max + 1);
}

unsigned int randBetween(int low, int high) {
  return low + randomizer(high - low);
}

class SoftwareGurusBar {
  public:
  // try with 50 chairs, then try with 40, 60, ...
  // in order to find out “optimal” profit prospects
  SoftwareGurusBar() : freeChairs(50), profit(0.0) {}
  bool canSeat(unsigned int numberOfPeople); // slide 15
  void order(unsigned int beerType); // slide 16
  void leave(unsigned int numberOfPeople); // slide 16

  unsigned int freeChairs;
  double profit;
};

// global variables
SoftwareGurusBar theBar;
SimulationFramework theSimulation;

bool SoftwareGurusBar::canSeat(unsigned int numberOfPeople)
{
  // if sufficient room, then seat customers
  cout << "Time: " << theSimulation.currentTime;
  cout << ": Group of " << numberOfPeople << " customers arrives.";
  if (numberOfPeople <= freeChairs){
    cout << " Group is seated." << endl;
    freeChairs -= numberOfPeople;
    return true;
  }
  else {
    cout << " No room, group leaves." << endl;
    return false;
  }
}

void SoftwareGurusBar::order(unsigned int beerType)
{
  // serve beer
  cout << "Time: " << theSimulation.currentTime;
  cout << ": Serviced order for beer #" << beerType << "." << endl;
  // update profit based on this beerType
  profit += beerType + 1;
}

void SoftwareGurusBar::leave(unsigned int numberOfPeople)
{
  // people leave, free up chairs
  cout << "Time: " << theSimulation.currentTime;
  cout << ": A group of size " << numberOfPeople << " leaves." << endl;
  freeChairs += numberOfPeople;
}

class ArriveEvent : public Event {
  public:
    ArriveEvent(unsigned int time, unsigned int gs) : Event(time), groupSize(gs) {}
    void processEvent();

  protected:
    unsigned int groupSize;
};


class OrderEvent : public Event {
  public:
    OrderEvent(unsigned int time, unsigned int gs) : Event(time), groupSize(gs) { }
    void processEvent();

  protected:
    unsigned int groupSize;
};


class LeaveEvent : public Event {
  public:
    LeaveEvent(unsigned int time, unsigned int gs) : Event(time), groupSize(gs) { }
    void processEvent();

  protected:
    unsigned int groupSize;
};


// overriding processEvent() function for all 3 different event subclasses
void ArriveEvent::processEvent()
{
  if (theBar.canSeat(groupSize)){
    // place an order within 2 & 10 minutes
    theSimulation.scheduleEvent(new OrderEvent(theSimulation.currentTime + randBetween(2,10), groupSize));
  }
}

void OrderEvent::processEvent()
{
  // each member of the group orders a beer (type 1,2,3)
  for (int i = 0; i < groupSize; i++){
    theBar.order(randBetween(1,3));
  }
  int t = theSimulation.currentTime + randBetween(15,35);
  // this group of drinkers leaves after 15-35 minutes
  theSimulation.scheduleEvent(new LeaveEvent(t, groupSize));
}

void LeaveEvent::processEvent()
{
  theBar.leave(groupSize);
}

int main() {
  srand(time(NULL)); //randomizes during runtime

  // load priority queue with initial ArriveEvents then run simulation
  unsigned int t = 0;

  // load 4 hours (240 minutes) of Arrive Events
  while (t < 240) {
  // new group every 2-5 minutes
  t += randBetween(2,5);

  // group size ranges from 1 to 5
  theSimulation.scheduleEvent(new ArriveEvent(t, randBetween(1,5)));
  }

  // then run simulation and print profits
  theSimulation.run();
  cout << "Total profit: $" << theBar.profit << endl;
}
