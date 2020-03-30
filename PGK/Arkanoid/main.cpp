// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "common/shader.hpp"

#include "extraFunctions.hpp"
#include "intersection.hpp"

using namespace std;

const char *vertexShaderSource = 
    "#version 330 core\n"
    "layout(location = 0) in vec2 aPos;\n"
    "out vec4 fragmentColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position.xy = vec2(aPos.x, aPos.y);\n"
    "   gl_Position.w = 1.0f;\n"
    "   if((gl_VertexID % 6) == 0 || (gl_VertexID % 6) == 1 || (gl_VertexID % 6) == 2)\n"
    "       fragmentColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
    "   else\n"
    "       fragmentColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

const char *fragmentShaderSource = 
    "#version 330 core\n"
    "in vec4 fragmentColor;\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "   color = fragmentColor;\n"
    "}\n\0";

const char *fragmentShaderBgSource = 
    "#version 330 core\n"
    "out vec3 bgfragmentColor;\n"
    "uniform vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   bgfragmentColor = ourColor;\n"
    "}\n\0";

GLFWwindow* window;
static int windowX, windowY;
static int currIndex = 0;
GLfloat bricks[10000];
static int over = 0;
vec2 middle;
vec2 velocity = vec2(0, 0);
vec2 ballMiddle;

GLfloat offset;
GLfloat brick_side_l;
GLfloat brick_height;
GLfloat draw_line_start;
int allIndex;

float sidex1,sidey1,sidex2,sidey2;

int cross();

int checkCollision();

void windowSizeCallback(GLFWwindow* window, int width, int height);

void newGame();

vec2 trianglesIntersection(double ax, double ay, double bx, double by, double cx, double cy,
                          double dx, double dy, double ex, double ey, double fx, double fy);

