#pragma once

//Used by ThirdApplication
#include "Explore/Core/Core.h"
#include "Explore/Core/Application.h"
#include "Explore/Core/input.h"
#include "Explore/Core/KeyCode.h"
#include "Explore/Core/Log.h"
#include "Explore/ImGui/ImGuiLayer.h"
#include "Explore/Core/Timestep.h"
#include "Explore/Debug/Instrumentor.h"

//Render function
#include "Explore/Renderer/Renderer.h"
#include "Explore/Renderer/Renderer2D.h"
#include "Explore/Renderer/RenderCommand.h"
#include "Explore/Renderer/FrameBuffer.h"
#include "Explore/Renderer/Shader.h"
#include "Explore/Renderer/Texture.h"
#include "Explore/Renderer//SubTexture.h"
#include "Explore/Renderer/OrthographicCamera.h"
#include "Explore/Renderer/OrthographicCameraController.h"

//Scene
#include "Explore/Scene/Scene.h"
#include "Explore/Scene/Entity.h"
#include "Explore/Scene/ScriptEntity.h"
#include "Explore/Scene/Components.h"
#include "Explore/Scene/SceneSerializer.h"

//entry point