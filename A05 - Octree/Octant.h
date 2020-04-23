#pragma once
// Octant file based off of example insolution

#ifndef __OCTANTCLASS_H_
#define __OCTANTCLASS_H_

#include "MyEntityManager.h"

namespace Simplex
{

	// class definition
	class Octant
	{

		static uint m_uOctantCount; // stores the number of octants created 
		static uint m_uMaxLevel; // stores the maximum level an octant can reach
		static uint m_uIdealEntityCount; // how many entities the octant can contain

		uint m_uID = 0; // stores octant id
		uint m_uLevel = 0; // stores octant level
		uint m_uChildren = 0; // number of children

		float m_fSize = 0.0f; // size of the octant

		MeshManager* m_pMeshMngr = nullptr; // mesh manager singleton
		MyEntityManager* m_pEntityMngr = nullptr; // entity manager singleton

		vector3 m_v3Center = vector3(0.0f); // store the center point of the octant
		vector3 m_v3Min = vector3(0.0f); // store the minimum vector of the octant
		vector3 m_v3Max = vector3(0.0f); // store the maximum vector of the octant

		Octant* m_pParent = nullptr;// store the parent of current octant
		Octant* m_pChild[8];// store the children of the current octant

		std::vector<uint> m_EntityList; //Index in Entity Manager

		Octant* m_pRoot = nullptr;//Root octant
		std::vector<Octant*> m_lChild; //list of nodes that contain objects (root only)
	public:
		// constructors
		// octant containing all entity instances in the mesh manager
		Octant(uint a_nMaxLevel = 2, uint a_nIdealEntityCount = 5);

		// octant with a specific center point and size
		Octant(vector3 a_v3Center, float a_fSize);

		// copy constructor
		Octant(Octant const& other);

		// copy assignment operator
		Octant& operator=(Octant const& other);

		// destructor
		~Octant(void);


		// methods
		// swaps contents of Octants
		void Swap(Octant& other);

		// returns octants size
		float GetSize(void);

		// returns octants center point in global space
		vector3 GetCenterGlobal(void);

		// returns minimum corner in global space
		vector3 GetMinGlobal(void);

		// returns max corner in global space
		vector3 GetMaxGlobal(void);

		// retunrs bool based on collisions
		bool IsColliding(uint a_uRBIndex);

		// displays octant volume
		void Display(uint a_nIndex, vector3 a_v3Color = C_YELLOW);
		void Display(vector3 a_v3Color = C_YELLOW);

		// displays non empty sectos of octree
		void DisplayLeafs(vector3 a_v3Color = C_YELLOW);

		// clears entity list
		void ClearEntityList(void);

		// divides octants into 8 sections
		void Subdivide(void);

		// returns the child of the current octant
		Octant* GetChild(uint a_nChild);

		// retunrs the parent of the current octant
		Octant* GetParent(void);

		// returns whether this octant is a leaf (has no children)
		bool IsLeaf(void);

		// checks to see if octant has more than desired number of entities within
		bool ContainsMoreThan(uint a_nEntities);

		// deletes this octant and its subsequent children
		void KillBranches(void);

		// creates a new tree using the max number of subdivisions
		void ConstructTree(uint a_nMaxLevel = 3);

		// traverse the tree and set index values for contained entities
		void AssignIDtoEntity(void);

		// get the total number of octants
		uint GetOctantCount(void);
		void BurnDownTree(void);

		std::vector<Octant*> GetPopulatedLeaves();
		std::vector<uint> GetEntityList();
	private:
		
		// Deallocates member fields
		void Release(void);

		// Allocates member fields
		void Init(void);

		//creates the list of all leafs that contains objects.
		void ConstructList(void);

	};

}
#endif // !__OCTANTCLASS_H_
