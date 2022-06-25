package kl.render;

import kl.math.*;

public class Camera {
    private static final Float3 globalUP = Float3.getPosY();

    private Float3 forward = Float3.getPosZ();
    private Float3 right = Float3.getPosX();
    private Float3 up = Float3.getPosY();

    public Float3 position = new Float3();
    public float aspect = 1.7778f;
    public float fov = 75.0f;
    public float near = 0.01f;
    public float far = 500.0f;
    public float speed = 2.0f;
    public float sens = 0.1f;

    public void forward(Float3 direction) {
        forward = direction.normalize();
        right = globalUP.cross(forward).normalize();
        up = forward.cross(right).normalize();
    }
    public Float3 forward() {
        return new Float3(forward);
    }
    public Float3 right() {
        return new Float3(right);
    }
    public Float3 up() {
        return new Float3(up);
    }

    public void moveForward(float deltaTime) {
        position = position.add(forward.multiply(speed * deltaTime));
    }
    public void moveBack(float deltaTime) {
        position = position.subtract(forward.multiply(speed * deltaTime));
    }
    public void moveRight(float deltaTime) {
        position = position.add(right.multiply(speed * deltaTime));
    }
    public void moveLeft(float deltaTime) {
        position = position.subtract(right.multiply(speed * deltaTime));
    }
    public void moveUp(float deltaTime) {
        position = position.add(globalUP.multiply(speed * deltaTime));
    }
    public void moveDown(float deltaTime) {
        position = position.subtract(globalUP.multiply(speed * deltaTime));
    }

    public void rotate(Float2 mousePos, Float2 frameCenter, float verticalAngleLimit) {
	    final Float2 rotation = mousePos.subtract(frameCenter).multiply(sens);
	    final Float3 forwardVert = forward.rotate(rotation.y, right);
        if (Math.abs(forwardVert.angle(globalUP) - 90.0f) <= verticalAngleLimit) {
            forward(forwardVert);
        }
        forward(forward.rotate(rotation.x, globalUP));
    }

    public Mat4 matrix() {
	    final Mat4 view = Mat4.lookAt(position, position.add(forward), globalUP);
	    final Mat4 proj = Mat4.perspective(fov, aspect, near, far);
        return proj.multiply(view);
    }
}
