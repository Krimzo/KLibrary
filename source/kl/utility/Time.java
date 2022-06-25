package kl.utility;

public abstract class Time {
    public static long get() {
        return System.nanoTime();
    }
    public static double calculate(long start, long end) {
        return (end - start) * 1e-9;
    }

    private static long intervalStart = Time.get();
    public static double interval() {
        final long intervalEndTime = Time.get();
        final double elapsedTime = Time.calculate(intervalStart, intervalEndTime);
        intervalStart = intervalEndTime;
        return elapsedTime;
    }

    public static void wait(double seconds) {
        final long startTime = Time.get();
        long endTime = Time.get();
        while (Time.calculate(startTime, endTime) < seconds) {
            endTime = Time.get();
        }
    }
}
