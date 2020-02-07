#include "AppClass.h"
void Application::InitVariables(void)
{
	//Make MyMesh object
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(2.0f, C_BLACK);

	//Make MyMesh object
	m_pMesh1 = new MyMesh();
	m_pMesh1->GenerateCube(1.0f, C_WHITE);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	// matrices
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	static float val = 0.0f;
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(val, 0.0f, 0.0f));
	val += 0.01f;
	matrix4 m4Model = m4Translate;

	// create space invader shape
	// center column
	m_pMesh->Render(m4Projection, m4View, m4Model);
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(val, 2.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(val, 4.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(val, 6.0f, 0.0f)));

	// right half
	//first column
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(2.0f + val, 0.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(2.0f + val, 2.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(2.0f + val, 4.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(2.0f + val, 6.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(2.0f + val, -4.0f, 0.0f)));

	// second column
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(4.0f + val, 0.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(4.0f + val, 2.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(4.0f + val, 6.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(4.0f + val, 8.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(4.0f + val, -4.0f, 0.0f)));

	// third column
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(6.0f + val, 0.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(6.0f + val, 2.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(6.0f + val, 4.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(6.0f + val, 6.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(6.0f + val, 10.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(6.0f + val, -2.0f, 0.0f)));

	// fourth column
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(8.0f + val, 2.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(8.0f + val, 4.0f, 0.0f)));

	// fifth column
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(10.0f + val, 0.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(10.0f + val, 2.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(10.0f + val, -2.0f, 0.0f)));

	// left half
	//first column
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-2.0f + val, 0.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-2.0f + val, 2.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-2.0f + val, 4.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-2.0f + val, 6.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-2.0f + val, -4.0f, 0.0f)));

	// second column
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-4.0f + val, 0.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-4.0f + val, 2.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-4.0f + val, 6.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-4.0f + val, 8.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-4.0f + val, -4.0f, 0.0f)));

	// third column
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-6.0f + val, 0.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-6.0f + val, 2.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-6.0f + val, 4.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-6.0f + val, 6.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-6.0f + val, 10.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-6.0f + val, -2.0f, 0.0f)));

	// fourth column
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-8.0f + val, 2.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-8.0f + val, 4.0f, 0.0f)));

	// fifth column
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-10.0f + val, 0.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-10.0f + val, 2.0f, 0.0f)));
	m_pMesh->Render(m4Projection, m4View, glm::translate(vector3(-10.0f + val, -2.0f, 0.0f)));
		
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	SafeDelete(m_pMesh1);
	//release GUI
	ShutdownGUI();
}