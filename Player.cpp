#include "Player.h"


Player::Player(entt::registry& Registry) :
	Entity(Registry)
{
	
	// Can these references be stored as a pointer in the class itself? That'd be nice
	auto &Pos = this->_entityHandle.emplace<Component_Position>();
	Pos.x = 0;
	Pos.y = 0;
	_position = &Pos;

	auto& Phys = this->_entityHandle.emplace<Component_Physics>();
	Phys.VelX = 0.f;
	Phys.VelY = 0.f;
	Phys.VelXMax = 25.f;
	Phys.VelYMax = 25.f;
	Phys.AccX = 0.f;
	Phys.AccY = 0.f;

	/*auto& Vel = this->_registry.emplace<Component_Velocity>(_id);
	Vel.x = 0;
	Vel.y = 0;
	Vel.maxY = 25.f;
	Vel.maxX = 25.f;

	auto& Acc = this->_registry.emplace<Component_Acceleration>(_id);
	Acc.x = 0;
	Acc.y = 0.f;*/
}

/*
	TODO: [optimization] Render terrain to chunk buffer?
*/

SDL_Point Player::GetPosition() const
{
	auto &Pos = this->_entityHandle.get<Component_Position>();
	
	int X = static_cast<int>(ceilf(Pos.x));
	int Y = static_cast<int>(ceilf(Pos.y));

	return { X, Y };
}

SDL_FPoint Player::GetPositionF() const
{
	auto& Pos = this->_entityHandle.get<Component_Position>();
	return { Pos.x, Pos.y };
}

void Player::SetAcceleration(float X, float Y)
{
	/*auto& Acc = this->_registry.get<Component_Acceleration>(_id);
	Acc.x = X;
	Acc.y = Y;*/

	auto& Phys = this->_entityHandle.get<Component_Physics>();
	Phys.AccX = X;
	Phys.AccY = Y;
}

//void Player::UpdateCamera()
//{
//	_camera.SetPosition(GetPosition());
//}