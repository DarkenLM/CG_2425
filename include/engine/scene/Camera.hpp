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
    float pitch, yaw;
    float alpha, beta;  // alpha vertical angle / beta horizontal angle
    float zoom;
    float aspectRatio;
} cameraSettings;

// TODO: Correctly implement FP and TP camera modes.
enum CameraMode {
    CAMERA_EX,  // Explorer Mode
    CAMERA_FP,  // First Person Mode
    CAMERA_TP   // Third Person Mode
};

class Camera : public Object {
   public:
    float ROTATION_SPEED;
    float MOVEMENT_SPEED;

    Camera(
        float posX, float posY, float posZ,
        float lookX, float lookY, float lookZ,
        float upX, float upY, float upZ,
        float fov, float near, float far) {
        for (int i = 0; i < 3; i++) {
            this->cameras[i].posX = posX;
            this->cameras[i].posY = posY;
            this->cameras[i].posZ = posZ;

            this->cameras[i].lookX = lookX;
            this->cameras[i].lookY = lookY;
            this->cameras[i].lookZ = lookZ,

            this->cameras[i].upX = upX;
            this->cameras[i].upY = upY;
            this->cameras[i].upZ = upZ;

            this->cameras[i].fov = fov;
            this->cameras[i].near = near;
            this->cameras[i].far = far;

            this->cameras[i].aspectRatio = 1.0f;
        }
        this->MOVEMENT_SPEED = 0.5f;
        this->ROTATION_SPEED = 0.01f;
        this->currentMode = CAMERA_EX;

        // Functions used to calculate de distance
        float dirX = abs(lookX - posX);
        float dirY = abs(lookY - posY);
        float dirZ = abs(lookZ - posZ);

        {
            float horizontalDist = sqrt(dirX * dirX + dirZ * dirZ);
            float verticalDist = sqrt(dirY * dirY + dirZ * dirZ);

            this->cameras[0].pitch = atan2(dirY, horizontalDist);
            this->cameras[0].yaw = atan2(dirX, dirZ);

            // Im adding this to be more consistent to what we use in class
            this->cameras[0].beta = asin(dirZ / horizontalDist);
            this->cameras[0].alpha = asin(dirY / verticalDist);

            this->cameras[0].zoom = sqrt(pow(dirX, 2) + pow(dirY, 2) + pow(dirZ, 2));

            this->cameras[0].posX = this->cameras[0].zoom * cos(this->cameras[0].alpha) * cos(this->cameras[0].beta);
            this->cameras[0].posY = this->cameras[0].zoom * sin(this->cameras[0].alpha);
            this->cameras[0].posZ = this->cameras[0].zoom * cos(this->cameras[0].alpha) * sin(this->cameras[0].beta);
        }
        {
            float dirX = abs(lookX - posX);
            float dirY = abs(lookY - posY);
            float dirZ = abs(lookZ - posZ);

            this->cameras[1].zoom = sqrt(pow(dirX, 2) + pow(dirY, 2) + pow(dirZ, 2));

            this->cameras[1].alpha = acos(dirY / this->cameras[1].zoom);

            // Azimuthal angle (yaw): beta is the angle in the XZ plane
            this->cameras[1].beta = atan2(this->cameras[1].posZ, this->cameras[1].posX);  // The right sign will be determined by the signs of posX and posZ

            // Adjust the lookAt position based on the spherical coordinates and zoom value
            this->cameras[1].lookX = this->cameras[1].posX - this->cameras[1].zoom * sin(this->cameras[1].alpha) * cos(this->cameras[1].beta);  // X component of lookAt
            this->cameras[1].lookY = this->cameras[1].posY - this->cameras[1].zoom * cos(this->cameras[1].alpha);                               // Y component of lookAt
            this->cameras[1].lookZ = this->cameras[1].posZ - this->cameras[1].zoom * sin(this->cameras[1].alpha) * sin(this->cameras[1].beta);  // Z component of lookAt
        }
    };

