#pragma once
#include "pch.hpp"
#include "Logger.hpp"
#include "Window.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/Mesh.hpp"
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
            Shader skinnedShader;
            {
                skinnedShader.Init();
                auto vertexShader = Assets::LoadFile("builtin/SkinnedMesh.vsh");
                auto fragmentShader = Assets::LoadFile("builtin/SkinnedMesh.fsh");
                skinnedShader.LoadVS(vertexShader);
                skinnedShader.LoadFS(fragmentShader);
            }
            Shader shader;
            {
                shader.Init();
                auto vertexShader = Assets::LoadFile("builtin/TexturedMesh.vsh");
                auto fragmentShader = Assets::LoadFile("builtin/TexturedMesh.fsh");
                shader.LoadVS(vertexShader);
                shader.LoadFS(fragmentShader);
            }

            // Creating the camera
            Camera camera;
            camera.transform.position.z = 8;
            camera.transform.position.x = 8;
            camera.transform.position.y = 1;
            camera.transform.rotation.y = -45;

            Animation anim;
            anim.length = 3.0f;
            anim.bonekeys.push_back(
                Animation::BoneKey(
                    "Thigh.L",
                    {
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, -50), glm::vec3(1)), 0.0f),
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, 30), glm::vec3(1)), 1.0f),
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, -50), glm::vec3(1)), 3.0f)
                    }
                )
            );
            anim.bonekeys.push_back(
                Animation::BoneKey(
                    "Leg.L",
                    {
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, 0), glm::vec3(1)), 0.0f),
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, 10), glm::vec3(1)), 1.5f),
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, 45), glm::vec3(1)), 2.0f),
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, 60), glm::vec3(1)), 2.5f),
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, 0), glm::vec3(1)), 3.0f)
                    }
                )
            );
            anim.bonekeys.push_back(
                Animation::BoneKey(
                    "Thigh.R",
                    {
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, -30), glm::vec3(1)), 0.0f),
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, 50), glm::vec3(1)), 1.0f),
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, -30), glm::vec3(1)), 3.0f)
                    }
                )
            );
            anim.bonekeys.push_back(
                Animation::BoneKey(
                    "Leg.R",
                    {
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, -10), glm::vec3(1)), 0.0f),
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, -45), glm::vec3(1)), 0.5f),
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, -60), glm::vec3(1)), 1.0f),
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, 0), glm::vec3(1)), 1.5f),
                        Animation::Keyframe(Transform(glm::vec3(0), glm::vec3(0, 0, -10), glm::vec3(1)), 3.0f)
                    }
                )
            );

            // Mesh Load
            Mesh tiger = Assets::LoadMesh("../FBX2Mesh/teste.mesh");

            // Skinned Mesh Load
            SkinnedMesh character = Assets::LoadSkinnedMesh("../FBX2SMesh/teste.smesh");

            //character.root.transform.rotation.y = 45;
            //tiger.transform.position.x = 100;
            //tiger.transform.position.y = 150;
            //tiger.transform.position.z = -400;
            //tiger.transform.scale *= 2;
            //tiger.transform.rotation.y = -30;

            // Game Loop
            while (window.IsRunning())
            {
                // Getting Events, Clearing the Screen and Using the Shader
                window.GetEvents();
                window.Clear();

                // Commands
                if (window.GetKeyDown(GLFW_KEY_ESCAPE))
                    break;

                // Rendering Mesh
                shader.Use();
                shader.UploadMat4("camera", camera.GetCameraMatrix());
                shader.UploadMat4("model", tiger.transform.GetTransformationMatrix());
                //tiger.Render();

                // Rendering Skinned Mesh
                skinnedShader.Use();
                skinnedShader.UploadMat4("camera", camera.GetCameraMatrix());
                skinnedShader.UploadMat4("model", character.transform.GetTransformationMatrix());
                character.ApplyAnimation(character.animations[0], glfwGetTime() * 10);
                //character.ApplyAnimation(anim, glfwGetTime());
                character.CalculateBoneTransform();
                skinnedShader.UploadBones(character.root);
                character.Render();

                // Rendering to the Screen
                window.Present();
            }

            // Cleaning the Memory
            shader.Terminate();
            skinnedShader.Terminate();
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
    };
}