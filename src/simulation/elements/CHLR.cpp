#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_CHLR()
{
	Identifier = "DEFAULT_PT_CHLR";
	Name = "CHLR";
	Colour = 0x80FF00_rgb;
	MenuVisible = 1;
	MenuSection = SC_GAS;
	Enabled = 1;

	Advection = 2.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.30f;
	Collision = -0.1f;
	Gravity = 0.05f;
	Diffusion = 1.50f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = 1;

	HeatConduct = 70;
	Description = "Chlorine. Reacts with water to form acid, and many other things.";

	Properties = TYPE_GAS|PROP_DEADLY;

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
	for (int rx = -2; rx <= 2; rx++)
		for (int ry = -2; ry <= 2; ry++)
			if (rx || ry)
			{
				int r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				if (TYP(r)==PT_FIRE)
				{
					parts[ID(r)].temp += sim->rng.between(0, 99);
					if (parts[ID(r)].tmp & 0x01)
						parts[ID(r)].temp = 3473;
					parts[ID(r)].tmp |= 2;

					sim->create_part(i,x,y,PT_FIRE);
					parts[i].temp += sim->rng.between(0, 99);
					parts[i].tmp |= 2;
				}
				else if (TYP(r)==PT_PLSM && !(parts[ID(r)].tmp&4))
				{
					sim->create_part(i,x,y,PT_FIRE);
					parts[i].temp += sim->rng.between(0, 99);
					parts[i].tmp |= 2;
				}
				else if (TYP(r) == PT_WTRV || TYP(r) == PT_H2)
				{
					if (sim->rng.chance(1, 250))
					{
						sim->part_change_type(i, x, y, PT_CAUS);
						parts[i].life = sim->rng.between(25, 75);
						sim->kill_part(ID(r));
					}
				}
				else if (TYP(r) == PT_CAUS)
				{
					if (sim->rng.chance(1, 250))
					{
						parts[ID(r)].life += sim->rng.between(1, 25);
						sim->kill_part(i);
						if(parts[ID(r)].life > 74)
						{
							parts[ID(r)].life = 74;
						}
					}
				}
				else if (TYP(r)==PT_DSTW || TYP(r)==PT_WATR)
				{
					sim->part_change_type(i, x, y, PT_ACID);
					parts[i].life = sim->rng.between(25, 75);
					sim->kill_part(ID(r));
				}
				else if (TYP(r)==PT_ACID && parts[ID(r)].life < 74)
				{
					parts[ID(r)].life += sim->rng.between(1, 25);
					if(parts[ID(r)].life > 74)
					{
						parts[ID(r)].life = 74;
					}
					sim->kill_part(i);
				}
				else if (TYP(r)==PT_SODM || TYP(r)==PT_LITH || TYP(r)==PT_RBDM || TYP(r)==PT_LRBD)
				{
					sim->part_change_type(ID(r), x+rx, y+ry, PT_SALT);
					sim->part_change_type(i, x, y, PT_FIRE);
					parts[i].temp += sim->rng.between(0, 99);
					parts[ID(r)].salt[0] = TYP(r);
					switch(TYP(r))
					{
						case PT_LRBD:
							parts[ID(r)].salt[0] = PT_RBDM;
							break;
					}
					parts[ID(r)].salt[1] = PT_CHLR;
				}
			}
	return 0;
}
