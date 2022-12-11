### GameEngine Design

- Entry Point
- Application Layer
- Window Layer: Input and Event
- Render
- Render API abstraction
- Debugging support
- Scipting Language
- Memory Systems
- Entity-Component System(ECS)
- Physics
- File I/O, Virtual File System
- Build System and Custom asset types

#### 关于引擎项目的构建

**思路：**==引擎项目最终输出成engine.dll，而游戏项目或者其他应用项目被设计成exe，最终会将engine.dll链接到应用中；==

**开发流程：**引擎项目中开发功能，最终编译会生成lib,dll等；而应用项目需要包含引擎方法的头文件，以及依赖引擎项目的lib/dll，这样应用项目可以使用引擎的功能；

**注意点：**1）将编译好的engine.dll手动放入exe同文件夹下；2）在应用项目中需要添加引擎项目的应用，目的在于将engine.lib引入到应用项目中；否则编译时报错，使用的引擎方法会是无法解析的外部命令；原因就是没有找到定义；

#### 为引擎添加入口点

**目的：**将整个应用的入口放置引擎项目中；

**步骤：**将应用项目中的main()写入引擎的entrypoint.h中，然后将entrypoint.h和应用项目依赖的所有头文件写入一个头文件，放置应用项目中；

#### 引入第三方日志库

**思路：**采用第三方日志库：`spdlog(https://github.com/gabime/spdlog)`实现控制台输出日志；

**步骤：**1）在引擎内部向上封装一个Log类，提供静态方法，而底层则调用spdlog方法；2）为了便于日志使用，可以考虑在Log类基础上定义宏；

```cpp
//log marcos for engine
#define EXPLORE_CORE_LOG(Level,...) \
	if(#Level == "error") ::Explore::Log::GetCoreLogger()->error(__VA_ARGS__); \
	else if(#Level == "warn") ::Explore::Log::GetCoreLogger()->warn(__VA_ARGS__); \
	else if(#Level == "info") ::Explore::Log::GetCoreLogger()->info(__VA_ARGS__); \
	else if(#Level == "trace") ::Explore::Log::GetCoreLogger()->trace(__VA_ARGS__); \
```

#### 解决方案生成：Premake

**目的：**类似于cmake，premake用于生成项目的解决方案；

**特征：**它支持Lua语言脚本，在脚本设置项目所需的各个属性，利用premake.exe运行该脚本，便可跨平台产生不同的项目解决方案；

```bash
call vendor\bin\premake\premake5.exe vs2019
PAUSE
```

==需要了解部分lua语法：==

```lua
workspace "ExploreEngine" //sln
	architecture "x64" //platform

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "ExploreEngine/ThirdLib/glfw" //类似于C++中的include,将GLFW工程的premake5.lua包含进来，因此多了一个“GLFW”工程

project "ExploreEngine"
	location "ExploreEngine"
	kind "SharedLib" //dll
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}") //输出目录
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "enginepch.h" //pch
	pchsource "enginepch.cpp"

	files //包含文件
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.cpp"
	}

	includedirs //包含目录
	{
		"%{prj.name}/Src",
		"%{prj.name}/ThirdLib/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links //引用
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines //预处理器
		{
			"EXPLORE_PLATFORM_WINDOWS",
			"EXPLORE_BUILD_DLL",
			"EXPLORE_ENABLE_ASSERTS"
		}

		postbuildcommands //复制dll
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}
```

#### 项目的预编译头pch，加速编译速度

**过程：**1）在引擎项目中，可以使用预编译头，即将频繁使用的且耗时的头文件统一放入一个头文件enginepch.h中，而enginepch.cpp中只包含其头文件；2）对enginepch.cpp设置属性创建编译头文件，对整个引擎项目设置属性使用预编译头；3）项目所有cpp文件要包含预编译头，且放在第一个；

**理解：**C++项目是从每个源文件编译后链接，而某些头文件可能含大量源码，因此编译很慢，如果其被多个源文件包含，则存在重复编译；预编译头本质上是提前编译，存放成中间格式，因此包含预编译头的源文件不会再花时间去编译预编译头内的头文件，加快编译速度；

#### Event事件系统

**思路：**Event class & EventDispatcher class，实现事件基类，以及事件分发类；

**Event Class:** 事件基类提供一组虚接口，并在后续继承的自定义事件(App/Window/Mouse/Key)中重写接口；

**EventDispatcher Class:** 以事件初始化分发器，并提供分发接口，入参是处理该事件的回调函数；

