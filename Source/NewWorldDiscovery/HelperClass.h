// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

namespace
{
	FVector RotateAroundS(FVector Center, FVector ToRotate, float StaticXPos, float StartRotation, float Amplitude = 1.0f, float Frequency = 1.0f) 
	{
		float MouseY = (StartRotation * 360);		//percent base x percent of 360
		float s = Amplitude * FMath::Sin(Frequency * MouseY);
		float c = Amplitude * FMath::Cos(Frequency * MouseY);

		float newY = Center.Y + (c * (ToRotate.Y - Center.Y) - s * (ToRotate.Z - Center.Z));
		float newZ = Center.Z + (s * (ToRotate.Y - Center.Y) + c * (ToRotate.Z - Center.Z));

		FVector newLocation = FVector(StaticXPos, newY, newZ);
		return newLocation;
	}
}

/**
 * 
 */
class NEWWORLDDISCOVERY_API HelperClass
{
public:
	HelperClass();
	~HelperClass();

	FVector static RotateAround(FVector Center, FVector ToRotate, float StaticXPos, float StartRotation, float Amplitude = 1.0f, float Frequency = 1.0f);
};
