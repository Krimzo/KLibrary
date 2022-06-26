package kl.graphics;

import java.awt.*;
import java.awt.image.*;
import javax.imageio.*;
import java.io.*;
import java.util.Arrays;

import kl.math.*;

public class Image extends BufferedImage {
    public static final int storageType = BufferedImage.TYPE_INT_ARGB;

    public Image() {
        super(0, 0, storageType);
    }
    public Image(Int2 size) {
        super(size.x, size.y, storageType);
    }
    public Image(Int2 size, Color color) {
        super(size.x, size.y, storageType);
        Arrays.fill(getBuffer(), color.getRGB());
    }
    public Image(BufferedImage image) {
        super(image.getWidth(), image.getHeight(), storageType);
        final Graphics graphics = getGraphics();
        graphics.drawImage(image, 0, 0, null);
        graphics.dispose();
    }
    public Image(String filePath) throws IOException {
        this(ImageIO.read(new File(filePath)));
    }

    public Int2 getSize() {
        return new Int2(getWidth(), getHeight());
    }

    public int[] getBuffer() {
        return ((DataBufferInt)getRaster().getDataBuffer()).getData();
    }

    public boolean setPixel(Int2 pos, Color color) {
        if (pos.x >= 0 && pos.x < getWidth() && pos.y >= 0 && pos.y < getHeight()) {
            getBuffer()[pos.y * getWidth() + pos.x] = color.getRGB();
            return true;
        }
        return false;
    }
    public Color getPixel(Int2 pos) {
        if (pos.x >= 0 && pos.x < getWidth() && pos.y >= 0 && pos.y < getHeight()) {
            return new Color(getBuffer()[pos.y * getWidth() + pos.x]);
        }
        return new Color();
    }

    public void fill(Color color) {
        final int colorValue = color.getRGB();
        final int[] colorBuffer = getBuffer();
        Arrays.fill(colorBuffer, colorValue);
    }

    public Image flipHorizontal() {
        final Image temp = new Image(getSize());
        final int[] readBuffer = getBuffer();
        final int[] writeBuffer = temp.getBuffer();
        for (int y = 0; y < getHeight(); y++) {
            for (int x = 0; x < getWidth(); x++) {
                writeBuffer[y * getWidth() + x] = readBuffer[y * getWidth() + getWidth() - 1 - x];
            }
        }
        return temp;
    }
    public Image flipVertical() {
        final Image temp = new Image(getSize());
        final int[] readBuffer = getBuffer();
        final int[] writeBuffer = temp.getBuffer();
        for (int x = 0; x < getWidth(); x++) {
            for (int y = 0; y < getHeight(); y++) {
                writeBuffer[y * getWidth() + x] = readBuffer[(getHeight() - 1 - y) * getWidth() + x];
            }
        }
        return temp;
    }

    public Image getRectangle(Int2 a, Int2 b) {
        a = new Int2(a);
        b = new Int2(b);
        if (b.x < a.x) {
            final int temp = a.x;
            a.x = b.x;
            b.x = temp;
        }
        if (b.y < a.y) {
            final int temp = a.y;
            a.y = b.y;
            b.y = temp;
        }
        Image temp = new Image(b.subtract(a));
        for (Int2 pos = new Int2(); pos.y < temp.getHeight(); pos.y++) {
            for (pos.x = 0; pos.x < temp.getWidth(); pos.x++) {
                temp.setPixel(pos, getPixel(pos.add(a)));
            }
        }
        return temp;
    }

    public void toFile(String filePath) throws IOException {
        String extension = "";
        final int periodIndex = filePath.lastIndexOf('.');
        if (periodIndex > 0) {
            extension = filePath.substring(periodIndex + 1);
        }
        if (!ImageIO.write(this, extension, new File(filePath))) {
            throw new IOException("Failed to find appropriate image writer for format: " + extension);
        }
    }

    public String toASCII(Int2 frameSize) {
        final Int2 increment = getSize().divide(frameSize);
        final StringBuilder frame = new StringBuilder();
        for (Int2 pos = new Int2(); pos.y < frameSize.y; pos.y++) {
            for (pos.x = 0; pos.x < frameSize.x; pos.x++) {
                frame.append(getPixel(pos.multiply(increment)).ascii());
            }
        }
        return frame.toString();
    }
}
