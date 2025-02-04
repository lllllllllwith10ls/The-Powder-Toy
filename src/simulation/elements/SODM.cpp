#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_SODM()
{
	Identifier = "DEFAULT_PT_SODM";
	Name = "SODM";
	Colour = 0x9D9FA6_rgb;
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

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 10;

	Weight = 100;

	HeatConduct = 70;
	Description = "Sodium. Reactive element that explodes on contact with water, and can be used for salt.";

	Properties = TYPE_PART | PROP_LIFE_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 370.94f;
	HighTemperatureTransition = PT_LAVA;

	Update = &update;
	Graphics = &graphics;
}

/*

life:  burn timer
tmp:   hydrogenation factor
tmp2:  oxygenation factor

*/

static int update(UPDATE_FUNC_ARGS)
{
	Particle &self = parts[i];

	int &hydrogenationFactor = self.tmp;
	int &oxygenationFactor = self.tmp2;
	int &burnTimer = self.life;

	for (int rx = -2; rx <= 2; ++rx)
	{
		for (int ry = -2; ry <= 2; ++ry)
		{
			if (rx || ry)
			{
				int neighborData = pmap[y + ry][x + rx];
				if (!neighborData)
				{
					if (burnTimer > 12 && sim->rng.chance(1, 10))
					{
						sim->create_part(-1, x + rx, y + ry, PT_FIRE);
					}
					continue;
				}
				Particle &neighbor = parts[ID(neighborData)];

				switch (TYP(neighborData))
				{
				case PT_SLTW:
				case PT_WTRV:
				case PT_WATR:
				case PT_DSTW:
				case PT_CBNW:
					if (burnTimer > 16)
					{
						sim->part_change_type(ID(neighborData), x + rx, y + ry, PT_WTRV);
						neighbor.temp = 440.f;
						continue;
					}
					if (hydrogenationFactor >= 25)
					{
						sim->part_change_type(i, x, y, PT_BASE);
						parts[i].salt[0] = PT_SODM;
						parts[i].salt[1] = PT_WATR;
						parts[i].life = sim->rng.between(25, 75);
						continue;
					}
					if (hydrogenationFactor == 24)
					{
						burnTimer = 144;
						sim->part_change_type(ID(neighborData), x + rx, y + ry, PT_H2);
						hydrogenationFactor = 25;
					}
					else if(sim->rng.chance(1, oxygenationFactor*20 + 1))
					{
						self.temp = restrict_flt(self.temp + 10.235f, MIN_TEMP, MAX_TEMP);
						sim->part_change_type(ID(neighborData), x + rx, y + ry, PT_H2);
						hydrogenationFactor += 1;
					}
					break;

				case PT_FIRE:
					if (self.temp > 350.f && sim->rng.chance(1, 40) && hydrogenationFactor < 20)
					{
						burnTimer = 13;
						hydrogenationFactor += 1;
					}
					break;

				case PT_O2:
					if (burnTimer > 0 && sim->rng.chance(1, 10))
					{
						sim->part_change_type(i, x, y, PT_PLSM);
						sim->part_change_type(ID(neighborData), x + rx, y + ry, PT_PLSM);
						sim->pv[y / CELL][x / CELL] += 4.0;
						return 0;
					}
					else if (oxygenationFactor < 25)
					{
						sim->kill_part(ID(neighborData));
						oxygenationFactor += 1;
					}
					break;
				}
			}
		}
	}
	
	for (int trade = 0; trade < 3; ++trade)
	{
		int rx = sim->rng.between(-1, 1);
		int ry = sim->rng.between(-1, 1);
		if (rx || ry)
		{
			int neighborData = pmap[y + ry][x + rx];
			if (TYP(neighborData) != PT_SODM)
			{
				continue;
			}
			Particle &neighbor = parts[ID(neighborData)];

			int &neighborOxygenation = neighbor.tmp2;
			if (oxygenationFactor > neighborOxygenation+5)
			{
				int transfer = oxygenationFactor - neighborOxygenation-5;
				transfer -= transfer / 2;
				neighborOxygenation += transfer;
				oxygenationFactor -= transfer;
				break;
			}
		}
	}
	
	if (burnTimer == 1)
	{
		burnTimer = 0;
		sim->part_change_type(i, x, y, PT_LAVA);
		self.temp = 600.f;
		self.ctype = PT_SODM;
	}
	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	if (cpart->life >= 1)
	{
		auto colour = 0xFFA040_rgb;
		*colr = colour.Red;
		*colg = colour.Green;
		*colb = colour.Blue;
		*pixel_mode |= PMODE_FLARE | PMODE_GLOW;
	}
	else
	{
	// Oxygenated sodm
		int s = cpart->tmp2;
		if (s > 25)
		{
			s = 25;
		}
		if (s < 0)
		{
			s = 0;
		}
		auto colour = 0xE2E2E2_rgb;
	
		*colr += (s*(colour.Red - *colr))/25;
		*colg += (s*(colour.Green - *colg))/25;
		*colb += (s*(colour.Blue - *colb))/25;
	}
	return 0;
}