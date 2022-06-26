import kl.window.Window;
import kl.graphics.Color;
import kl.graphics.Image;
import kl.render.*;
import kl.math.*;
import kl.utility.*;

import java.awt.*;
import java.util.Locale;
import java.util.concurrent.atomic.AtomicBoolean;

public class Testing {
    private static final Window window = new Window(new Int2(1600, 900), "Testing");
    private static final Image frameBuffer = new Image(new Int2(window.getSize()));
    private static final Timer timer = new Timer();
    private static final double fpsLimit = 165.0;

    public static void main(String[] args) {
        final Graphics frameBufferGraphics = frameBuffer.getGraphics();

        window.keys.f.press = () -> {
            renderTriangles = !renderTriangles;
        };

        window.start = () -> {
            Time.interval();
            timer.interval();
            timer.reset();
        };

        window.update = () -> {
            final double elapsedT = timer.elapsed();
            final double deltaT = timer.interval();

            frameBuffer.fill(new Color(50, 50, 50));

            draw(frameBufferGraphics);

            window.draw(frameBuffer);

            window.setTitle(String.format(Locale.US, "%.2f", (1.0 / deltaT)));

            final double neededFrameTime = 1.0 / fpsLimit;
            Time.wait(neededFrameTime - (timer.elapsed() - elapsedT));
        };

        window.end = () -> {
            final double elapsed = Time.interval();
            System.out.println("Finished in: " + elapsed + "s");
        };

        window.run();
    }

