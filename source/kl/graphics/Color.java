package kl.graphics;

import kl.math.*;

public class Color extends java.awt.Color {
    public Color() {
        super(0, 0, 0, 255);
    }
    public Color(int rgba) {
        super(rgba, true);
    }
    public Color(int r, int g, int b) {
        super(r, g, b, 255);
    }
    public Color(int r, int g, int b, int a) {
        super(r, g, b, a);
    }
    public Color(float r, float g, float b) {
        super(MathExt.minmax(r, 0.0f, 1.0f),
                MathExt.minmax(g, 0.0f, 1.0f),
                MathExt.minmax(b, 0.0f, 1.0f),
                1.0f);
    }
    public Color(float r, float g, float b, float a) {
        super(MathExt.minmax(r, 0.0f, 1.0f),
                MathExt.minmax(g, 0.0f, 1.0f),
                MathExt.minmax(b, 0.0f, 1.0f),
                MathExt.minmax(a, 0.0f, 1.0f));
    }
    public Color (Int3 values) {
        this(values.x, values.y, values.z);
    }
    public Color (Int4 values) {
        this(values.x, values.y, values.z, values.w);
    }
    public Color (Float3 values) {
        this(values.x, values.y, values.z);
    }
    public Color (Float4 values) {
        this(values.x, values.y, values.z, values.w);
    }

    public boolean equals(Color color) {
        return getRed() == color.getRed() &&
                getBlue() == color.getBlue() &&
                getGreen() == color.getGreen() &&
                getAlpha() == color.getAlpha();
    }

    public Color gray() {
        final int grayValue = (int)(getRed() * 0.3 + getGreen() * 0.585 + getBlue() * 0.115);
        return new Color(grayValue, grayValue, grayValue, getAlpha());
    }

    public Color invert() {
        return new Color(255 - getRed(), 255 - getGreen(), 255 - getBlue(), getAlpha());
    }

    private static final double tableGetter = 9.0 / 255.0;
    private final char[] asciiTable = { '@', '%', '#', 'x', '+', '=', ':', '-', '.', ' ' };
    public char ascii() {
        return asciiTable[(int)(gray().getRed() * tableGetter)];
    }

    private static final double alphaMix = 1.0 / 255.0;
    public Color mix(Color color) {
        return mix(color, color.getAlpha() * alphaMix);
    }
    public Color mix(Color color, double ratio) {
        ratio = Math.min(Math.max(ratio, 0.0), 1.0);
        final double iratio = 1.0 - ratio;
        return new Color(
                (int)(getRed() * iratio) + (int)(color.getRed() * ratio),
                (int)(getGreen() * iratio) + (int)(color.getGreen() *  ratio),
                (int)(getBlue() * iratio) + (int)(color.getBlue() * ratio)
	    );
    }

    public String toString() {
        return "[" + getRed() + ", " + getGreen() + ", " + getBlue() + ", " + getAlpha() + "]";
    }
}
