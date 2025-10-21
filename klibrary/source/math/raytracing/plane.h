#pragma once

#include "math/vector/vector4.h"


namespace kl
{
template<typename T>
struct Plane_T
{
    union { Vector3<T> position; struct { T x, y, z; }; };

    constexpr Plane_T()
    {
    }

    constexpr Plane_T( Vector3<T> const& position, Vector3<T> const& normal )
        : position( position )
    {
        set_normal( normal );
    }

    constexpr void set_normal( Vector3<T> const& normal )
    {
        m_normal = normalize( normal );
    }

    constexpr Vector3<T> normal() const
    {
        return m_normal;
    }

    constexpr bool in_front( Vector3<T> const& point ) const
    {
        T result = dot( point - position, m_normal );
        return result >= T( 0 );
    }

private:
    Vector3<T> m_normal = { T( 0 ), T( 1 ), T( 0 ) };
};
}

namespace kl
{
using Plane = Plane_T<float>;
using PlaneD = Plane_T<double>;
}

namespace kl
{
template<typename T>
std::ostream& operator<<( std::ostream& stream, Plane_T<T> const& plane )
{
    stream << "{" << plane.position << ", " << plane.normal() << "}";
    return stream;
}
}
