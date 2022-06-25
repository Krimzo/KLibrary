package kl.render;

import kl.math.*;

public class Ray {
    public Float3 origin;
    public Float3 direction;

    public Ray() {
        origin = new Float3();
        direction = new Float3();
    }
    public Ray(Float3 origin, Float3 direction) {
        this.origin = origin;
        this.direction = direction.normalize();
    }
    public Ray(Float3 origin, Mat4 invCamMat, Float2 ndc) {
        this.origin = origin;
	    final Float4 pixelDir = invCamMat.multiply(new Float4(ndc, 1.0f, 1.0f));
        direction = pixelDir.divide(pixelDir.w).xyz().normalize();
    }
    public Ray(Camera camera, Float2 ndc) {
        this(camera.position, camera.matrix().inverse(), ndc);
    }
    public Ray(Ray r) {
        origin = new Float3(r.origin);
        direction = new Float3(r.direction);
    }

    public boolean intersect(Plane plane, Float3 outInter) {
	    final float dnDot = direction.dot(plane.normal);
        if (dnDot != 0.0f) {
            if (outInter != null) {
			    outInter = origin.subtract(direction.multiply(origin.subtract(plane.point).dot(plane.normal) / dnDot));
            }
            return true;
        }
        return false;
    }

    public boolean intersect(Triangle triangle, Float3 outInter) {
	    final Float3 edge1 = triangle.b.world.subtract(triangle.a.world);
	    final Float3 edge2 = triangle.c.world.subtract(triangle.a.world);
	    final Float3 h = direction.cross(edge2);
	    final Float3 s = origin.subtract(triangle.a.world);
	    final float f = 1.0f / edge1.dot(h);
	    final float u = s.dot(h) * f;
        if (u < 0.0f || u > 1.0f) {
            return false;
        }

	    final Float3 q = s.cross(edge1);
	    final float v = direction.dot(q) * f;
        if (v < 0.0f || (u + v) > 1.0f) {
            return false;
        }

	    final float t = edge2.dot(q) * f;
        if (t > 0.0f) {
            if (outInter != null) {
			    outInter = origin.add(direction.multiply(t));
            }
            return true;
        }
        return false;
    }

    public boolean intersect(Sphere sphere, Float3 outInter, float outDis) {
	    final Float3 centerRay = sphere.center.subtract(origin);
	    final float cdDot = centerRay.dot(direction);
        if (cdDot < 0.0f) {
            return false;
        }

	    final float ccDot = centerRay.dot(centerRay) - cdDot * cdDot;
	    final float rr = sphere.radius * sphere.radius;
        if (ccDot > rr) {
            return false;
        }

	    final float thc = (float)Math.sqrt(rr - ccDot);
	    final float dis0 = cdDot - thc;
	    final float dis1 = cdDot + thc;
        outInter = origin.add(direction.multiply((dis0 < 0.0f) ? dis1 : dis0));
        outDis = (dis0 < 0.0f) ? dis1 : dis0;
        return true;
    }
    public boolean intersect(Sphere sphere) {
	    final float rayDis = sphere.center.subtract(origin).dot(direction);
	    final Float3 rayPoint = origin.add(direction.multiply(rayDis));
	    final float sphRayDis = sphere.center.subtract(rayPoint).length();
        return !(sphRayDis > sphere.radius);
    }

    public String toString() {
        return "{" + origin + ", " + direction + "}";
    }
}
