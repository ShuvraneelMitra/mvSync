#include <stdlib.h>
#include <iostream>
#include <memory>
#include <GLFW/glfw3.h>

struct GLFWDeleter {
    void operator()(GLFWwindow* window) const {
        if (window) glfwDestroyWindow(window);
    }
};

bool load_frame(std::string filepath, int& w, int& h, unsigned char** data);

int main(int argc, const char** argv) {
    if (!glfwInit()) {
        std::cerr << "Couldn't initialise GLFW\n";
        return 1;
    }

    std::unique_ptr<GLFWwindow, GLFWDeleter> window{glfwCreateWindow(640, 480, "Hello World", NULL, NULL)};
    if (!window) {
        std::cerr << "Could not open window\n";
        return 1;
    }

    int frame_width{0}, frame_height{0};
    unsigned char* frame_data;
    if (!load_frame("/home/shuvraneel/Desktop/mvSync/Demo.mp4", frame_width, frame_height, &frame_data)) {
        std::cerr << "Couldn't load video frame\n";
        return 1;
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_width, frame_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame_data);

    while (!glfwWindowShouldClose(window.get())) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int w, h;
        glfwGetFramebufferSize(window.get(), &w, &h);
        glViewport(0, 0, w, h);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, w, h, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glBindTexture(GL_TEXTURE_2D, texture_hdl);
        glBegin(GL_QUADS);
        glTexCoord2d(0, 0); glVertex2i(200, 200);
        glTexCoord2d(0, 1); glVertex2i(200, 200 + frame_height);
        glTexCoord2d(1, 1); glVertex2i(200 + frame_width, 200 + frame_height);
        glTexCoord2d(1, 0); glVertex2i(200 + frame_width, 200);
        glEnd();

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    glDeleteTextures(1, &texture_hdl);
    glfwTerminate();
    return 0;
}
