package kl.window;

public class Key {
    public interface KeyCallback { void method(); }
    public KeyCallback press = () -> {};
    public KeyCallback down = () -> {};
    public KeyCallback release = () -> {};

    private boolean state = false;

    public Key() {}

    public boolean isDown() {
        return state;
    }

    protected void update(boolean newState) {
        if (!state && newState) {
            press.method();
        }
        else if (state && !newState) {
            release.method();
        }
        state = newState;
    }
    protected void update() {
        if (state) {
            down.method();
        }
    }
}
