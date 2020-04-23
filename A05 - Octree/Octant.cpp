// octant class 
#include "Octant.h"

using namespace Simplex;
using namespace std;

uint Octant::m_uMaxLevel = 3;
uint Octant::m_uIdealEntityCount = 5;
uint Octant::m_uOctantCount = 0;

// property definitions
uint Octant::GetOctantCount(void) { return m_uOctantCount; }
Octant* Octant::GetParent(void) { return m_pParent; }

float Octant::GetSize(void) { return m_fSize; }
vector3 Octant::GetCenterGlobal(void) { return m_v3Center; }
vector3 Octant::GetMinGlobal(void) { return m_v3Min; }
vector3 Octant::GetMaxGlobal(void) { return m_v3Max; }


// initialization
void Octant::Init(void)
{
	// stores singleton references
	m_pEntityMngr = MyEntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();

	// initializeing variables at 0
	m_uID = m_uOctantCount;
	m_uLevel = 0;
	m_uChildren = 0;

	m_fSize = 0.0f;
	m_v3Center = vector3(0.0f); 
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	// start nodes as null pointers
	m_pRoot = nullptr;
	m_pParent = nullptr;

	// set child nodes as null pointers
	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i] = nullptr;
	}
}

// swap method
void Octant::Swap(Octant& other)
{
	// stores singleton references
	m_pEntityMngr = MyEntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();

	// swap the two octants variables
	std::swap(m_uID, other.m_uID);
	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_uChildren, other.m_uChildren);

	std::swap(m_fSize, other.m_fSize);
	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);

	// swap related node information
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_pParent, other.m_pParent);

	//swap child nodes
	for (uint i = 0; i < 8; i++)
	{
		std::swap(m_pChild[i], other.m_pChild[i]);
	}
}

// release method
void Octant::Release()
{
	// check level value
	if (m_uLevel == 0)
	{
		KillBranches();
	}

	// reset variables
	m_uChildren = 0;
	m_fSize = 0.0f;

	// clear the node and entity lists
	m_lChild.clear();
	m_EntityList.clear();

}

// base constructor
Octant::Octant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	// initialize variables
	m_uOctantCount = 0;
	Init();
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;

	// set as root node
	m_pRoot = this;
	m_lChild.clear();

	// get entities from entity manager
	vector<vector3> vertices;
	uint entityCount = m_pEntityMngr->GetEntityCount();

	// get global max and min for eac h entity
	for (uint i = 0; i < entityCount; i++)
	{
		// get the entities rigid body 
		MyRigidBody* rb = m_pEntityMngr->GetEntity(i)->GetRigidBody();

		// push max and min values to list
		vertices.push_back(rb->GetMinGlobal());
		vertices.push_back(rb->GetMaxGlobal());
	}

	// using bounds info create a rigidbody and track its half width
	MyRigidBody* boundingRb = new MyRigidBody(vertices);
	vector3 boundsHalfWidth = boundingRb->GetHalfWidth();

	// keep track of new max half width
	float maxOctantHalfWidth = 0.0f;

	// go through and get max half width
	if (maxOctantHalfWidth < boundsHalfWidth.x) { maxOctantHalfWidth = boundsHalfWidth.x; }
	if (maxOctantHalfWidth < boundsHalfWidth.y) { maxOctantHalfWidth = boundsHalfWidth.y; }
	if (maxOctantHalfWidth < boundsHalfWidth.z) { maxOctantHalfWidth = boundsHalfWidth.z; }

	// set landmark values 
	m_v3Center = boundingRb->GetCenterLocal();
	m_fSize = maxOctantHalfWidth * 2.0f;
	vector3 octantHalfWidth = vector3(maxOctantHalfWidth, maxOctantHalfWidth, maxOctantHalfWidth);

	// set boundary min and max values
	m_v3Min = m_v3Center - octantHalfWidth;
	m_v3Max = m_v3Center + octantHalfWidth;

	// delete vertex list and rigidbody
	vertices.clear();
	SafeDelete(boundingRb);

	// iterate octant count and build tree
	m_uOctantCount++;
	ConstructTree(m_uMaxLevel);
}

// child node constructor
Octant::Octant(vector3 a_v3Center, float a_fSize)
{
	// initialize variables
	Init();

	// set parameters
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	// set bounds
	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);

	// iterate octant count
	m_uOctantCount++;
}

