#pragma once
#include "pch.hpp"
#include "Event.hpp"

namespace Engine
{
    class Window
    {
    public:
        void Init(std::string title, int width, int height)
        {
            glfwInit();
            window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
            mouseX = (int)(width/2.0f);
            mouseY = (int)(height/2.0f);
            glfwSetWindowUserPointer(window, (void*)this);
            glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
                Window* handler = (Window*)glfwGetWindowUserPointer(window);
                handler->ResizeFramebuffer(width, height);
                EventWindowResize event(width, height);
                handler->EventCallback(&event);
            });
            glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
                Window* handler = (Window*)glfwGetWindowUserPointer(window);
                if (action == GLFW_PRESS)
                {
                    EventKeyDown event(key);
                    handler->EventCallback(&event);
                }
                else if (action == GLFW_RELEASE)
                {
                    EventKeyUp event(key);
                    handler->EventCallback(&event);
                }
            });
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos){
                Window* handler = (Window*)glfwGetWindowUserPointer(window);
                EventMouseMove event((int)xpos, (int)ypos, (int)xpos - handler->mouseX, handler->mouseY - (int)ypos);
                handler->mouseX = (int)xpos;
                handler->mouseY = (int)ypos;
                handler->EventCallback(&event);
            });
            glfwMakeContextCurrent(window);
            gladLoaderLoadGL();
            glEnable(GL_DEPTH_TEST);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        }
        void Terminate()
        {
            glfwDestroyWindow(window);
            glfwTerminate();
        }
        void GetEvents()
        {
            glfwPollEvents();
        }
        void Clear()
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        void Present()
        {
            glfwSwapBuffers(window);
        }
        bool IsRunning()
        {
            return !glfwWindowShouldClose(window);
        }
        void SetEventCallback(std::function<void(Event*)> fn)
        {
            EventCallback = fn;
        }
        bool GetKeyDown(int keycode)
        {
            return glfwGetKey(window, keycode) == GLFW_PRESS;
        }
        bool GetKeyUp(int keycode)
        {
            return glfwGetKey(window, keycode) == GLFW_RELEASE;
        }
    private:
        std::function<void(Event*)> EventCallback;
        void ResizeFramebuffer(int w, int h)
        {
            glViewport(0, 0, w, h);
        }
    private:
        GLFWwindow* window;
        int mouseX, mouseY;
    };
}