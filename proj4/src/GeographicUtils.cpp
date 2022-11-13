#include "GeographicUtils.h"
#include <sstream>
#include <iomanip>
#include <cmath>

double SGeographicUtils::DegreesToRadians(double deg){
    return M_PI * (deg) / 180.0;
}

double SGeographicUtils::RadiansToDegrees(double rad){
    return 180.0 * (rad) / M_PI;
}

double SGeographicUtils::HaversineDistanceInMiles(CStreetMap::TLocation loc1, CStreetMap::TLocation loc2){
    double LatRad1 = DegreesToRadians(std::get<0>(loc1));
    double LatRad2 = DegreesToRadians(std::get<0>(loc2));
    double LonRad1 = DegreesToRadians(std::get<1>(loc1));
    double LonRad2 = DegreesToRadians(std::get<1>(loc2));
    double DeltaLat = LatRad2 - LatRad1;
    double DeltaLon = LonRad2 - LonRad1;
    double DeltaLatSin = sin(DeltaLat/2);
    double DeltaLonSin = sin(DeltaLon/2);
    double Computation = asin(sqrt(DeltaLatSin * DeltaLatSin + cos(LatRad1) * cos(LatRad2) * DeltaLonSin * DeltaLonSin));
    const double EarthRadiusMiles = 3959.88;

    return 2 * EarthRadiusMiles * Computation;
}

double SGeographicUtils::CalculateBearing(CStreetMap::TLocation src, CStreetMap::TLocation dest){
    double LatRad1 = DegreesToRadians(std::get<0>(src));
    double LatRad2 = DegreesToRadians(std::get<0>(dest));
    double LonRad1 = DegreesToRadians(std::get<1>(src));
    double LonRad2 = DegreesToRadians(std::get<1>(dest));
    double X = cos(LatRad2)*sin(LonRad2-LonRad1);
    double Y = cos(LatRad1)*sin(LatRad2)-sin(LatRad1)*cos(LatRad2)*cos(LonRad2-LonRad1);
    return RadiansToDegrees(atan2(X,Y));
}

std::string SGeographicUtils::BearingToDirection(double bearing){
    if(-22.5 <= bearing){
        if(67.5 > bearing){
            if(22.5 >= bearing){
                return "N";   
            }
            return "NE";   
        }
        else{
            if(112.5 >= bearing){
                return "E";   
            }
            else if(157.5 >= bearing){
                return "SE";   
            }
            return "S";
        }
    }
    else{
        if(-112.5 <= bearing){
            if(-67.5 >= bearing){
                return "W";   
            }
            return "NW";
        }
        else{
            if(-157.5 >= bearing){
                return "S";
            }
            return "SW";
        }
    }
}

std::string SGeographicUtils::ConvertLLToDMS(CStreetMap::TLocation loc){
    double Lat = std::get<0>(loc);
    double Lon = std::get<1>(loc);
    double LatAbs = fabs(Lat);
    double LonAbs = fabs(Lon);
    std::stringstream OutStream;
    
    OutStream<<(int)LatAbs<<"d ";
    double Remainder = LatAbs - (int)LatAbs;
    Remainder *= 60.0;
    OutStream<<(int)Remainder<<"' ";
    Remainder = Remainder - (int)Remainder;
    Remainder *= 60.0;
    Remainder = Remainder < 0.0005 ? 0.0 : Remainder;
    OutStream<<std::setprecision(4)<<std::noshowpoint<<Remainder<<(Lat < 0.0 ? "\" S, " : "\" N, ");
    
    OutStream<<(int)LonAbs<<"d ";
    Remainder = LonAbs - (int)LonAbs;
    Remainder *= 60.0;
    OutStream<<(int)Remainder<<"' ";
    Remainder = Remainder - (int)Remainder;
    Remainder *= 60.0;
    Remainder = Remainder < 0.0005 ? 0.0 : Remainder;
    OutStream<<Remainder<<(Lon < 0.0 ? "\" W" : "\" E");
    
    return OutStream.str();
}