// copy constructor
Octant::Octant(Octant const& other)
{
	// store singleton references
	m_pEntityMngr = MyEntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();

	// copy over node values
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;
	m_uChildren = other.m_uChildren;

	// copy node bounds
	m_fSize = other.m_fSize;
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	// copy root nodes
	m_pRoot = other.m_pRoot;
	m_pParent = other.m_pParent;

	// copy children
	for(uint i = 0; i < 8; i++)
	{
		m_pChild[i] = other.m_pChild[i];
	}
}

// copy assignment operator
Octant& Octant::operator=(Octant const& other)
{
	// check if trying to copy self
	if (this != &other)
	{
		Release();

		Init();

		// swap data
		Octant temp(other);
		Swap(temp);
	}

	// retunr pointer of this node
	return *this;
}

// destructor
Octant::~Octant()
{
	Release();
}

//Tree Construction methods
// create tree
void Octant::ConstructTree(uint a_nMaxLevel)
{
	// check if current is root node
	if (m_uLevel != 0) return;

	// set max level and octant count
	m_uMaxLevel = a_nMaxLevel;
	m_uOctantCount = 1;

	//clear lists and branches
	m_EntityList.clear();
	KillBranches();
	m_lChild.clear();

	// determine if subdivision is needed
	if (ContainsMoreThan(m_uIdealEntityCount))
	{
		Subdivide();
	}

	// assign IDs
	AssignIDtoEntity();

	// construct list
	ConstructList();
}

// subdivision method
void Octant::Subdivide(void)
{
	// check if max level or previously subdivided octant
	if (m_uLevel >= m_uMaxLevel) return;
	if (m_uChildren != 0) return;

	// create subdivided bodes halfwidth by halving current halfwidth
	float subHalfWidth = m_fSize / 4.0f;
	float actualHalfWidth = m_fSize / 2.0f;

	// create child nodes
	m_pChild[0] = new Octant(vector3(m_v3Center.x + subHalfWidth, m_v3Center.y + subHalfWidth, m_v3Center.z + subHalfWidth), actualHalfWidth);
	m_pChild[1] = new Octant(vector3(m_v3Center.x + subHalfWidth, m_v3Center.y + subHalfWidth, m_v3Center.z - subHalfWidth), actualHalfWidth);
	m_pChild[2] = new Octant(vector3(m_v3Center.x + subHalfWidth, m_v3Center.y - subHalfWidth, m_v3Center.z + subHalfWidth), actualHalfWidth);
	m_pChild[3] = new Octant(vector3(m_v3Center.x + subHalfWidth, m_v3Center.y - subHalfWidth, m_v3Center.z - subHalfWidth), actualHalfWidth);
	m_pChild[4] = new Octant(vector3(m_v3Center.x - subHalfWidth, m_v3Center.y + subHalfWidth, m_v3Center.z + subHalfWidth), actualHalfWidth);
	m_pChild[5] = new Octant(vector3(m_v3Center.x - subHalfWidth, m_v3Center.y + subHalfWidth, m_v3Center.z - subHalfWidth), actualHalfWidth);
	m_pChild[6] = new Octant(vector3(m_v3Center.x - subHalfWidth, m_v3Center.y - subHalfWidth, m_v3Center.z + subHalfWidth), actualHalfWidth);
	m_pChild[7] = new Octant(vector3(m_v3Center.x - subHalfWidth, m_v3Center.y - subHalfWidth, m_v3Center.z - subHalfWidth), actualHalfWidth);

	// loop through and set values for child nodes
	for (int i = 0; i < 8; i++)
	{
		// set parent, level and root
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_uLevel = m_uLevel + 1;
		m_pChild[i]->m_pRoot = m_pRoot;

		// subdivide again if needed
		if (m_pChild[i]->ContainsMoreThan(m_uIdealEntityCount))
		{
			m_pChild[i]->Subdivide();
		}
	}
	m_uChildren = 8;
}

// ID method
void Octant::AssignIDtoEntity(void)
{
	// recursively loop and assign IDs
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->AssignIDtoEntity();
	}

	// check for leaf
	if (m_uChildren == 0)
	{
		// get entity count
		uint entityCount = m_pEntityMngr->GetEntityCount();

		// loop through and check for contained entities
		for (int i = 0; i < entityCount; i++)
		{
			// check collision
			if(IsColliding(i))
			{
				// add to entity list and add dimension to entity manager
				m_EntityList.push_back(i);
				m_pEntityMngr->AddDimension(i, m_uID);
			}
		}
	}
}

