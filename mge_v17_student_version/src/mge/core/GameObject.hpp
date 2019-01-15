#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <string>
#include <iostream>
#include <glm.hpp>
#include <btBulletDynamicsCommon.h>
#include "mge\core\Physics\Colliders\BoxCollider.h"
#include "mge\core\Physics\Colliders\SphereCollider.h"
#include "mge\core\Physics\Colliders\MeshCollider.h"
#include "mge\core\Physics\Colliders\CapsuleCollider.h"

class AbstractBehaviour;
class AbstractMaterial;
class World;
class Mesh;

/**
 * A GameObject wraps all data required to display an (interactive / dynamic) object, but knows nothing about OpenGL or rendering.
 * You will need to alter this class to add colliders etc.
 */
class GameObject
{
	public:
		GameObject(std::string pName = "", glm::vec3 pPosition = glm::vec3( 0.0f, 0.0f, 0.0f ) );
		virtual ~GameObject();

        void setName (std::string pName);
        std::string getName() const;

        //contains local rotation, scale, position
		void setTransform (const glm::mat4& pTransform);
		void setWorldTransform_TEST(const glm::mat4 & pTransform);
        const glm::mat4& getTransform() const;

		btTransform getBulletPhysicsTransform() const;

		glm::vec3 getLocalScale();
		btVector3 getBulletPhysicsLocalScale();

        //access just the local position
		void setLocalPosition (glm::vec3 pPosition);
		glm::vec3 getLocalPosition() const;

        //get the objects world position by combining transforms
        //expensive operations!! Improvement would be to cache these transforms!
		glm::vec3 getWorldPosition() const;
		glm::mat4 getWorldTransform() const;

        //change local position, rotation, scaling
		void translate(glm::vec3 pTranslation);
		void rotate(float pAngle, glm::vec3 pAxis);
		void scale(glm::vec3 pScale);

        //mesh and material should be shared as much as possible
		void setMesh(Mesh* pMesh);
		Mesh* getMesh() const;

		void setMaterial (AbstractMaterial* pMaterial);
		AbstractMaterial* getMaterial() const;

		void addBehaviour(AbstractBehaviour* pBehaviour);
		std::vector<AbstractBehaviour*> getBehaviours();

		std::vector<Collider*> colliders;
		Collider& addCollider(BoxColliderArgs& colliderArgs, bool isTrigger, bool isStatic);
		Collider& addCollider(MeshColliderArgs & colliderArgs, bool isTrigger, bool isStatic);
		Collider& addCollider(SphereColliderArgs& colliderArgs, bool isTrigger, bool isStatic);
		Collider& addCollider(CapsuleColliderArgs & colliderArgs, bool isTrigger, bool isStatic);

		template <class BehaviourType>
		BehaviourType* getBehaviour();
		template <class BehaviourType>
		std::vector<BehaviourType*> getBehavioursOfType();

		virtual void update(float pStep);

        //child management

        //shortcut to set the parent of pChild to ourselves
		void add (GameObject* pChild);
		//shortcut to set the parent of pChild to NULL
		void remove (GameObject* pChild);

		virtual void setParent (GameObject* pGameObject);
		GameObject* getParent();

        int getChildCount();
        GameObject* getChildAt (int pIndex);
		const int constantBufferID;

	protected:
		glm::vec3 * _cachedWorldPosition;
		std::string _name;
		glm::mat4 _transform = glm::mat4(1);
		bool _gotWorldPosition = false;

        GameObject* _parent;
		std::vector<GameObject*> _children;

        Mesh* _mesh;
		std::vector<AbstractBehaviour*> _behaviours;
		AbstractMaterial* _material;

        //update children list administration
        void _innerAdd (GameObject* pChild);
		void _innerRemove (GameObject* pChild);
    private:
		static int _gameobjects;
        GameObject(const GameObject&);
        GameObject& operator=(const GameObject&);
};

template<class BehaviourType>
inline BehaviourType * GameObject::getBehaviour()
{
	BehaviourType* castedBehaviour;
	for each (AbstractBehaviour* behaviour in _behaviours)
	{
		castedBehaviour = dynamic_cast<BehaviourType*>(behaviour);
		if (castedBehaviour) return castedBehaviour;
	}
	return nullptr;
}

template<class BehaviourType>
inline std::vector<BehaviourType*> GameObject::getBehavioursOfType()
{
	std::vector<BehaviourType*> castedBehaviours;
	BehaviourType* castedBehaviour;
	for each (AbstractBehaviour* behaviour in _behaviours)
	{
		castedBehaviour = dynamic_cast<BehaviourType*>(behaviour);
		if (castedBehaviour) castedBehaviours.push_back(castedBehaviour);
	}
	return castedBehaviours;
}

#endif // GAMEOBJECT_H