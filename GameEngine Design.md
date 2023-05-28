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

##### 思路：

==引擎项目最终输出成engine.dll，而游戏项目或者其他应用项目被设计成exe，最终会将engine.dll链接到应用中；==

##### 开发流程：

引擎项目中开发功能，最终编译会生成lib,dll等；而应用项目需要包含引擎方法的头文件，以及依赖引擎项目的lib/dll，这样应用项目可以使用引擎的功能；

##### 注意点：

1）将编译好的engine.dll手动放入exe同文件夹下；2）在应用项目中需要添加引擎项目的应用，目的在于将engine.lib引入到应用项目中；否则编译时报错，使用的引擎方法会是无法解析的外部命令；原因就是没有找到定义；

##### ==问题：==

后期如果引擎使用到第三方库，而第三方库以lib方式构建在引擎dll中，而dll有能力从lib中删除没有使用的内容，因此如果链接该dll的exe文件，使用不存在的东西，就会出现链接错误； 理想情况下，确保lib中所有东西都导出到dll文件中，方案就是使用一个模块定义文件def；

#### 为引擎添加入口点

##### 目的：

将整个应用的入口放置引擎项目中；

##### 步骤：

将应用项目中的main()写入引擎的entrypoint.h中，然后将entrypoint.h和应用项目依赖的所有头文件写入一个头文件，放置应用项目中；

#### 引入第三方日志库

##### 思路：

采用第三方日志库：`spdlog(https://github.com/gabime/spdlog)`实现控制台输出日志；

##### 步骤：

1）在引擎内部向上封装一个Log类，提供静态方法，而底层则调用spdlog方法；2）为了便于日志使用，可以考虑在Log类基础上定义宏；

```cpp
//log marcos for engine
#define EXPLORE_CORE_LOG(Level,...) \
	if(#Level == "error") ::Explore::Log::GetCoreLogger()->error(__VA_ARGS__); \
	else if(#Level == "warn") ::Explore::Log::GetCoreLogger()->warn(__VA_ARGS__); \
	else if(#Level == "info") ::Explore::Log::GetCoreLogger()->info(__VA_ARGS__); \
	else if(#Level == "trace") ::Explore::Log::GetCoreLogger()->trace(__VA_ARGS__); \
```

#### 解决方案生成：Premake

##### 目的：

类似于cmake，premake用于生成项目的解决方案；

##### 特征：

它支持Lua语言脚本，在脚本设置项目所需的各个属性，利用premake.exe运行该脚本，便可跨平台产生不同的项目解决方案；

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

##### 过程：

1）在引擎项目中，可以使用预编译头，即将频繁使用的且耗时的头文件统一放入一个头文件enginepch.h中，而enginepch.cpp中只包含其头文件；2）对enginepch.cpp设置属性创建编译头文件，对整个引擎项目设置属性使用预编译头；3）项目所有cpp文件要包含预编译头，且放在第一个；

##### 理解：

C++项目是从每个源文件编译后链接，而某些头文件可能含大量源码，因此编译很慢，如果其被多个源文件包含，则存在重复编译；预编译头本质上是提前编译，存放成中间格式，因此包含预编译头的源文件不会再花时间去编译预编译头内的头文件，加快编译速度；

#### Event事件系统

##### 思路：

Event class & EventDispatcher class，实现事件基类，以及事件分发类；

##### Event Class: 

事件基类提供一组虚接口，并在后续继承的自定义事件(App/Window/Mouse/Key)中重写接口；

##### EventDispatcher Class: 

以事件初始化分发器，并提供分发接口，入参是处理该事件的回调函数；

#### 窗口创建与窗口事件

##### 思路：

创建window class，包含一组虚接口，不同平台重新实现，实现跨平台；windows系统上暂时采用GLFW实现窗口类，但需要注意的是GLFW并不支持DirectX，因此后续如果在windows系统上使用DirectX时，将采用Win32API重构窗口；

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

##### 窗口事件：