    float getPosX() {
        if (this->currentMode == CAMERA_EX) {
            return this->cameras[0].posX;
        } else if (this->currentMode == CAMERA_FP) {
            return this->cameras[1].posX;
        } else {
            return this->cameras[2].posX;
        }
    };
    float getPosY() {
        if (this->currentMode == CAMERA_EX) {
            return this->cameras[0].posY;
        } else if (this->currentMode == CAMERA_FP) {
            return this->cameras[1].posY;
        } else {
            return this->cameras[2].posY;
        }
    };
    float getPosZ() {
        if (this->currentMode == CAMERA_EX) {
            return this->cameras[0].posZ;
        } else if (this->currentMode == CAMERA_FP) {
            return this->cameras[1].posZ;
        } else {
            return this->cameras[2].posZ;
        }
    };
    float getLookX() {
        if (this->currentMode == CAMERA_EX) {
            return this->cameras[0].lookX;
        } else if (this->currentMode == CAMERA_FP) {
            return this->cameras[1].lookX;
        } else {
            return this->cameras[2].lookX;
        }
    };
    float getLookY() {
        if (this->currentMode == CAMERA_EX) {
            return this->cameras[0].lookY;
        } else if (this->currentMode == CAMERA_FP) {
            return this->cameras[1].lookY;
        } else {
            return this->cameras[2].lookY;
        }
    };
    float getLookZ() {
        if (this->currentMode == CAMERA_EX) {
            return this->cameras[0].lookZ;
        } else if (this->currentMode == CAMERA_FP) {
            return this->cameras[1].lookZ;
        } else {
            return this->cameras[2].lookZ;
        }
    };

#pragma region------- Overrides -------
    void render() override {
        glLoadIdentity();
        switch (this->currentMode) {
            case CAMERA_EX:
                gluLookAt(
                    this->cameras[0].posX, this->cameras[0].posY, this->cameras[0].posZ,
                    this->cameras[0].lookX, this->cameras[0].lookY, this->cameras[0].lookZ,
                    this->cameras[0].upX, this->cameras[0].upY, this->cameras[0].upZ);
                break;
            case CAMERA_FP:
                gluLookAt(
                    this->cameras[1].posX, this->cameras[1].posY, this->cameras[1].posZ,
                    this->cameras[1].lookX, this->cameras[1].lookY, this->cameras[1].lookZ,
                    this->cameras[1].upX, this->cameras[1].upY, this->cameras[1].upZ);
                break;
            case CAMERA_TP:
                gluLookAt(
                    this->cameras[2].posX, this->cameras[2].posY, this->cameras[2].posZ,
                    this->cameras[2].lookX, this->cameras[2].lookY, this->cameras[2].lookZ,
                    this->cameras[2].upX, this->cameras[2].upY, this->cameras[2].upZ);
                break;
            default:
                break;
        }
    };

    /**
     * Sets the camera center position.
     * The center position is the pivot of the camera. To move the camera around this point, use
     * @ref Camera::setRotation "Camera::setRotation".
     */
    void setExPosition(float x, float y, float z) {
        this->cameras[0].posX = x;
        this->cameras[0].posY = y;
        this->cameras[0].posZ = z;
    };

    void setFpPosition(float x, float y, float z) {
        this->cameras[1].posX = x;
        this->cameras[1].posY = y;
        this->cameras[1].posZ = z;
    };

    void setTpPosition(float x, float y, float z) {
        this->cameras[2].posX = x;
        this->cameras[2].posY = y;
        this->cameras[2].posZ = z;
    };

    void setExLookAt(float x, float y, float z) {
        this->cameras[0].lookX = x;
        this->cameras[0].lookY = y;
        this->cameras[0].lookZ = z;
    };

    void setFpLookAt(float x, float y, float z) {
        this->cameras[1].lookX = x;
        this->cameras[1].lookY = y;
        this->cameras[1].lookZ = z;
    };

