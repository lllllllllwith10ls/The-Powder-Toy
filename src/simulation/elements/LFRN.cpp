#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_LFRN()
{
	Identifier = "DEFAULT_PT_LFRN";
	Name = "LFRN";
	Colour = PIXPACK(0x90db1f);
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
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

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 5;

	Weight = 50;

	DefaultProperties.temp = R_TEMP + 273.15f + 50.0f;
	HeatConduct = 255;
	Description = "Liquid Francium.";

	Properties = TYPE_LIQUID | PROP_RADIOACTIVE | PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = 300.0f;
	LowTemperatureTransition = PT_FRAN;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;


	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	if(parts[i].temp < 773.15f)
	{
		parts[i].temp += 1;
	}
	if ((RNG::Ref().chance(1, 100) && RNG::Ref().chance(int(5.0f * sim->pv[y / CELL][x / CELL]), 200)) || RNG::Ref().chance(1, 2000))
	{
		sim->create_part(-3, x, y, PT_NEUT);
		if (RNG::Ref().chance(1, 100))
		{
			sim->part_change_type(i,x,y,PT_RDON);
		}
	}
	return 0;
}