（1）利用GLFW的事件回调函数捕获对应事件，并生成我们自定义的事件，利用Window Class中m_Data的回调函数处理事件；（2）在App层定义回调函数并绑定在window/m_Data中，回调函数里利用事件生成分配器，并分配给不同处理函数；

#### Layer层栈设计

##### Layer class:  

抽象出层的概念，并提供一组虚接口，如允许每帧被调用的接口OnUpdata()，以及支持处理事件的接口OnEvent()；

而继承自Layer的层，允许每帧更新逻辑，以及接受事件处理；

在后续引擎中，会相继抽出GameLayer/DebugLayer/UILayer等；游戏层负责更新游戏逻辑，渲染游戏画面，调试层负责渲染一些调试工具，而UI层负责渲染引擎的editorUI；

##### LayerStack class：

层栈的底层维护一个vector<Layer>，用于管理层；

而在App中，Run()中会遍历层栈中所有的层，调用其对应的OnUpdate()；同理，在事件回调里，调用各层的OnEvent()；

#### Glad与OpenGl

##### 功能：

检查图形驱动程序dll，然后加载我们知道的函数，便于c++代码调用OpenGL；

##### 配置：

使用网站在线配置下载，利用premake使其成为单独项目，在整个解决方案的premake文件包含Glad的premake文件；

#### ImGui模块

##### ImGui：

一个C++的立即模式的图形用户界面库，没有任何第三方依赖，可以将ImGUI的源码直接加到项目中使用，也可以编译成dll, ImGUI使用DX或者OpenGL进行界面渲染；能够创建内容创建工具和可视化 / 调试工具（而不是普通最终用户的 UI）；

特点在于UI没有状态，每帧需要重新渲染；它输出优化的顶点缓冲区，你可以在你的 3D 管道启用的应用程序中随时呈现这些缓冲区。它是快速的、可移植的、与渲染器无关的和自包含的（没有外部依赖）。

##### ImGui开发流程：

1）最终形态在于ui渲染是基于引擎的渲染器实现的，而不是直接依赖图形API；2）但考虑目前阶段引擎还没实现渲染功能，且后续渲染开发需要借助调式工具，因此在渲染功能之前引入ImGui，实现一部分ui功能；

##### ImGui事件回调：

1）事件分发流程GLFW->App->ImGuiLayer->ImGuiEvent；2）ImGui通过设置ImGuiIO来处理对应事件；

##### docking/viewport的实现：

目前ImGui支持浮动窗口；

```cpp
ImGuiIO& io = ImGui::GetIO(); (void)io;
io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
```

#### 输入轮询Input Polling

##### 含义：

引擎提供一个查询功能，允许用户查询当前输入状态，如某个键是否被按下、鼠标是否按下、当前光标位置等；

##### 实现：

1）参考window实现方式，引擎实现一个Input Class，向外提供静态查询方法，并且提供内部实现的虚接口；2）不同平台输入继承该类，重新实现接口；以及初始化Input静态对象；

#### glm数学库

##### 数学库的实现：

数学库为了加速，通常会使用SIMD(Single instruction multiple data，单指令多数据)指令集，本质上会在CPU上使用128位宽的寄存器进行乘法运算，实际上只需要使用一条指令实现矩阵运算；其中SSE/AVX均是SIMD指令集；

而为了使用这些指令集实现数学库，一般两种方式：1）使用汇编代码，调用SIMD指令；2）使用编译器内部函数，不同编译器可能变量命名，以及函数不一样；

##### glm：

opengl mathmetics，基于C++的数学库，底层实现方式中有用到了SIMD指令；

#### 调整引擎为静态库

##### 现状：

目前引擎项目是输出dll，链接到应用项目exe中的，这种方式有一个问题，即当dll链接一个第三方库，dll有能力从lib中删除不使用的函数，当exe使用该第三方库时，会出现链接错误；

##### dll的优势：

dll负责链接所有第三方库，而应用项目只需要引用dll即可；当会出现上述问题；

