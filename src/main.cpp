#include <stdlib.h>
#include <iostream>
#include <memory>
#include <GLFW/glfw3.h>

struct GLFWDeleter {
    void operator()(GLFWwindow* window) const {
        if (window) glfwDestroyWindow(window);
    }
};

int main(int argc, const char** argv){
    if (!glfwInit()) {
        std::cout << "Couldn't initialise GLFW\n";
        return 1; 
    }

    std::unique_ptr<GLFWwindow, GLFWDeleter> window(glfwCreateWindow(640, 480, "Hello World", NULL, NULL));
    if (!window) {
        std::cout << "Could not open window\n"; 
        return 1;
    }

    unsigned char* data = new unsigned char[100 * 100 * 3];
    for(int y=0; y<100; y++){
        for(int x=0; x<100; x++){
            data[y * 100 * 3 + x * 3    ] = 0xff;
            data[y * 100 * 3 + x * 3 + 1] = 0x04;
            data[y * 100 * 3 + x * 3 + 2] = 0xf3;
        }
    }

    glfwMakeContextCurrent(window.get());
    while(!glfwWindowShouldClose(window.get())) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawPixels(100, 100, GL_RGB, GL_UNSIGNED_BYTE, data);
        glfwSwapBuffers(window.get());
        glfwWaitEvents();
    }
    return 0;
}