#### 窗口创建与窗口事件

**思路：**创建window class，包含一组虚接口，不同平台重新实现，实现跨平台；windows系统上暂时采用GLFW实现窗口类，但需要注意的是GLFW并不支持DirectX，因此后续如果在windows系统上使用DirectX时，将采用Win32API重构窗口；

```c++
void WindowsWindow::Init(const WindowProps& props)
{
    m_Data.Title = props.Title;
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;

    EXPLORE_CORE_LOG(info, "Creating window: {0} ({1}, {2})", props.Title, props.Width, props.Height);

    if (!s_GLFWInitialized)
    {
        int success = glfwInit();
        EXPLORE_CORE_ASSERT(success, "Could not initialize GLFW");
        s_GLFWInitialized = true;
    }

    m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr); //GLFW实现窗口
    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);
}
```

**窗口事件：**（1）利用GLFW的事件回调函数捕获对应事件，并生成我们自定义的事件，利用Window Class中m_Data的回调函数处理事件；（2）在App层定义回调函数并绑定在window/m_Data中，回调函数里利用事件生成分配器，并分配给不同处理函数；

#### Layer层栈设计

**Layer class:**  抽象出层的概念，并提供一组虚接口，如允许每帧被调用的接口OnUpdata()，以及支持处理事件的接口OnEvent()；

而继承自Layer的层，允许每帧更新逻辑，以及接受事件处理；

在后续引擎中，会相继抽出GameLayer/DebugLayer/UILayer等；游戏层负责更新游戏逻辑，渲染游戏画面，调试层负责渲染一些调试工具，而UI层负责渲染引擎的editorUI；

**LayerStack class：**层栈的底层维护一个vector<Layer>，用于管理层；

而在App中，Run()中会遍历层栈中所有的层，调用其对应的OnUpdate()；同理，在事件回调里，调用各层的OnEvent()；

#### Glad与OpenGl

**功能：**检查图形驱动程序dll，然后加载我们知道的函数，便于c++代码调用OpenGL；

**配置：**使用网站在线配置下载，利用premake使其成为单独项目，在整个解决方案的premake文件包含Glad的premake文件；

#### ImGui模块

**ImGui：**一个C++的立即模式的图形用户界面库，没有任何第三方依赖，可以将ImGUI的源码直接加到项目中使用，也可以编译成dll, ImGUI使用DX或者OpenGL进行界面渲染；能够创建内容创建工具和可视化 / 调试工具（而不是普通最终用户的 UI）；

特点在于UI没有状态，每帧需要重新渲染；它输出优化的顶点缓冲区，你可以在你的 3D 管道启用的应用程序中随时呈现这些缓冲区。它是快速的、可移植的、与渲染器无关的和自包含的（没有外部依赖）。

**ImGui开发流程：**1）最终形态在于ui渲染是基于引擎的渲染器实现的，而不是直接依赖图形API；2）但考虑目前阶段引擎还没实现渲染功能，且后续渲染开发需要借助调式工具，因此在渲染功能之前引入ImGui，实现一部分ui功能；

**ImGui事件回调：**1）事件分发流程GLFW->App->ImGuiLayer->ImGuiEvent；2）ImGui通过设置ImGuiIO来处理对应事件；

**docking/viewport的实现：**目前ImGui支持浮动窗口；

```cpp
ImGuiIO& io = ImGui::GetIO(); (void)io;
io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
```

#### 输入轮询Input Polling

**含义：**引擎提供一个查询功能，允许用户查询当前输入状态，如某个键是否被按下、鼠标是否按下、当前光标位置等；

**实现：**1）参考window实现方式，引擎实现一个Input Class，向外提供静态查询方法，并且提供内部实现的虚接口；2）不同平台输入继承该类，重新实现接口；以及初始化Input静态对象；

#### glm数学库

**数学库的实现：**数学库为了加速，通常会使用SIMD(Single instruction multiple data，单指令多数据)指令集，本质上会在CPU上使用128位宽的寄存器进行乘法运算，实际上只需要使用一条指令实现矩阵运算；其中SSE/AVX均是SIMD指令集；

而为了使用这些指令集实现数学库，一般两种方式：1）使用汇编代码，调用SIMD指令；2）使用编译器内部函数，不同编译器可能变量命名，以及函数不一样；

**glm：**opengl mathmetics，基于C++的数学库，底层实现方式中有用到了SIMD指令；




