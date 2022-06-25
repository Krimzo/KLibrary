import kl.graphics.*;
import kl.math.*;
import kl.utility.*;


public class Testing {
    public static void main(String[] args) throws Exception {
        Time.interval();

        Async.loop(0, 10, (int t, long i) -> {
            System.out.println(t + " => " + i);
        });

        final double elapsed = Time.interval();
        System.out.println(elapsed);
    }
}
