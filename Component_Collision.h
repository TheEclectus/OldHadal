#pragma once

/* 
	TODO: Maybe multiple collision boxes can be merged into a single polygon? Maxima calculated to create
	a cheap bounding box to test against first?
*/
struct Component_Collision
{
	int Width = 0;
	int Height = 0;
};