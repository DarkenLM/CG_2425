#include "engine/scene/Group.hpp"

#include "common/geometry/BaseGeometry.hpp"
#include "common/util/xmlutil.hpp"
#include "engine/scene/Model.hpp"

using namespace tinyxml2;

Group::Group() = default;

Group::Group(std::vector<Model*> objects) {
    this->objects = objects;
};

Group::Group(std::vector<Model*> objects, std::vector<Group*> groups) {
    this->objects = objects;
    this->groups = groups;
};

// TODO: Read nested groups
Group* Group::fromXML(XMLElement* xml) {
    // GET_XML_ELEMENT_OR_FAIL(xml, "transform", transform);

    // Load all models from the "models" tag.
    GET_XML_ELEMENT(xml, "models", modelsElem);
    std::vector<Model*> models;
    if (modelsElem != NULL) {
        GET_XML_ELEMENT(modelsElem, "model", _model);
        do {
            if (!_model) break;  // Element does not exist in XML.
            Model* model = Model::fromXML(_model);
            if (!model) return nullptr;  // Element exists, but has errors.1

            models.push_back(model);
        } while ((_model = _model->NextSiblingElement("model")));
    }

    // Load all group children
    std::vector<Group*> groups;
    GET_XML_ELEMENT(xml, "group", _group);
    do {
        if (!_group) break;  // Element does not exist in XML.
        Group* group = Group::fromXML(_group);
        if (!group) return nullptr;  // Element exists, but has errors.

        groups.push_back(group);
    } while ((_group = _group->NextSiblingElement("group")));

    // Load transforms, if they exist
    // std::optional<Vector3<float>> translate = std::nullopt;
    std::optional<ObjectTranslation> translate = std::nullopt;
    // std::optional<Vector4<float>> rotate = std::nullopt;
    std::optional<ObjectRotation> rotate = std::nullopt;
    std::optional<Vector3<float>> scale = std::nullopt;
    std::vector<TransformType> tfStack;

    GET_XML_ELEMENT(xml, "transform", _transform);
    if (_transform != NULL) {
        XMLElement* _tfProp = _transform->FirstChildElement();
        do {
            std::string name = std::string(_tfProp->Name());
            if (name == "translate") {
                ObjectTranslation _trans;

                GET_XML_ELEMENT_ATTRIB(_tfProp, "time", ttime, int)
                if (ttime != 0) {
                    GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "align", talign, bool);

                    std::vector<Point3D> points;
                    GET_XML_ELEMENT(_tfProp, "point", _point);
                    do {
                        if (!_point) break;  // Element does not exist in XML.
                        GET_XML_ELEMENT_ATTRIB_OR_FAIL(_point, "x", px, float);
                        GET_XML_ELEMENT_ATTRIB_OR_FAIL(_point, "y", py, float);
                        GET_XML_ELEMENT_ATTRIB_OR_FAIL(_point, "z", pz, float);

                        Point3D point = Point3D(px, py, pz);
                        points.push_back(point);
                    } while ((_point = _point->NextSiblingElement("point")));

                    _trans = ObjectTranslation(ttime, talign, points);
                } else {
                    GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "x", tx, float);
                    GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "y", ty, float);
                    GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "z", tz, float);

                    // translate = Vector3(tx, ty, tz);
                    _trans = ObjectTranslation(tx, ty, tz);
                }

                translate = _trans;
                tfStack.push_back(TRANSFORM_TRANSLATE);
            } else if (name == "rotate") {
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "x", rx, float);
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "y", ry, float);
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "z", rz, float);

                GET_XML_ELEMENT_ATTRIB(_tfProp, "angle", rangle, float);
                if (rangle != 0) {
                    rotate = ObjectRotation(rangle, rx, ry, rz);
                } else {
                    GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "time", rtime, int);
                    rotate = ObjectRotation(rtime, rx, ry, rz);
                }
                // rotate = Vector4(rx, ry, rz, rangle);
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

    // Instantize a new instance
    Group* retGroup = new Group(models, groups);
    if (translate.has_value()) retGroup->setTranslation(translate);
    if (rotate.has_value()) retGroup->setRotation(rotate);
    if (scale.has_value()) retGroup->setScale(scale.value());
    retGroup->setTfStack(tfStack);

    return retGroup;
}

