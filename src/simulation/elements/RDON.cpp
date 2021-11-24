#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_RDON()
{
	Identifier = "DEFAULT_PT_RDON";
	Name = "RDON";
	Colour = PIXPACK(0xb8c797);
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	Advection = 2.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.30f;
	Collision = -0.1f;
	Gravity = 0.1f;
	Diffusion = 1.0f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = 1;

	HeatConduct = 88;
	Description = "Radon. Heavy and radioactive gas. Produces neutrons.";

	Properties = TYPE_GAS|PROP_NEUTPASS|PROP_RADIOACTIVE;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	if(parts[i].temp < 288.15f)
	{
		parts[i].temp += 0.1f;
	}
	if ((RNG::Ref().chance(1, 100) && RNG::Ref().chance(int(5.0f*sim->pv[y/CELL][x/CELL]), 500)) || RNG::Ref().chance(1, 5000))
	{
		sim->create_part(-3, x, y, PT_NEUT);
		if (RNG::Ref().chance(1, 100))
		{
			sim->part_change_type(i,x,y,PT_POLO);
		}
	}
	
	return 0;
}