void setupBricks();
int main(){
    // Initialise GLFW
    srand(time(NULL));
    glewExperimental = true; // Needed for core profile
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL


    window = glfwCreateWindow( 1000, 1000, "Tutorial 01", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window.\n" );
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental=true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);        // input from keyboard

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);


    // Create and compile our GLSL program from the O
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);     // fragment shader for lines / triangles
    GLuint fragmentShaderBg = glCreateShader(GL_FRAGMENT_SHADER); // fragment shader for background

    GLuint shaderProgram  = glCreateProgram();
    GLuint shaderProgramBg = glCreateProgram(); // background shader program

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glShaderSource(fragmentShaderBg, 1, &fragmentShaderBgSource, NULL);
    glCompileShader(fragmentShaderBg);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glAttachShader(shaderProgramBg, vertexShader);
    glAttachShader(shaderProgramBg, fragmentShaderBg);
    glLinkProgram(shaderProgramBg);

    GLuint VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    static GLfloat bg[] = {
        -1.0f,  -1.0f,
        -1.0f,  0.0f,
         0.0f, -1.0f,

         0.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  0.0f,

         1.0f,  0.0f,
         1.0f,  1.0f,
         0.0f,  1.0f,

         0.0f,  1.0f,
        -1.0f,  1.0f,
        -1.0f,  0.0f,

        -0.5f,  0.0f,
         0.0f, -0.5f,
         0.0f,  0.5f,

         0.25f,  0.0f,
         0.0f, -0.25f,
         0.0f,  0.25f,

         0.25f,  0.0f,
         0.5f, -0.25f,
         0.5f,  0.25f,

         0.75f,  0.0f,
         0.5f, -0.25f,
         0.5f,  0.25f
    };


     // setup of background
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bg), bg, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    offset = (float)2/40;
    brick_side_l = (float)(2 - 2*offset)/12;
    brick_height = (float)sqrt(3)*brick_side_l/2;
    draw_line_start = 0.5f;

    setupBricks();

    int start = 0;

    // setup of objects (lines / triangles)
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bricks), bricks, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glfwGetWindowSize(window, &windowX, &windowY);
    //glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    do{
    	double startTime = glfwGetTime();
        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double h = (brick_side_l * sqrt(3))/2;
        double x = 2*h/3;
        ballMiddle = vec2(bricks[0], bricks[1] + x);

        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) { newGame(); }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && start != 0 && bricks[10]>=-0.98f + offset) { bricks[6]-=0.02f; bricks[8]-=0.02f; bricks[10]-=0.02f; }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && start != 0 && bricks[8]<=0.98f - offset) { bricks[6]+=0.02f; bricks[8]+=0.02f; bricks[10]+=0.02f; }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && start == 0)
        {
            float xVel = (float)(rand() % 100 - 50)/10000;
            float yVel = (float)(rand() % 100)/10000;

            velocity.x = xVel;
            velocity.y = yVel;

            start = 1;
        }

        if(checkCollision() == true ){
        }

        if(bricks[4] <= -1.0f - velocity.x + offset)
        {
            velocity.x = -velocity.x;
        }

        if(bricks[2] > 1.0f - velocity.x - offset)
        {
            velocity.x = -velocity.x;
        }

        if(bricks[3] > 1.0f - velocity.y - offset)
        {
            velocity.y = -velocity.y;
        }


        bricks[0] += velocity.x;
        bricks[2] += velocity.x;
        bricks[4] += velocity.x;

        bricks[1] += velocity.y;
        bricks[3] += velocity.y;
        bricks[5] += velocity.y;



         // Set the background and its color by using uniform:
        float timeValue = glfwGetTime() * 0.75f;
        float colorValue = (sin(timeValue) / 2.0f) + 0.5f;
        float rValue, gValue, bValue;
        int vertexColorLocation = glGetUniformLocation(shaderProgramBg, "ourColor");
        glUseProgram(shaderProgramBg);

        if(over == 0){
        rValue = 1.0f - 0.98f * colorValue; 
        gValue = 0.25f + abs(0.5f - colorValue);
        bValue = colorValue;
        }
        else
        {
            if (rValue < 1.0f)   rValue *= 1.01f;
            if (gValue > 0.0f)   gValue *= 0.95f;
            if (bValue > 0.0f)   bValue *= 0.95f;

            if (rValue >= 0.98f) rValue = 1.0f;
            if (gValue <= 0.05f) gValue = 0.0f;
            if (bValue <= 0.05f) bValue = 0.0f;

            for (int n = 0; n < currIndex; n++)
                bricks[n] *= 0.96f;
        }

        glUniform3f(vertexColorLocation, rValue, gValue, bValue);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
        glBindVertexArray(VAOs[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(bg), bg, GL_STREAM_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 24);
        glEnableVertexAttribArray(0);



        glUseProgram(shaderProgram);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
        glBindVertexArray(VAOs[0]);

        glBufferData(GL_ARRAY_BUFFER, sizeof(bricks), bricks, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLES, 0, allIndex); 
        glDisableVertexAttribArray(0);


        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        double elapsedTime = glfwGetTime() - startTime;

        while(elapsedTime > 0)
        {
        	elapsedTime--;
        }

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

            // Cleanup VBO
    glDeleteBuffers(2, VBOs);
    glDeleteVertexArrays(2, VAOs);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;

}

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    windowY = height;
    windowX  = width;
    glViewport(0, 0, windowX, windowY);
}

void newGame()
{

    over = 0;
}

