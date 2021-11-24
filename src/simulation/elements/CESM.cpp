#include "simulation/ElementCommon.h"

void Element::Element_CESM()
{
	Identifier = "DEFAULT_PT_CESM";
	Name = "CESM";
	Colour = PIXPACK(0xD1E765);
	MenuVisible = 1;
	MenuSection = SC_EXPLOSIVE;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 1250;
	Explosive = 1;
	Meltable = 50;
	Hardness = 1;

	Weight = 100;

	HeatConduct = 240;
	Description = "Cesium. Extremely explosive, reacts with water and oxygen. Very low melting point.";

	Properties = TYPE_SOLID|PROP_CONDUCTS|PROP_LIFE_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 301.59f;
	HighTemperatureTransition = PT_LCSM;
}
