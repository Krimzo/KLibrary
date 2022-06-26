package kl.window;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

public class Keys {
    public Keys() {}

    protected KeyListener getListener() {
        return new KeyListener() {
            public void keyTyped(KeyEvent e) {}
            public void keyPressed(KeyEvent event) {
                switch (event.getKeyCode()) {
                    case KeyEvent.VK_Q -> q.update(true);
                    case KeyEvent.VK_W -> w.update(true);
                    case KeyEvent.VK_E -> e.update(true);
                    case KeyEvent.VK_R -> r.update(true);
                    case KeyEvent.VK_T -> t.update(true);
                    case KeyEvent.VK_Z -> z.update(true);
                    case KeyEvent.VK_U -> u.update(true);
                    case KeyEvent.VK_I -> i.update(true);
                    case KeyEvent.VK_O -> o.update(true);
                    case KeyEvent.VK_P -> p.update(true);
                    case KeyEvent.VK_A -> a.update(true);
                    case KeyEvent.VK_S -> s.update(true);
                    case KeyEvent.VK_D -> d.update(true);
                    case KeyEvent.VK_F -> f.update(true);
                    case KeyEvent.VK_G -> g.update(true);
                    case KeyEvent.VK_H -> h.update(true);
                    case KeyEvent.VK_J -> j.update(true);
                    case KeyEvent.VK_K -> k.update(true);
                    case KeyEvent.VK_L -> l.update(true);
                    case KeyEvent.VK_Y -> y.update(true);
                    case KeyEvent.VK_X -> x.update(true);
                    case KeyEvent.VK_C -> c.update(true);
                    case KeyEvent.VK_V -> v.update(true);
                    case KeyEvent.VK_B -> b.update(true);
                    case KeyEvent.VK_N -> n.update(true);
                    case KeyEvent.VK_M -> m.update(true);
                    case KeyEvent.VK_0 -> num0.update(true);
                    case KeyEvent.VK_1 -> num1.update(true);
                    case KeyEvent.VK_2 -> num2.update(true);
                    case KeyEvent.VK_3 -> num3.update(true);
                    case KeyEvent.VK_4 -> num4.update(true);
                    case KeyEvent.VK_5 -> num5.update(true);
                    case KeyEvent.VK_6 -> num6.update(true);
                    case KeyEvent.VK_7 -> num7.update(true);
                    case KeyEvent.VK_8 -> num8.update(true);
                    case KeyEvent.VK_9 -> num9.update(true);
                    case KeyEvent.VK_PERIOD -> period.update(true);
                    case KeyEvent.VK_COMMA -> comma.update(true);
                    case KeyEvent.VK_PLUS -> plus.update(true);
                    case KeyEvent.VK_MINUS -> minus.update(true);
                    case KeyEvent.VK_ESCAPE -> esc.update(true);
                    case KeyEvent.VK_TAB -> tab.update(true);
                    case KeyEvent.VK_CAPS_LOCK -> caps.update(true);
                    case KeyEvent.VK_SHIFT -> shift.update(true);
                    case KeyEvent.VK_CONTROL -> ctrl.update(true);
                    case KeyEvent.VK_ALT -> alt.update(true);
                    case KeyEvent.VK_SPACE -> space.update(true);
                    case KeyEvent.VK_ENTER -> enter.update(true);
                    case KeyEvent.VK_INSERT -> insert.update(true);
                    case KeyEvent.VK_DELETE -> delete.update(true);
                    case KeyEvent.VK_UP -> up.update(true);
                    case KeyEvent.VK_LEFT -> left.update(true);
                    case KeyEvent.VK_DOWN -> down.update(true);
                    case KeyEvent.VK_RIGHT -> right.update(true);
                    case KeyEvent.VK_F1 -> f1.update(true);
                    case KeyEvent.VK_F2 -> f2.update(true);
                    case KeyEvent.VK_F3 -> f3.update(true);
                    case KeyEvent.VK_F4 -> f4.update(true);
                    case KeyEvent.VK_F5 -> f5.update(true);
                    case KeyEvent.VK_F6 -> f6.update(true);
                    case KeyEvent.VK_F7 -> f7.update(true);
                    case KeyEvent.VK_F8 -> f8.update(true);
                    case KeyEvent.VK_F9 -> f9.update(true);
                    case KeyEvent.VK_F10 -> f10.update(true);
                    case KeyEvent.VK_F11 -> f11.update(true);
                    case KeyEvent.VK_F12 -> f12.update(true);
                }
            }
            public void keyReleased(KeyEvent event) {
                switch (event.getKeyCode()) {
                    case KeyEvent.VK_Q -> q.update(false);
                    case KeyEvent.VK_W -> w.update(false);
                    case KeyEvent.VK_E -> e.update(false);
                    case KeyEvent.VK_R -> r.update(false);
                    case KeyEvent.VK_T -> t.update(false);
                    case KeyEvent.VK_Z -> z.update(false);
                    case KeyEvent.VK_U -> u.update(false);
                    case KeyEvent.VK_I -> i.update(false);
                    case KeyEvent.VK_O -> o.update(false);
                    case KeyEvent.VK_P -> p.update(false);
                    case KeyEvent.VK_A -> a.update(false);
                    case KeyEvent.VK_S -> s.update(false);
                    case KeyEvent.VK_D -> d.update(false);
                    case KeyEvent.VK_F -> f.update(false);
                    case KeyEvent.VK_G -> g.update(false);
                    case KeyEvent.VK_H -> h.update(false);
                    case KeyEvent.VK_J -> j.update(false);
                    case KeyEvent.VK_K -> k.update(false);
                    case KeyEvent.VK_L -> l.update(false);
                    case KeyEvent.VK_Y -> y.update(false);
                    case KeyEvent.VK_X -> x.update(false);
                    case KeyEvent.VK_C -> c.update(false);
                    case KeyEvent.VK_V -> v.update(false);
                    case KeyEvent.VK_B -> b.update(false);
                    case KeyEvent.VK_N -> n.update(false);
                    case KeyEvent.VK_M -> m.update(false);
                    case KeyEvent.VK_0 -> num0.update(false);
                    case KeyEvent.VK_1 -> num1.update(false);
                    case KeyEvent.VK_2 -> num2.update(false);
                    case KeyEvent.VK_3 -> num3.update(false);
                    case KeyEvent.VK_4 -> num4.update(false);
                    case KeyEvent.VK_5 -> num5.update(false);
                    case KeyEvent.VK_6 -> num6.update(false);
                    case KeyEvent.VK_7 -> num7.update(false);
                    case KeyEvent.VK_8 -> num8.update(false);
                    case KeyEvent.VK_9 -> num9.update(false);
                    case KeyEvent.VK_PERIOD -> period.update(false);
                    case KeyEvent.VK_COMMA -> comma.update(false);
                    case KeyEvent.VK_PLUS -> plus.update(false);
                    case KeyEvent.VK_MINUS -> minus.update(false);
                    case KeyEvent.VK_ESCAPE -> esc.update(false);
                    case KeyEvent.VK_TAB -> tab.update(false);
                    case KeyEvent.VK_CAPS_LOCK -> caps.update(false);
                    case KeyEvent.VK_SHIFT -> shift.update(false);
                    case KeyEvent.VK_CONTROL -> ctrl.update(false);
                    case KeyEvent.VK_ALT -> alt.update(false);
                    case KeyEvent.VK_SPACE -> space.update(false);
                    case KeyEvent.VK_ENTER -> enter.update(false);
                    case KeyEvent.VK_INSERT -> insert.update(false);
                    case KeyEvent.VK_DELETE -> delete.update(false);
                    case KeyEvent.VK_UP -> up.update(false);
                    case KeyEvent.VK_LEFT -> left.update(false);
                    case KeyEvent.VK_DOWN -> down.update(false);
                    case KeyEvent.VK_RIGHT -> right.update(false);
                    case KeyEvent.VK_F1 -> f1.update(false);
                    case KeyEvent.VK_F2 -> f2.update(false);
                    case KeyEvent.VK_F3 -> f3.update(false);
                    case KeyEvent.VK_F4 -> f4.update(false);
                    case KeyEvent.VK_F5 -> f5.update(false);
                    case KeyEvent.VK_F6 -> f6.update(false);
                    case KeyEvent.VK_F7 -> f7.update(false);
                    case KeyEvent.VK_F8 -> f8.update(false);
                    case KeyEvent.VK_F9 -> f9.update(false);
                    case KeyEvent.VK_F10 -> f10.update(false);
                    case KeyEvent.VK_F11 -> f11.update(false);
                    case KeyEvent.VK_F12 -> f12.update(false);
                }
            }
        };
    }