##### 改成静态库：

静态的好处在于，所有内容会链接到exe文件中；

#### 渲染架构

##### **渲染分为两部分：**

1）与平台无关的渲染功能/Renderer，e.g:场景管理/裁剪/渲染等，以及材质/相机/视觉效果/后期等；2）与平台有关的图形API/RendererAPI，这部分涉及到一定的术语，如缓冲区/纹理/管线等；

![image-20221211225630591](C:\Users\HUAWEI\AppData\Roaming\Typora\typora-user-images\image-20221211225630591.png)

#### ==RendererAPI==

**渲染上下文：** 

context for RendererAPI，为渲染API服务，提供一个渲染目标；在openGL中，窗口句柄就是一个渲染上下文；

##### 着色器：

一段运行在GPU上的代码，主要包括顶点着色器/片段着色器，顶点着色器负责计算顶点在裁剪空间中的位置，片段着色器负责计算每个像素的颜色，同时需要做一些光照计算；

##### 内存缓冲区：

vertex buffer和index buffer，在GPU中分别存放顶点数据和索引数据； 

##### 缓冲区布局：

buffer layout，描述了内存缓冲区的数据布局，如含哪些数据，以及顶点/法线/颜色等数据长度和偏移量等；在OpenGL中通常使用vertexAttribute/顶点属性来描述布局； 

##### DrawCall:  

a call or command to draw something on the screen，该指令会告知显卡一组信息，如object对应的buffer信息，如运行哪个vertexShader以及piexlShader；在openGL中glDrawElements是一个drawcall；

##### 渲染流程：

当渲染一个3D scene，需要两方面信息，一方面与environment有关，如描述light分布或者cubemap的环境参数，提供viewMatrix和projectMatrix的相机；一方面与object有关，如自身的世界坐标，携带的材质material；而object自身的mesh数据会放入buffer中，在vertexShader中计算；自身的材质会塞入piexlShader，参与光照计算，获取对应颜色；

```c++
Renderer::BeginScene(camera,lights,environment); //contain infos related to scene
Renderer::Submit(); //submit vertexShader containing mesh data and piexl shader containing material data
Renderer::EndScene();
Renderer::Flush(); //using in the multi-thread game engine design
```

##### 视口：

viewport，通过glviewport告知opengl渲染区域（rendering area），尤其当窗口大小发生变化时，需要告知；

#### 相机

##### 相机理解：

相机可以理解成数据集合，包含Transform、FOV、near/far plane、aspect ratio等；Transform提供viewMatrix，可以用于转换场景所有物体在相机视角下的位置；FOV和aspect ratio提供projectionMatrix，对视锥内的物体投影到正交中，用于下一步屏幕转换；

```cpp
//顶点数据的处理
In GLM: column-major
Proj * view * model * vertexPos;
In DirectX: row-major
vertexPos * model * view * proj
```

##### 代码相关：

关于相机数据，可以在beginScene(camera)塞入，然后通过unifrom buffer，设置到shader中；而在uniform数据中可以分为两类，一类关于渲染相关的，如cameraMatrix，ModelMatrix，会设置到vertexShader中，还有环境参数光线数据，会设置到piexlShader中；一类关于材质的，会设置到piexlShader中；

##### timestep：

没有时间间隔参数时，相机的更新依赖于帧率，帧率高的引擎，相机移动快；为了解决该问题，在tick中引入时间间隔，确保相同时间内移动与帧率无关；

##### 相机控制器：

将相机控制相关的代码抽出放在一个专门的类里，本质上是一个camera warapper，该类支持tick和event，控制相机transform，以及响应resize和scroll事件；

#### 材质

##### 材质理解：

材质本质上是由一组数据和piexlshader组成，数据用来描述物体表面属性，通常以输入的方式（uniform）写入到piexlshader中，参与光照计算，获取最终颜色；

