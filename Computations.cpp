#include "Computations.h"

/*
    Made for the cross product that we need when computing the impact parameter.
*/

double crossProductPointsMagnitudeAU(const StartingPoint& a, const DestinationPoint& b)
{
    double finalX = (a.getY() * b.getZ()) - (a.getZ() * b.getY());
    double finalY = (a.getZ() * b.getX()) - (a.getX() * b.getZ());
    double finalZ = (a.getX() * b.getY()) - (a.getY() * b.getX());

    return std::hypot(finalX, finalY, finalZ);
}

double unperturbedTime(const StartingPoint& a, const DestinationPoint& b)
{
    const double dx = b.getX() - a.getX();
    const double dy = b.getY() - a.getY();
    const double dz = b.getZ() - a.getZ();

    double distanceAU = std::hypot(dx, dy, dz);
    double distanceMeters = distanceAU * AU;
    return distanceMeters/ c;
}

double impactParameterComputation(const StartingPoint& s, const DestinationPoint& d)
{
    double numeratorAU = crossProductPointsMagnitudeAU(s, d);

    const double dx = d.getX() - s.getX();
    const double dy = d.getY() - s.getY();
    const double dz = d.getZ() - s.getZ();

    double denominatorAU = std::hypot(dx, dy, dz);

    return (numeratorAU / denominatorAU) * AU;
}

double shapiroTimeDelayFixed(const StartingPoint& s, const IntermediaryObject& o, const DestinationPoint& d)
{
    //Because in this project we consider the intermediary object to always be located at coordinates(0, 0, 0), the position vector for
    //both the starting and the destination points is simply the space coordinates.

    const double magnitudePositionStarting = std::hypot(s.getX(), s.getY(), s.getZ())*AU;
    const double magnitudePositionDestination = std::hypot(d.getX(), d.getY(), d.getZ())*AU;

    const double distanceX = d.getX() - s.getX();
    const double distanceY = d.getY() - s.getY();
    const double distanceZ = d.getZ() - s.getZ();

    double distance = std::hypot(distanceX, distanceY, distanceZ)*AU;

    auto logValue = log((magnitudePositionStarting + magnitudePositionDestination + distance) /
        (magnitudePositionStarting + magnitudePositionDestination - distance));

    //THE FORMULA ABOVE IS GOOD, BUT THE SUBTRACTION IN THE DENOMINATOR CAUSES ERROR FOR SMALL VALUES
    // s-d = (s-d)(s+d)/(s+d) = (s^2-d^2)/(s+d)
    //(s+d)*((s+d)/(s^2-d^2)) = (s+d)^2/(s^2-d^2)

    return ((2 * G * o.getMass()) / pow(c, 3)) * logValue;
}

double shapiroTimeDelayApproximation(const StartingPoint& s, const IntermediaryObject& o, const DestinationPoint& d)
{
    const double magnitudePositionStarting = std::hypot(s.getX(), s.getY(), s.getZ()) * AU;
    const double magnitudePositionDestination = std::hypot(d.getX(), d.getY(), d.getZ()) * AU;
    const double impactParameter = impactParameterComputation(s, d);

    return 2 * G * o.getMass() / pow(c, 3) * log((4 * magnitudePositionStarting * magnitudePositionDestination) / pow(impactParameter, 2));
}

double approximationError(const double& stdf, const double& stda)
{
    double dif = abs(stda - stdf);
    return dif * 100 / stdf;
}

double schwarzschildRadius(const IntermediaryObject& o)
{
    return 2 * G * o.getMass() / pow(c, 2);
}

double criticalImpactParameter(const IntermediaryObject& o)
{
    return 3 * sqrt(3) / 2 * schwarzschildRadius(o);
}
