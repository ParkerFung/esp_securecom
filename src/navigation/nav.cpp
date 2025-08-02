#include "main.h"
#include "gps/gps.h"
#include "navigation/nav.h"
#include <cmath>



const double EARTH_RADIUS = 6371000.0; //radius in meters

double degreesToRadians(double degrees){
    return degrees * (3.14159265358979323846 / 180.0);
}

double getDistance(double lat1_deg, double lng1_deg, double lat2_deg, double lng2_deg){

    //convert lat and lng from degrees to rads
    double lat1_rad = degreesToRadians(lat1_deg);
    double lng1_rad = degreesToRadians(lng1_deg);
    double lat2_rad = degreesToRadians(lat2_deg);
    double lng2_rad = degreesToRadians(lng2_deg);

    //difference between coordinates
    double d_lat = lat2_rad - lat1_rad;
    double d_lng = lng2_rad - lng1_rad;

    //haversine formula
    double a = sin(d_lat / 2) * sin(d_lat / 2) + cos(lat1_rad) * cos(lat2_rad) * sin(d_lng / 2) * sin(d_lng / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));

    double distance = EARTH_RADIUS * c;

    return distance;

}

double latDistance(double lat1_deg, double lat2_deg){
    return (lat2_deg - lat1_deg) * 111132.0;
}

double lngDistance(double lng1_deg, double lng2_deg, double lat_deg){
    double lat_rad = degreesToRadians(lat_deg);
    double meters_per_degree = 111320.0 * cos(lat_rad);
    return (lng2_deg - lng1_deg) * meters_per_degree;
}
