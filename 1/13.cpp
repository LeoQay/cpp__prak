#include <iostream>
#include <cmath>

class Coordinates
{
public:
    Coordinates(double n_latitude, double n_longitude);
    bool valid() const;
    friend std::ostream & operator<< (std::ostream & stream, const Coordinates & obj);

private:
    constexpr static double lat_min = -90.0, lat_max = 90.0;
    constexpr static double lon_min = -180.0, lon_max = 180.0;
    double latitude, longitude;
};

Coordinates::Coordinates(double n_latitude, double n_longitude)
: latitude(n_latitude), longitude(n_longitude) {}

bool Coordinates::valid() const
{
    return (latitude >= lat_min) && (latitude <= lat_max) &&
    (longitude >= lon_min) && (longitude < lon_max);
}

std::ostream & operator<< (std::ostream & stream, const Coordinates & obj)
{
    if (!obj.valid())
    {
        stream << "Invalid";
    }
    else
    {
        std::string lat_side, lon_side;
        unsigned lat_val, lon_val;

        if (obj.latitude >= 0)
        {
            lat_side = "N";
            lat_val = static_cast<unsigned>(trunc(obj.latitude));
        }
        else
        {
            lat_side = "S";
            lat_val = static_cast<unsigned>(trunc(-1 * obj.latitude));
        }

        if (obj.longitude >= 0)
        {
            lon_side = "E";
            lon_val = static_cast<unsigned>(trunc(obj.longitude));
        }
        else
        {
            lon_side = "W";
            lon_val = static_cast<unsigned>(trunc(-1 * obj.longitude));
        }

        stream << lat_val << lat_side << " " << lon_val << lon_side;
    }

    return stream;
}
