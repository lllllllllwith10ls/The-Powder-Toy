#include "simulation/ElementCommon.h"

static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_SALT()
{
	Identifier = "DEFAULT_PT_SALT";
	Name = "SALT";
	Colour = 0xFFFFFF_rgb;
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
	Description = "Salt, dissolves in water.";

	Properties = TYPE_PART;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = MIN_TEMP;
	HighTemperatureTransition = PT_LAVA;
	
	DefaultProperties.salt[0] = PT_SODM;
	DefaultProperties.salt[1] = PT_CHLR;
	
	Graphics = &graphics;
}


static int graphics(GRAPHICS_FUNC_ARGS)
{
	if(cpart->salt[0] == PT_CESM && cpart->salt[1] == PT_GOLD)
	{
		auto colour = 0xF0F265_rgb;
		*colr = colour.Red;
		*colg = colour.Green;
		*colb = colour.Blue;
	}
	return 0;
}
