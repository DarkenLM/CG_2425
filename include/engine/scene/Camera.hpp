#pragma once

#include <limits>

#include "common/util/xmlutil.hpp"
#include "engine/scene/Object.hpp"
using namespace tinyxml2;

typedef struct cameraSettings {
    float posX;
    float posY;
    float posZ;
    float lookX;
    float lookY;
    float lookZ;
    float upX;
    float upY;
    float upZ;
    float fov;
    float near;
    float far;
    float alpha, beta;  // alpha vertical angle / beta horizontal angle
    float zoom;
    float aspectRatio;
} cameraSettings;

enum CameraMode {
    CAMERA_EX,  // Explorer Mode
    CAMERA_FP,  // First Person Mode
    CAMERA_TP   // Third Person Mode
};

class Camera : public Object {
   public:
    float ROTATION_SPEED;
    float MOVEMENT_SPEED;
    float ZOOM_STEP;

    Camera(
        float posX, float posY, float posZ,
        float lookX, float lookY, float lookZ,
        float upX, float upY, float upZ,
        float fov, float near, float far);

    CameraMode getCameraMode();

    Vector3<float> getPosEX();

    Vector3<float> getPosFP();

    Vector3<float> getPosTP();

    Vector3<float> getCurrentPos();

    Vector3<float> getLookAtEX();

    Vector3<float> getLookAtFP();

    Vector3<float> getLookAtTP();

    Vector3<float> getCurrentLookAt();

#pragma region------- Overrides -------
    void render();

    /**
     * Sets the camera center position.
     * The center position is the pivot of the camera. To move the camera around this point, use
     * @ref Camera::setRotation "Camera::setRotation".
     */
    void setExPosition(float x, float y, float z);

    void setFpPosition(float x, float y, float z);

    void setTpPosition(float x, float y, float z);

    void setExLookAt(float x, float y, float z);

    void setFpLookAt(float x, float y, float z);

    void setTpLookAt(float x, float y, float z);

    /**
     * Sets the distance the camera is from it's target point.
     */
    void setExZoom(float zoom);

    /**
     * Sets the camera mode to be used by this camera and normalizes the camera variables
     */
    void setCameraMode(CameraMode mode);

    void setAspectRatio(float aspectRatio);

#pragma endregion

    static Camera* fromXML(XMLElement* xml);

    /**
     * This function processes the input for the Explorer camera.
     */
    void exMovement(unsigned char key, int mx, int my);

    /**
     * This function processes the input for the first person camera.
     */
    void fpMovement(unsigned char key, int mx, int my);

    /**
     * This function processes the input for the third person camera.
     */
    void tpMovement(unsigned char key, int mx, int my);

    /**
     * This function evaluates the camera mode and executes the action associated to the par key/mode
     * (e.g The 'k' key may have a behavior in one camera mode, and a different one in another)
     */
    void processMovement(unsigned char key, int mx, int my);

   private:
    // Esta função devolve o produto externo de dois vetores (o vetor que lhes é prependicular)
    void cross(float dx, float dy, float dz, float fx, float fy, float fz, float& rx, float& ry, float& rz);

    // Esta função devolve o produto interno de dois vetores (o angulo entre eles)
    float dotProduct(float ax, float ay, float az, float bx, float by, float bz);

    void findPerpendicularVector(float x, float y, float z, float& px, float& py, float& pz);

    cameraSettings cameras[3];  // 0 - Explorer | 1 - First Person | 2 - Third Person
    CameraMode currentMode;
};