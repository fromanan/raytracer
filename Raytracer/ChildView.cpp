
// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "Raytracer.h"
#include "ChildView.h"
#include "graphics/OpenGLRenderer.h"
#include "CMyRaytraceRenderer.h"

#include "Debugger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	m_camera.Set(20., 10., 50., 0., 0., 0., 0., 1., 0.);

	m_raytrace = false;
	m_rayimage = nullptr;

	CGrPtr<CGrComposite> scene = new CGrComposite;
	m_scene = scene;

	// Load the Skybox
	/*CGrCubeTexture m_cubeTex;
	CMesh m_skybox;
	
	m_cubeTex.LoadFiles(L"textures/right.jpg", L"textures/left.jpg", L"textures/top.jpg",
		L"textures/bottom.jpg", L"textures/front.jpg", L"textures/back.jpg");
	m_skybox.CreateCube();
	m_skybox.m_program = LoadShaders("ShaderWnd/vertexSky.glsl", "ShaderWnd/fragmentSky.glsl");
	m_skybox.InitGL();*/

	// Floor
	CGrPtr<CGrMaterial> floorpaint = new CGrMaterial;
	floorpaint->AmbientAndDiffuse(1, 1, 1);
	floorpaint->Specular(1, 1, 1);
	floorpaint->Shininess(1.);
	scene->Child(floorpaint);

	CGrPtr<CGrTexture> floortile = new CGrTexture;
	floortile->LoadFile(L"textures/wood_tile.bmp");

	CGrPtr<CGrComposite> floor = new CGrComposite;
	floorpaint->Child(floor);
	//sphere->Sphere(-10, 10, 10, 5, marble);
	floor->Box(-20, -10, -20, 40, 0.1, 40, floortile);

	#pragma region Red Box
	CGrPtr<CGrMaterial> redPaint = new CGrMaterial;
	redPaint->AmbientAndDiffuse(0.8f, 0.0f, 0.0f);
	scene->Child(redPaint);

	CGrPtr<CGrComposite> redbox = new CGrComposite;
	redPaint->Child(redbox);
	redbox->Box(1, 1, 1, 5, 5, 5);
	#pragma endregion

	#pragma region White Box
	CGrPtr<CGrMaterial> whitepaint = new CGrMaterial;
	whitepaint->AmbientAndDiffuse(0.8f, 0.8f, 0.8f);
	whitepaint->Specular(0.75, 0.75, 0.75);
	whitepaint->Shininess(16.);
	scene->Child(whitepaint);

	CGrPtr<CGrTexture> plankTexture = new CGrTexture;
	plankTexture->LoadFile(L"textures/plank01.bmp");

	CGrPtr<CGrComposite> whitebox = new CGrComposite;
	whitepaint->Child(whitebox);
	whitebox->Box(-10, -10, -10, 5, 5, 5, plankTexture);
	#pragma endregion
	
	#pragma region Blue Pyramid
	CGrPtr<CGrMaterial> bluepaint = new CGrMaterial;
	bluepaint->AmbientAndDiffuse(0.0f, 0.0f, 0.8f);
	scene->Child(bluepaint);

	CGrPtr<CGrComposite> bluepyramid = new CGrComposite;
	bluepaint->Child(bluepyramid);
	bluepyramid->Pyramid(-10, 1, 1, 5, 5, 5);
	#pragma endregion

	#pragma region Sphere
	CGrPtr<CGrMaterial> shinypaint = new CGrMaterial;
	shinypaint->AmbientAndDiffuse(0.8f, 0.8f, 0.8f);
	shinypaint->Specular(0.75, 0.75, 0.75);
	shinypaint->Shininess(16.);
	scene->Child(shinypaint);
	
	CGrPtr<CGrTexture> marble = new CGrTexture;
	marble->LoadFile(L"textures/marble10.bmp");

	CGrPtr<CGrComposite> sphere = new CGrComposite;
	shinypaint->Child(sphere);
	//sphere->Sphere(-10, 10, 10, 5, marble);
	sphere->Box(-5, -7.5, 2.5, 5, 5, 5, marble);
	#pragma endregion
}

CChildView::~CChildView()
{
	
}