代码层面上，material class会接受一个shader参数，并且支持修改参数；同时会引入材质实例，其继承材质，用于区分base material，材质实例是材质的一个实例，继承同一个base material的不同材质实例的uniform值不同；其次，后续mesh接受材质实例，用于修改材质，并且渲染提交时不再是shader，而是材质；

##### 资产管理：

考虑到后续渲染器会采用延迟渲染，以及多线程，因此整个资产管理（如材质/shader/vertexArray等）需要考虑内存管理、线程安全；目前堆栈分配的资产采用shader_prt进行引用计数管理；

##### Texture：

本质上是一段数据集合buffer，可以去采样它获取数据；常见的应用，包括加载外部的贴图数据放入Texture，或者在Texture中存放预计算的数据，如LookUp Texture；

texture相关属性：mipmap多级纹理；filter贴图的过滤方式，决定了uv坐标在贴图中的采样方式；Alpha值：在混合中决定了自身颜色的占比；

##### shaderlibrary：

本质上是一个shader数组，存放已有的shader，并且提供接口进行访问；

#### ==2D Renderer==

##### plan：

1)batch rendering; 2)Animation; 3)UI layout; 4)HDR; 5)particle system; 6)post processing;

7)Interaction; 8)Scripting; 9)ECS；

#### Profile

##### 性能分析：

类似于IDE的性能分析工具，包括cpu使用率分析、内存使用分析等；

##### 关于引擎性能分析模块：

目前打算在引擎内实现一个简单的MiniProfiling，实现范围内时间计算，整体思路包括：抓取数据以及可视化数据；其中抓取时间数据底层本质上采用C++时钟差实现，而可视化采用google tracing实现，提供必要的json文件；

#### Batch Rendering

##### 含义：

以2d renderer为例，当一次draw call只渲染一个正方形时，则每次draw call绑定必要的buffer数据和shader即可，但当需要渲染1000个正方形时，则渲染提交1000次draw call，显然效率不高；批量渲染即在一次draw call时，渲染出1000个正方形；

##### 顶点处理：

正常情况下，一次draw call中，buffer存放顶点相对数据，transform设在vertexshader中，但显然在批量渲染中无法使用，因为每个正方形有不用的transform；因次方法是：把transform应用在顶点上，buffer中存放顶点的绝对位置，实现一次draw call渲染多个正方形；

##### 颜色处理：

通常情况下，渲染一个正方形时，会把颜色设置到fragmentShader中，实现颜色效果；在批量渲染中，考虑每个正方形有不同颜色，因此把颜色值写入vertexBuffer中，在vertexShader中读取并传给fragmentShader；

##### 贴图处理：

当渲染一个正方形时，会将贴图绑定在默认槽，并在fragmentShader中设置sampler为对应贴图槽即可；当一个draw call渲染多个正方形，首先依旧需要加载多张贴图并绑定贴图槽，以及设置fragmenet中多个sampler的贴图槽，区别在于需要在顶点buffer中添加贴图槽数据，确保不同正方形获取对应的贴图槽；

##### 索引处理：

当渲染一个正方形时，索引值为6个，而当渲染多个正方形时，索引值数据量等于正方形数*6，且每个正方形索引值与前一个正方形索引值相差4，因此相比于顶点数据，索引值数据在初始化时即可确定，只是索引数量需要根据实际的正方形数获取；

##### 顶点动态数据：

在opengl中vertexBuffer支持dynamic，即在每帧中可以动态修改顶点buffer，实现每帧顶点数据变化；

```cpp
//初始化VAO,VBO,IBO
Renderer::Init();
//初始化顶点数据指针；
Renderer::BeginBatch();
//处理顶点数据，将position和size写入顶点数据中，实现绝对坐标，以及确定颜色和贴图索引数据；同时递增顶点数据指针以及计算索引数量；
Renderer::DrawQuat();
//计算顶点数据size，将顶点数据写入VertexBuffer中
Renderer::EndBatch();
//draw call
Renderer::Flush();
```

#### ParticleSystem

