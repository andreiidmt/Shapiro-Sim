#pragma once
#include <cmath>
#include "Objects.h"
#include "PhysicsConstants.h"

double crossProductPointsMagnitudeAU(const StartingPoint& a, const DestinationPoint& b);

double unperturbedTime(const StartingPoint& a, const DestinationPoint& b);

double impactParameterComputation(const StartingPoint& s, const DestinationPoint& d);

double shapiroTimeDelayFixed(const StartingPoint& s, const IntermediaryObject& o, const DestinationPoint& d);

double shapiroTimeDelayApproximation(const StartingPoint& s, const IntermediaryObject& o, const DestinationPoint& d);

double approximationError(const double& stdf, const double& stda);

double schwarzschildRadius(const IntermediaryObject& o);

double criticalImpactParameter(const IntermediaryObject& o);