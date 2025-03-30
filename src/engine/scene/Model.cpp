#include "engine/scene/Model.hpp"

using namespace tinyxml2;

Model::Model(const char* source) {
    this->source = source;
    this->texture = std::string("");
    this->color = std::string("");
    this->geometry = nullptr;
    this->_loaded = false;
}

#pragma region ------- Overrides -------
void Model::setPosition(float x, float y, float z) {
    this->translation = Vector3(x, y, z);
};

void Model::setPosition(Vector3<float> pos) {
    this->translation = pos;
};

void Model::moveTo(float x, float y, float z) {
    if (this->translation.has_value()) {
        this->translation.value() += Vector3(x, y, z);
    } else {
        this->translation = Vector3(x, y, z);
    }
};

void Model::moveTo(Vector3<float> pos) {
    if (this->translation.has_value()) {
        this->translation.value() += pos;
    } else {
        this->translation = pos;
    }
};

/**
 * Sets the unit vector for the rotation axis.  
 * The axis is a unit vector, and it's parameters must be normalized.
 * 
 * @param axisX The X parameter of the axis vector.
 * @param axisY The Y parameter of the axis vector.
 * @param axisZ The Z parameter of the axis vector.
 */
void Model::setRotation(float axisX, float axisY, float axisZ) {
    if (this->rotation.has_value()) {
        this->rotation.value().first  = axisX;
        this->rotation.value().second = axisY;
        this->rotation.value().third  = axisZ;
    } else {
        this->rotation = Vector4(axisX, axisY, axisZ, 0.0f);
    }
}

// void Model::rotate(float angle) {
//     if (this->rotation.has_value()) {
//         this->rotation.value().fourth = angle;
//     }
// }

void Model::rotateAlong(float axisX, float axisY, float axisZ, float angle) {
    this->rotation = Vector4(axisX, axisY, axisZ, angle);
}

void Model::rotateAlong(Vector4<float> vec) {
    this->rotation = vec;
}

void Model::setScale(Vector3<float> sv) {
    this->scale = sv;
}

void Model::scaleTo(Vector3<float> sv) {
    if (this->scale.has_value()) {
        this->scale.value() += sv;
    } else {
        this->scale = sv;
    }
}

void Model::render() {
    std::vector<Point3D> vertices = this->geometry->getVertices();
    
    glPushMatrix();
    for (auto tt : this->tfStack) {
        switch (tt) {
            case TRANSFORM_TRANSLATE: {
                _TRANSFORM_IF3(glTranslatef, this->translation);
                break;
            }
            case TRANSFORM_ROTATE: {
                if (this->rotation.has_value()) {
                    glRotatef(
                        this->rotation.value().fourth,
                        this->rotation.value().first, 
                        this->rotation.value().second, 
                        this->rotation.value().third
                    );
                }
                break;
            }
            case TRANSFORM_SCALE: {
                _TRANSFORM_IF3(glScalef, this->scale);
                break;
            }
        }
    }
        
    glBegin(GL_TRIANGLES);
        for (Point3D vertex : vertices) {
            glVertex3f(vertex.getX(), vertex.getY(), vertex.getZ());
        }
    glEnd();
    glPopMatrix();
}
#pragma endregion ------- Overrides -------

const char* Model::getTexture() {
    return this->texture.c_str();
}

Model* Model::setTexture(const char *texture) {
    this->texture = texture;
    return this;
}

const char* Model::getColor() {
    return this->color.c_str();
}

Model* Model::setColor(const char *color) {
    this->color = color;
    return this;
}

Model* Model::fromXML(XMLElement* xml) {
    GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "file", file, const char*);
    GET_XML_ELEMENT_ATTRIB(xml, "texture", texture, const char*);
    GET_XML_ELEMENT_ATTRIB(xml, "color", color, const char*);

    // Load transforms, if they exist
    std::optional<Vector3<float>> translate = std::nullopt;
    std::optional<Vector4<float>> rotate = std::nullopt;
    std::optional<Vector3<float>> scale = std::nullopt;
    std::vector<TransformType> tfStack;

    GET_XML_ELEMENT(xml, "transform", _transform);
    if (_transform != NULL) {
        XMLElement* _tfProp = _transform->FirstChildElement();
        do {
            std::string name = std::string(_tfProp->Name());
            if (name == "translate") {
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "x", tx, float);
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "y", ty, float);
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "z", tz, float);

                translate = Vector3(tx, ty, tz);
                tfStack.push_back(TRANSFORM_TRANSLATE);
            } else if (name == "rotate") {
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "angle", rangle, float);
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "x", rx, float);
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "y", ry, float);
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "z", rz, float);

                rotate = Vector4(rx, ry, rz, rangle);
                tfStack.push_back(TRANSFORM_ROTATE);
            } else if (name == "scale") {
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "x", sx, float);
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "y", sy, float);
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "z", sz, float);

                scale = Vector3(sx, sy, sz);
                tfStack.push_back(TRANSFORM_SCALE);
            }
        } while ((_tfProp = _tfProp->NextSiblingElement()));
    }

    Model* obj = new Model(file);
    if (texture) obj->setTexture(texture);
    if (color) obj->setTexture(color);
    if (translate.has_value()) obj->setPosition(translate.value());
    if (rotate.has_value()) obj->rotateAlong(rotate.value());
    if (scale.has_value()) obj->setScale(scale.value());
    obj->setTfStack(tfStack);

    return obj;
}

void Model::load() {
    this->geometry = this->getOrLoadModel(this->source);
    this->_loaded = true;
}

Map<BaseGeometry*, std::string> Model::_geometryCache;
BaseGeometry* Model::getOrLoadModel(std::string modelName) {
    auto model = _geometryCache.get(modelName);
    if (model.has_value()) return model.value();

    // BaseGeometry* geometry = new BaseGeometry();
    BaseGeometry *geometry = nullptr;
    if (modelName.size() > 3 && modelName.substr(modelName.size() - 3) == ".3d") {
        geometry = Parser3D::load3DFile(modelName);
    } else if (modelName.size() > 4 && modelName.substr(modelName.size() - 4) == ".obj") {
        geometry = Parser3D::loadObjFile(modelName);
    } else {
        throw std::runtime_error("Unsupported file format: " + modelName);
    }

    if (geometry == nullptr) yeet std::string("Unable to load model.");

    _geometryCache.add(modelName, geometry);

    return geometry;
}