    private static final Int2 windowCenter = new Int2(window.getSize()).divide(2);
    private static void drawArrow(Graphics graphics, Int2 point, Color color) {
        graphics.setColor(color);
        graphics.drawLine(windowCenter.x, windowCenter.y, point.x, point.y);
    }
    private static void drawLength(Graphics graphics, Int2 vec, Color color, boolean negative) {
        final double length = new Float2(vec.subtract(windowCenter)).length();
        graphics.setColor(color);
        graphics.drawString(String.format(Locale.US, "%.2f", (negative ? -length : length)), vec.x, vec.y);
    }
    private static void drawArc(Graphics graphics, Int2 pos, int radius, Color color,  boolean aroundXAxis) {
        final int posXAngle = (int)new Float2(pos.subtract(windowCenter)).normalize().angle(Float2.getPosX());
        final int negXAngle = (int)new Float2(pos.subtract(windowCenter)).normalize().angle(Float2.getNegX());
        graphics.setColor(color);
        if (aroundXAxis) {
            radius = Math.min(radius, Math.abs(windowCenter.x - pos.x));
            graphics.drawArc(windowCenter.x - radius, windowCenter.y - radius, radius * 2, radius * 2,
                    (((pos.x < windowCenter.x) ? 180 : 0)),
                    ((pos.y < windowCenter.y) ? ((pos.x < windowCenter.x) ? -negXAngle : posXAngle) : ((pos.x < windowCenter.x) ? negXAngle : -posXAngle))
            );
        }
        else {
            radius = Math.min(radius, Math.abs(windowCenter.y - pos.y));
            graphics.drawArc(windowCenter.x - radius, windowCenter.y - radius, radius * 2, radius * 2,
                    ((pos.y < windowCenter.y) ? 90 : 270),
                    ((pos.y < windowCenter.y) ? ((pos.x < windowCenter.x) ? (90 - negXAngle) : (posXAngle - 90)) : (negXAngle - 90))
            );
        }
    }
    private static void drawAngle(Graphics graphics, Int2 vec, int radius, Color color, boolean aroundXAxis) {
        graphics.setColor(color);
        if (aroundXAxis) {
            radius = Math.min(radius, Math.abs(windowCenter.x - vec.x)) - 25;
            final Float2 textVec = new Float2(vec.x, (windowCenter.y + vec.y) * 0.5f).subtract(new Float2(windowCenter)).normalize();
            final Int2 textPos = new Int2(textVec.multiply(radius).add(new Float2(windowCenter)));
            final float vecAngle = ((vec.x < windowCenter.x) ? Float2.getNegX() : Float2.getPosX()).angle(new Float2(vec.subtract(windowCenter)));
            graphics.drawString(String.format(Locale.US, "%.2f°", vecAngle), textPos.x, textPos.y);
        }
        else {
            radius = Math.min(radius, Math.abs(windowCenter.y - vec.y)) - 15;
            final Float2 textVec = new Float2((windowCenter.x + vec.x) * 0.5f, vec.y).subtract(new Float2(windowCenter)).normalize();
            final Int2 textPos = new Int2(textVec.multiply(radius).add(new Float2(windowCenter)));
            final float vecAngle = ((vec.y < windowCenter.y) ? Float2.getNegY() : Float2.getPosY()).angle(new Float2(vec.subtract(windowCenter)));
            graphics.drawString(String.format(Locale.US, "%.2f°", vecAngle), textPos.x, textPos.y);
        }
    }
    private static boolean renderTriangles = false;
    private static void drawTriangle(Image image, Triangle triangle, Color colorA, Color colorB, Color colorC) {
        final Int2 topLeft = new Int2(
            (int)Math.min(Math.min(triangle.a.world.x, triangle.b.world.x), triangle.c.world.x),
            (int)Math.min(Math.min(triangle.a.world.y, triangle.b.world.y), triangle.c.world.y)
        );
        final Int2 bottomRight = new Int2(
            (int)Math.max(Math.max(triangle.a.world.x, triangle.b.world.x), triangle.c.world.x),
            (int)Math.max(Math.max(triangle.a.world.y, triangle.b.world.y), triangle.c.world.y)
        );

        final Float4 triangleConstants = triangle.getConstants();
        final int[] buffer = image.getBuffer();

        for (Int2 point = new Int2(topLeft); point.y <= bottomRight.y; point.y++) {
            for (point.x = topLeft.x; point.x <= bottomRight.x; point.x++) {
                final Float3 pointTriangleWeights = triangle.getWeights(triangleConstants, new Float2(point));
                if (triangle.in(pointTriangleWeights)) {
                    buffer[point.y * image.getWidth() + point.x] = new Color(
                        (int)triangle.interpolate(pointTriangleWeights, new Float3(colorA.getRed(), colorB.getRed(), colorC.getRed())),
                        (int)triangle.interpolate(pointTriangleWeights, new Float3(colorA.getGreen(), colorB.getGreen(), colorC.getGreen())),
                        (int)triangle.interpolate(pointTriangleWeights, new Float3(colorA.getBlue(), colorB.getBlue(), colorC.getBlue()))
                    ).getRGB();
                }
            }
        }
    }
    private static void draw(Graphics graphics) {
        final Color red = new Color(150, 50, 50);
        final Color green = new Color(50, 150, 50);
        final Color blue = new Color(50, 50, 150);
        final Color white = new Color(255, 255, 255);

        final Int2 mousePos = window.mouse.getPosition();

        drawArrow(graphics, new Int2(mousePos.x, windowCenter.y), red);
        drawArrow(graphics, new Int2(windowCenter.x, mousePos.y), green);
        drawArrow(graphics, mousePos, white);

        drawLength(graphics, new Int2(mousePos.x, windowCenter.y), red, mousePos.x < windowCenter.x);
        drawLength(graphics, new Int2(windowCenter.x, mousePos.y), green, windowCenter.y < mousePos.y);
        drawLength(graphics, mousePos, white, false);

        final float lineLen = new Float2(mousePos.subtract(windowCenter)).length();
        int arcRadius = (int)(lineLen * 0.33f);
        arcRadius = Math.max(arcRadius, 50);
        arcRadius = Math.min(arcRadius, (int)lineLen);

        drawArc(graphics, mousePos, arcRadius, red, true);
        drawArc(graphics, mousePos, arcRadius, green, false);

        drawAngle(graphics, mousePos, arcRadius, red, true);
        drawAngle(graphics, mousePos, arcRadius, green, false);

        if (renderTriangles) {
            final Triangle triangle1 = new Triangle(
                new Vertex(new Float3(windowCenter.x, windowCenter.y, 0.5f)),
                new Vertex(new Float3(mousePos.x, windowCenter.y, 0.5f)),
                new Vertex(new Float3(mousePos.x, mousePos.y, 0.5f))
            );
            final Triangle triangle2 = new Triangle(
                new Vertex(new Float3(windowCenter.x, windowCenter.y, 0.5f)),
                new Vertex(new Float3(windowCenter.x, mousePos.y, 0.5f)),
                new Vertex(new Float3(mousePos.x, mousePos.y, 0.5f))
            );
            drawTriangle(frameBuffer, triangle1, blue, red, white);
            drawTriangle(frameBuffer, triangle2, blue, green, white);
        }
    }
}