    protected void update() {
        q.update();
        w.update();
        e.update();
        r.update();
        t.update();
        z.update();
        u.update();
        i.update();
        o.update();
        p.update();
        a.update();
        s.update();
        d.update();
        f.update();
        g.update();
        h.update();
        j.update();
        k.update();
        l.update();
        y.update();
        x.update();
        c.update();
        v.update();
        b.update();
        n.update();
        m.update();
        num0.update();
        num1.update();
        num2.update();
        num3.update();
        num4.update();
        num5.update();
        num6.update();
        num7.update();
        num8.update();
        num9.update();
        period.update();
        comma.update();
        plus.update();
        minus.update();
        esc.update();
        tab.update();
        caps.update();
        shift.update();
        ctrl.update();
        alt.update();
        space.update();
        enter.update();
        insert.update();
        delete.update();
        up.update();
        left.update();
        down.update();
        right.update();
        f1.update();
        f2.update();
        f3.update();
        f4.update();
        f5.update();
        f6.update();
        f7.update();
        f8.update();
        f9.update();
        f10.update();
        f11.update();
        f12.update();
    }

    // Letters
    public final Key q = new Key();
    public final Key w = new Key();
    public final Key e = new Key();
    public final Key r = new Key();
    public final Key t = new Key();
    public final Key z = new Key();
    public final Key u = new Key();
    public final Key i = new Key();
    public final Key o = new Key();
    public final Key p = new Key();
    public final Key a = new Key();
    public final Key s = new Key();
    public final Key d = new Key();
    public final Key f = new Key();
    public final Key g = new Key();
    public final Key h = new Key();
    public final Key j = new Key();
    public final Key k = new Key();
    public final Key l = new Key();
    public final Key y = new Key();
    public final Key x = new Key();
    public final Key c = new Key();
    public final Key v = new Key();
    public final Key b = new Key();
    public final Key n = new Key();
    public final Key m = new Key();