在2D引擎中，==粒子系统本质上是一个粒子池，而粒子本身是一个正方形面片==，因此粒子需要管理一组属性：location/rotation/size/color/lifetime/active等；

粒子系统一般提供三个API:

(1) Emit: 一般根据index去刷新粒子池中某个粒子状态信息，确保重新初始化；

(2) OnUpdate: GameTick，根据ts去更新每个粒子的激活状态和lifetime，以及location和rotation；

(3) OnRender: RenderTick，根据lifetime去插值粒子的size和color，并draw;

#### SpriteSheet

##### 含义：

当加载关卡时可能需要多张贴图(texture/tile)，而OpenGL贴图绑定存在一定数量限制，因此采用batchRendering处理大量贴图时，可能需要多个drawCall，且存在贴图绑定之间切换，存在一定效率问题；考虑到tile本身较小，因此完全可以考虑==采用spriteSheet(精灵表单)，本质上将多张贴图（sprite）放置在一张大图上，因此只需要绑定一次大的贴图，计算正确的UV而获取sprite渲染==；

##### SubTexture:

相比于Texture2D需要绑定GPU，SubTexture不需要绑定，它只是一个封装的概念，接受一张贴图和必要的参数，能够允许计算出对应的UV坐标即可；本质上仍然用的原始那张贴图，只是不同subTexture对应的uv坐标不同；

##### Map of Tiles:

当加载一张有多个区块构成的地图或者关卡时，有一种做法时预先定义好一组字符串，每个字母代表每个区块所用的贴图，然后从已有的贴图表中批量取出渲染；

#### FrameBufffer

##### 默认/自定义帧缓冲：

我们已经使用了很多屏幕缓冲了：用于写入颜色值的颜色缓冲、用于写入深度信息的深度缓冲和允许我们根据一些条件丢弃特定片段的模板缓冲。==这些缓冲结合起来叫做帧缓冲(Framebuffer)，它被储存在内存中。OpenGL允许我们定义我们自己的帧缓冲==，也就是说我们能够定义我们自己的颜色缓冲，甚至是深度缓冲和模板缓冲。

目前所做的所有操作都是在默认帧缓冲的渲染缓冲上进行的。默认的帧缓冲是在你创建窗口的时候生成和配置的（GLFW帮我们做了这些）。有了我们自己的帧缓冲，我们就能够有更多方式来渲染了。

##### off-screen rendering:

==由于我们的帧缓冲不是默认帧缓冲，渲染指令将不会对窗口的视觉输出有任何影响。出于这个原因，渲染到一个不同的帧缓冲被叫做离屏渲染(Off-screen Rendering)==。

##### Attachment附件：

完整的帧缓冲，我们需要为它创建一些附件，并将附件附加到帧缓冲上。在完成所有的条件之后，我们可以以GL_FRAMEBUFFER为参数调用glCheckFramebufferStatus，检查帧缓冲是否完整。

##### 纹理附件：

当把一个纹理附加到帧缓冲的时候，所有的渲染指令将会写入到这个纹理中，就像它是一个普通的颜色/深度或模板缓冲一样。==使用纹理的优点是，所有渲染操作的结果将会被储存在一个纹理图像中，我们之后可以在着色器中很方便地使用它==。

##### 渲染缓冲对象(render buffer)附件:

和纹理图像一样，渲染缓冲对象是一个真正的缓冲，即一系列的字节、整数、像素等。渲染缓冲对象附加的好处是，它会将数据储存为OpenGL原生的渲染格式，它是为离屏渲染到帧缓冲优化过的。

渲染缓冲对象直接将所有的渲染数据储存到它的缓冲中，不会做任何针对纹理格式的转换，让它变为一个更快的可写储存介质。然而，渲染缓冲对象通常都是只写的，所以你不能读取它们（比如使用纹理访问）。当然你仍然还是能够使用glReadPixels来读取它，这会从当前绑定的帧缓冲，而不是附件本身，中返回特定区域的像素。

##### 附件选择：

