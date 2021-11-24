#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_CSAU()
{
	Identifier = "DEFAULT_PT_CSAU";
	Name = "CSAU";
	Colour = PIXPACK(0xF0F265);
	MenuVisible = 1;
	MenuSection = SC_POWDERS;
	Enabled = 1;

	Advection = 0.4f;
	AirDrag = 0.04f * CFDS;
	AirLoss = 0.94f;
	Loss = 0.95f;
	Collision = -0.1f;
	Gravity = 0.3f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 5;
	Hardness = 1;

	Weight = 75;

	HeatConduct = 110;
	Description = "Cesium auride, decomposes in water to form cesium and gold.";

	Properties = TYPE_PART;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1173.0f;
	HighTemperatureTransition = PT_LAVA;
	
	DefaultProperties.salt[0] = PT_CESM;
	DefaultProperties.salt[1] = PT_GOLD;
	
	Update = &update;
}


static int update(UPDATE_FUNC_ARGS)
{
	sim->part_change_type(i,x,y,PT_SALT);
	return 0;
}