#ifndef NAV_H
#define NAV_H

double degreesToRadians(double degrees);
double getDistance(double lat1_deg, double lng1_deg, double lat2_deg, double lng2_deg);
double latDistance(double lat1_deg, double lat2_deg);
double lngDistance(double lng1_deg, double lng2_deg, double lat_deg);

#endif