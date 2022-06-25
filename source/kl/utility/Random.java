package kl.utility;

import kl.graphics.Color;
import kl.math.*;

public class Random extends java.util.Random {
    public Random() {}

    public char nextChar(boolean upper) {
        if (upper) {
            return (char)nextInt(65, 91);
        }
        return (char)nextInt(97, 123);
    }
    public String nextString(int len) {
        final StringBuilder builder = new StringBuilder();
        for (int i = 0; i < len; i++) {
            builder.append(nextChar(nextBoolean()));
        }
        return builder.toString();
    }

    public Color nextColorGray() {
        final int grayVal = nextInt(256);
        return new Color(grayVal, grayVal, grayVal, 255);
    }
    public Color nextColorRGB() {
        return new Color(nextInt(256), nextInt(256), nextInt(256), 255);
    }
    public Color nextColorRGBA() {
        return new Color(nextInt(256), nextInt(256), nextInt(256), nextInt(256));
    }

    public Int2 nextInt2(int endExclusive) {
        return new Int2(nextInt(endExclusive), nextInt(endExclusive));
    }
    public Int2 nextInt2(int startInclusive, int endExclusive) {
        return new Int2(nextInt(startInclusive, endExclusive), nextInt(startInclusive, endExclusive));
    }
    public Int3 nextInt3(int endExclusive) {
        return new Int3(nextInt(endExclusive), nextInt(endExclusive), nextInt(endExclusive));
    }
    public Int3 nextInt3(int startInclusive, int endExclusive) {
        return new Int3(nextInt(startInclusive, endExclusive), nextInt(startInclusive, endExclusive), nextInt(startInclusive, endExclusive));
    }
    public Int4 nextInt4(int endExclusive) {
        return new Int4(nextInt(endExclusive), nextInt(endExclusive), nextInt(endExclusive), nextInt(endExclusive));
    }
    public Int4 nextInt4(int startInclusive, int endExclusive) {
        return new Int4(nextInt(startInclusive, endExclusive), nextInt(startInclusive, endExclusive), nextInt(startInclusive, endExclusive), nextInt(startInclusive, endExclusive));
    }

    public Float2 nextFloat2() {
        return new Float2(nextFloat(), nextFloat());
    }
    public Float3 nextFloat3() {
        return new Float3(nextFloat(), nextFloat(), nextFloat());
    }
    public Float4 nextFloat4() {
        return new Float4(nextFloat(), nextFloat(), nextFloat(), nextFloat());
    }
}
