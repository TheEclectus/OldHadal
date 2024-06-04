#include "System_Position.h"

void System_Position::_ResolveAcceleration(Component_Physics& Phys, float TimestepSeconds, float& AccX, float& AccY)
{
	const float DragAccel = 6.f;	// 6ft/s^2
	const float MovementThreshold = 0.05f;

	//float AccelerationX = 0.f;
	bool bIsMovingX = fabsf(Phys.VelX) >= MovementThreshold;
	if (!bIsMovingX)
		Phys.VelX = 0.f;

	bool bIsMovingY = fabsf(Phys.VelY) >= MovementThreshold;
	if (!bIsMovingY)
		Phys.VelY = 0.f;
	
	// No outside acceleration, but still moving. Apply drag.
	// TODO: Check and make sure 
	if (Phys.AccX == 0.f && bIsMovingX)
	{
		bool bMovingRight = !signbit(Phys.VelX);
		if (bMovingRight)	// Apply drag leftward
			Phys.AccX -= DragAccel;
		else
			Phys.AccX += DragAccel;
	}
	AccX = Phys.AccX * TimestepSeconds;

	if (Phys.AccY == 0.f && bIsMovingY)
	{
		bool bMovingDown = !signbit(Phys.VelY);
		if (bMovingDown)	// Apply drag upward
			Phys.AccY -= DragAccel;
		else
			Phys.AccY += DragAccel;
	}
	AccY = Phys.AccY * TimestepSeconds;



	//float XAbs = fabsf(Phys.VelX);
	//if (XAbs <= 0.1f)
	//	Phys.VelX = 0.f;

	//// If the velocity is below a certain threshold, it should be set to 0.
	//if (Phys.AccX == 0.f && fabsf(Phys.VelX) > 0.f) // If no outside acceleration acting and moving, drag
	//	// Positive acceleration if negative velocity, and vice versa
	//	AccelerationX = (signbit(Phys.VelX) ? DragAccel : -DragAccel) * TimestepSeconds;
	//else
	//	AccelerationX = Phys.AccX * TimestepSeconds;
}

void System_Position::Update(entt::registry& Registry, Uint32 TimestepMS)
{
	float TimestepSeconds = static_cast<float>(TimestepMS) / 1000.f;
	//entt::registry::em
	// Has an Exclude template param, make collision and collisionless
	/*auto View = Registry.view<
		Component_Position,
		Component_Velocity,
		Component_Acceleration
	>(entt::exclude<
		Component_Collision
	>);*/

	auto View = Registry.view<
		Component_Position,
		Component_Physics

		//Component_Velocity,
		//Component_Acceleration
	>();

	//entt::basic_view<

	const float DragAccel = 6.f;	// 3ft/s^2
	//for (auto [Ent, Pos, Vel, Acc] : View.each())
	for (auto [Ent, Pos, Phys] : View.each())
	{
		// TODO: Order might need reversed to respond to timestep better

		// Acceleration acts on Velocity first
		float AccelerationX = 0.f;
		float AccelerationY = 0.f;

		//float XAbs = fabsf(Phys.VelX);
		//if (XAbs <= 0.1f)
		//	Phys.VelX = 0.f;

		//// If the velocity is below a certain threshold, it should be set to 0.
		//if (Phys.AccX == 0.f && fabsf(Phys.VelX) > 0.f) // If no outside acceleration acting and moving, drag
		//	// Positive acceleration if negative velocity, and vice versa
		//	AccelerationX = (signbit(Phys.VelX) ? DragAccel : -DragAccel) * TimestepSeconds;
		//else
		//	AccelerationX = Phys.AccX * TimestepSeconds;

		//float YAbs = fabsf(Phys.VelY);
		//if (YAbs <= 0.1f)
		//	Phys.VelY = 0.f;

		//if (Phys.AccY == 0.f && fabsf(Phys.VelY) > 0.f) // If no outside acceleration acting and moving, drag
		//	// Positive acceleration if negative velocity, and vice versa
		//	AccelerationY = (signbit(Phys.VelY) ? DragAccel : -DragAccel) * TimestepSeconds;
		//else
		//	AccelerationY = Phys.AccY * TimestepSeconds;

		_ResolveAcceleration(Phys, TimestepSeconds, AccelerationX, AccelerationY);

		if (Pos.y < 0)
			AccelerationY = 32.f * TimestepSeconds;

		Phys.VelX += AccelerationX;
		if (fabsf(Phys.VelX) > Phys.VelXMax)	Phys.VelX = (signbit(Phys.VelX) ? -Phys.VelXMax : Phys.VelXMax);

		Phys.VelY += AccelerationY;
		if (fabsf(Phys.VelY) > Phys.VelYMax)	Phys.VelY = (signbit(Phys.VelY) ? -Phys.VelYMax : Phys.VelYMax);

		// Velocity now acts on position
		float DisplacementX = Phys.VelX * TimestepSeconds;
		float DisplacementY = Phys.VelY * TimestepSeconds;
		Pos.x += DisplacementX;
		Pos.y += DisplacementY;
	}
}