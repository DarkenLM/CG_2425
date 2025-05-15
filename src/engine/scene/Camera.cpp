#include "engine/scene/Camera.hpp"

Camera::Camera(
    float posX, float posY, float posZ,
    float lookX, float lookY, float lookZ,
    float upX, float upY, float upZ,
    float fov, float near, float far,
    Model* fallbackModel, std::string target) {
    this->fallbackTarget = fallbackModel;
    this->targetId = target;

    this->targetRef = fallbackModel;

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
    this->MOVEMENT_SPEED = 2.0f;
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

        // Normalizamos o vetor para calcular o angulo corretamente
        dirX /= zoom;
        dirY /= zoom;
        dirZ /= zoom;

        // Calcular o angulo alpha com ajuda do produto interno
        this->cameras[0].alpha = acos(dotProduct(-dirX, -dirY, -dirZ, 0, 1, 0));
        this->cameras[0].alpha = M_PI_2 - this->cameras[0].alpha;

        dirY = 0;
        float zoom = sqrt(pow(dirX, 2) + pow(dirY, 2) + pow(dirZ, 2));
        dirX /= zoom;
        dirZ /= zoom;
        // Calcular o angulo beta com ajuda do produto interno
        this->cameras[0].beta = acos(dotProduct(-dirX, -dirY, -dirZ, 1, 0, 0));
        this->cameras[0].beta = M_PI_2 - this->cameras[0].beta;

        this->cameras[0].posX = this->cameras[0].zoom * cos(this->cameras[0].alpha) * sin(this->cameras[0].beta);
        this->cameras[0].posY = this->cameras[0].zoom * sin(this->cameras[0].alpha);
        this->cameras[0].posZ = this->cameras[0].zoom * cos(this->cameras[0].alpha) * cos(this->cameras[0].beta);
    }
    // Recalcular os valores do vetor direção
    dirX = lookX - posX;
    dirY = lookY - posY;
    dirZ = lookZ - posZ;
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

        // Normalizamos o vetor para calcular o angulo corretamente
        dirX /= zoom;
        dirY /= zoom;
        dirZ /= zoom;

        // Calcular o angulo alpha com ajuda do produto interno
        this->cameras[1].alpha = acos(dotProduct(dirX, dirY, dirZ, 0, 1, 0));
        this->cameras[1].alpha = M_PI_2 - this->cameras[1].alpha;

        dirY = 0;
        float zoom = sqrt(pow(dirX, 2) + pow(dirY, 2) + pow(dirZ, 2));
        dirX /= zoom;
        dirZ /= zoom;

        // Calcular o angulo beta com ajuda do produto interno
        this->cameras[1].beta = acos(dotProduct(dirX, dirY, dirZ, 1, 0, 0));
        this->cameras[1].beta = M_PI_2 + this->cameras[1].beta;

        // Normalizamos a distancia da camera ao lookAt como 1
        this->cameras[1].zoom = 1;

        dirX = cos(this->cameras[1].alpha) * sin(this->cameras[1].beta);
        dirY = sin(this->cameras[1].alpha);
        dirZ = cos(this->cameras[1].beta) * cos(this->cameras[1].alpha);

        // Adjust the lookAt position based on the spherical coordinates and zoom value
        this->cameras[1].lookX = this->cameras[1].posX + dirX;
        this->cameras[1].lookY = this->cameras[1].posY + dirY;
        this->cameras[1].lookZ = this->cameras[1].posZ + dirZ;
    }
    {
        // TODO: Implementar third person
    }
};

CameraMode Camera::getCameraMode() {
    return this->currentMode;
}

Vector3<float> Camera::getPosEX() {
    return Vector3(this->cameras[0].posX, this->cameras[0].posY, this->cameras[0].posZ);
};

Vector3<float> Camera::getPosFP() {
    return Vector3(this->cameras[1].posX, this->cameras[1].posY, this->cameras[1].posZ);
};

