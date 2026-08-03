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
char color[height][width];
float d = 3.0f;
float xscale = 12.0;
float yscale = 7.0;

float angle = 0.2;
float speed=0.1;
struct Vec3
{
float x;
float y;
float z;


};

enum COLOR
{
RED, GREEN, BLUE, ORANGE, GRAY
};

Vec3 rotateY(Vec3 p, float angle){


    float x0 = p.x;
    
    float z0=p.z;
    
    float retx = x0 * cos(angle) - z0 *  sin(angle);
    float retz = x0 *sin(angle) + z0* cos(angle);

    
    return{retx,p.y,retz};

}


struct Vec2
{
    float x; 
    float y;

};

Vec3 cubeVerts[8]=
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


    
    {0, 1},
    {1, 2},
    {2, 3},
    {3, 0},

    
    {4, 5},
    {5, 6},
    {6, 7},
    {7, 4},

   
    {0, 4},
    {1, 5},
    {2, 6},
    {3, 7}
};

int cubeFaces[6][4] =
{
    {0,1,2,3}, // back
    {4,5,6,7}, // front
    {0,1,5,4}, // bottom
    {2,3,7,6}, // top
    {0,3,7,4}, // left
    {1,2,6,5}  // right
};

Vec3 rotatedVerts[8];

void clearScreen()
{
    for(int i =0; i<height; i++)
    {
        for(int j =0; j<width; j++)
        {
            framebuffer[i][j] = ' ';
            color [i][j] = ' ';
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


void plot(int x, int y, char pixel, COLOR col )
{
if (x < 0 || x >= width) return;
if (y < 0 || y >= height) return;

    framebuffer[y][x] = pixel;
    color[y][x] = col;
}



 Vec2 project(Vec3 p)
    {
        float projx = ((p.x/(p.z+d))*xscale);
        float projy = ((p.y/(p.z+d))*yscale);


        float screenx = projx + (width/2);
        float screeny = projy + (height/2);
      
        return {screenx, screeny};

    };

//dda alg
void drawLine(int x0, int y0, int x1, int y1)
{
    int dx=x1-x0;
    int dy=y1-y0;

    int steps = max(abs(dx), abs(dy));

    if (steps == 0)
    {
        plot(x0, y0, '@', RED);
        return;
    }

    float xInc = dx / (float)steps;
    float yInc = dy / (float)steps;

    float x = x0;
    float y = y0;

    for (int i = 0; i <= steps; i++)
    {
        plot((int)round(x), (int)round(y), '3', RED);
        x += xInc;
        y += yInc;
}
}


void swap(Vec2& a, Vec2& b)
{
    Vec2 temp = a;
    a=b;
    b=temp;
}

void drawtriangleTop(int x0, int y0, int x1, int y1, int x2, int y2)
{

    float x_start=x0;
    float x_end=x0;

    

    //top triangle
    float inv_slope1 = (float) (x1-x0)/(y1-y0);
    float inv_slope2 = (float) (x2-x0)/(y2-y0); 

    for(int i =y0; i<=y2; i++)
    {
        drawLine(x_start,i,x_end,i);
        x_start+=inv_slope1;
        x_end+=inv_slope2;
    }


}

void drawtriangleBottom(int x0, int y0, int x1, int y1, int x2, int y2){

     float x_start=x2;
        float x_end=x2;
    

      float inv_slope3 = (float) (x2-x0)/(y2-y1);
    float inv_slope4 = (float) (x2-x1)/(y2-y1);

    for(int i = y2; i>=y1; i--)
    {
        drawLine(x_start,i,x_end,i);
        x_start-=inv_slope3;
        x_end-=inv_slope4;
    }

}


void drawTriangle(Vec2 a, Vec2 b, Vec2 c)
{

    //find y0<y1<y2
if (a.y > b.y)
    swap(a, b);

if (a.y > c.y)
    swap(a, c);

if (b.y > c.y)
    swap(b, c);
    
   

    if(b.y==c.y){
       drawtriangleTop(a.x, a.y,
    b.x, b.y,
    c.x, c.y);
    
}
    

    else if(a.y==b.y){
        drawtriangleBottom(a.x, a.y,
    b.x, b.y,
    c.x, c.y);
    }
    

    else
    {
    
        Vec2 m;
         m.y = b.y;
     m.x = ((float)((c.x-a.x)*(b.y-a.y)))/((float)(c.y-a.y)) + a.x;

   drawtriangleTop(
    a.x, a.y,
    b.x, b.y,
    m.x, m.y);

drawtriangleBottom(
    b.x, b.y,
    m.x, m.y,
    c.x, c.y);

    }





    return; 
}







void present(){

cout << "\x1b[H";
     for(int i =0; i<height; i++)
    {
        for(int j =0; j<width; j++)
        {
            cout<<"\x1b[1;36m"<<framebuffer[i][j];
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
        Vec2 p1 = project(rotatedVerts[cubeEdges[i][0]]);
        Vec2 p2 = project(rotatedVerts[cubeEdges[i][1]]);

        drawLine(
            (int)round(p1.x),
            (int)round(p1.y),
            (int)round(p2.x),
            (int)round(p2.y)
            
        );
    }
    


for (int i = 0; i < 6; i++)
{
    Vec2 v0 = project(rotatedVerts[cubeFaces[i][0]]);
    Vec2 v1 = project(rotatedVerts[cubeFaces[i][1]]);
    Vec2 v2 = project(rotatedVerts[cubeFaces[i][2]]);
    Vec2 v3 = project(rotatedVerts[cubeFaces[i][3]]);   

    drawTriangle(v0, v1, v2);
    drawTriangle(v0, v2, v3);
}

    
   
    present();
    cout.flush();
   this_thread::sleep_for(std::chrono::milliseconds(35));
}
    return 0;
}