std::optional<Vector3<float>>& Group::getScale() {
    return this->scale;
};

const std::vector<Group*>& Group::getGroups() const {
    return this->groups;
}

const std::vector<Model*>& Group::getObjects() const {
    return this->objects;
}

const Model* Group::getObjectById(const char* id) const {
    for (const auto& object : this->objects) {
        if (object->getId().compare(id) == 0) return object;
    }

    for (const auto& group : this->groups) {
        const Model* target = group->getObjectById(id);
        if (target != nullptr) return target;
    }

    return nullptr;
}

const std::vector<TransformType>& Group::getTransformations() const {
    return this->tfStack;
}

std::optional<ObjectTranslation>& Group::getTranslation() {
    return this->translation;
};

void Group::setTranslation(std::optional<ObjectTranslation> t) {
    this->translation = t;
}

std::optional<ObjectRotation>& Group::getRotation() {
    return this->rotation;
};

void Group::setRotation(std::optional<ObjectRotation> r) {
    this->rotation = r;
}

void Group::setTfStack(std::vector<TransformType> tfstack) {
    this->tfStack = tfstack;
}

void Group::addModel(Model* object) {
    this->objects.push_back(object);
}

void Group::addModelsFrom(std::vector<Model*> objects) {
    this->objects.insert(this->objects.end(), objects.begin(), objects.end());
}

void Group::setModels(std::vector<Model*> objects) {
    this->objects = objects;
}

void Group::addGroup(Group* group) {
    this->groups.push_back(group);
}

void Group::addGroupsFrom(std::vector<Group*> groups) {
    this->groups.insert(this->groups.end(), groups.begin(), groups.end());
}

void Group::setGroups(std::vector<Group*> groups) {
    this->groups = groups;
}

void Group::load() {
    for (Model* m : this->objects) {
        fuckAround {
            m->load();
        }
        findOut(std::string e) {
            yeet e;
        }
    }

    for (Group* g : this->groups) {
        fuckAround {
            g->load();
        }
        findOut(std::string e) {
            yeet std::string("Unable to load subgroup: ") + e;
        }
    }
}

#pragma region------- Overrides -------

void Group::setPosition(float x, float y, float z) {
    if (this->translation.has_value()) {
        if (!this->translation.value().isDynamic()) this->translation = ObjectTranslation(x, y, z);
    } else {
        this->translation = ObjectTranslation(x, y, z);
    }
};

void Group::setPosition(Vector3<float> pos) {
    if (this->translation.has_value()) {
        if (!this->translation.value().isDynamic()) this->translation = ObjectTranslation(pos.first, pos.second, pos.third);
    } else {
        this->translation = ObjectTranslation(pos.first, pos.second, pos.third);
    }
};

void Group::moveTo(float x, float y, float z) {
    if (this->translation.has_value()) {
        if (!this->translation.value().isDynamic()) this->translation.value() += Vector3<float>(x, y, z);
    } else {
        this->translation = ObjectTranslation(x, y, z);
    }
};

void Group::moveTo(Vector3<float> pos) {
    if (this->translation.has_value()) {
        if (!this->translation.value().isDynamic()) this->translation.value() += pos;
    } else {
        this->translation = ObjectTranslation(pos.first, pos.second, pos.third);
    }
};

void Group::setRotation(float axisX, float axisY, float axisZ) {
    this->rotation = ObjectRotation(0.0f, axisX, axisY, axisZ);
}

