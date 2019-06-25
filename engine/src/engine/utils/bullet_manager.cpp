#include "pch.h"

#include "engine/utils/bullet_manager.h"
#include "LinearMath/btIDebugDraw.h"
#include "BulletDynamics/Dynamics/btDynamicsWorld.h"

#include "BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"//picking
#include "BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"//picking

#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "BulletCollision/CollisionShapes/btUniformScalingShape.h"
#include "BulletDynamics/ConstraintSolver/btConstraintSolver.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btDefaultMotionState.h"
#include "LinearMath/btSerializer.h"

#include "math.h"

///btBulletDynamicsCommon.h is the main Bullet include file, contains most common include files.
#include "btBulletDynamicsCommon.h"

#include "engine/entities/game_object.h"


physical_object::physical_object(btRigidBody* body) : body(body) {}

//returns the forward vector of the physical object
btVector3 physical_object::GetForward() {
	btVector3 boidForwardVector;
	btTransform trans = body->getWorldTransform();
	boidForwardVector = (trans * ForwardVector - trans.getOrigin());
	return boidForwardVector;
}

//Returns the up vector of the physical object
btVector3 physical_object::GetUp() {
	btVector3 boidUpVector;
	btTransform trans = body->getWorldTransform();
	boidUpVector = (trans * UpVector - trans.getOrigin());
	return boidUpVector;
}

bullet_manager::bullet_manager(std::vector<game_object> game_objects)
//see btIDebugDraw.h for modes
	:
	m_dynamicsWorld(0),
	m_pickConstraint(0),
	m_mouseOldX(0),
	m_mouseOldY(0),
	m_mouseButtons(0),
	m_modifierKeys(0),
	m_scaleBottom(0.5f),
	m_scaleFactor(2.f),
	m_stepping(true),
	m_singleStep(false),
	m_idle(false),

	m_sundirection(btVector3(1, -2, 1) * 1000),
	m_defaultContactProcessingThreshold(BT_LARGE_FLOAT)
{
#ifndef BT_NO_PROFILE
	m_profileIterator = CProfileManager::Get_Iterator();
#endif //BT_NO_PROFILE
	initPhysics(game_objects, m_dynamicsWorld);
}


bullet_manager::~bullet_manager()
{
#ifndef BT_NO_PROFILE
	CProfileManager::Release_Iterator(m_profileIterator);
#endif //BT_NO_PROFILE
	exitPhysics();
}

void bullet_manager::myinit(void)
{


}

void bullet_manager::add_physical_object(game_object game_object, btDynamicsWorld* dynamicsWorld) {
	btCollisionShape* shape;

	switch (game_object.type())
	{
	//if type is a box shape
	case 0:
		{
			shape = new btBoxShape(btVector3(btScalar(game_object.bounding_shape().x), btScalar(game_object.bounding_shape().y), btScalar(game_object.bounding_shape().z)));
			break;
		}
	//if type is a sphere shape
	case 1:
		{
			shape = new btSphereShape(btScalar(game_object.bounding_shape().x));
			break;
		}
	//if type is a convex shape 
	case 2:
		{
			btConvexHullShape * c_shape = new btConvexHullShape();
			for (int i = 0; i < game_object.get_mesh().vertices().size(); i++) {
				engine::mesh::vertex vertex = game_object.get_mesh().vertices().at(i);
				c_shape->addPoint(btVector3(btScalar(vertex.position.x), btScalar(vertex.position.y), btScalar(vertex.position.z)));
			}
			shape = c_shape;
			break;
		}
	}
	m_collisionShapes.push_back(shape);

	btTransform trans;
	trans.setIdentity();
	btVector3 pos(btScalar(game_object.position().x), btScalar(game_object.position().y), btScalar(game_object.position().z));
	trans.setOrigin(pos);
	trans.setRotation(btQuaternion(btVector3(btScalar(game_object.rotation_axis().x), btScalar(game_object.rotation_axis().y), btScalar(game_object.rotation_axis().z)), btRadians(game_object.rotation_amount())));


	btScalar mass(game_object.mass());
	btVector3 local_inertia;
	shape->calculateLocalInertia(mass, local_inertia);

	btRigidBody * body = localCreateRigidBody(mass, trans, shape, dynamicsWorld);
	physical_object * object = new physical_object(body);
	physical_objects.push_back(object);
}


void bullet_manager::clientMoveAndDisplay()
{
	///step the simulation
	if (m_dynamicsWorld)
	{
		m_dynamicsWorld->stepSimulation(1. / 60., 0);//ms / 1000000.f);
	}
}

void MyTickCallback(btDynamicsWorld *world, btScalar timeStep) {
	world->clearForces();

	bullet_manager* man = static_cast<bullet_manager *>(world->getWorldUserInfo());

	bool first = true;

	//Loop over each physical object
	for (int i = 0; i < man->physical_objects.size(); i++) {
		btVector3 posD(0, 0, 0), dirD(0, 0, 0);
		physical_object* boidI = man->physical_objects[i];
		//Check for other birds
		for (int j = i + 1; j < man->physical_objects.size(); j++) {
			
		}

		//apply forces
	}
}

