// Note: Make sure you return something from these functions before you run or you may (will) have segfaults
#include <cmath>
#include <iostream>
using std::cout;
using std::endl;

//helper function for DFS
template<typename pickerType>
animation DFSfill_aux(BMP& img, int x, int y, pickerType pickColor, int tolerance, int frameFreq) {
   //save original pixel colors
   int orig_r=img(x,y)->Red, orig_g=img(x,y)->Green, orig_b=img(x,y)->Blue;
   Stack<int> x_S,y_S;
   int count=0; //counter for frameFreq
   int width=img.TellWidth(), height=img.TellHeight();
   animation ret;
   //initialize 2d array to check if coordinate has been processed
   bool processed[width][height];
   x_S.push(x);
   y_S.push(y);
   for(int i=0;i<height;i++){
      for(int j=0;j<width;j++){
         processed[j][i]=0;
      }
   }
   //if img is empty
   if ((width==0) || (height==0)) 
      return ret;  
   //while stack isn't empty
   while ( (!x_S.isEmpty()) && (!y_S.isEmpty()) ){
      x=x_S.pop();
      y=y_S.pop();
      //cout<<x<<','<<y<<endl;
      // check if coordinate has not been processed and if it's in the right
      // constraints
      if ( (x<width) && (x>=0) && (y<height) && (y>=0) && (!processed[x][y]) ){
         //if under tolerance then color
         if ( tolerance >= ( pow((orig_r-img(x,y)->Red),2) + 
pow((orig_g-img(x,y)->Green),2) + pow((orig_b-img(x,y)->Blue),2)) ){
            //color pixel
            img(x,y)->Red= pickColor(x,y).Red;
            img(x,y)->Green= pickColor(x,y).Green;
            img(x,y)->Blue= pickColor(x,y).Blue;
            //determine if frame needs to be added
            //cout<<"----------------------------------------";
            count++;
            if (count==frameFreq){
               ret.addFrame(img);
               count=0;
            }
         
            //push right
            x_S.push(x+1);
            y_S.push(y);
            //push down
            x_S.push(x);
            y_S.push(y+1);
            //push left
            x_S.push(x-1);
            y_S.push(y);
            //push up
            x_S.push(x);
            y_S.push(y-1);
         } 
         processed[x][y]=1;
       }
      //cout<<endl;
   }
   /*cout<<endl;
   cout<<"not being read"<<endl;
   for(int i=0;i<height;i++){
      for(int j=0;j<width;j++){
         
         if (!processed[j][i])
            cout<<'('<<j<<','<<i<<')'<<endl;
      }
   }*/
   return ret;       
}

//helper function for BFS
template<typename pickerType>
animation BFSfill_aux(BMP& img, int x, int y, pickerType pickColor, int tolerance, int frameFreq) {
   //save original pixel colors
   int orig_r=img(x,y)->Red, orig_g=img(x,y)->Green, orig_b=img(x,y)->Blue;
   Queue<int> x_Q,y_Q;
   int count=0; //counter for frameFreq
   int width=img.TellWidth(), height=img.TellHeight();
   animation ret;
   //initialize 2d array to check if coordinate has been processed
   bool processed[width][height];
   x_Q.enqueue(x);
   y_Q.enqueue(y);
   for(int i=0;i<height;i++){
      for(int j=0;j<width;j++){
         processed[j][i]=0;
      }
   }      
   //if img is empty
   if ((width==0) || (height==0)) 
      return ret;  
   //while stack isn't empty
   while ( (!x_Q.isEmpty()) && (!y_Q.isEmpty()) ){
      x=x_Q.dequeue();
      y=y_Q.dequeue();
      //cout<<x<<','<<y<<endl;
      // check if coordinate has not been processed and if it's in the right
      // constraints
      if ( (x<width) && (x>=0) && (y<height) && (y>=0) && (!processed[x][y]) ){
         //if under tolerance then color
         if ( tolerance >= ( pow((orig_r-img(x,y)->Red),2) +
pow((orig_g-img(x,y)->Green),2) + pow((orig_b-img(x,y)->Blue),2)) ){
            //color pixel
            img(x,y)->Red= pickColor(x,y).Red;
            img(x,y)->Green= pickColor(x,y).Green;
            img(x,y)->Blue= pickColor(x,y).Blue;
            //determine if frame needs to be added
            count++;
            //cout<<"----------------------------------------";
               if (count==frameFreq){
                  ret.addFrame(img);
                  count=0;
               }
            //enqueue right
            x_Q.enqueue(x+1);
            y_Q.enqueue(y);
            //enqueue down
            x_Q.enqueue(x);
            y_Q.enqueue(y+1);
            //enqueue left
            x_Q.enqueue(x-1);
            y_Q.enqueue(y);
            //enqueue up
            x_Q.enqueue(x);
            y_Q.enqueue(y-1);
         }
         processed[x][y]=1;
      }
      //cout<<endl;
   }
   /*cout<<endl;
   cout<<"not being read"<<endl;
   for(int i=0;i<height;i++){
      for(int j=0;j<width;j++){
         
         if (!processed[j][i])
            cout<<'('<<j<<','<<i<<')'<<endl;
      }
   } */     
   return ret;       
}

/******
                Parent functions for each type of fill
                                                                  *******/


animation DFSfillSolid(BMP& img, int x, int y, RGBApixel fillColor, int tolerance, int frameFreq) {
   solidColorPicker solid(fillColor);
   return DFSfill_aux(img,x,y,solid,tolerance,frameFreq);
}

animation DFSfillGrid(BMP& img, int x, int y, RGBApixel gridColor, int gridSpacing, int tolerance, int frameFreq) {
   gridColorPicker grid(gridColor,gridSpacing);
   return DFSfill_aux(img,x,y,grid,tolerance,frameFreq);
}

animation DFSfillGradient(BMP& img, int x, int y, RGBApixel fadeColor1, RGBApixel fadeColor2, int radius, int tolerance, int frameFreq) {
   gradientColorPicker grad(fadeColor1,fadeColor2,radius,x,y);
   return DFSfill_aux(img,x,y,grad,tolerance,frameFreq);
}

animation BFSfillSolid(BMP& img, int x, int y, RGBApixel fillColor, int tolerance, int frameFreq) {
   solidColorPicker solid(fillColor);
   return BFSfill_aux(img,x,y,solid,tolerance,frameFreq);
}

animation BFSfillGrid(BMP& img, int x, int y, RGBApixel gridColor, int gridSpacing, int tolerance, int frameFreq) {
   gridColorPicker grid(gridColor,gridSpacing);
   return BFSfill_aux(img,x,y,grid,tolerance,frameFreq);
}

animation BFSfillGradient(BMP& img, int x, int y, RGBApixel fadeColor1, RGBApixel fadeColor2, int radius, int tolerance, int frameFreq) {
   gradientColorPicker grad(fadeColor1,fadeColor2,radius,x,y);
   return BFSfill_aux(img,x,y,grad,tolerance,frameFreq);
}
