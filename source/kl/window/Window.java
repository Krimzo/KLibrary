package kl.window;

import kl.graphics.Color;
import kl.graphics.Image;
import kl.math.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.concurrent.atomic.AtomicBoolean;

public class Window extends JFrame implements Runnable {
    public static Int2 getScreenSize() {
        final Dimension size = Toolkit.getDefaultToolkit().getScreenSize();
        return new Int2(size.width, size.height);
    }
    public static Int2 getScreenCenter() {
        return getScreenSize().divide(2);
    }

    public Window(Int2 size, String name) {
        this(size, name, true);
    }
    public Window(Int2 size, String name, boolean resizeable) {
        addWindowListener(new WindowListener() {
            public void windowOpened(WindowEvent e) {}
            public void windowClosing(WindowEvent e) {
                stop(); while (!finishedRunning.get()); dispose();
            }
            public void windowClosed(WindowEvent e) {}
            public void windowIconified(WindowEvent e) {}
            public void windowDeiconified(WindowEvent e) {}
            public void windowActivated(WindowEvent e) {}
            public void windowDeactivated(WindowEvent e) {}
        });
        setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        setLocation(getScreenSize().subtract(size).divide(2));
        setResizable(resizeable);
        setPreferredSize(size);
        setSize(size);
        setTitle(name);
        setVisible(true);
        pack();

        addKeyListener(keys.getListener());
        addMouseListener(mouse.getListener());
        addMouseMotionListener(mouse.getMotionListener());
        addMouseWheelListener(mouse.getWheelListener());

        graphics = getGraphics();
    }

    public void setLocation(Int2 location) {
        setLocation(location.x, location.y);
    }
    public void setPreferredSize(Int2 size) {
        setPreferredSize(new Dimension(size.x, size.y));
    }
    public void setSize(Int2 size) {
        setSize(size.x, size.y);
    }

    public final Mouse mouse = new Mouse();
    public final Keys keys = new Keys();

    public interface StageCallback { void method(); }
    public StageCallback start = () -> {};
    public StageCallback update = () -> {};
    public StageCallback end = () -> {};
    private final AtomicBoolean shouldRun = new AtomicBoolean(false);
    private final AtomicBoolean finishedRunning = new AtomicBoolean(true);
    public void run() {
        finishedRunning.set(false);
        shouldRun.set(true);
        start.method();
        while (shouldRun.get()) {
            mouse.update();
            keys.update();
            update.method();
        }
        end.method();
        finishedRunning.set(true);
    }
    public void stop() {
        shouldRun.set(false);
    }

    public interface DrawCallback { void method(Graphics graphics); }
    private final Graphics graphics;
    public void draw(DrawCallback callback) {
        callback.method(graphics);
    }
    public void draw(Image image) {
        graphics.drawImage(image, 0, 0, null);
    }
    public void clear(Color color) {
        graphics.setColor(color);
        graphics.fillRect(0, 0, getWidth(), getHeight());
    }
}
