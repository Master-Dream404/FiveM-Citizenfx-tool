#pragma once
#include "imgui/imgui.h"

namespace Misc {
	static const char* CrosshirS[]{ "5F", "7F", "9F", "10F", "13F", "Custom", "FiveM" };
	static int selectedS = 0;
	static float crosshairsize = 50;
	bool crosshair = false;
	ImColor FovColor = ImColor(255, 0, 0, 255);
	ImColor crosshairColor = ImColor(255, 0, 0, 255);
	static float sizef = 240;
	bool crosshairColoroo = false;
	static float sizefC = 7;
	bool fov = false;
	//"Normal", "Custom", "Valorant"
	bool scripthookbypass;
}