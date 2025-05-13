#pragma once

#include <limits>

#include "common/util/xmlutil.hpp"
#include "engine/scene/Model.hpp"
#include "engine/scene/Object.hpp"
using namespace tinyxml2;

/**
 * @struct cameraSettings
 * @brief Represents the configuration parameters of a camera.
 */
typedef struct cameraSettings {
    float posX;         ///< X position of the camera.
    float posY;         ///< Y position of the camera.
    float posZ;         ///< Z position of the camera.
    float lookX;        ///< X coordinate of the look-at vector.
    float lookY;        ///< Y coordinate of the look-at vector.
    float lookZ;        ///< > coordinate of the look-at vector.
    float upX;          ///< X component of the up vector.
    float upY;          ///< Y component of the up vector.
    float upZ;          ///< Z component of the up vector.
    float fov;          ///< Field of view.
    float near;         ///< Near clipping plane.
    float far;          ///< Far clipping plane.
    float alpha, beta;  ///< Vertical (alpha) and horizontal (beta) rotation angles.
    float zoom;         ///< Zoom level.
    float aspectRatio;  ///< Aspect ratio (width / height).
    int targetId;       ///< The id for the model for the camera to track.
} cameraSettings;

/**
 * @enum CameraMode
 * @brief Enum for the camera operating modes.
 */
enum CameraMode {
    CAMERA_EX,  ///< Explorer Mode
    CAMERA_FP,  ///< First Person Mode
    CAMERA_TP   ///< Third Person Mode
};

/**
 * @class Camera
 * @brief Handles multiple camera modes (Explorer, First Person, Third Person) and their respective transformations.
 */
class Camera : public Object {
   public:
    float ROTATION_SPEED;  ///< Speed of rotation for camera controls.
    float MOVEMENT_SPEED;  ///< Speed of translation for FP mode.
    float ZOOM_STEP;       ///< Zoom increment for Explorer mode.

    /**
     * @brief Constructor to initialize the camera with position, orientation and projection parameters.
     */
    Camera(
        float posX, float posY, float posZ,
        float lookX, float lookY, float lookZ,
        float upX, float upY, float upZ,
        float fov, float near, float far,
        Model* fallbackModel, std::string target);

    /**
     * @brief Returns the current camera mode.
     */
    CameraMode getCameraMode();

    /**
     * @brief Gets Explorer mode camera position.
     */
    Vector3<float> getPosEX();

    /**
     * @brief Gets First Person mode camera position.
     */
    Vector3<float> getPosFP();

    /**
     * @brief Gets Third Person mode camera position.
     */
    Vector3<float> getPosTP();

    /**
     * @brief Gets current camera position based on the active mode.
     */
    Vector3<float> getCurrentPos();

    /**
     * @brief Gets Explorer mode look-at vector.
     */
    Vector3<float> getLookAtEX();

    /**
     * @brief Gets First Person mode look-at vector.
     */
    Vector3<float> getLookAtFP();

    /**
     * @brief Gets Third Person mode look-at vector.
     */
    Vector3<float> getLookAtTP();

    /**
     * @brief Gets current look-at vector based on the active mode.
     */
    Vector3<float> getCurrentLookAt();

    /**
     * @brief Whether the camera is tracking a model.
     */
    bool isTracking();

    /**
     * @brief Whether the camera is tracking it's fallback model.
     */
    bool isTrackingFallback();

    /**
     * @brief Gets the id of the model that the camera is tracking. If it is tracking it's fallback model, it will
     * return "<self>".
     */
    std::string getTrackingId();

    /**
     * @brief Sets this camera to track a specific object.
     */
    void track(const Model* ref, std::string id);

#pragma region------- Overrides -------
    /**
     * @brief Renders the camera object (override from Object class).
     */
    void render();

    /**
     * @brief Sets the Explorer mode position.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param z Z coordinate.
     */
    void setExPosition(float x, float y, float z);

    /**
     * @brief Sets the First Person mode position.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param z Z coordinate.
     */
    void setFpPosition(float x, float y, float z);

    /**
     * @brief Sets the Third Person mode position.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param z Z coordinate.
     */
    void setTpPosition(float x, float y, float z);

    /**
     * @brief Sets the Explorer mode look-at point.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param z Z coordinate.
     */
    void setExLookAt(float x, float y, float z);

    /**
     * @brief Sets the First Person mode look-at point.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param z Z coordinate.
     */
    void setFpLookAt(float x, float y, float z);

    /**
     * @brief Sets the Third Person mode look-at point.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param z Z coordinate.
     */
    void setTpLookAt(float x, float y, float z);

    /**
     * @brief Sets the zoom distance for Explorer mode.
     * @param zoom Zoom level.
     */
    void setExZoom(float zoom);

    /**
     * @brief Sets the current camera mode.
     * @param mode The camera mode to set.
     */
    void setCameraMode(CameraMode mode);

    /**
     * @brief Sets the aspect ratio for the camera's projection.
     * @param aspectRatio Width/height ratio.
     */
    void setAspectRatio(float aspectRatio);

#pragma endregion
    /**
     * @brief Constructs a Camera instance from an XML element.
     * @param xml Pointer to the XML element.
     * @return A new Camera object.
     */
    static Camera* fromXML(XMLElement* xml);

    /**
     * @brief Loads all models and nested groups, with error handling.
     * @throws std::string if a model or group fails to load.
     */
    void load();

    /**
     * @brief Processes input keys in Explorer mode
     * @param key Unsigned char corresponding to the pressed key code.
     * @param mx Integer corresponding to the mouse x location.
     * @param my Integer corresponding to the mouse y location.
     */
    void exMovement(unsigned char key, int mx, int my);

    /**
     * @brief Processes input keys in First Person mode
     * @param key Unsigned char corresponding to the pressed key code.
     * @param mx Integer corresponding to the mouse x location.
     * @param my Integer corresponding to the mouse y location.
     */
    void fpMovement(unsigned char key, int mx, int my);

    /**
     * @brief Processes input keys in Third Person mode
     * @param key Unsigned char corresponding to the pressed key code.
     * @param mx Integer corresponding to the mouse x location.
     * @param my Integer corresponding to the mouse y location.
     */
    void tpMovement(unsigned char key, int mx, int my);

    /**
     * @brief Dispatches movement commands based on current camera mode.
     * @param key Unsigned char corresponding to the pressed key code.
     * @param mx Integer corresponding to the mouse x location.
     * @param my Integer corresponding to the mouse y location.
     */
    void processMovement(unsigned char key, int mx, int my);

   private:
    // TODO: Remove this (this function already exist elsewhere)
    /**
     * @brief Computes the cross product of two vectors.
     */
    void cross(float dx, float dy, float dz, float fx, float fy, float fz, float& rx, float& ry, float& rz);

    /**
     * @brief Computes the dot product of two vectors.
     */
    float dotProduct(float ax, float ay, float az, float bx, float by, float bz);

    /**
     * @brief Finds a vector perpendicular to the input.
     */
    void findPerpendicularVector(float x, float y, float z, float& px, float& py, float& pz);

    cameraSettings cameras[3];  ///< Stores settings for all camera modes.
    CameraMode currentMode;     ///< Currently active camera mode.
    Model* fallbackTarget;
    std::string targetId;
    const Model* targetRef;
};