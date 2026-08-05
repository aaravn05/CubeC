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

char pixel;

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
        plot((int)round(x), (int)round(y), pixel, RED);
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





float edge(Vec2 a, Vec2 b, Vec2 c)
{
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}
//barycentric rasterizer version
void drawTriangle(Vec2 a, Vec2 b, Vec2 c, char pixel)
{

    int minX = floor(min(a.x, min(b.x, c.x)));
int maxX = ceil (max(a.x, max(b.x, c.x)));

int minY = floor(min(a.y, min(b.y, c.y)));
int maxY = ceil (max(a.y, max(b.y, c.y)));
    

    
for (int y = minY; y <= maxY; y++)

{
    for (int x = minX; x <= maxX; x++)
    {
        Vec2 p = {(float)x,(float)y};

        float w0 = edge(b, c, p);
            float w1 = edge(c, a, p);
            float w2 = edge(a, b, p);

        if ((w0 >= 0 && w1 >= 0 && w2 >= 0) ||
    (w0 <= 0 && w1 <= 0 && w2 <= 0))
        {
            plot(x, y, pixel, RED);
        }

        
    }
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

    pixel = '@';
    drawTriangle(v0, v1, v2, pixel);
    drawTriangle(v0, v2, v3, pixel);
}

    
   
    present();
    cout.flush();
   this_thread::sleep_for(std::chrono::milliseconds(35));
}
    return 0;
}