    // Numbers
    public final Key num0 = new Key();
    public final Key num1 = new Key();
    public final Key num2 = new Key();
    public final Key num3 = new Key();
    public final Key num4 = new Key();
    public final Key num5 = new Key();
    public final Key num6 = new Key();
    public final Key num7 = new Key();
    public final Key num8 = new Key();
    public final Key num9 = new Key();

    // Signs
    public final Key period = new Key();
    public final Key comma = new Key();
    public final Key plus = new Key();
    public final Key minus = new Key();

    // Special
    public final Key esc = new Key();
    public final Key tab = new Key();
    public final Key caps = new Key();
    public final Key shift = new Key();
    public final Key ctrl = new Key();
    public final Key alt = new Key();
    public final Key space = new Key();
    public final Key enter = new Key();
    public final Key insert = new Key();
    public final Key delete = new Key();
    public final Key up = new Key();
    public final Key left = new Key();
    public final Key down = new Key();
    public final Key right = new Key();

    // Function
    public final Key f1 = new Key();
    public final Key f2 = new Key();
    public final Key f3 = new Key();
    public final Key f4 = new Key();
    public final Key f5 = new Key();
    public final Key f6 = new Key();
    public final Key f7 = new Key();
    public final Key f8 = new Key();
    public final Key f9 = new Key();
    public final Key f10 = new Key();
    public final Key f11 = new Key();
    public final Key f12 = new Key();
}