    void setTpLookAt(float x, float y, float z) {
        this->cameras[2].lookX = x;
        this->cameras[2].lookY = y;
        this->cameras[2].lookZ = z;
    };

#pragma endregion-- -- -- -Overrides-- -- -- -

    /**
     * Sets the distance the camera is from it's target point.
     */
    void setExZoom(float zoom) {
        this->cameras[0].zoom = zoom;
    }

    /**
     * Sets the camera mode to be used by this camera and normalizes the camera variables
     */
    void setCameraMode(CameraMode mode) {
        this->currentMode = mode;
    }

    void setAspectRatio(float aspectRatio) {
        for (int i = 0; i < 3; i++) {
            this->cameras[i].aspectRatio = aspectRatio;

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(this->cameras[i].fov, this->cameras[i].aspectRatio, this->cameras[i].near, this->cameras[i].far);
            glMatrixMode(GL_MODELVIEW);
        }
    }

    static Camera* fromXML(XMLElement* xml) {
        GET_XML_ELEMENT_OR_FAIL(xml, "position", position);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(position, "x", posX, float);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(position, "y", posY, float);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(position, "z", posZ, float);

        GET_XML_ELEMENT_OR_FAIL(xml, "lookAt", lookAt);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(lookAt, "x", lookX, float);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(lookAt, "y", lookY, float);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(lookAt, "z", lookZ, float);

        GET_XML_ELEMENT_OR_FAIL(xml, "up", up);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(up, "x", upX, float);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(up, "y", upY, float);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(up, "z", upZ, float);

        GET_XML_ELEMENT_OR_FAIL(xml, "projection", projection);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(projection, "fov", fov, float);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(projection, "near", near, float);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(projection, "far", far, float);

        return new Camera(
            posX, posY, posZ,
            lookX, lookY, lookZ,
            upX, upY, upZ,
            fov, near, far);
    }

    /**
     * This function processes the input for the Explporer camera.
     */
    void exMovement(unsigned char key, int mx, int my) {
        switch (key) {
            case 'w':
            case 'W': {
                if (this->cameras[0].alpha > -M_PI / 2 + this->ROTATION_SPEED) this->cameras[0].alpha -= this->ROTATION_SPEED;
                break;
            }
            case 's':
            case 'S': {
                if (this->cameras[0].alpha < M_PI / 2 - this->ROTATION_SPEED) this->cameras[0].alpha += this->ROTATION_SPEED;
                break;
            }
            case 'a':
            case 'A': {
                this->cameras[0].beta += this->ROTATION_SPEED;
                break;
            }
            case 'd':
            case 'D': {
                this->cameras[0].beta -= this->ROTATION_SPEED;
                break;
            }
            case '-': {
                this->cameras[0].zoom++;
                break;
            }
            case '+': {
                this->cameras[0].zoom--;
                break;
            }
            default: {
                // Didn't to shit, do not reprocess.
                return;
            }
        }
        this->cameras[0].posX = this->cameras[0].zoom * cos(this->cameras[0].alpha) * cos(this->cameras[0].beta);
        this->cameras[0].posY = this->cameras[0].zoom * sin(this->cameras[0].alpha);
        this->cameras[0].posZ = this->cameras[0].zoom * cos(this->cameras[0].alpha) * sin(this->cameras[0].beta);
    }

