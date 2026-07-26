//rotating cube in c++


#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
using namespace std;


int const height = 20;
int const width = 80;

char framebuffer[height][width];
float d = 3.0f;
float xscale = 12.0;
float yscale = 7.0;

float angle = 0.2;
float speed=0.1;
struct Vect3
{
float x;
float y;
float z;


};


Vect3 rotateY(Vect3 p, float angle){


    float x0 = p.x;
    
    float z0=p.z;
    
    float retx = x0 * cos(angle) - z0 *  sin(angle);
    float retz = x0 *sin(angle) + z0* cos(angle);
return{
  retx,
p.y,
retz
};

}


struct Vect2
{
    float x; 
    float y;

};

Vect3 cubeVerts[8]=
{
     {-1, -1, -1}, 
    { 1, -1, -1}, 
    { 1,  1, -1}, 
    {-1,  1, -1}, 

    {-1, -1,  1}, 
    { 1, -1,  1}, 
    { 1,  1,  1}, 
    {-1,  1,  1}  
};

int cubeEdges[12][2] =
{
    // Back face (z = -1)
    {0, 1},
    {1, 2},
    {2, 3},
    {3, 0},

    // Front face (z = +1)
    {4, 5},
    {5, 6},
    {6, 7},
    {7, 4},

    // Connecting edges
    {0, 4},
    {1, 5},
    {2, 6},
    {3, 7}
};


Vect3 rotatedVerts[8];

void clearScreen()
{
    for(int i =0; i<height; i++)
    {
        for(int j =0; j<width; j++)
        {
            framebuffer[i][j] = ' ';
        }
    }

}


void update(){
    angle+=speed;

    for(int i =0; i <8; i++)
    {
        rotatedVerts[i]=rotateY(cubeVerts[i],angle);
    }
}


void plot(int x, int y, char pixel)
{


    framebuffer[y][x] = pixel;
}



 Vect2 project(Vect3 p)
    {
        float projx = ((p.x/(p.z+d))*xscale);
        float projy = ((p.y/(p.z+d))*yscale);


        float screenx = projx + (width/2);
        float screeny = projy + (height/2);
      
        return {screenx, screeny};

    }

//dda alg
void drawLine(int x0, int y0, int x1, int y1){

    int dx=x1-x0;
    int dy=y1-y0;


    
    int steps = max(abs(dx), abs(dy));

    if (steps == 0)
    {
        plot(x0, y0, '@');
        return;
    }

    float xInc = dx / (float)steps;
    float yInc = dy / (float)steps;

    float x = x0;
    float y = y0;

    for (int i = 0; i <= steps; i++)
    {
        plot((int)round(x), (int)round(y), '@');
        x += xInc;
        y += yInc;
    

    

}


void drawCube(){
 

    plot(0,0,'@');
    plot (10,10,'x');
}






void present(){

cout << "\x1b[H";
     for(int i =0; i<height; i++)
    {
        for(int j =0; j<width; j++)
        {
            cout<<framebuffer[i][j];
        }
        cout<<"\n";
    }
    
}



int main(){


    cout << "\x1b[?25l";
    while (true)
{
   

clearScreen();


update();
    for (int i = 0; i < 12; i++)
    {
        Vect2 p1 = project(rotatedVerts[cubeEdges[i][0]]);
        Vect2 p2 = project(rotatedVerts[cubeEdges[i][1]]);

        drawLine(
            (int)round(p1.x),
            (int)round(p1.y),
            (int)round(p2.x),
            (int)round(p2.y)
        );
    }

   
    present();
    cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
}
    return 0;
}

 






