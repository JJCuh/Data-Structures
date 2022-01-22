// create a 2D array; catch exception thrown by new when
// sufficient memory is not available
// return true if successful in creating the 2d array

#ifndef Make2DArray_
#define Make2DArray_

#include <exception>

using namespace std;

template <class T>
bool Make2DArray(T ** &x, int numberOfRows, int numberOfColumns){
  // Create a 2-D array.
   try {
          // create pointers for the rows
          x = new T * [numberOfRows];

          // get memory for each row
          for (int i = 0; i < numberOfRows; i++)
              x[i] = new T [numberOfColumns];
          return true;
       }
   catch (bad_alloc) {return false;}
}

#endif
