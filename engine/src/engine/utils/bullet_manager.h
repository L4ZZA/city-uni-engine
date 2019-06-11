#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../bullet/LinearMath/btVector3.h"
#include "../bullet/LinearMath/btMatrix3x3.h"
#include "../bullet/LinearMath/btTransform.h"
#include "../bullet/LinearMath/btQuickprof.h"
#include "../bullet/LinearMath/btAlignedObjectArray.h"

class game_object;

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

			btBroadphaseInterface*	m_overlappingPairCache;

			btCollisionDispatcher*	m_dispatcher;

			btConstraintSolver*	m_constraintSolver;

			btDefaultCollisionConfiguration* m_collisionConfiguration;

			enum

			{
				USE_CCD = 1,
				USE_NO_CCD
			};
			int 	m_ccdMode;

		protected:
#ifdef USE_BT_CLOCK
			btClock m_clock;
#endif //USE_BT_CLOCK

			///this is the most important class
			btDynamicsWorld*		m_dynamicsWorld;

			///constraint for mouse picking
			btTypedConstraint*		m_pickConstraint;

			virtual void removePickingConstraint();

			virtual void pickObject(const btVector3& pickPos, const class btCollisionObject* hitObj);

			int	m_mouseOldX;
			int	m_mouseOldY;
			int	m_mouseButtons;

			float m_scaleBottom;
			float m_scaleFactor;

			bool	m_stepping;
			bool m_singleStep;
			bool m_idle;
			int m_lastKey;

			btVector3		m_sundirection;
			btScalar		m_defaultContactProcessingThreshold = 0;

		public:
			int	m_modifierKeys;
			//keep the collision shapes, for deletion/cleanup
			btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;

			std::vector<physical_object*> physical_objects;



			bullet_manager(std::vector<game_object> game_objects);

			virtual ~bullet_manager();

			void	initPhysics(std::vector<game_object> game_objects, btDynamicsWorld* dynamicsWorld);

			void	exitPhysics();

			std::vector<game_object> get_current_state();

			void add_physical_object(game_object game_object, btDynamicsWorld* dynamicsWorld);

			virtual void clientMoveAndDisplay();

			virtual void	clientResetScene();



			btDynamicsWorld*		getDynamicsWorld()
			{
				return m_dynamicsWorld;
			}


			virtual void myinit();

			void toggleIdle();

			btScalar	getDeltaTimeMicroseconds()
			{
#ifdef USE_BT_CLOCK
				btScalar dt = (btScalar)m_clock.getTimeMicroseconds();
				m_clock.reset();
				return dt;
#else
				return btScalar(16666.);
#endif
			}


			btVector3	getRayTo(int x, int y);

			static btRigidBody*	localCreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape, btDynamicsWorld* dynamicsWorld);

			bool	isIdle() const
			{
				return	m_idle;
			}

			void	setIdle(bool idle)
			{
				m_idle = idle;
			}


		};
