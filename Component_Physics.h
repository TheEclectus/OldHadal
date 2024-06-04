#pragma once

struct Component_Physics
{

	// Velocity
	float VelX = 0.f;
	float VelY = 0.f;
	float VelXMax = 0.f;	// 0.f = no limit
	float VelYMax = 0.f;	// 0.f = no limit

	// Acceleration
	float AccX = 0.f;
	float AccY = 0.f;
	float AccXMax = 0.f;	// 0.f = no limit
	float AccYMax = 0.f;	// 0.f = no limit
};