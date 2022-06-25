package kl.render;

import kl.math.*;

public class Plane {
    public Float3 normal;
    public Float3 point;

    public Plane() {
        normal = new Float3();
        point = new Float3();
    }
    public Plane(Float3 normal, Float3 point) {
        this.normal = normal;
        this.point = point;
    }
    public Plane(Plane p) {
        normal = new Float3(p.normal);
        point = new Float3(p.point);
    }

    public String toString() {
        return "{" + normal + ", " + point + "}";
    }
}