Vector3<float> Camera::getPosTP() {
    return Vector3(this->cameras[2].posX, this->cameras[2].posY, this->cameras[2].posZ);
};

Vector3<float> Camera::getCurrentPos() {
    switch (this->currentMode) {
        case CAMERA_EX:
            return Vector3(this->cameras[0].posX, this->cameras[0].posY, this->cameras[0].posZ);
            break;
        case CAMERA_FP:
            return Vector3(this->cameras[1].posX, this->cameras[1].posY, this->cameras[1].posZ);
            break;
        case CAMERA_TP:
            return Vector3(this->cameras[2].posX, this->cameras[2].posY, this->cameras[2].posZ);
            break;
        default:
            std::cout << "The camera mode is incorrect!";
            break;
    }
    return Vector3<float>(0.0f, 0.0f, 0.0f);
};

Vector3<float> Camera::getLookAtEX() {
    return Vector3(this->cameras[0].lookX, this->cameras[0].lookY, this->cameras[0].lookZ);
};

Vector3<float> Camera::getLookAtFP() {
    return Vector3(this->cameras[1].lookX, this->cameras[1].lookY, this->cameras[1].lookZ);
};

Vector3<float> Camera::getLookAtTP() {
    return Vector3(this->cameras[2].lookX, this->cameras[2].lookY, this->cameras[2].lookZ);
};

Vector3<float> Camera::getCurrentLookAt() {
    switch (this->currentMode) {
        case CAMERA_EX:
            return Vector3(this->cameras[0].lookX, this->cameras[0].lookY, this->cameras[0].lookZ);
            break;
        case CAMERA_FP:
            return Vector3(this->cameras[1].lookX, this->cameras[1].lookY, this->cameras[1].lookZ);
            break;
        case CAMERA_TP:
            return Vector3(this->cameras[2].lookX, this->cameras[2].lookY, this->cameras[2].lookZ);
            break;
        default:
            std::cout << "The camera mode is incorrect!";
            break;
    }
    return Vector3(0.0f, 0.0f, 0.0f);
};

void Camera::setExPosition(float x, float y, float z) {
    this->cameras[0].posX = x;
    this->cameras[0].posY = y;
    this->cameras[0].posZ = z;
};

void Camera::setFpPosition(float x, float y, float z) {
    this->cameras[1].posX = x;
    this->cameras[1].posY = y;
    this->cameras[1].posZ = z;
};

void Camera::setTpPosition(float x, float y, float z) {
    this->cameras[2].posX = x;
    this->cameras[2].posY = y;
    this->cameras[2].posZ = z;
};

void Camera::setExLookAt(float x, float y, float z) {
    this->cameras[0].lookX = x;
    this->cameras[0].lookY = y;
    this->cameras[0].lookZ = z;
};

void Camera::setFpLookAt(float x, float y, float z) {
    this->cameras[1].lookX = x;
    this->cameras[1].lookY = y;
    this->cameras[1].lookZ = z;
};

void Camera::setTpLookAt(float x, float y, float z) {
    this->cameras[2].lookX = x;
    this->cameras[2].lookY = y;
    this->cameras[2].lookZ = z;
};

void Camera::setExZoom(float zoom) {
    this->cameras[0].zoom = zoom;
}

void Camera::setCameraMode(CameraMode mode) {
    this->currentMode = mode;
}

void Camera::setAspectRatio(float aspectRatio) {
    for (int i = 0; i < 3; i++) {
        this->cameras[i].aspectRatio = aspectRatio;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(this->cameras[i].fov, this->cameras[i].aspectRatio, this->cameras[i].near, this->cameras[i].far);
        glMatrixMode(GL_MODELVIEW);
    }
}

bool Camera::isTracking() {
    return this->currentMode == CAMERA_TP && !this->targetId.empty();
}

bool Camera::isTrackingFallback() {
    return this->targetId.compare("<self>") == 0;
}

std::string Camera::getTrackingId() {
    return this->targetId;
}

void Camera::track(const Model* ref, std::string id) {
    this->targetId = id;
    this->targetRef = ref;
}