int checkCollision()
{
        for(int i=6; i<currIndex; i=i+6){
        	vec2 vect = trianglesIntersection(bricks[0], bricks[1], bricks[2], bricks[3], bricks[4], bricks[5],
                                     bricks[i], bricks[i+1], bricks[i+2], bricks[i+3], bricks[i+4], bricks[i+5]);
            if(vect != vec2(0,0))
                {
                    if(i >= 12){
                        bricks[i] = 0.0f;
                        bricks[i+1] = 0.0f;
                        bricks[i+2] = 0.0f;
                        bricks[i+3] = 0.0f;
                        bricks[i+4] = 0.0f;
                        bricks[i+5] = 0.0f;
                    }

                	double vectLen = sqrt(vect.x*vect.x + vect.y*vect.y);

                	double velLen = sqrt(velocity.x*velocity.x + velocity.y*velocity.y);

                	vec2 vectNorm = vec2(vect.x/vectLen, vect.y/vectLen);

                	vec2 perpClockwise = vec2(vectNorm.y, -vectNorm.x);

                	vec2 perpCounterClockwise = vec2(-vectNorm.y, vectNorm.x);

                	vec2 velocityNorm = vec2(velocity.x / velLen, velocity.y / velLen);

                	ballMiddle = ballMiddle - velocity*2.0f;

                	int ballSide = sign((ballMiddle.x - sidex1)*(sidey2 - sidey1) - (ballMiddle.y - sidey1)*(sidex2 - sidex1));

                	vec2 determineTemp = ballMiddle + perpClockwise * 5.0f;

                	int determinedSide = sign((determineTemp.x - sidex1)*(sidey2 - sidey1) - (determineTemp.y - sidey1)*(sidex2 - sidex1));

                	if(determinedSide == ballSide)
                		velocity = velocityNorm - perpClockwise * (2.0f * dot(velocityNorm, perpClockwise));
                	else
                		velocity = velocityNorm - perpCounterClockwise * (2.0f * dot(velocityNorm, perpCounterClockwise));
                	velocity*= velLen;
                    return true;
                }
        }

    return false;
}

vec2 trianglesIntersection(double ax, double ay, double bx, double by, double cx, double cy,
                          double dx, double dy, double ex, double ey, double fx, double fy)
{
    if(doIntersect(ax,ay,bx,by,dx,dy,ex,ey) || doIntersect(ax,ay,cx,cy,dx,dy,ex,ey) || doIntersect(bx,by,cx,cy,dx,dy,ex,ey))
    {
    	middle = vec2((dx + ex)/2, (dy + ey)/2);
    	sidex1 = dx;
    	sidey1 = dy;
    	sidex2 = ex;
    	sidey2 = ey;
    	return vec2((ex-dx), (ey - dy));
    }
    else if(doIntersect(ax,ay,bx,by,dx,dy,fx,fy) || doIntersect(ax,ay,cx,cy,dx,dy,fx,fy) || doIntersect(bx,by,cx,cy,dx,dy,fx,fy))
    {
    	middle = vec2((dx + fx)/2, (dy+fy)/2);
    	sidex1 = dx;
    	sidey1 = dy;
    	sidex2 = fx;
    	sidey2 = fy;
    	return vec2((fx - dx), (fy - dy));
    }
    else if(doIntersect(ax,ay,bx,by,ex,ey,fx,fy) || doIntersect(ax,ay,cx,cy,ex,ey,fx,fy) || doIntersect(bx,by,cx,cy,ex,ey,fx,fy))
    {
    	middle = vec2((ex+fx)/2, (ey + fy)/2);
    	sidex1 = ex;
    	sidey1 = ey;
    	sidex2 = fx;
    	sidey2 = fy;
    	return vec2((fx-ex), (fy - ey));
    }

    return vec2(0,0);
}