// list methods
// list construction
void Octant::ConstructList(void)
{
	// loop through children and recursively create their lists
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->ConstructList();
	}

	if (m_EntityList.size() > 0)
	{
		// add this node to roots children
		m_pRoot->m_lChild.push_back(this);
	}
}

// clear list
void Octant::ClearEntityList(void)
{
	// loop and recursively clear entity lists for each node
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->ClearEntityList();
	}

	// clear this nodes entity list
	m_EntityList.clear();
}

// diplay methods
// displays octant with specified color
void Octant::Display(uint a_nIndex, vector3 a_v3Color)
{
	// check if this is the correct octant
	if (m_uID == a_nIndex)
	{
		// get a matrix representing this cube and add a wire cube to the render list
		glm::mat4 octMatrix = glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize));
		m_pMeshMngr->AddWireCubeToRenderList(octMatrix, a_v3Color, RENDER_WIRE);

		return;
	}

	// if not search for correct octant
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_nIndex);
	}
}

void Octant::Display(vector3 a_v3Color)
{
	// loop child nodes and display them
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_v3Color);
	}

	// get a matrix representing this cube and add a wire cube to the render list
	glm::mat4 octMatrix = glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize));
	m_pMeshMngr->AddWireCubeToRenderList(octMatrix, a_v3Color, RENDER_WIRE);
}

void Octant::DisplayLeafs(vector3 a_v3Color)
{
	// loop child nodes and display leaf nodes
	for (int i = 0; i < m_lChild.size(); i++)
	{
		m_lChild[i]->DisplayLeafs(a_v3Color);
	}

	// get a matrix representing this cube and add a wire cube to the render list
	glm::mat4 octMatrix = glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize));
	m_pMeshMngr->AddWireCubeToRenderList(octMatrix, a_v3Color, RENDER_WIRE);
}

// helper methods
bool Octant::IsColliding(uint a_uRBIndex)
{
	// check entity count and whether this is a valid entry
	uint entityCount = m_pEntityMngr->GetEntityCount();
	if (a_uRBIndex >= entityCount) return false;

	// get a rigidbody for this entity and its max and min bounds
	MyRigidBody* rb = m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody();
	vector3 entityMin = rb->GetMinGlobal();
	vector3 entityMax = rb->GetMaxGlobal();

	// check for valid collision
	if (m_v3Min.x > entityMax.x) return false;
	if (m_v3Max.x < entityMin.x) return false;

	if (m_v3Min.y > entityMax.y) return false;
	if (m_v3Max.y < entityMin.y) return false;

	if (m_v3Min.z > entityMax.z) return false;
	if (m_v3Max.z < entityMin.z) return false;

	// retunr true if collision passes
	return true;
}

// get children at index
Octant* Octant::GetChild(uint a_nChild)
{
	// check for valid number of children
	if (a_nChild > 7) return nullptr;

	// reurn child at given index
	return m_pChild[a_nChild];
}

// check for children
bool Octant::IsLeaf(void)
{
	if (m_uChildren != 0) return false;

	return true;
}

// check if node has too many entities
bool Octant::ContainsMoreThan(uint a_nEntities)
{
	// get entity count and create counter variable
	uint entityCount = m_pEntityMngr->GetEntityCount();
	uint count = 0;

	// loop through and check each entities collisions and compare
	for (int i = 0; i < entityCount; i++)
	{
		if (IsColliding(i)) count++;

		if (count > a_nEntities) return true;
	}

	return false;
}

// recursively kills all branches
void Octant::KillBranches(void)
{
	// loop through child nodes
	for(int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->KillBranches();

		// delete node
		delete m_pChild[i];
		m_pChild[i] = nullptr;
	}

	// set child count to 0
	m_uChildren = 0;
}

// list of populated leaves
vector<Octant*> Octant::GetPopulatedLeaves()
{
	return m_lChild;
}

// list of entities in scene
vector<uint> Octant::GetEntityList()
{
	return m_EntityList;
}

// destroy tree
void Octant::BurnDownTree(void)
{
	// find root
	if(m_uLevel == 0)
	{
		KillBranches();
	}

	// reset variables
	m_uChildren = 0;
	m_fSize = 0.0f;

	// clear lists
	m_lChild.clear();
	m_EntityList.clear();
}