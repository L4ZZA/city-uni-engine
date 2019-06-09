#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "LinearMath/btVector3.h"
#include "LinearMath/btMatrix3x3.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btAlignedObjectArray.h"

class	btCollisionShape;
class	btDynamicsWorld;
class	btRigidBody;
class	btTypedConstraint;

class bullet_manager
{
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
public:
	int	m_modifierKeys;
protected:

	float m_scaleBottom;
	float m_scaleFactor;

	bool	m_stepping;
	bool m_singleStep;
	bool m_idle;
	int m_lastKey;

	btVector3		m_sundirection;
	btScalar		m_defaultContactProcessingThreshold;

public:

	bullet_manager();

	virtual ~bullet_manager();

	btDynamicsWorld*		getDynamicsWorld()
	{
		return m_dynamicsWorld;
	}

	virtual	void initPhysics() = 0;

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


	virtual void	clientResetScene();


	btVector3	getRayTo(int x, int y);

	btRigidBody*	localCreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape);

	///callback methods by glut	

	virtual void keyboardCallback(unsigned char key, int x, int y);

	virtual void keyboardUpCallback(unsigned char key, int x, int y) {}

	virtual void specialKeyboard(int key, int x, int y) {}

	virtual void specialKeyboardUp(int key, int x, int y) {}

	virtual void mouseFunc(int button, int state, int x, int y);

	virtual void	mouseMotionFunc(int x, int y);

	virtual void displayCallback();

	virtual		void	updateModifierKeys() = 0;

	bool	isIdle() const
	{
		return	m_idle;
	}

	void	setIdle(bool idle)
	{
		m_idle = idle;
	}


};
