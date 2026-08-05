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



struct Vec3
{
float x;
float y;
float z;


};

enum COLOR
{

    GRAY,
    CYAN
   
};

COLOR  colors[2] =
{
    GRAY,
    CYAN
};


COLOR colorBuffer[height][width];

Vec3 rotateY(Vec3 p, float angle){


    float x0 = p.x;
    
    float z0=p.z;
    
    float retx = x0 * cos(angle) - z0 *  sin(angle);
    float retz = x0 *sin(angle) + z0* cos(angle);

    
    return{retx,p.y,retz};

}

Vec3 rotateX(Vec3 p, float angle)
{
    float y0 = p.y;
    float z0 = p.z;

    float rety = y0 * cos(angle) - z0 * sin(angle);
    float retz = y0 * sin(angle) + z0 * cos(angle);

    return {p.x, rety, retz};
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
    {0,3,2,1}, // back  (was {0,1,2,3})
    {4,5,6,7}, // front
    {0,1,5,4}, // bottom
    {2,3,7,6}, // top
    {0,4,7,3}, // left  (was {0,3,7,4})
    {1,2,6,5}
};

Vec3 rotatedVerts[8];

void clearScreen()
{
    for(int i =0; i<height; i++)
    {
        for(int j =0; j<width; j++)
        {
            framebuffer[i][j] = ' ';
            colorBuffer[i][j] = GRAY;
        }
    }

}


void update(){
    angle+=speed;

    for(int i =0; i <8; i++)
    {
        Vec3 p = rotateX(cubeVerts[i], angle * 0.5f);
        rotatedVerts[i] = rotateY(p, angle);
    }
}


void plot(int x, int y, char pixel, COLOR col )
{
if (x < 0 || x >= width) return;
if (y < 0 || y >= height) return;

    framebuffer[y][x] = pixel;
    colorBuffer[y][x] = col;
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
void drawLine(int x0, int y0, int x1, int y1, char pixel)
{
    int dx=x1-x0;
    int dy=y1-y0;

    int steps = max(abs(dx), abs(dy));

    if (steps == 0)
    {
        plot(x0, y0, '@', CYAN);
        return;
    }

    float xInc = dx / (float)steps;
    float yInc = dy / (float)steps;

    float x = x0;
    float y = y0;

    for (int i = 0; i <= steps; i++)
    {
        plot((int)round(x), (int)round(y), pixel, GRAY);
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

//vector math
Vec3 subtract(Vec3 a, Vec3 b)
{
    return{b.x-a.x, b.y-a.y, b.z-a.z};
}

Vec3 cross(Vec3 a, Vec3 b)
{
    float a1 = a.x;
    float a2 = a.y;
    float a3 = a.z;

    float b1 = b.x;
    float b2 = b.y;
    float b3 = b.z;

    Vec3 c = {a2*b3 - a3*b2, a3*b1 - a1*b3, a1*b2 - a2*b1};

    return c;
}

float dot(Vec3 a, Vec3 b)
    {
        float a1 = a.x;
    float a2 = a.y;
    float a3 = a.z;

    float b1 = b.x;
    float b2 = b.y;
    float b3 = b.z;

    float temp = (a1*b1)+(a2*b2)+(a3*b3);

    return temp;

    }


    Vec3 normalize(Vec3 v)
    {
        
        float length = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);

    if (length == 0)
        return {0, 0, 0};

    return
    {
        v.x / length,
        v.y / length,
        v.z / length
    };


    }

float edge(Vec2 a, Vec2 b, Vec2 c)
{
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}
//barycentric rasterizer 
void drawTriangle(Vec2 a, Vec2 b, Vec2 c, char pixel, COLOR col)
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
            plot(x, y, pixel, col);
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
switch (colorBuffer[i][j])
{
    case GRAY:
    cout << "\033[37m";   // Light Gray
    break;

case CYAN:
    cout << "\033[36m";   // Normal Cyan
    break;

    default:
        cout << "\033[0m";
        break;
}

cout << framebuffer[i][j];
cout << "\033[0m";
        
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

    
for (int i = 0; i < 6; i++)
{

    Vec3 ve0 = rotatedVerts[cubeFaces[i][0]];
Vec3 ve1 = rotatedVerts[cubeFaces[i][1]];
Vec3 ve2 = rotatedVerts[cubeFaces[i][2]];

    Vec3 edge1 = subtract(ve0, ve1);
    Vec3 edge2 = subtract(ve0, ve2);

    
Vec3 normal = normalize(cross(edge1, edge2));

   Vec3 light = normalize({0, 0, -1});

    COLOR col = colors[i];

float brightness = dot(normal, light);

       
        brightness = max(0.0f, brightness);

const char shades[] = "@$#~;";

int idx = min(4, max(0, (int)(brightness * 4)));

char pixel = shades[idx];


if (brightness > 0.0f)
{
    Vec2 v0 = project(rotatedVerts[cubeFaces[i][0]]);
    Vec2 v1 = project(rotatedVerts[cubeFaces[i][1]]);
    Vec2 v2 = project(rotatedVerts[cubeFaces[i][2]]);
    Vec2 v3 = project(rotatedVerts[cubeFaces[i][3]]);   
            
    
    drawTriangle(v0, v1, v2, pixel, col);
    drawTriangle(v0, v2, v3, pixel, col);
}
}

    
   
    present();
    cout.flush();
   this_thread::sleep_for(std::chrono::milliseconds(35));
}
    return 0;
}

