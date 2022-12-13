#pragma once
#include "Component.h"

#include <reactphysics3d/reactphysics3d.h>
using namespace reactphysics3d;

class PhysicsComponent : public Component
{
public:
	PhysicsComponent();
	~PhysicsComponent();

public:
	void Start() override;
	void Update(float deltaTime) override;
	void ChangeBodyType(int type);
	RigidBody* GetRigidbody();
	
private:
	RigidBody* rigidBody = nullptr;
};

