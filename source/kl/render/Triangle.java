package kl.render;

import kl.math.*;

public class Triangle {
    public Vertex a, b, c;

    public Triangle() {
        a = new Vertex();
        b = new Vertex();
        c = new Vertex();
    }
    public Triangle(Vertex a, Vertex b, Vertex c) {
        this.a = a;
        this.b = b;
        this.c = c;
    }
    public Triangle(Triangle t) {
        a = new Vertex(t.a);
        b = new Vertex(t.b);
        c = new Vertex(t.c);
    }

    public Float4 getConstants() {
	    final float tempConst = 1.0f / ((b.world.y - c.world.y) * (a.world.x - c.world.x) + (c.world.x - b.world.x) * (a.world.y - c.world.y));
        return new Float4(
            (b.world.y - c.world.y) * tempConst,
            (c.world.x - b.world.x) * tempConst,
            (c.world.y - a.world.y) * tempConst,
            (a.world.x - c.world.x) * tempConst
        );
    }
    public Float3 getWeights(Float4 constants, Float2 position) {
        final float dx = position.x - c.world.x;
        final float dy = position.y - c.world.y;
        final float w1 = dx * constants.x + dy * constants.y;
        final float w2 = dx * constants.z + dy * constants.w;
        return new Float3(w1, w2, 1.0f - w1 - w2);
    }
    public Float3 getWeights(Float3 point) {
        final Float3 v0 = new Float3(a.world.subtract(c.world));
        final Float3 v1 = new Float3(b.world.subtract(c.world));
        final Float3 v2 = new Float3(point.subtract(c.world));
        final float d00 = v0.dot(v0);
        final float d01 = v0.dot(v1);
        final float d11 = v1.dot(v1);
        final float d20 = v2.dot(v0);
        final float d21 = v2.dot(v1);
        final float invDenom = 1.0f / (d00 * d11 - d01 * d01);
        final float w1 = (d11 * d20 - d01 * d21) * invDenom;
        final float w2 = (d00 * d21 - d01 * d20) * invDenom;
        return new Float3(w1, w2, 1.0f - w1 - w2);
    }

    public boolean in(Float3 weights) {
        return !(weights.x < 0.0f || weights.y < 0.0f || weights.z < 0.0f);
    }

    public float interpolate(Float3 weights, Float3 values) {
        return weights.dot(values);
    }
    public Vertex interpolate(Float3 weights) {
        return new Vertex(
            new Float3(
                interpolate(weights, new Float3(a.world.x, b.world.x, c.world.x)),
                interpolate(weights, new Float3(a.world.y, b.world.y, c.world.y)),
                interpolate(weights, new Float3(a.world.z, b.world.z, c.world.z))
            ),
            new Float2(
                interpolate(weights, new Float3(a.texture.x, b.texture.x, c.texture.x)),
                interpolate(weights, new Float3(a.texture.y, b.texture.y, c.texture.y))
            ),
            new Float3(
                    interpolate(weights, new Float3(a.normal.x, b.normal.x, c.normal.x)),
                    interpolate(weights, new Float3(a.normal.y, b.normal.y, c.normal.y)),
                    interpolate(weights, new Float3(a.normal.z, b.normal.z, c.normal.z))
            )
        );
    }

    public String toString() {
        return "{" + a + ", " + b + ", " + c + "}";
    }
}
