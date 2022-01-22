#include <iostream>
using namespace std;

#ifndef MinPQ_
#define MinPQ_

template<class T, class Compare>
class MinPQ {
   public:
      // functions
      MinPQ();
            // create an empty priority queue
      MinPQ(T k[]);
            // create a priority queue w/ given keys
      void insert(const T& k);
            // add key into the priority queue
      T delMin();
            // return and remove the element w/ the smallest key
      bool isEmpty() const;
            // return true if the priority queue is empty
      T min() const;
            // return element with the smallest key
      int size() const;
            // return number of elements in the priority queue

    private:
      bool greater(int i, int j) const;
            // returns true if element at i is greater than element at j
      void swim(int k);
            // bubble up/promote element until the order is restored
      void sink(int k);
            // sift down/demote element until order is restored

      // variables
      T* pq;
      int n;
};

template<class T, class Compare>
MinPQ<T, Compare>::MinPQ(){
  n = 0;
  pq = new T[1];
  pq[0] = NULL;
}

template<class T, class Compare>
MinPQ<T, Compare>::MinPQ(T k[]){
  n = sizeof(k)/sizeof(k[0]);
  pq = new T[n + 1];
  pq[0] = NULL;
  for(int i = 1; i <= n; i++)
    pq[i] = k[i - 1];
}

template<class T, class Compare>
void MinPQ<T, Compare>::insert(const T& k){
  T* temp = new T[++n + 1];
  for(int i = 0; i < n; i++){
    temp[i] = pq[i];
  }
  temp[n] = k;
  delete [] pq;
  pq = temp;
  swim(n);
}

template<class T, class Compare>
T MinPQ<T, Compare>::delMin(){
  T t = pq[1];
  pq[1] = pq[n];
  pq[n] = t;
  T* temp = new T[n--];
  for(int i = 0; i <= n; i++){
    temp[i] = pq[i];
  }
  delete [] pq;
  pq = temp;
  sink(1);
  return t;
}

template<class T, class Compare>
bool MinPQ<T, Compare>::isEmpty() const { return n == 0; }

template<class T, class Compare>
T MinPQ<T, Compare>::min() const { return pq[1]; }

template<class T, class Compare>
int MinPQ<T, Compare>::size() const { return n; }

template<class T, class Compare>
bool MinPQ<T, Compare>::greater(int i, int j) const{
  Compare c;
  return c(pq[i], pq[j]);
}

template<class T, class Compare>
void MinPQ<T, Compare>::swim(int k){
  while (k > 1 && greater(k/2, k)){
    T t = pq[k/2];
    pq[k/2] = pq[k];
    pq[k] = t;
    k = k/2; // node k is at k/2
  }
}

template<class T, class Compare>
void MinPQ<T, Compare>::sink(int k){
  while (2 * k <= n) {
    int j = 2 * k;
    if (j < n && greater(j, j+1)) j++;
    if (!greater(k, j)) break;
    T t = pq[k];
    pq[k] = pq[j];
    pq[j] = t; // children of element at are 2k and 2k + 1
    k = j;
  }
}

#endif
