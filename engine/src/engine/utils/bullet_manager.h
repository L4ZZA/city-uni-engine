#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "LinearMath/btVector3.h"
#include "LinearMath/btMatrix3x3.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btAlignedObjectArray.h"

#include "engine/entities/game_object.h"

class btBroadphaseInterface;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;

class	btCollisionShape;
class	btDynamicsWorld;
class	btRigidBody;
class	btTypedConstraint;

namespace engine
{
	class physical_object {
		btRigidBody* body;	//boid's rigidBody

	public:
		physical_object(btRigidBody* body);

		btRigidBody* GetBody() { return body; };	//returns boid's rigidBody
		btVector3 GetForward();	//returns boid's direction vector
		btVector3 GetUp();	//returns boid's up vector

		void ApplyForces(); //aplies drag, thrust, lift and gravity
	};

	//vectors
	const btVector3 UpVector(0, 1, 0);
	const btVector3 ForwardVector(0, 0, 1);

	//PI
	const btScalar PI = 3.1415926535897;

	class bullet_manager
	{
	public:
		bullet_manager();

		bullet_manager(std::vector<engine::game_object *> game_objects);

		virtual ~bullet_manager();

		void	initPhysics(std::vector<engine::game_object *> game_objects, btDynamicsWorld* dynamicsWorld);

		btRigidBody*	localCreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape, btDynamicsWorld* dynamicsWorld);

		void add_physical_object(engine::game_object * game_object, btDynamicsWorld* dynamicsWorld);

		btDynamicsWorld*		getDynamicsWorld()
		{
			return m_dynamicsWorld;
		}

		inline btVector3 to_btVector3(const glm::vec3& vec3) {
			return btVector3(btScalar(vec3.x), btScalar(vec3.y), btScalar(vec3.z));
		}

		inline glm::vec3 to_vec3(const btVector3& bt_vec3) {
			return { bt_vec3.getX(), bt_vec3.getY(), bt_vec3.getZ() };
		}

		void DynamicsWorldStep(std::vector<engine::game_object *> gameObjects);

		btBroadphaseInterface*	m_overlappingPairCache;

		btCollisionDispatcher*	m_dispatcher;

		btConstraintSolver*	m_constraintSolver;

		btDefaultCollisionConfiguration* m_collisionConfiguration;

		btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;

		std::vector<physical_object*> physical_objects;

	protected:

		btDynamicsWorld*		m_dynamicsWorld;
	};
}