渲染缓冲对象能为你的帧缓冲对象提供一些优化，但知道什么时候使用渲染缓冲对象，什么时候使用纹理是很重要的。==通常的规则是，如果你不需要从一个缓冲中采样数据，那么对这个缓冲使用渲染缓冲对象会是明智的选择。如果你需要从缓冲中采样颜色或深度值等数据，那么你应该选择纹理附件==。

##### 用于后期处理：

既然整个场景都被渲染到了一个纹理上，我们可以简单地通过修改纹理数据创建出一些非常有意思的效果。

大部分后处理离不开一个方法，核处理(kernel or convolution matrix)；它的中心为当前的像素，它会用它的核值乘以周围的像素值，并将结果相加变成一个值。比如锐化/模糊/边缘检测；

#### ECS(entity compoment system)

##### feature:

1）相比于传统的继承方式构建场景中各个entity，ECS采用组件和组合的设计模式去定义entity，每个entity由不同功能的组件组合而成；

2）组件的管理方面，ecs并没有按照entity管理其所有组件，而是采用component array方式去管理同一类型下的所有组件，确保获取同类型一系列组件时提高cpu缓存命中率；且大部分应用场合中，系统只关心一系列相关组件数据，并不关心它们来自哪个entity；

3）entity采用签名方式，去追踪其含有的所有组件；而component array会维护好每个组件索引值以及对应的entity id关系；

4）system和entity一样有对应的签名，表示其所关心的组件类型，不同系统关心着不同类型组件；而system本身会维护着一组entity，这些entity一定是含system所规定的组件的；当 Entity 的签名发生更改（添加或删除 Component 时）或 Entity 被销毁时，则需要更新该 System 正在追踪的 Entity 列表；

文章参考：https://zhuanlan.zhihu.com/p/618971664

##### Entt:

entt作为第三方库，只需要包含一个头文件即可，它的核心在于注册表，使用注册表创建实体entity，以及对实体进行增/删/查组件操作component，利用view可以获取含某一类型组件的实体列表，类似于ecs中system功能；

```cpp
//常见api
auto entity = m_Registry.create();
T& acomponent = m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
auto view = m_Registry.view<T>();
```

##### Camera Entity:

引入相机实体后，可在场景中添加多个相机实体，实现不同相机间的切换；

相机实体一般包含两个组件：Transform/Camera，Transform Component提供相机实体的位置信息，而Camera Component提供相机的投影信息，2D Engine中提供正交信息；

一般在场景中支持修改和提交相机实体信息；

#### UI Pannel Design

##### SceneHierarchy Panel:

场景列表，展示当前场景中所有实体信息，支持选中、展示Property Panel、以及Add/Remove Entity；

Entity UI的实现，采用ImGui::TreeNodeEx；

##### Property Panel:

属性列表，展示选中实体的component信息，支持重命名tag、修改每个component的相关属性、以及Add/Remove Component；

UI的风格修改：涉及到color-style，还有一些Imgui component的属性修改；

#### Serialization & Deserialization

场景的序列化：本质上将场景内实体及其所含组件的必要信息采集并存放于一个文件中，即存档；考虑采用文本文件yaml格式，可读性和可操作性强；

反序列化：序列化逆过程，解析文件并生成对应的场景实体和组件，即加载；

#### MousePicking

##### 实现思路：

渲染整个场景，在fragment时并不是计算每个像素的颜色，而是计算每个像素此时所在的entity id，生成一张贴图；当用户点击屏幕时，计算对应的uv去取值；在渲染层，仍然采用一次render pass，但会在frambuffer输出两个结果，一个代表颜色，一个代表entity id；

##### MultiRenderTarget:

一个renderPass后的结果可以有多个，即multiRenderTarget；通常情况下renderpass需要绑定frameBuffer，而frameBuffer可以绑定多个附件，包括多个colorAttachment，实现multiRenderTarget;

对于openGL，可以在fragment shader中指定多个输出out，对应不同的附件上；
