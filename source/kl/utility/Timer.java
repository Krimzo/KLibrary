package kl.utility;

public class Timer {
    private long intervalStart;
    private long stopwatchStart;

    public Timer() {
        interval();
        reset();
    }

    public double interval() {
	    final long endTime = Time.get();
	    final double elapsedTime = Time.calculate(intervalStart, endTime);
        intervalStart = endTime;
        return elapsedTime;
    }

    public void reset() {
        stopwatchStart = Time.get();
    }
    public double elapsed() {
        return Time.calculate(stopwatchStart, Time.get());
    }
}
