#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_FRAN()
{
	Identifier = "DEFAULT_PT_FRAN";
	Name = "FRAN";
	Colour = 0xa5ff1f_rgb;
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	Advection = 0.4f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.6f;
	Diffusion = 0.00f;
	HotAir = 0.000f * CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 5;

	Weight = 100;

	DefaultProperties.temp = R_TEMP + 273.15f;
	HeatConduct = 255;
	Description = "Francium. Very unstable and radioactive. Explodes on contact with water.";

	Properties = TYPE_PART | PROP_RADIOACTIVE | PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 300.0f;
	HighTemperatureTransition = PT_LFRN;


	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	if(parts[i].temp < 773.15f)
	{
		parts[i].temp += 1;
	}
	if ((sim->rng.chance(1, 100) && sim->rng.chance(int(5.0f * sim->pv[y / CELL][x / CELL]), 200)) || sim->rng.chance(1, 2000))
	{
		sim->create_part(-3, x, y, PT_NEUT);
		if (sim->rng.chance(1, 20))
		{
			sim->part_change_type(i,x,y,PT_RDON);
		}
	}
	return 0;
}