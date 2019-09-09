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

engine::physical_object::physical_object(btRigidBody* body) : body(body) {}

//returns the forward vector of the physical object
btVector3 engine::physical_object::get_forward()
{
    btVector3 body_forward_vector;
    btTransform trans = body->getWorldTransform();
    body_forward_vector = (trans * forward_vector - trans.getOrigin());
    return body_forward_vector;
}

//Returns the up vector of the physical object
btVector3 engine::physical_object::get_up()
{
    btVector3 body_up_vector;
    btTransform trans = body->getWorldTransform();
    body_up_vector = (trans * up_vector - trans.getOrigin());
    return body_up_vector;
}

//Constructor passing the game object vector
engine::bullet_manager::bullet_manager(std::vector<engine::game_object* > game_objects)
{
    init_physics(game_objects, m_dynamics_world);
}

//Default constructor
engine::bullet_manager::bullet_manager()
{
    init_physics({}, m_dynamics_world);
}

//Destructor
engine::bullet_manager::~bullet_manager() {}

//Physics initialisation
void	engine::bullet_manager::init_physics(std::vector<engine::game_object* > game_objects, btDynamicsWorld* dynamics_world)
{

    // init world
    m_collision_configuration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collision_configuration);
    btVector3 world_min(-1000, -1000, -1000);
    btVector3 world_max(1000, 1000, 1000);
    m_overlapping_pair_cache = new btAxisSweep3(world_min, world_max);

    m_constraint_solver = new btSequentialImpulseConstraintSolver();

    btDiscreteDynamicsWorld* wp = new btDiscreteDynamicsWorld(m_dispatcher, m_overlapping_pair_cache, m_constraint_solver, m_collision_configuration);
    //	wp->getSolverInfo().m_numIterations = 20; // default is 10
    m_dynamics_world = wp;
    //m_dynamicsWorld->setInternalTickCallback(MyTickCallback, static_cast<void *>(this), true);

    for(int32_t i = 0; i < game_objects.size(); i++)
    {
        add_physical_object(game_objects.at(i), dynamics_world);
    }

    wp->setGravity(btVector3(0, -9.8, 0));
}

//Creating a rigidBody from a collision shape
btRigidBody* engine::bullet_manager::local_create_rigid_body(float mass, const btTransform& start_transform, btCollisionShape* shape, btDynamicsWorld* dynamics_world)
{
    btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool is_dynamic = (mass != 0.f);

    btVector3 local_inertia(0, 0, 0);
    if(is_dynamic)
        shape->calculateLocalInertia(mass, local_inertia);

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

#define USE_MOTIONSTATE 1
#ifdef USE_MOTIONSTATE
    btDefaultMotionState * my_motion_state = new btDefaultMotionState(start_transform);



    btRigidBody::btRigidBodyConstructionInfo c_info(mass, my_motion_state, shape, local_inertia);

    btRigidBody* body = new btRigidBody(c_info);
    body->setContactProcessingThreshold(BT_LARGE_FLOAT);

#else
    btRigidBody* body = new btRigidBody(mass, 0, shape, local_inertia);
    body->setWorldTransform(start_transform);
#endif//

    get_dynamics_world()->addRigidBody(body);

    return body;
}

//Adding a new physical object to the physics simulation by using info from the passed game object
void engine::bullet_manager::add_physical_object(engine::game_object* game_object, btDynamicsWorld* dynamics_world)
{
    btCollisionShape* shape;

    switch(game_object->type())
    {
        //if type is a box shape
        case 0:
        {
            shape = new btBoxShape(btVector3(btScalar(game_object->bounding_shape().x), btScalar(game_object->bounding_shape().y), btScalar(game_object->bounding_shape().z)));
            break;
        }
        //if type is a sphere shape
        case 1:
        {
            shape = new btSphereShape(btScalar(game_object->bounding_shape().x));
            break;
        }
        //if type is a convex shape 
        case 2:
        {
            btConvexHullShape* c_shape = new btConvexHullShape();
            //for(int32_t i = 0; i < game_object->get_mesh().vertices().size(); i++)
            {
                //engine::mesh::vertex vertex = game_object->get_mesh().vertices().at(i);
                //c_shape->addPoint(btVector3(btScalar(vertex.position.x), btScalar(vertex.position.y), btScalar(vertex.position.z)));
            }
            shape = c_shape;
            break;
        }
    }
    m_collision_shapes.push_back(shape);

    btTransform trans;
    trans.setIdentity();
    btVector3 pos(btScalar(game_object->position().x), btScalar(game_object->position().y), btScalar(game_object->position().z));
    trans.setOrigin(pos);
    trans.setRotation(btQuaternion(btVector3(btScalar(game_object->rotation_axis().x), btScalar(game_object->rotation_axis().y), btScalar(game_object->rotation_axis().z)), btRadians(game_object->rotation_amount())));


    btScalar mass(game_object->mass());
    btVector3 local_inertia;
    shape->calculateLocalInertia(mass, local_inertia);


    btRigidBody* body = local_create_rigid_body(mass, trans, shape, dynamics_world);
    physical_object* object = new physical_object(body);
    physical_objects.push_back(object);
}

//Step the sinulation (update method)
void engine::bullet_manager::dynamics_world_update(std::vector<engine::game_object*> game_objects)
{
    if(physical_objects.size() == game_objects.size())
    {
        int32_t i = 0;
        for(int32_t i = 0; i < physical_objects.size(); i++)
        {
            btTransform trans;
            engine::game_object* game_object_i = game_objects.at(i);
            physical_object* physical_object_i = physical_objects.at(i);

            btVector3 pos = to_bt_vector3(game_object_i->position());
            trans.setOrigin(pos);
            trans.setRotation(btQuaternion(to_bt_vector3(game_object_i->rotation_axis()), btRadians(game_object_i->rotation_amount())));
            btDefaultMotionState* myMotionState = new btDefaultMotionState(trans);

            physical_object_i->get_body()->setMotionState(myMotionState); to_bt_vector3(game_object_i->velocity());
            physical_object_i->get_body()->setLinearVelocity(to_bt_vector3(game_object_i->velocity()));
        }
    }
    ///step the simulation
    if(m_dynamics_world)
    {
        m_dynamics_world->stepSimulation(1. / 60., 0);//ms / 1000000.f);
    }
    for(int32_t i = 0; i < game_objects.size(); i++)
    {
        engine::game_object* game_object_i = game_objects.at(i);
        physical_object* physical_object_i = physical_objects.at(i);

        game_object_i->set_position(to_vec3(physical_object_i->get_body()->getCenterOfMassPosition()));
        game_object_i->set_rotation_axis(to_vec3(physical_object_i->get_body()->getCenterOfMassTransform().getRotation().getAxis()));
        game_object_i->set_rotation_amount(glm::degrees(physical_object_i->get_body()->getCenterOfMassTransform().getRotation().getAngle()));
        game_object_i->set_velocity(to_vec3(physical_object_i->get_body()->getLinearVelocity()));
    }
}
