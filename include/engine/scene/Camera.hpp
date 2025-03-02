#pragma once

#include <limits>

#include "engine/scene/Object.hpp"
#include "common/util/xmlutil.hpp"
using namespace tinyxml2;

// TODO: Correctly implement FP and TP camera modes.
enum CameraMode {
    CAMERA_EX, // Explorer Mode
    CAMERA_FP, // First Person Mode
    CAMERA_TP  // Third Person Mode
};

class Camera: public Object {
    public:
        static constexpr float CAMERA_STEP = 0.01f;

        Camera(
            float posX, float posY, float posZ,
            float lookX, float lookY, float lookZ,
            float upX, float upY, float upZ,
            float fov, float near, float far
        ) {
            this->posX = posX;
            this->posY = posY;
            this->posZ = posZ,

            this->lookX = lookX;
            this->lookY = lookY;
            this->lookZ = lookZ,

            this->upX = upX;
            this->upY = upY;
            this->upZ = upZ;

            this->fov = fov;
            this->near = near;
            this->far = far;

            // Calculate initial pitch / yaw
            {
                float dirX = lookX - posX;
                float dirY = lookY - posY;
                float dirZ = lookZ - posZ;

                float horizontalDist = sqrt(dirX * dirX + dirZ * dirZ);
                this->pitch = atan2(dirY, horizontalDist);
                this->yaw = atan2(dirX, dirZ);

                this->zoom = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ); 
            }

            this->mode = CAMERA_FP;
            this->aspectRatio = 1.0f;
        };

        #pragma region ------- Overrides -------
        void render() override {
            glLoadIdentity();
            switch (this->mode) {
                case CAMERA_EX: {
                    auto [px, py, pz] = this->_calculateCameraOffset();
                    float ppx = px;
                    float ppy = py;
                    float ppz = pz;

                    std::cout << "LOOKAT: " 
                        << ppx << " " << ppy << " " << ppz << " "
                        << this->lookX << " " << this->lookY << " " << this->lookZ << " "
                        << 0.0f << " " << 1.0f << " " << 0.0f << std::endl;

                    gluLookAt(
                        ppx, ppy, ppz,
                        this->lookX, this->lookY, this->lookZ,
                        0.0f, 1.0f, 0.0f
                    );
                    break;
                }
                case CAMERA_FP:
                case CAMERA_TP: {
                    // NOTE: WORKEN'T
                    auto [px, py, pz] = this->_calculateCameraOffset();

                    std::cout << "LOOKAT: " << this->posX << " " << this->posY << " " << this->posZ << " " <<
                        px + this->posX << " " << py + this->posY << " " << pz + this->posZ << " " <<
                        0.0f << " " << 1.0f << " " << 0.0f << std::endl;

                    gluLookAt(
                        this->posX, this->posY, this->posZ,
                        px + this->posX, py + this->posY, pz + this->posZ,
                        0.0f, 1.0f, 0.0f
                    );
                    break;
                }
            }
        };

        /**
         * Sets the camera center position. 
         * The center position is the pivot of the camera. To move the camera around this point, use 
         * @ref Camera::setRotation "Camera::setRotation".
         */
        void setPosition(float x, float y, float z) override {
            this->posX = x;
            this->posY = y;
            this->posZ = z;
        };

        /**
         * Sets the camera rotation around it's center point. 
         * Angles are expected to be in radians.
         * 
         * @param roll has no effect, and is ignored.
         * @param pitch controls the up/down rotation.
         * @param yaw controls the legt/right rotation.
         */
        void setRotation(float roll, float pitch, float yaw) override {
            // this->lookY = fmax(-M_PI/2, fmin(M_PI/2, pitch));
            // this->lookZ = fmod(yaw, 2 * M_PI);

            switch (this->mode) {
                case CAMERA_EX: {
                    this->pitch = fmax(-M_PI/2 + CAMERA_STEP, fmin(M_PI/2 - CAMERA_STEP, pitch));
                    this->yaw = fmod(yaw, 2 * M_PI);
                    break;
                }
                case CAMERA_FP:
                case CAMERA_TP: {
                    this->lookY = fmax(-M_PI/2 + CAMERA_STEP, fmin(M_PI/2 - CAMERA_STEP, pitch));
                    this->lookZ = fmod(yaw, 2 * M_PI);
                    break;
                }
            }
        };

        /**
         * Changes the camera rotation around it's center point, accumulating with it's current rotation. 
         * Angles are expected to be in radians.
         * 
         * @param roll has no effect, and is ignored.
         * @param pitch controls the up/down rotation.
         * @param yaw controls the legt/right rotation.
         */
        void rotateTo(float roll, float pitch, float yaw) override {
            switch (this->mode) {
                case CAMERA_EX: {
                    this->pitch = fmax(-M_PI/2 + CAMERA_STEP, fmin(M_PI/2 - CAMERA_STEP, this->pitch + pitch));
                    this->yaw = fmod(this->yaw + yaw, 2 * M_PI);
                    break;
                }
                case CAMERA_FP:
                case CAMERA_TP: {
                    this->lookY = fmax(-M_PI/2 + CAMERA_STEP, fmin(M_PI/2 - CAMERA_STEP, this->lookY + pitch));
                    this->lookZ = fmod(this->lookZ + yaw, 2 * M_PI);
                    break;
                }
            }
        }
        #pragma endregion ------- Overrides -------

        /**
         * Sets the distance the camera is from it's target point.
         */
        void setZoom(float zoom) {
            this->zoom = zoom;
        }

        /**
         * Sets the camera mode to be used by this camera.
         */
        void setCameraMode(CameraMode mode) {
            // TODO: Seamlessly handle changes from EX to FP/TP modes.

            this->mode = mode;
        }

        void setAspectRatio(float aspectRatio) {
            this->aspectRatio = aspectRatio;
            
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(this->fov, this->aspectRatio, this->near, this->far);
            glMatrixMode(GL_MODELVIEW);
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
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(projection, "fov",  fov, float);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(projection, "near", near, float);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(projection, "far",  far, float);

            return new Camera(
                posX, posY, posZ,
                lookX, lookY, lookZ,
                upX, upY, upZ,
                fov, near, far
            );
        }

    private:
        float posX; float posY; float posZ;
        float lookX; float lookY; float lookZ;
        float upX; float upY; float upZ;
        float fov; float near; float far;
        float pitch, yaw;
        float zoom;
        float aspectRatio;
        CameraMode mode;

        
        struct camera_offset { float px, py, pz; };
        struct camera_offset _calculateCameraOffset() {
            float px = 0, py = 0, pz = 0;

            switch (this->mode) {
                case CAMERA_EX: {
                    pz = - this->zoom * cos(this->yaw) * cos(this->pitch);
                    py = - this->zoom * sin(this->pitch);
                    px = - this->zoom * sin(this->yaw) * cos(this->pitch);
                    break; 
                }
                default: {
                    px = this->zoom * cos(this->lookY) * sin(this->lookZ);
                    py = this->zoom * sin(this->lookY);
                    pz = this->zoom * cos(this->lookY) * cos(this->lookZ);
                }
            }

            return { px, py, pz };
        }
};