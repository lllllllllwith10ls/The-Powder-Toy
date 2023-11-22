#include "simulation/ElementCommon.h"

void Element::Element_LCSM()
{
	Identifier = "DEFAULT_PT_LCSM";
	Name = "LCSM";
	Colour = 0xBED15E_rgb;
	MenuVisible = 1;
	MenuSection = SC_EXPLOSIVE;
	Enabled = 1;

	Advection = 0.3f;
	AirDrag = 0.02f * CFDS;
	AirLoss = 0.95f;
	Loss = 0.80f;
	Collision = 0.0f;
	Gravity = 0.15f;
	Diffusion = 0.00f;
	HotAir = 0.000001f* CFDS;
	Falldown = 2;

	Flammable = 1250;
	Explosive = 1;
	Meltable = 0;
	Hardness = 2;

	Weight = 50;

	DefaultProperties.temp = R_TEMP + 45.0f + 273.15f;
	HeatConduct = 170;
	Description = "Liquid Cesium. Basically just gold LRBD";

	Properties = TYPE_LIQUID|PROP_CONDUCTS|PROP_LIFE_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = 301.59f;
	LowTemperatureTransition = PT_CESM;
	HighTemperature = 943.95f;
	HighTemperatureTransition = PT_FIRE;
}
