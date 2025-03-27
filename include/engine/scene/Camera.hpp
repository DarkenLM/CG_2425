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
    float ZOOM_STEP;

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

        this->MOVEMENT_SPEED = 10.0f;
        this->ROTATION_SPEED = 0.01f;
        this->ZOOM_STEP = 2.0f;
        this->currentMode = CAMERA_EX;

        // Calcular os valores do vetor direção
        float dirX = lookX - posX;
        float dirY = lookY - posY;
        float dirZ = lookZ - posZ;

        // Calcular o zoom (Distancia da posição ao lookup)
        float zoom = sqrt(pow(dirX, 2) + pow(dirY, 2) + pow(dirZ, 2));

        {
            // Colocamos o lookup inicial
            this->cameras[0].lookX = lookX;
            this->cameras[0].lookY = lookY;
            this->cameras[0].lookZ = lookZ;

            // Colocamos o vetor up inicial
            this->cameras[0].upX = upX;
            this->cameras[0].upY = upY;
            this->cameras[0].upZ = upZ;

            // colocamos o fov, near, far e aspect ration iniciais
            this->cameras[0].fov = fov;
            this->cameras[0].near = near;
            this->cameras[0].far = far;
            this->cameras[0].aspectRatio = 1.0f;

            this->cameras[0].zoom = zoom;

            // Calcular o angulo alpha com ajuda do produto interno
            this->cameras[0].alpha = M_PI_2 - acos(dotProduct(-dirX, -dirY, -dirZ, 0, 1, 0) / zoom);

            // Calcular o angulo beta com ajuda do produto interno
            this->cameras[0].beta = M_PI_2 - acos(dotProduct(-dirX, 0, -dirZ, 1, 0, 0) / zoom);

            this->cameras[0].posX = this->cameras[0].zoom * cos(this->cameras[0].alpha) * sin(this->cameras[0].beta);
            this->cameras[0].posY = this->cameras[0].zoom * sin(this->cameras[0].alpha);
            this->cameras[0].posZ = this->cameras[0].zoom * cos(this->cameras[0].alpha) * cos(this->cameras[0].beta);
        }
        {
            // Colocamos a posição inicial
            this->cameras[1].posX = posX;
            this->cameras[1].posY = posY;
            this->cameras[1].posZ = posZ;

            // Colocamos o vetor up inicial
            this->cameras[1].upX = upX;
            this->cameras[1].upY = upY;
            this->cameras[1].upZ = upZ;

            // colocamos o fov, near, far e aspect ration iniciais
            this->cameras[1].fov = fov;
            this->cameras[1].near = near;
            this->cameras[1].far = far;
            this->cameras[1].aspectRatio = 1.0f;

            // Calcular o angulo alpha com ajuda do produto interno
            this->cameras[1].alpha = M_PI_2 - acos(dotProduct(dirX, dirY, dirZ, 0, 1, 0) / zoom);

            // Calcular o angulo beta com ajuda do produto interno
            this->cameras[1].beta = M_PI + cos(dotProduct(dirX, 0, dirZ, 1, 0, 0) / zoom);

            // Normalizamos a distancia da camera ao lookAt como 1
            this->cameras[1].zoom = 1;

            dirX = cos(this->cameras[1].beta) * cos(this->cameras[1].alpha);
            dirY = sin(this->cameras[1].alpha);
            dirZ = sin(this->cameras[1].beta) * cos(this->cameras[1].alpha);

            // Adjust the lookAt position based on the spherical coordinates and zoom value
            this->cameras[1].lookX = this->cameras[1].posX + dirX;
            this->cameras[1].lookY = this->cameras[1].posY + dirY;
            this->cameras[1].lookZ = this->cameras[1].posZ + dirZ;
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
                this->cameras[0].zoom += this->ZOOM_STEP;
                break;
            }
            case '+': {
                this->cameras[0].zoom -= this->ZOOM_STEP;
                break;
            }
            default: {
                // Didn't to shit, do not reprocess.
                return;
            }
        }
        this->cameras[0].posX = this->cameras[0].zoom * cos(this->cameras[0].alpha) * sin(this->cameras[0].beta);
        this->cameras[0].posY = this->cameras[0].zoom * sin(this->cameras[0].alpha);
        this->cameras[0].posZ = this->cameras[0].zoom * cos(this->cameras[0].alpha) * cos(this->cameras[0].beta);
    }

    void fpMovement(unsigned char key, int mx, int my) {
        float moveStep = this->MOVEMENT_SPEED;
        float dirX, dirY, dirZ;
        float dirX2, dirY2, dirZ2;

        switch (key) {
            case 'j': {
                this->cameras[1].beta -= this->ROTATION_SPEED;
                break;
            }
            case 'l': {
                this->cameras[1].beta += this->ROTATION_SPEED;
                break;
            }
            case 'i': {
                if (this->cameras[1].alpha < M_PI / 2 - this->ROTATION_SPEED) this->cameras[1].alpha += this->ROTATION_SPEED;
                std::cout << "alpha" << this->cameras[1].alpha << std::endl;
                break;
            }
            case 'k': {
                if (this->cameras[1].alpha > -M_PI / 2 + this->ROTATION_SPEED) this->cameras[1].alpha -= this->ROTATION_SPEED;
                break;
            }
        }

        dirX = cos(this->cameras[1].beta) * cos(this->cameras[1].alpha);
        dirY = sin(this->cameras[1].alpha);
        dirZ = sin(this->cameras[1].beta) * cos(this->cameras[1].alpha);

        cross(dirX, dirY, dirZ, 0, 1, 0, dirX2, dirY2, dirZ2);

        switch (key) {
            case 'a':
            case 'A': {  // Strafe left
                this->cameras[1].posX -= moveStep * dirX2;
                this->cameras[1].posY -= moveStep * dirY2;
                this->cameras[1].posZ -= moveStep * dirZ2;
                break;
            }
            case 'w':
            case 'W': {  // Move forward
                this->cameras[1].posX += moveStep * dirX;
                this->cameras[1].posY += moveStep * dirY;
                this->cameras[1].posZ += moveStep * dirZ;
                break;
            }
            case 's':
            case 'S': {  // Move backward
                this->cameras[1].posX -= moveStep * dirX;
                this->cameras[1].posY -= moveStep * dirY;
                this->cameras[1].posZ -= moveStep * dirZ;
                break;
            }
            case 'd':
            case 'D': {  // Strafe right
                this->cameras[1].posX += moveStep * dirX2;
                this->cameras[1].posY += moveStep * dirY2;
                this->cameras[1].posZ += moveStep * dirZ2;
                break;
            }
            default: {
                // No movement key pressed
                break;
            }
        }

        // Update look position based on new direction
        this->cameras[1].lookX = this->cameras[1].posX + dirX;
        this->cameras[1].lookY = this->cameras[1].posY + dirY;
        this->cameras[1].lookZ = this->cameras[1].posZ + dirZ;
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
    // Esta função devolve o produto externo de dois vetores (o vetor que lhes é prependicular)
    void cross(float dx, float dy, float dz, float fx, float fy, float fz, float& rx, float& ry, float& rz) {
        rx = dy * fz - dz * fy;
        ry = dz * fx - dx * fz;
        rz = dx * fy - dy * fx;
    }

    // Esta função devolve o produto interno de dois vetores (o angulo entre eles)
    float dotProduct(float ax, float ay, float az, float bx, float by, float bz) {
        return ax * bx + ay * by + az * bz;
    }

    void findPerpendicularVector(float x, float y, float z, float& px, float& py, float& pz) {
        px = 0.0f;
        py = z;
        pz = -y;

        if (py == 0.0f && pz == 0.0f) {
            px = -z;
            py = 0.0f;
            pz = x;
        }

        float length = std::sqrt(px * px + py * py + pz * pz);
        if (length > 0.0f) {
            px /= length;
            py /= length;
            pz /= length;
        }
    }

    cameraSettings cameras[3];  // 0 - Explorer | 1 - First Person | 2 - Third Person
    CameraMode currentMode;
};