#include "EditorLayer.h"
#include "Platform/OpenGl/OpenGLShader.h"
#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Explore
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1.7f, true)
	{

	}

	void EditorLayer::OnAttach()
	{
		Renderer2D::Init();
		std::string path = "assets/textures/basketball.jpg";
		m_Texture = Texture2D::Create(path);
		m_SubTexture = SubTexture::CrereFromTexture(m_Texture, { 128,128 }, { 2,2 }, { 1,1 });
	
		FrameBufferSpecification spec;
		spec.Width = 1080; spec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(spec);

		m_ActiveScene = std::make_shared<Scene>();
		m_SpriteEntity = m_ActiveScene->CreateEntity("Sprite Entity");
		m_SpriteEntity.AddComponent<SpriteComponent>(glm::vec4{ 0.0f,1.0f,0.0f,1.0f });

		m_PrimaryCameraEntity = m_ActiveScene->CreateEntity("Primary Camera Entity");
		m_PrimaryCameraEntity.AddComponent<CameraComponent>();

		m_SecondCameraEntity = m_ActiveScene->CreateEntity("Second Camera Entity");
		m_SecondCameraEntity.AddComponent<CameraComponent>();
		m_SecondCameraEntity.GetComponent<CameraComponent>().bPrimary = false;
		m_SecondCameraEntity.GetComponent<CameraComponent>().Camera.SetOrthographicSize(2.0f);

		//camera controller script
		class CameraControllerEntity : public ScriptEntity
		{
			virtual void OnUpdate(Timestep ts) override
			{
				glm::mat4& transform = GetComponent<TransformComponent>().Transform;
				float speed = 5.0f;
				//change the third column (pos) in transform
				if (Input::IsKeyPressed(EXPLORE_KEY_A))
				{
					transform[3][0] -= ts * speed;
				}
				if (Input::IsKeyPressed(EXPLORE_KEY_D))
				{
					transform[3][0] += ts * speed;
				}
				if (Input::IsKeyPressed(EXPLORE_KEY_S))
				{
					transform[3][1] -= ts * speed;
				}
				if (Input::IsKeyPressed(EXPLORE_KEY_W))
				{
					transform[3][1] += ts * speed;
				}
			}
		};
		m_PrimaryCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraControllerEntity>();
		m_SecondCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraControllerEntity>();

	}

	void EditorLayer::OnDetach()
	{
		Renderer2D::Shutdown();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		EXPLORE_PROFILE_FUNCTION()

		if(m_ViewportFocused) m_CameraController.OnUpdate(ts);
		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		Renderer2D::ResetStats();
		//Renderer2D::BeginScene(m_CameraController.GetCamera());
		//Renderer2D::DrawQuad({ -1.0f,0.5f,-0.1f },0, { 1.0f,1.0f }, m_Color);
		//Renderer2D::DrawQuad({ 0.5f,0.5f,-0.1f }, 0,{ 1.0f,1.0f }, m_Color);
		//Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f },0, { 5.0f,5.0f }, m_Texture,5.0f);
		//Renderer2D::DrawQuad({ 2.0f,0.0f,-0.1f }, 0, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f }, m_SubTexture);
		m_ActiveScene->OnUpdate(ts);
		//Renderer2D::EndScene();
		m_FrameBuffer->UnBind();
	}

	void EditorLayer::OnImGuiRender()
	{
		EXPLORE_PROFILE_FUNCTION()

		static bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);
                ImGui::Separator();

                if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
                if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
                if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
                ImGui::Separator();

				if (ImGui::MenuItem("Close", NULL, false))
				{
					Application::Get().Close();
					dockspaceOpen = false;
				}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

		ImGui::Begin("Setting");
		auto Stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Status:");
		ImGui::Text("Draw Calls: %d", Stats.DrawCalls);
		ImGui::Text("Draw Quads: %d", Stats.DrawQuads);
		auto& color = m_SpriteEntity.GetComponent<SpriteComponent>().Color;
		ImGui::ColorEdit4("ColorPicker", glm::value_ptr(color));
		ImGui::Checkbox("Select CameraB", &m_SwitchCamera);
		m_PrimaryCameraEntity.GetComponent<CameraComponent>().bPrimary = !m_SwitchCamera;
		m_SecondCameraEntity.GetComponent<CameraComponent>().bPrimary = m_SwitchCamera;
		float& OrthoSize = m_SecondCameraEntity.GetComponent<CameraComponent>().Camera.GetOrthophicSize();
		ImGui::DragFloat("SetCameraSize", &OrthoSize);
		m_SecondCameraEntity.GetComponent<CameraComponent>().Camera.SetOrthographicSize(OrthoSize);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBolockEvent(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *(glm::vec2*)&viewportSize)
		{
			m_FrameBuffer->Resize(viewportSize.x, viewportSize.y);
			m_ViewportSize = { viewportSize.x,viewportSize.y };
			m_CameraController.OnResize(viewportSize.x, viewportSize.y);
			m_ActiveScene->OnSetViewportSize(viewportSize.x, viewportSize.y);
		}
		uint32_t textureId = m_FrameBuffer->GetColorAttachmentRenderId();
		ImGui::Image((void*)textureId, ImVec2{ m_ViewportSize.x,m_ViewportSize.y });
		ImGui::End();
		ImGui::PopStyleVar();

        ImGui::End();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_CameraController.OnEvent(event);
	}
}