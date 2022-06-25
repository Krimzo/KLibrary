package kl.window;

import kl.graphics.*;
import kl.math.*;

public class DoubleBuffer {
    private final Image buffer1;
    private final Image buffer2;
    private Image frontBuffer;

    public DoubleBuffer(Int2 size) {
        buffer1 = new Image(size);
        buffer2 = new Image(size);
        frontBuffer = buffer1;
    }

    public Int2 getSize() {
        return buffer1.getSize();
    }

    public Image getBackBuffer() {
        return (frontBuffer == buffer1) ? buffer2 : buffer1;
    }

    public void draw(Window window) {
        synchronized (this) {
            window.draw(frontBuffer);
        }
    }

    public void swap() {
        synchronized (this) {
            frontBuffer = getBackBuffer();
        }
    }
}
