#pragma once

#include "math/vector/vector4.h"


namespace kl
{
template<typename T>
struct Polygon_T
{
    std::vector<Vector2<T>> coords;

    constexpr Polygon_T()
    {}

    constexpr bool contains( Vector2<T> const& point ) const
    {
        bool inside = false;
        const size_t n = this->coords.size();
        for ( size_t i = 0; i < n; i++ )
        {
            auto const& p1 = coords[i];
            auto const& p2 = coords[( i + 1 ) % n];
            if ( ( p1.y > point.y ) == ( p2.y > point.y ) )
                continue;
            const auto x_intersect = ( p2.x - p1.x ) * ( point.y - p1.y ) / ( p2.y - p1.y ) + p1.x;
            if ( x_intersect < point.x )
                continue;
            inside = !inside;
        }
        return inside;
    }

    template<typename A = T>
    constexpr A area() const
    {
        A area = {};
        const size_t n = coords.size();
        for ( size_t i = 0; i < n; i++ )
        {
            auto const& p1 = coords[i];
            auto const& p2 = coords[( i + 1 ) % n];
            area += A( 0.5 * ( p1.x * p2.y - p2.x * p1.y ) );
        }
        return abs<A>( area );
    }

    template<typename A = T>
    constexpr A spherical_area( A radius ) const
    {
        A sum = {};
        const size_t n = coords.size();
        for ( size_t i = 0; i < n; i++ )
        {
            const auto lon_prev = to_radians<A>() * coords[( i + n - 1 ) % n].y;
            const auto lon_next = to_radians<A>() * coords[( i + 1 ) % n].y;
            sum += A( ( lon_next - lon_prev ) * sin_d<A>( coords[i].x ) );
        }
        return abs<A>( sum ) * radius * radius * A( 0.5 );
    }
};
}

namespace kl
{
using Polygon = Polygon_T<float>;
using PolygonD = Polygon_T<double>;
}

namespace kl
{
template<typename T>
std::ostream& operator<<( std::ostream& stream, Polygon_T<T> const& polygon )
{
    if ( polygon.coords.empty() )
        return stream << "[]";
    stream << "[";
    for ( int i = 0; i < (int) polygon.coords.size() - 1; i++ )
        stream << polygon.coords[i] << ", ";
    stream << polygon.coords.back() << "]";
    return stream;
}
}