// SEE: The object ref for the target can be get using this->target. If the targetId is "<self>", that means the
// fallback model _SHOULD_ be used.
void Camera::render() {
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
}

Camera* Camera::fromXML(XMLElement* xml) {
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

    Model* targetModel = nullptr;
    std::string targetId;
    GET_XML_ELEMENT(xml, "target", target);
    if (target != NULL) {
        GET_XML_ELEMENT_OR_FAIL(target, "model", _model);
        targetModel = Model::fromXML(_model);
        if (!targetModel) yeet std::string("Unable to load camera target.");

        GET_XML_ELEMENT_ATTRIB(target, "targetId", _targetId, const char*);
        if (_targetId != NULL)
            targetId = _targetId;
        else
            targetId = "<self>";
    }

    return new Camera(
        posX, posY, posZ,
        lookX, lookY, lookZ,
        upX, upY, upZ,
        fov, near, far,
        targetModel, targetId);
}

void Camera::load() {
    fuckAround {
        this->fallbackTarget->load();
    }
    findOut(std::string e) {
        yeet e;
    }
}

void Camera::exMovement(unsigned char key, int mx, int my) {
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
            return;
        }
    }
    this->cameras[0].posX = this->cameras[0].zoom * cos(this->cameras[0].alpha) * sin(this->cameras[0].beta);
    this->cameras[0].posY = this->cameras[0].zoom * sin(this->cameras[0].alpha);
    this->cameras[0].posZ = this->cameras[0].zoom * cos(this->cameras[0].alpha) * cos(this->cameras[0].beta);
}

void Camera::fpMovement(unsigned char key, int mx, int my) {
    float moveStep = this->MOVEMENT_SPEED;
    float dirX, dirY, dirZ;
    float dirX2, dirY2, dirZ2;

    switch (key) {
        case 'j':
        case 'J': {
            this->cameras[1].beta += this->ROTATION_SPEED;
            break;
        }
        case 'l':
        case 'L': {
            this->cameras[1].beta -= this->ROTATION_SPEED;
            break;
        }
        case 'i':
        case 'I': {
            if (this->cameras[1].alpha < M_PI / 2 - this->ROTATION_SPEED) this->cameras[1].alpha += this->ROTATION_SPEED;
            break;
        }
        case 'k':
        case 'K': {
            if (this->cameras[1].alpha > -M_PI / 2 + this->ROTATION_SPEED) this->cameras[1].alpha -= this->ROTATION_SPEED;
            break;
        }
    }

    dirX = cos(this->cameras[1].alpha) * sin(this->cameras[1].beta);
    dirY = sin(this->cameras[1].alpha);
    dirZ = cos(this->cameras[1].beta) * cos(this->cameras[1].alpha);

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
            break;
        }
    }

    this->cameras[1].lookX = this->cameras[1].posX + dirX;
    this->cameras[1].lookY = this->cameras[1].posY + dirY;
    this->cameras[1].lookZ = this->cameras[1].posZ + dirZ;
}

void Camera::tpMovement(unsigned char key, int mx, int my) {
    // Estatico
    if (!this->targetRef) {
        // std::cout << "estatico" << std::endl;
    }
    // Dinamico
    else {
        // std::cout << "dinamico" << std::endl;
    }
}

void Camera::processMovement(unsigned char key, int mx, int my) {
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

// TODO: This functions can probably go to Vector3
void Camera::cross(float dx, float dy, float dz, float fx, float fy, float fz, float& rx, float& ry, float& rz) {
    rx = dy * fz - dz * fy;
    ry = dz * fx - dx * fz;
    rz = dx * fy - dy * fx;
}

float Camera::dotProduct(float ax, float ay, float az, float bx, float by, float bz) {
    float dotProduct = ax * bx + ay * by + az * bz;

    return ax * bx + ay * by + az * bz;
}

void Camera::findPerpendicularVector(float x, float y, float z, float& px, float& py, float& pz) {
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