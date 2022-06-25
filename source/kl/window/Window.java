package kl.window;

import kl.graphics.Image;

import javax.swing.*;
import java.awt.*;

public class Window extends JFrame {
    private final Graphics graphics;

    public Window() {
        graphics = getGraphics();
    }

    public void draw(Image image) {
        graphics.drawImage(image, 0, 0, null);
    }
}