    /**
     * This function processes the input for the First person camera.
     */
    void fpMovement(unsigned char key, int mx, int my) {
        float moveStep = this->MOVEMENT_SPEED;  // Step size for movement

        switch (key) {
            case 's':
            case 'S': {
                // Move forward: we need to move in the direction the camera is looking
                this->cameras[1].posX += moveStep * sin(this->cameras[1].alpha) * cos(this->cameras[1].beta);
                this->cameras[1].posY += moveStep * cos(this->cameras[1].alpha);  // Corrected: use cos(alpha) for vertical movement
                this->cameras[1].posZ += moveStep * sin(this->cameras[1].alpha) * sin(this->cameras[1].beta);
                break;
            }
            case 'w':
            case 'W': {
                // Move backward: opposite of the direction the camera is looking
                this->cameras[1].posX -= moveStep * sin(this->cameras[1].alpha) * cos(this->cameras[1].beta);
                this->cameras[1].posY -= moveStep * cos(this->cameras[1].alpha);  // Corrected: use cos(alpha) for vertical movement
                this->cameras[1].posZ -= moveStep * sin(this->cameras[1].alpha) * sin(this->cameras[1].beta);
                break;
            }
            case 'a':
            case 'A': {
                // Move right perpendicular to the look-at vector
                this->cameras[1].posX -= moveStep * sin(this->cameras[1].beta);
                this->cameras[1].posZ += moveStep * cos(this->cameras[1].beta);
                break;
            }
            case 'd':
            case 'D': {
                // Move left perpendicular to the look-at vector
                this->cameras[1].posX += moveStep * sin(this->cameras[1].beta);
                this->cameras[1].posZ -= moveStep * cos(this->cameras[1].beta);
                break;
            }
            case 'j': {
                // Rotate look-at to the left (decrease beta)
                this->cameras[1].beta -= this->ROTATION_SPEED;
                break;
            }
            case 'l': {
                // Rotate look-at to the right (increase beta)
                this->cameras[1].beta += this->ROTATION_SPEED;
                break;
            }
            case 'i': {
                // Rotate look-at up (increase alpha)
                this->cameras[1].alpha += this->ROTATION_SPEED;
                if (this->cameras[1].alpha > M_PI - this->ROTATION_SPEED) this->cameras[1].alpha = M_PI - this->ROTATION_SPEED;  // Clamp to avoid flipping
                break;
            }
            case 'k': {
                // Rotate look-at down (decrease alpha)
                this->cameras[1].alpha -= this->ROTATION_SPEED;
                if (this->cameras[1].alpha < 0 + this->ROTATION_SPEED) this->cameras[1].alpha = this->ROTATION_SPEED;  // Clamp to avoid flipping
                break;
            }
            case '-': {
                // Zoom out
                this->cameras[1].zoom += 1.0f;
                break;
            }
            case '+': {
                // Zoom in
                this->cameras[1].zoom -= 1.0f;
                if (this->cameras[1].zoom < 1.0f) this->cameras[1].zoom = 1.0f;  // Clamp to avoid zooming too close
                break;
            }
            default: {
                // No valid key pressed, do nothing
                return;
            }
        }

        // Update the look-at point based on the new alpha and beta angles
        this->cameras[1].lookX = this->cameras[1].posX - this->cameras[1].zoom * sin(this->cameras[1].alpha) * cos(this->cameras[1].beta);
        this->cameras[1].lookY = this->cameras[1].posY - this->cameras[1].zoom * cos(this->cameras[1].alpha);
        this->cameras[1].lookZ = this->cameras[1].posZ - this->cameras[1].zoom * sin(this->cameras[1].alpha) * sin(this->cameras[1].beta);
    }

    /**
     * This function processes the input for the third person camera.
     */
    void tpMovement(unsigned char key, int mx, int my) {
    }

    /**
     * This function evaluates the camera mode and executes the action associated to the par key/mode
     * (e.g The 'k' key may have a behavior in one camera mode, and a different one in another)
     */
    void processMovement(unsigned char key, int mx, int my) {
        switch (this->currentMode) {
            case CAMERA_EX: {
                exMovement(key, mx, my);
                break;
            }
            case CAMERA_FP: {
                fpMovement(key, mx, my);
                break;
            }
            case CAMERA_TP: {
                tpMovement(key, mx, my);
                break;
            }
            default: {
                // Didn't to shit, do not reprocess.
                return;
            }
        }
        return;
    }

   private:
    cameraSettings cameras[3];  // 0 - Explorer | 1 - First Person | 2 - Third Person
    CameraMode currentMode;
};