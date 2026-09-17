#include "Objects.h"

StartingPoint::StartingPoint(double sx, double sy, double sz)
{
	this->startingCoordinates.x = sx;
	this->startingCoordinates.y = sy;
	this->startingCoordinates.z = sz;
}

double StartingPoint::getX() const
{
	return this->startingCoordinates.x;
}

double StartingPoint::getY() const
{
	return this->startingCoordinates.y;
}

double StartingPoint::getZ() const
{
	return this->startingCoordinates.z;
}

IntermediaryObject::IntermediaryObject(double m, double r)
{
	this->mass = m;
	this->radius = r;

}

double IntermediaryObject::getX() const
{
	return 0.0;
}

double IntermediaryObject::getY() const
{
	return 0.0;
}

double IntermediaryObject::getZ() const
{
	return 0.0;
}

double IntermediaryObject::getMass() const
{
	return this->mass;
}

DestinationPoint::DestinationPoint(double dx, double dy, double dz)
{
	this->destinationCoordinates.x = dx;
	this->destinationCoordinates.y = dy;
	this->destinationCoordinates.z = dz;
}

double DestinationPoint::getX() const
{
	return this->destinationCoordinates.x;
}

double DestinationPoint::getY() const
{
	return this->destinationCoordinates.y;
}

double DestinationPoint::getZ() const
{
	return this->destinationCoordinates.z;
}
