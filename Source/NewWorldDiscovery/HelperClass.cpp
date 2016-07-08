// Fill out your copyright notice in the Description page of Project Settings.

#include "NewWorldDiscovery.h"
#include "HelperClass.h"

HelperClass::HelperClass()
{
}

HelperClass::~HelperClass()
{
}

//ToDo Choose Space (xy,xz,zy,xyz....)
FVector HelperClass::RotateAround(FVector Center, FVector ToRotate,float StaticXPos,float StartRotation, float Amplitude, float Frequency)
{
	return RotateAroundS(Center,ToRotate,StaticXPos,StartRotation,Amplitude,Frequency);
}