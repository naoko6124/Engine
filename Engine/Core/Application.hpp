#pragma once
#include "Logger.hpp"
#include "Window.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/SkinnedMesh.hpp"
#include "Utils/Assets.hpp"

namespace Engine
{
    class Application
    {
    public:
        void Run()
        {
            Logger::Log("Engine %s %.1f", "Alpha", 0.1);
            
            // Creating Window
            window.Init("Engine", 1280, 720);
            window.SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
            
            // Loading Shaders
            shader.Init();
            auto vertexShader = Assets::LoadFile("builtin/SkinnedMesh.vsh");
            shader.LoadVS(vertexShader);
            auto fragmentShader = Assets::LoadFile("builtin/SkinnedMesh.fsh");
            shader.LoadFS(fragmentShader);

            // Creating the camera
            Camera camera;
            camera.transform.position.z = 7;

            // Mesh Test
            SkinnedMesh mesh;
            std::vector<SkinnedMesh::Vertex> vertices = 
            {
                SkinnedMesh::Vertex({-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0, 0, 0, 0 }, {1.0f, 0.0f, 0.0f, 0.0f}),
                SkinnedMesh::Vertex({ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, { 0, 0, 0, 0 }, {1.0f, 0.0f, 0.0f, 0.0f}),

                SkinnedMesh::Vertex({ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, { 0, 1, 0, 0 }, {1.0f, 1.0f, 0.0f, 0.0f}),
                SkinnedMesh::Vertex({-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, { 0, 1, 0, 0 }, {1.0f, 1.0f, 0.0f, 0.0f}),
                
                SkinnedMesh::Vertex({ 0.5f,  1.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, { 1, 0, 0, 0 }, {1.0f, 1.0f, 0.0f, 0.0f}),
                SkinnedMesh::Vertex({-0.5f,  1.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 1, 0, 0, 0 }, {1.0f, 1.0f, 0.0f, 0.0f}),

                SkinnedMesh::Vertex({ 0.5f,  2.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, { 1, 0, 0, 0 }, {1.0f, 0.0f, 0.0f, 0.0f}),
                SkinnedMesh::Vertex({-0.5f,  2.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, { 1, 0, 0, 0 }, {1.0f, 0.0f, 0.0f, 0.0f})
            };
            mesh.SetVertices(vertices);
            std::vector<unsigned int> indices =
            {
                0, 1, 2,
                2, 3, 0,
                3, 2, 4,
                4, 5, 3,
                5, 4, 6,
                6, 7, 5
            };
            mesh.SetIndices(indices);
            mesh.SetTexture("builtin/tex.png");
            mesh.root.name = "Root";
            Bone b;
            b.name = "Teste";
            b.offset.position.y = 1;
            b.transform.rotation.z = 0;
            mesh.root.children.push_back(b);

            Animation anim;
            anim.length = 4.0f;
            anim.bonekeys.push_back(
                Animation::BoneKey(
                    "Teste",
                    {
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, 30.0f), glm::vec3(1)), 0.0f),
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, -30.0f), glm::vec3(1)), 2.0f),
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, 30.0f), glm::vec3(1)), 4.0f)
                    }
                )
            );

            // Game Loop
            while (window.IsRunning())
            {
                // Getting Events, Clearing the Screen and Using the Shader
                window.GetEvents();
                window.Clear();
                shader.Use();

                // Commands
                if (window.GetKeyDown(GLFW_KEY_ESCAPE))
                    break;
                if (window.GetKeyDown(GLFW_KEY_A))
                {
                    mesh.root.children[0].transform.rotation.z += 1;
                }
                if (window.GetKeyDown(GLFW_KEY_D))
                {
                    mesh.root.children[0].transform.rotation.z -= 1;
                }
                if (window.GetKeyDown(GLFW_KEY_LEFT))
                {
                    mesh.root.transform.rotation.z += 1;
                }
                if (window.GetKeyDown(GLFW_KEY_RIGHT))
                {
                    mesh.root.transform.rotation.z -= 1;
                }

                // Setting the Camera
                shader.UploadMat4("camera", camera.GetCameraMatrix());

                // Rendering Mesh
                shader.UploadMat4("model", mesh.transform.GetTransformationMatrix());
                mesh.ApplyAnimation(anim, glfwGetTime());
                mesh.CalculateBoneTransform();
                shader.UploadBones(mesh.root);
                mesh.Render();

                // Rendering to the Screen
                window.Present();
            }

            // Cleaning the Memory
            shader.Terminate();
            window.Terminate();
        }
        void OnEvent(Event* e)
        {
            switch(e->GetType())
            {
                case Event::MouseMove:
                {
                    EventMouseMove* event = reinterpret_cast<EventMouseMove*>(e);
                    break;
                }
                default:
                    break;
            }
        }
    private:
        Window window;
        Shader shader;
    };
}