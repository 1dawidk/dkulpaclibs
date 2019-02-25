#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

float xRot;
float yRot;
float zoom;
int zoomKeyDown=0;
int rotKeyDown=0;

void onKey(GLFWwindow *wnd, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(wnd, 1);
    } else if(key==GLFW_KEY_C && action==GLFW_PRESS){
        xRot=0;
        yRot=0;
        zoom=-10;
    } else if (key==GLFW_KEY_LEFT_SHIFT ){
        if(action==GLFW_REPEAT || action==GLFW_PRESS)
            zoomKeyDown=1;
        else
            zoomKeyDown=0;
    } else if (key==GLFW_KEY_LEFT_CONTROL ){
        if(action==GLFW_REPEAT || action==GLFW_PRESS)
            rotKeyDown=1;
        else
            rotKeyDown=0;
    }
}

void onCursorMove(GLFWwindow *wnd, double x, double y){
    if(rotKeyDown) {
        xRot += (float) (5 * (y - 100) * 2 * 3.14 / 360.0);
        yRot += (float) (5 * (100 - x) * 2 * 3.14 / 360.0);
    } else if(zoomKeyDown) {
        zoom -= (float) (1 * (y - 100) * 2 * 3.14 / 360.0);
    }

    glfwSetCursorPos(wnd, 100, 100);
}

GLFWwindow* initWindow(const int resX, const int resY, GLint *w, GLint *h)
{
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    xRot=0;
    yRot=0;
    zoom=-10;

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(resX, resY, "Mort Motion Simulator", glfwGetPrimaryMonitor(), NULL);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, 100, 100);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, onKey);
    glfwSetCursorPosCallback(window, onCursorMove);

    // Get info of GPU and supported OpenGL version
    cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "OpenGL version supported " << glGetString(GL_VERSION) << endl;

    glEnable(GL_DEPTH_TEST); // Depth Testing
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    /* We have a color array and a vertex array */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glfwGetWindowSize(window, w, h);
    cout << "Window size: " << *w << ", " << *h << endl;
    glViewport(0, 0, *w, *h);

    return window;
}

int main() {
    srand(time(NULL));

    GLint windowWidth, windowHeight;
    GLFWwindow *window = initWindow(1024, 620, &windowWidth, &windowHeight);

    while (!glfwWindowShouldClose(window)) {
        // Draw stuff

        //Clear screen
        glClearColor(0.4, 0.4, 0.4, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION_MATRIX);
        glLoadIdentity();
        gluPerspective( 60, (double)windowWidth / (double)windowHeight, 0.1, 100 );

        glMatrixMode(GL_MODELVIEW_MATRIX);
        glTranslatef(0,0,zoom);
        glRotatef(xRot, 1, 0, 0);
        glRotatef(yRot, 0, 1, 0);


        //Draw elements to Screen Buffer


        // Update Screen
        glfwSwapBuffers(window);

        // Check for any input, or window movement
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}