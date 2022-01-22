#include <iostream>
#include "Pixel.cpp"
#include "Make2DArray.h"
#include "Position.h"
#include "arrayQueue.h"
#include "arrayStack.h"
using namespace std;

/*
 * Image Component Labeling
 * Project 1
 * Justin Li
 * April 22, 2019
 *
 * Purpose and usage of this application:
 * This project is used to label different Image Components.
 */


// functions
void welcome(int &dim, double &den){
  cout << "Welcome to the Image Component Labeling program!" << endl;
  cout << "Use default dimension and density? (y/n):" << endl;
  string a;
  cin >> a;
  if (a == "y"){
    dim = 15;
    den = 0.33;
  }
  if (a == "n"){
    while (!(dim >= 5 && dim <= 20)){
      cout << "Please enter a dimension between 5 and 20:" << endl;
      cin >> dim;
    }
    while (!(den >= 0 && den <= 1)){
      cout << "Please enter a density between 0 and 1:" << endl;
      cin >> den;
    }
  }
}

// Input the image.
void inputImage(Pixel** &bfsGrid, Pixel** &dfsGrid, int dim, double den){

   Make2DArray(bfsGrid, dim + 2, dim + 2);
   Make2DArray(dfsGrid, dim + 2, dim + 2);

   // initialize wall of 0 pixels for both grids
   for (int i = 0; i <= dim + 1; i++)
   {
      bfsGrid[i][0].setLabel(0);
      bfsGrid[i][dim + 1].setLabel(0);
      bfsGrid[0][i].setLabel(0);
      bfsGrid[dim + 1][i].setLabel(0);

      dfsGrid[i][0].setLabel(0);
      dfsGrid[i][dim + 1].setLabel(0);
      dfsGrid[0][i].setLabel(0);
      dfsGrid[dim + 1][i].setLabel(0);
   }

   //generating random values for pixels' label in 2D-array
   srand(time(NULL));
   for (int i = 1; i <= dim; i++){
      for (int j = 1; j <= dim; j++){
          double r = rand() % 100 / 100.0;
          if (r < den) {
              bfsGrid[i][j].setLabel(1);
              dfsGrid[i][j].setLabel(1);
          }
          else {
              bfsGrid[i][j].setLabel(0);
              dfsGrid[i][j].setLabel(0);
          }
      }
   }
}

void scanByBfs(Pixel** &bfsGrid, int row, int col, int id, int dim){
  // initialize offsets
  position offset[4];
  offset[0].row = 0; offset[0].col = 1;   // right
  offset[1].row = 1; offset[1].col = 0;   // down
  offset[2].row = 0; offset[2].col = -1;  // left
  offset[3].row = -1; offset[3].col = 0;  // up

  int numOfNbrs = 4; // neighbors of a pixel position

  arrayQueue<position> q;
  position here, nbr;
  here.row = row;
  here.col = col;

  int order = 0;
  bfsGrid[here.row][here.col].setLabel(id);
  bfsGrid[here.row][here.col].setOrder(++order);
  bool emp = false;

  // scan all pixels labeling components
  do
   {// label neighbors of here
       for (int i = 0; i < numOfNbrs; i++)
       {// check out neighbors of here
          nbr.row = here.row + offset[i].row;
          nbr.col = here.col + offset[i].col;
          if (bfsGrid[nbr.row][nbr.col].getLabel() == 1)
          {// unlabeled nbr, label it
             bfsGrid[nbr.row][nbr.col].setLabel(id);
             bfsGrid[nbr.row][nbr.col].setOrder(++order);
             // put on queue for later expansion
   	         q.push(nbr);
          }
       }

      // finish not reached, can we move to a nbr?
      if (q.empty()){
        emp = true; // no path
        break;
      }
      here = q.front();         // get next position
      q.pop();
   } while(!emp);
}

void scanByDfs(Pixel** &dfsGrid, int row, int col, int id, int dim){
  arrayStack<position> *path = new arrayStack<position>;

  // initialize offsets
  position offset[4];
  offset[0].row = 0; offset[0].col = 1;   // right
  offset[1].row = 1; offset[1].col = 0;   // down
  offset[2].row = 0; offset[2].col = -1;  // left
  offset[3].row = -1; offset[3].col = 0;  // up

  position here;
  here.row = row;
  here.col = col;
  int option = 0; // next move
  int lastOption = 3;

  int order = 0;
  dfsGrid[here.row][here.col].setLabel(id);
  dfsGrid[here.row][here.col].setOrder(++order);
  bool emp = false;

  // search for a path
   while (!emp){// not done
      // find a neighbor to move to
      int r, c;
      while (option <= lastOption)
      {
         r = here.row + offset[option].row;
         c = here.col + offset[option].col;
         if (dfsGrid[r][c].getLabel() == 1) break;
         option++; // next option
      }

      // was a neighbor found?
      if (option <= lastOption)
      {
         path->push(here);
         // move to dfsGrid[r][c]
         here.row = r;
         here.col = c;
         dfsGrid[here.row][here.col].setLabel(id); // set to id to prevent relabel
         dfsGrid[here.row][here.col].setOrder(++order);
         option = 0;
      }
      else
      {// no neighbor to move to, back up
         if (path->empty()) {
           emp = true;
           break;
         }
         position next = path->top();
         path->pop();
         if (next.row == here.row){
            option = 2 + next.col - here.col;
         }
         else {
           option = 3 + next.row - here.row;
         }
         here = next;
      }
   }
 }

void labelComponents(Pixel** &bfsGrid, Pixel** &dfsGrid, int dim)
{// Label the components.
   int id = 1;  // component id
   for (int r = 1; r <= dim; r++) {     // row r of image
      for (int c = 1; c <= dim; c++) {  // column c of image
        if (bfsGrid[r][c].getLabel() == 1) {
            id++;
            scanByBfs(bfsGrid, r, c, id, dim);
            scanByDfs(dfsGrid, r, c, id, dim);
        } //end of if, for c, and for r
      }
    }
}


void outputImage(Pixel** const &bfsGrid, Pixel** const &dfsGrid, int dim){
  //Output bfsGrid
   cout << "\nThe labeled image for the bfsGrid is:\n" << endl;
   for (int i = 1; i <= dim; i++) {
      for (int j = 1; j <= dim; j++) {
         cout << bfsGrid[i][j].getLabel() << "," << bfsGrid[i][j].getOrder() << "  ";
      }
      cout << endl;
   }

   //Output dfsGrid
   cout << "\nThe labeled image for the dfsGrid is\n" << endl;
   for (int i = 1; i <= dim; i++) {
      for (int j = 1; j <= dim; j++) {
         cout << dfsGrid[i][j].getLabel() << "," << dfsGrid[i][j].getOrder() << "  ";
      }
      cout << endl;
   }
}

int main(){
  Pixel **bfsGrid;   // grid for bfs
  Pixel **dfsGrid;   // grid for dfs (identical)
  int dim = -1;      // number of rows and columns in the image
  double den = -1;   // density: amount of grid that is to be relabeled

   welcome(dim, den);
   inputImage(bfsGrid, dfsGrid, dim, den);
   labelComponents(bfsGrid, dfsGrid, dim);
   outputImage(bfsGrid, dfsGrid, dim);

   return 0;
}
