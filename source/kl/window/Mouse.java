package kl.window;

import kl.math.*;

import java.awt.*;
import java.awt.event.*;

public class Mouse {
    public final Key lmb = new Key();
    public final Key mmb = new Key();
    public final Key rmb = new Key();
    private final Int2 position = new Int2();
    private int scroll = 0;

    public Mouse() {}

    public Int2 getPosition() {
        return new Int2(position);
    }
    public int getScroll() {
        return scroll;
    }

    protected MouseListener getListener() {
        return new MouseListener() {
            public void mouseClicked(MouseEvent e) {}
            public void mousePressed(MouseEvent e) {
                switch (e.getButton()) {
                    case MouseEvent.BUTTON1 -> lmb.update(true);
                    case MouseEvent.BUTTON2 -> rmb.update(true);
                    case MouseEvent.BUTTON3 -> mmb.update(true);
                }
            }
            public void mouseReleased(MouseEvent e) {
                switch (e.getButton()) {
                    case MouseEvent.BUTTON1 -> lmb.update(false);
                    case MouseEvent.BUTTON2 -> rmb.update(false);
                    case MouseEvent.BUTTON3 -> mmb.update(false);
                }
            }
            public void mouseEntered(MouseEvent e) {}
            public void mouseExited(MouseEvent e) {}
        };
    }
    protected MouseMotionListener getMotionListener() {
        return new MouseMotionListener() {
            public void mouseDragged(MouseEvent e) {}
            public void mouseMoved(MouseEvent e) {
                final Point pos = e.getPoint();
                position.x = pos.x;
                position.y = pos.y;
            }
        };
    }
    protected MouseWheelListener getWheelListener() {
        return e -> scroll += e.getScrollAmount();
    }

    protected void update() {
        lmb.update();
        mmb.update();
        rmb.update();
    }
}