void	bullet_manager::initPhysics(std::vector<game_object> game_objects, btDynamicsWorld* dynamicsWorld)
{

	// init world
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	btVector3 worldMin(-1000, -1000, -1000);
	btVector3 worldMax(1000, 1000, 1000);
	m_overlappingPairCache = new btAxisSweep3(worldMin, worldMax);

	m_constraintSolver = new btSequentialImpulseConstraintSolver();

	btDiscreteDynamicsWorld* wp = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_constraintSolver, m_collisionConfiguration);
	//	wp->getSolverInfo().m_numIterations = 20; // default is 10
	m_dynamicsWorld = wp;
	m_dynamicsWorld->setInternalTickCallback(MyTickCallback, static_cast<void *>(this), true);

	for (int i = 0; i < game_objects.size(); i++) {
		add_physical_object(game_objects.at(i), dynamicsWorld);
	}
}

//void	bullet_manager::clientResetScene(){
	//exitPhysics();
	//initPhysics();}




void	bullet_manager::exitPhysics()
{

	//cleanup in the reverse order of creation/initialization

	//remove the rigidbodies from the dynamics world and delete them
	int i;
	for (i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < m_collisionShapes.size(); j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}
	m_collisionShapes.clear();

	//delete physical objects
	for (int j = 0; j < physical_objects.size(); j++)
	{
		physical_object* object = physical_objects[j];
		delete object;
	}
	physical_objects.clear();

	delete m_dynamicsWorld;

	//	delete m_solver;

	//	delete m_broadphase;

	//	delete m_dispatcher;

	//	delete m_collisionConfiguration;


}




extern bool gDisableDeactivation;
int numObjects = 0;
const int maxNumObjects = 16384;
btTransform startTransforms[maxNumObjects];
btCollisionShape* gShapePtr[maxNumObjects];//1 rigidbody has 1 shape (no re-use of shapes)
#define SHOW_NUM_DEEP_PENETRATIONS 1

extern int gNumClampedCcdMotions;

#ifdef SHOW_NUM_DEEP_PENETRATIONS 
extern int gNumDeepPenetrationChecks;

extern int gNumSplitImpulseRecoveries;
extern int gNumGjkChecks;
extern int gNumAlignedAllocs;
extern int gNumAlignedFree;
extern int gTotalBytesAlignedAllocs;

#endif //






void bullet_manager::toggleIdle() {
	if (m_idle) {
		m_idle = false;
	}
	else {
		m_idle = true;
	}
}





#define NUM_SPHERES_ON_DIAGONAL 9


btRigidBody*	bullet_manager::localCreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape, btDynamicsWorld* dynamicsWorld)
{
	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

#define USE_MOTIONSTATE 1
#ifdef USE_MOTIONSTATE
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	

	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);

	btRigidBody* body = new btRigidBody(cInfo);
	body->setContactProcessingThreshold(BT_LARGE_FLOAT);

#else
	btRigidBody* body = new btRigidBody(mass, 0, shape, localInertia);
	body->setWorldTransform(startTransform);
#endif//

	dynamicsWorld->addRigidBody(body);

	return body;
}

#include "BulletCollision/BroadphaseCollision/btAxisSweep3.h"


void	bullet_manager::clientResetScene()
{
	removePickingConstraint();

#ifdef SHOW_NUM_DEEP_PENETRATIONS
	gNumDeepPenetrationChecks = 0;
	gNumGjkChecks = 0;
#endif //SHOW_NUM_DEEP_PENETRATIONS

	gNumClampedCcdMotions = 0;
	int numObjects = 0;
	int i;

	if (m_dynamicsWorld)
	{
		int numConstraints = m_dynamicsWorld->getNumConstraints();
		for (i = 0; i < numConstraints; i++)
		{
			m_dynamicsWorld->getConstraint(0)->setEnabled(true);
		}
		numObjects = m_dynamicsWorld->getNumCollisionObjects();

		///create a copy of the array, not a reference!
		btCollisionObjectArray copyArray = m_dynamicsWorld->getCollisionObjectArray();




		for (i = 0; i < numObjects; i++)
		{
			btCollisionObject* colObj = copyArray[i];
			btRigidBody* body = btRigidBody::upcast(colObj);
			if (body)
			{
				if (body->getMotionState())
				{
					btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
					myMotionState->m_graphicsWorldTrans = myMotionState->m_startWorldTrans;
					body->setCenterOfMassTransform(myMotionState->m_graphicsWorldTrans);
					colObj->setInterpolationWorldTransform(myMotionState->m_startWorldTrans);
					colObj->forceActivationState(ACTIVE_TAG);
					colObj->activate();
					colObj->setDeactivationTime(0);
					//colObj->setActivationState(WANTS_DEACTIVATION);
				}
				//removed cached contact points (this is not necessary if all objects have been removed from the dynamics world)
				if (m_dynamicsWorld->getBroadphase()->getOverlappingPairCache())
					m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(colObj->getBroadphaseHandle(), getDynamicsWorld()->getDispatcher());

				btRigidBody* body = btRigidBody::upcast(colObj);
				if (body && !body->isStaticObject())
				{
					btRigidBody::upcast(colObj)->setLinearVelocity(btVector3(0, 0, 0));
					btRigidBody::upcast(colObj)->setAngularVelocity(btVector3(0, 0, 0));
				}
			}

		}

		///reset some internal cached data in the broadphase
		m_dynamicsWorld->getBroadphase()->resetPool(getDynamicsWorld()->getDispatcher());
		m_dynamicsWorld->getConstraintSolver()->reset();

	}

}
