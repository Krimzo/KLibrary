package kl.math;

public class MathExt {
    public static float lineX(Float2 a, Float2 b, float y) {
        return ((y - a.y) * (b.x - a.x)) / (b.y - a.y) + a.x;
    }

    public static float lineY(Float2 a, Float2 b, float x) {
        return ((b.y - a.y) * (x - a.x)) / (b.x - a.x) + a.y;
    }

    public static Float4 toQuat(Float3 eul) {
        final float cr = (float)Math.cos(Math.toRadians(eul.x) * 0.5f);
        final float sr = (float)Math.sin(Math.toRadians(eul.x) * 0.5f);
        final float cp = (float)Math.cos(Math.toRadians(eul.y) * 0.5f);
        final float sp = (float)Math.sin(Math.toRadians(eul.y) * 0.5f);
        final float cy = (float)Math.cos(Math.toRadians(eul.z) * 0.5f);
        final float sy = (float)Math.sin(Math.toRadians(eul.z) * 0.5f);
        return new Float4(
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy,
            cr * cp * cy + sr * sp * sy
        );
    }
    public static Float3 toEul(Float4 quat) {
        final float sinp = 2.0f * (quat.w * quat.y - quat.z * quat.x);
        final float sinrCosp = 2.0f * (quat.w * quat.x + quat.y * quat.z);
        final float cosrCosp = 1.0f - 2.0f * (quat.x * quat.x + quat.y * quat.y);
        final float sinyCosp = 2.0f * (quat.w * quat.z + quat.x * quat.y);
        final float cosyCosp = 1.0f - 2.0f * (quat.y * quat.y + quat.z * quat.z);
        return new Float3(
            (float)Math.toDegrees(Math.atan2(sinrCosp, cosrCosp)),
            (float)Math.toDegrees((Math.abs(sinp) >= 1.0) ? Math.copySign(Math.PI * 0.5, sinp) : Math.asin(sinp)),
            (float)Math.toDegrees(Math.atan2(sinyCosp, cosyCosp))
        );
    }

    public static int minmax(int val, int min, int max) {
        return Math.min(Math.max(val, min), max);
    }
    public static long minmax(long val, long min, long max) {
        return Math.min(Math.max(val, min), max);
    }
    public static float minmax(float val, float min, float max) {
        return Math.min(Math.max(val, min), max);
    }
    public static double minmax(double val, double min, double max) {
        return Math.min(Math.max(val, min), max);
    }
}
