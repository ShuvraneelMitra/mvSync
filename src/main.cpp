#include <stdlib.h>
#include <iostream>
#include <memory>
#include <GLFW/glfw3.h>

struct GLFWDeleter {
    void operator()(GLFWwindow* window) const {
        if (window) glfwDestroyWindow(window);
    }
};

int main(int argc, const char** argv) {
    if (!glfwInit()) {
        std::cout << "Couldn't initialise GLFW\n";
        return 1;
    }

    std::unique_ptr<GLFWwindow, GLFWDeleter> window{glfwCreateWindow(640, 480, "Hello World", NULL, NULL)};
    if (!window) {
        std::cout << "Could not open window\n";
        return 1;
    }

    unsigned char* data = new unsigned char[100 * 100 * 3];
    for (int y = 0; y < 100; y++) {
        for (int x = 0; x < 100; x++) {
            data[y * 100 * 3 + x * 3] = 0xff;       
            data[y * 100 * 3 + x * 3 + 1] = 0x04;  
            data[y * 100 * 3 + x * 3 + 2] = 0xf3;  
        }
    }

    glfwMakeContextCurrent(window.get());
    glEnable(GL_TEXTURE_2D);

    GLuint texture_hdl;
    glGenTextures(1, &texture_hdl);
    glBindTexture(GL_TEXTURE_2D, texture_hdl);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 100, 100, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    delete[] data;

    while (!glfwWindowShouldClose(window.get())) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int w, h;
        glfwGetFramebufferSize(window.get(), &w, &h);
        glViewport(0, 0, w, h);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, w, 0, h, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glBindTexture(GL_TEXTURE_2D, texture_hdl);
        glBegin(GL_QUADS);
        glTexCoord2d(0, 0); glVertex2i(200, 200);
        glTexCoord2d(0, 1); glVertex2i(200, 200 + 100);
        glTexCoord2d(1, 1); glVertex2i(200 + 100, 200 + 100);
        glTexCoord2d(1, 0); glVertex2i(200 + 100, 200);
        glEnd();

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    glDeleteTextures(1, &texture_hdl);
    glfwTerminate();
    return 0;
}
