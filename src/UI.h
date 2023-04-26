#pragma once

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_impl_glfw.h>

#include <stb/stb_image_write.h>

#include "Scene/Scene.h"
#include "Camera.h"
#include "Scene/Image.h"
#include "Renderer.h"
#include <filesystem>

#include "Loader.h"
#include "OpenGL/Window.h"
#include "OpenGL/OpenGL.h"

class UI
{
private:
	char fileName[200];
	char imageName[200];

	Window* m_window = nullptr;
	Renderer* m_ActiveRenderer = nullptr;

	float m_sceneFPS;
	float m_sceneDiff;
	int m_width, m_height;
	bool m_firstClick = true;
	float m_mouseSensitivity = 50.05f;
	float m_speed = 10.05f;

	void ImGuiRender();
	void UpdateImGui();
	void SetImGuiDarkTheme();

	void CameraEditor();
	void ShapesEditor();
	void ImageSave();
	void ContentBrowser();

	std::filesystem::path m_assetsDir = "assets";
	std::filesystem::path m_currentDir = "assets";

public:
	UI(glm::ivec2 dimensions);
	void Update(Renderer& renderer, float FPS, float diff);
	bool IsRunning();
	GLFWwindow* GetGlfwWndPtr() { return m_window->GetID(); }
};

