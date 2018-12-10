#pragma once
#include "stdafx.h"

struct Hiding
{
public:
	bool activatedHiding;
	bool onChange;
	float hidingFactor;
	float currentHidingTime;
	float hidingTime;
	float bumpness;

	Hiding() : activatedHiding(false), onChange(false), hidingFactor(0),
		currentHidingTime(0), hidingTime(1.5f), bumpness(0.1f) {}

	void Activate(bool act)
	{
		if (activatedHiding != act)
		{
			activatedHiding = act;
			onChange = true;
		}
	}

	void UpdateHidingTime(float deltaTime)
	{
		if (activatedHiding)
		{
			currentHidingTime += deltaTime;
			hidingFactor = currentHidingTime / hidingTime;

			if (currentHidingTime >= hidingTime)
			{
				onChange = false;
				hidingFactor = 1.0f;
				currentHidingTime = hidingTime;
			}
		}
		else if (activatedHiding == false)
		{
			currentHidingTime -= deltaTime;
			hidingFactor = currentHidingTime / hidingTime;

			if (currentHidingTime <= 0)
			{
				onChange = false;
				hidingFactor = 0.0f;
				currentHidingTime = 0;
			}
		}
	}
};