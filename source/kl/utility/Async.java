package kl.utility;

public abstract class Async {
    public static final int numberOfCores = Runtime.getRuntime().availableProcessors();

    public interface LoopCallback {
        void method(int t, long i);
    }

    public static void loop(long startInclusive, long endExclusive, LoopCallback callback) throws InterruptedException {
        final Thread[] workers = new Thread[numberOfCores];

	    final long runsPerThread = (endExclusive - startInclusive) / numberOfCores;
        for (int t = 0; t < numberOfCores; t++) {
		    final long loopStart = runsPerThread * t + startInclusive;
            final long loopEnd = (t == numberOfCores - 1) ? endExclusive : (loopStart + runsPerThread);
            final int finalT = t;
            workers[t] = new Thread(() -> {
                for (long i = loopStart; i < loopEnd; i++) {
                    callback.method(finalT, i);
                }
            });
            workers[t].start();
        }

        for (Thread worker : workers) {
            worker.join();
        }
    }
}
