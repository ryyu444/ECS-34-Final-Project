#ifndef GEOGRAPHICUTILS_H
#define GEOGRAPHICUTILS_H

#include "StreetMap.h"

struct SGeographicUtils{
    static double DegreesToRadians(double deg);
    static double RadiansToDegrees(double rad);
    static double HaversineDistanceInMiles(CStreetMap::TLocation loc1, CStreetMap::TLocation loc2);
    static double CalculateBearing(CStreetMap::TLocation src, CStreetMap::TLocation dest);
    static std::string BearingToDirection(double bearing);
    static std::string ConvertLLToDMS(CStreetMap::TLocation loc);
};

#endif
