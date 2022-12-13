#include "PhysicsComponent.h"
#include "GameObject.h"
#include "GameObjectManager.h"

#include "TransformComponent.h"
#include "EngineBackend.h"

#include <iostream>

PhysicsComponent::PhysicsComponent()
{
}

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::Start()
{
	PhysicsCommon* physicsCommon = GameObjectManager::Get()->GetPhysicsCommon();
	PhysicsWorld* physicsWorld = GameObjectManager::Get()->GetPhysicsWorld();

	// Create a rigid body in the world
	SimpleMath::Vector3 scale = this->GetOwner()->GetTransform()->GetScale();

	float* m = this->GetOwner()->GetTransform()->GetPhysicsMatrix();

	//if (scale.y < 1.0f)
		//scale += SimpleMath::Vector3::One * 2.0f;

	Transform transform; transform.setFromOpenGL(m);
	BoxShape* boxShape = physicsCommon->createBoxShape(Vector3(scale.x / 2, scale.y /2, scale.z /2)); //half extent
	
	this->rigidBody = physicsWorld->createRigidBody(transform);

	this->rigidBody->addCollider(boxShape, transform);
	this->rigidBody->updateMassPropertiesFromColliders();
	this->rigidBody->setMass(1.0f);
	this->rigidBody->setType(BodyType::DYNAMIC);

	transform = this->rigidBody->getTransform();
	float matrix[16];
	transform.getOpenGLMatrix(matrix);

	GetOwner()->GetTransform()->SetPhysicsMatrix(matrix);
}

void PhysicsComponent::Update(float deltaTime)
{
	if (EngineBackend::Get()->GetMode() != EditorMode::PLAY)
		return;

	const Transform transform = this->rigidBody->getTransform();
	float* matrix = new float[16];
	transform.getOpenGLMatrix(matrix);

	const auto pos = transform.getPosition();

	GetOwner()->GetTransform()->SetPhysicsMatrix(matrix);
}

RigidBody* PhysicsComponent::GetRigidbody()
{
	return rigidBody;
}