void setupBricks()
{
	bricks[0] = 0.0f;
    bricks[1] = -1 + (float)2/5 + 0.002f;

    bricks[2] = 0.0f + (float)2/50;
    bricks[3] = -1 + (float)5/10;

    bricks[4] = 0.0f - (float)2/50;
    bricks[5] = -1 + (float)5/10;

    currIndex += 6;

    bricks[6] = 0.0f;
    bricks[7] = -1 + (float)2/10;

    bricks[8] = 0.0f + (float)2/20;
    bricks[9] = -1 + (float)2/5;

    bricks[10] = 0.0f - (float)2/20;
    bricks[11] = -1 + (float)2/5;

    currIndex += 6;


    for(int i = 12; i < 156; i = i+12)
    {
        bricks[i] = -1 + offset + ((i-12)/12)*brick_side_l;
        bricks[i+1] = draw_line_start;

        bricks[i+2] = -1 + offset + ((i-12)/12+1)*brick_side_l;
        bricks[i+3] = draw_line_start;


        bricks[i+4] = -1 + offset + (float)(2*(i-12)/12+1)/2*brick_side_l;
        bricks[i+5] = draw_line_start - brick_height;

        currIndex += 6;
    }
    for(int i = 18; i < 144; i = i+12)
    {
        bricks[i] = -1 + offset + (float)(2*(i - 18)/12+1)/2*brick_side_l;
        bricks[i+1] = draw_line_start - brick_height;

        bricks[i+2] = -1 + offset + (float)(2*(i - 6)/12+1)/2*brick_side_l;
        bricks[i+3] = draw_line_start - brick_height;


        bricks[i+4] = -1 + offset + ((i-18)/12+1)*brick_side_l;
        bricks[i+5] = draw_line_start;

        currIndex += 6;
    }

    /*for(int i = 24; i < 156; i = i+18)
    {
        bricks[i] = -1 + offset + (float)(2*(i - 18)/12+1)/2*brick_side_l;
        bricks[i+1] = -2*draw_line_start - brick_height;

        bricks[i+2] = -1 + offset + (float)(2*(i - 6)/12+1)/2*brick_side_l;
        bricks[i+3] = -2*draw_line_start - brick_height;


        bricks[i+4] = -1 + offset + ((i-18)/12+1)*brick_side_l;
        bricks[i+5] = -2*draw_line_start;

        currIndex += 6;
    }*/

    allIndex = currIndex;
    // left-left
    bricks[allIndex]     = -1.0f;
    bricks[allIndex + 1] = -1.0f;
    bricks[allIndex + 2] = -1.0f + offset;
    bricks[allIndex + 3] = -1.0f;
    bricks[allIndex + 4] =  -1.0f;
    bricks[allIndex + 5] =  1.0f;
    allIndex += 6;

    // left-right
    bricks[allIndex]     = -1.0f + offset;
    bricks[allIndex + 1] = -1.0f;
    bricks[allIndex + 3] =  1.0f;
    bricks[allIndex + 4] = -1.0f;
    bricks[allIndex + 5] =  1.0f;
    bricks[allIndex + 2] = -1.0f + offset;

    allIndex += 6;


    // right-right
    bricks[allIndex]     = 1.0f;
    bricks[allIndex + 1] = 1.0f;
    bricks[allIndex + 2] = 1.0f - offset;
    bricks[allIndex + 3] = 1.0f;
    bricks[allIndex + 4] = 1.0f - offset;
    bricks[allIndex + 5] = -1.0f;
    allIndex += 6;

        // right -left
    bricks[allIndex]     = 1.0f;
    bricks[allIndex + 1] = 1.0f;
    bricks[allIndex + 2] = 1.0f - offset;
    bricks[allIndex + 3] = -1.0f;
    bricks[allIndex + 4] = 1.0f;
    bricks[allIndex + 5] = -1.0f;
    allIndex += 6;


        // up-up
    bricks[allIndex]     = -1.0f + offset;
    bricks[allIndex + 1] = 1.0f;
    bricks[allIndex + 2] = 1.0f - offset;
    bricks[allIndex + 3] = 1.0f;
    bricks[allIndex + 4] = 1.0f - offset;
    bricks[allIndex + 5] = 1.0f - offset;
    allIndex += 6;

    // up-down
    bricks[allIndex]     = -1.0f + offset;
    bricks[allIndex + 1] = 1.0f;
    bricks[allIndex + 2] = -1.0f + offset;
    bricks[allIndex + 3] = 1.0f - offset;
    bricks[allIndex + 4] = 1.0f - offset;
    bricks[allIndex + 5] = 1.0f - offset;
    allIndex += 6;
}