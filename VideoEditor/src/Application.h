#pragma once
#include "src/Window.h"
#include <queue>
#include <memory>
namespace VideoEditor {

	struct ApplicationSpecification
	{

	};
	

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application();

		void Run();
		void Close();

		virtual void OnInit();
		virtual void OnShutdown();
		virtual void OnUpdate();

		virtual void OnEvent();

		void RenderImGui();

		inline Window& GetWindow() { return *m_window; }


	private:
		std::unique_ptr<Window> m_Window;
	};

	Application* CreateApplication(int argc, char** argv);
}