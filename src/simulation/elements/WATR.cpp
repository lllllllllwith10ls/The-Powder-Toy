#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_WATR()
{
	Identifier = "DEFAULT_PT_WATR";
	Name = "WATR";
	Colour = PIXPACK(0x2030D0);
	MenuVisible = 1;
	MenuSection = SC_LIQUID;
	Enabled = 1;

	Advection = 0.6f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.98f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 20;

	Weight = 30;

	DefaultProperties.temp = R_TEMP - 2.0f + 273.15f;
	HeatConduct = 29;
	Description = "Water. Conducts electricity, freezes, and extinguishes fires.";

	Properties = TYPE_LIQUID|PROP_CONDUCTS|PROP_LIFE_DEC|PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = 273.15f;
	LowTemperatureTransition = PT_ICEI;
	HighTemperature = 373.0f;
	HighTemperatureTransition = PT_WTRV;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry;
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				if (TYP(r)==PT_SALT && RNG::Ref().chance(1, 50))
				{
					sim->part_change_type(i,x,y,PT_SLTW);
					parts[i].salt[0] = parts[ID(r)].salt[0];
					parts[i].salt[1] = parts[ID(r)].salt[1];
					// on average, convert 3 WATR to SLTW before SALT turns into SLTW
					if (RNG::Ref().chance(1, 3))
						sim->part_change_type(ID(r),x+rx,y+ry,PT_SLTW);
				}
				else if ((TYP(r)==PT_RBDM||TYP(r)==PT_LRBD||TYP(r)==PT_CESM||TYP(r)==PT_LCSM) && (sim->legacy_enable||parts[i].temp>(273.15f+12.0f)) && RNG::Ref().chance(1, 100))
				{
					sim->part_change_type(i,x,y,PT_FIRE);
					parts[i].life = 4;
					parts[i].salt[0] = TYP(r);
					parts[i].salt[1] = PT_WATR;
					if(parts[i].salt[0] == PT_LRBD)
					{
						parts[i].salt[0] = PT_RBDM;
					}
					if(parts[i].salt[0] == PT_LCSM)
					{
						parts[i].salt[0] = PT_CESM;
					}
					parts[i].ctype = PT_BASE;
				}
				else if (TYP(r)==PT_FRAN || TYP(r)==PT_LFRN)
				{
					sim->part_change_type(i,x,y,PT_FIRE);
					sim->part_change_type(ID(r),x+rx,y+ry,PT_RDON);
					parts[i].temp += 1000.f;
					parts[ID(r)].temp += 1000.f;
					sim->pv[y/CELL][x/CELL] += 2.0f * CFDS;
				}
				else if (TYP(r)==PT_FIRE && parts[ID(r)].ctype!=PT_WATR)
				{
					sim->kill_part(ID(r));
					if (RNG::Ref().chance(1, 30))
					{
						sim->kill_part(i);
						return 1;
					}
				}
				else if (TYP(r)==PT_SLTW && RNG::Ref().chance(1, 2000))
				{
					sim->part_change_type(i,x,y,PT_SLTW);
					parts[i].salt[0] = parts[ID(r)].salt[0];
					parts[i].salt[1] = parts[ID(r)].salt[1];
				}
				else if (TYP(r)==PT_ROCK && fabs(parts[i].vx)+fabs(parts[i].vy) >= 0.5 && RNG::Ref().chance(1, 1000)) // ROCK erosion
				{
					if (RNG::Ref().chance(1,3))
						sim->part_change_type(ID(r),x+rx,y+ry,PT_SAND);
					else
						sim->part_change_type(ID(r),x+rx,y+ry,PT_STNE);
				}
			}
	return 0;
}