void Group::rotate(float angle) {
    if (this->rotation.has_value()) {
        if (this->rotation.value().isDynamic())
            this->rotation.value().setTime(angle);
        else
            this->rotation.value().setAngle(angle);
    }
}

void Group::rotateAlong(float axisX, float axisY, float axisZ, float angle) {
    // this->rotation = Vector4(axisX, axisY, axisZ, angle);
    if (this->rotation.has_value()) {
        if (!this->rotation.value().isDynamic()) this->rotation = ObjectRotation(angle, axisX, axisY, axisZ);
    } else {
        this->rotation = ObjectRotation(angle, axisX, axisY, axisZ);
    }
}

void Group::rotateAlong(Vector4<float> vec) {
    if (this->rotation.has_value()) {
        if (!this->rotation.value().isDynamic()) {
            this->rotation = ObjectRotation(this->rotation.value().getAngle(), vec.first, vec.second, vec.third);
        }
    } else {
        this->rotation = ObjectRotation(0.0f, vec.first, vec.second, vec.third);
    }
}

void Group::setScale(Vector3<float> sv) {
    this->scale = sv;
}

void Group::scaleTo(Vector3<float> sv) {
    if (this->scale.has_value()) {
        this->scale.value() += sv;
    } else {
        this->scale = sv;
    }
}

void Group::render() {
    // TODO: Push transformation matrices and pop them at the end.
    glPushMatrix();

    for (auto tt : this->tfStack) {
        switch (tt) {
            case TRANSFORM_TRANSLATE: {
                // _TRANSFORM_IF3(glTranslatef, this->translation);
                if (this->translation.has_value()) {
                    if (this->translation.value().isDynamic()) {
                        float pos[3], deriv[3];
                        float rotationMatrix[4][4];
                        float xAxis[3], yAxis[3], zAxis[3];

                        float elapsedTime = (float)glutGet(GLUT_ELAPSED_TIME) / (1000.0f * this->translation.value().getTime());
                        this->translation.value().getInterpolatedPosition(pos, deriv, elapsedTime);
                        this->translation.value().getCurrentRotation(xAxis, yAxis, zAxis, deriv, &rotationMatrix[0][0]);

                        // TODO: Implement the ui for this
                        if (this->translation.value().getShowCurve()) {
                            this->translation.value().getCurve().renderCatmullRomCurve();
                        }

                        // TODO: Implement the ui for this
                        if (true) {
                            this->translation.value().drawMyCordSystem(pos, xAxis, yAxis, zAxis);
                        }

                        glTranslatef(pos[0], pos[1], pos[2]);

                        if (this->translation.value().getAlign()) {
                            glMultMatrixf(&rotationMatrix[0][0]);
                        }
                    } else {
                        Vector3<float> vec = this->translation.value().getVector();
                        glTranslatef(vec.first, vec.second, vec.third);
                    }
                }
                break;
            }
            case TRANSFORM_ROTATE: {
                if (this->rotation.has_value()) {
                    if (this->rotation.value().isDynamic()) {
                        float elapsedTime = (float)glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
                        float t = this->rotation.value().getTime();  // Duration in seconds to complete 360
                        float angle = fmodf((elapsedTime / t) * 360.0f, 360.0f);
                        this->rotation.value().setAngle(angle);

                        Vector4<float> vec = this->rotation.value().getVector4();
                        glRotatef(vec.first, vec.second, vec.third, vec.fourth);
                    } else {
                        Vector4<float> vec = this->rotation.value().getVector4();
                        glRotatef(vec.first, vec.second, vec.third, vec.fourth);
                    }
                }
                break;
            }
            case TRANSFORM_SCALE: {
                _TRANSFORM_IF3(glScalef, this->scale);
                break;
            }
        }
    }

    for (Model* m : this->objects) {
        m->render();
    }

    for (Group* g : this->groups) {
        g->render();
    }
    glPopMatrix();
};

#pragma endregion-- -- -- -Overrides-- -- -- -
