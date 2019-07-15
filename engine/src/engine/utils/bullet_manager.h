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

		btRigidBody* get_body() { return body; };	//returns boid's rigidBody
		btVector3 get_forward();	//returns boid's direction vector
		btVector3 get_up();	//returns boid's up vector

		void apply_forces(); //aplies drag, thrust, lift and gravity
	};

	//vectors
	const btVector3 up_vector(0, 1, 0);
	const btVector3 forward_vector(0, 0, 1);

	//PI
	const btScalar PI = 3.1415926535897;

	class bullet_manager
	{
	public:
		bullet_manager();

		bullet_manager(std::vector<engine::game_object *> game_objects);

		virtual ~bullet_manager();

		void	init_physics(std::vector<engine::game_object *> game_objects, btDynamicsWorld* dynamics_world);

		btRigidBody*	local_create_rigid_body(float mass, const btTransform& start_transform, btCollisionShape* shape, btDynamicsWorld* dynamics_world);

		void add_physical_object(engine::game_object * game_object, btDynamicsWorld* dynamics_world);

		btDynamicsWorld*		get_dynamics_world()
		{
			return m_dynamics_world;
		}

		inline btVector3 to_bt_vector3(const glm::vec3& vec3) {
			return btVector3(btScalar(vec3.x), btScalar(vec3.y), btScalar(vec3.z));
		}

		inline glm::vec3 to_vec3(const btVector3& bt_vec3) {
			return { bt_vec3.getX(), bt_vec3.getY(), bt_vec3.getZ() };
		}

		void dynamics_world_update(std::vector<engine::game_object *> gameObjects);

		btBroadphaseInterface*	m_overlapping_pair_cache;

		btCollisionDispatcher*	m_dispatcher;

		btConstraintSolver*	m_constraint_solver;

		btDefaultCollisionConfiguration* m_collision_configuration;

		btAlignedObjectArray<btCollisionShape*>	m_collision_shapes;

		std::vector<physical_object*> physical_objects;

	protected:

		btDynamicsWorld*		m_dynamics_world;
	};
}