BEGIN_MESSAGE_MAP(CChildView, COpenGLWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_RENDER_RAYTRACE, &CChildView::OnRenderRaytrace)
	ON_UPDATE_COMMAND_UI(ID_RENDER_RAYTRACE, &CChildView::OnUpdateRenderRaytrace)
	ON_COMMAND(ID_FILE_SAVE32772, &CChildView::OnFileSave)
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!COpenGLWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnGLDraw(CDC* pDC)
{
	if (m_raytrace)
	{
		// Clear the color buffer
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set up for parallel projection
		int width, height;
		GetSize(width, height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width, 0, height, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// If we got it, draw it
		if (m_rayimage)
		{
			glRasterPos3i(0, 0, 0);
			glDrawPixels(m_rayimagewidth, m_rayimageheight,
				GL_RGB, GL_UNSIGNED_BYTE, m_rayimage[0]);
		}

		glFlush();
	}
	else
	{
		//
		// Instantiate a renderer
		//

		COpenGLRenderer renderer;

		// Configure the renderer
		ConfigureRenderer(&renderer);

		//
		// Render the scene
		//

		renderer.Render(m_scene);
	}
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_camera.MouseDown(point.x, point.y);

	COpenGLWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_camera.MouseMove(point.x, point.y, nFlags))
		Invalidate();

	COpenGLWnd::OnMouseMove(nFlags, point);
}


void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_camera.MouseDown(point.x, point.y, 2);

	COpenGLWnd::OnRButtonDown(nFlags, point);
}

//
// Name :         CChildView::ConfigureRenderer()
// Description :  Configures our renderer so it is able to render the scene.
//                Indicates how we'll do our projection, where the camera is,
//                and where any lights are located.
//

void CChildView::ConfigureRenderer(CGrRenderer* p_renderer)
{
	// Determine the screen size so we can determine the aspect ratio
	int width, height;
	GetSize(width, height);
	double aspectratio = double(width) / double(height);

	//
	// Set up the camera in the renderer
	//

	p_renderer->Perspective(m_camera.FieldOfView(),
		aspectratio, // The aspect ratio.
		20., // Near clipping
		1000.); // Far clipping

	// m_camera.FieldOfView is the vertical field of view in degrees.

	//
	// Set the camera location
	//

	p_renderer->LookAt(m_camera.Eye()[0], m_camera.Eye()[1], m_camera.Eye()[2],
		m_camera.Center()[0], m_camera.Center()[1], m_camera.Center()[2],
		m_camera.Up()[0], m_camera.Up()[1], m_camera.Up()[2]);

	//
	// Set the light locations and colors
	//

	float dimd = 0.5f;
	GLfloat dim[] = { dimd, dimd, dimd, 1.0f };
	GLfloat brightwhite[] = { 1.f, 1.f, 1.f, 1.0f };

	p_renderer->AddLight(CGrPoint(1, 0.5, 1.2, 0),
		dim, brightwhite, brightwhite);

	p_renderer->AddLight(CGrPoint(-10, 5, -15, 0),
		dim, dim, dim);
}


void CChildView::OnRenderRaytrace()
{
	m_raytrace = !m_raytrace;
	Invalidate();
	if (!m_raytrace)
		return;

	GetSize(m_rayimagewidth, m_rayimageheight);

	m_rayimage = new BYTE * [m_rayimageheight];

	int rowwid = m_rayimagewidth * 3;
	while (rowwid % 4)
		rowwid++;

	m_rayimage[0] = new BYTE[m_rayimageheight * rowwid];
	for (int i = 1; i < m_rayimageheight; i++)
	{
		m_rayimage[i] = m_rayimage[0] + i * rowwid;
	}

	for (int i = 0; i < m_rayimageheight; i++)
	{
		// Fill the image with blue
		for (int j = 0; j < m_rayimagewidth; j++)
		{
			m_rayimage[i][j * 3] = 0;               // red
			m_rayimage[i][j * 3 + 1] = 0;           // green
			m_rayimage[i][j * 3 + 2] = BYTE(255);   // blue
		}
	}
	
	// Instantiate a raytrace object
	CMyRaytraceRenderer raytrace;

	// Generic configurations for all renderers
	ConfigureRenderer(&raytrace);

	//
	// Render the Scene
	//
	raytrace.SetImage(m_rayimage, m_rayimagewidth, m_rayimageheight);
	raytrace.SetWindow(this);
	raytrace.Render(m_scene);
	Invalidate();
}


void CChildView::OnUpdateRenderRaytrace(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_raytrace);
}


void CChildView::OnFileSave()
{
	OnSaveImage();
}

/*
GLuint CChildView::LoadShaders(const char* vertex_file_path, const char* fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		TRACE("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}



	GLint Result = GL_FALSE;
	int InfoLogLength;



	// Compile Vertex Shader
	TRACE("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		TRACE("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	TRACE("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		TRACE("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	TRACE("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		TRACE("%s\n", &ProgramErrorMessage[0]);
	}

	//Mark the shaders for deletion when no longer in use.
	//glDeleteShader(VertexShaderID);
	//glDeleteShader(FragmentShaderID);
	glUseProgram(ProgramID);

	return ProgramID;
}
*/