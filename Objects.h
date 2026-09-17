#pragma once

//COORDINATES ARE AU!!!!
struct SpaceCoordinates {
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;
};

class StartingPoint {
private:
	SpaceCoordinates startingCoordinates;
public:
	StartingPoint(double sx, double sy, double sz);
	double getX() const;
	double getY() const;
	double getZ() const;

	void setX(double x) { this->startingCoordinates.x = x; }
	void setY(double y) { this->startingCoordinates.y = y; }
	void setZ(double z) { this->startingCoordinates.z = z; }
};

// intermediary objects sits at the center of the "universe"
class IntermediaryObject {
private:
	double mass;
	double radius;
	SpaceCoordinates intermediaryCoordinates{ 0.0,0.0,0.0 };
public:
	IntermediaryObject(double m, double r);

	double getX() const;
	double getY() const;
	double getZ() const;

	double getMass() const;
	double getRadius() const { return this->radius; }

	void setMass(double m) { this->mass = m; }
	void setRadius(double r) { this->radius = r; }
};

class DestinationPoint {
private:
	SpaceCoordinates destinationCoordinates;
public:
	DestinationPoint(double dx, double dy, double dz);
	double getX() const;
	double getY() const;
	double getZ() const;

	void setX(double x) { this->destinationCoordinates.x = x; }
	void setY(double y) { this->destinationCoordinates.y = y; }
	void setZ(double z) { this->destinationCoordinates.z = z; }
};