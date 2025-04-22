#pragma once

#include "common/geometry/BaseGeometry.hpp"
#include "common/util/xmlutil.hpp"
#include "engine/scene/Model.hpp"
#include "engine/scene/Object.hpp"

using namespace tinyxml2;

// #define _TRANSFORM_IF3(transformer, source) if (source.has_value()) \
//     transformer(source.value().first, source.value().second, source.value().third)

// #define _TRANSFORM_IF4(transformer, source) if (source.has_value()) \
//     transformer(source.value().first, source.value().second, source.value().third, source.value().fourth)

// typedef enum transform_type {
//     TRANSFORM_RESET_ALL,
//     TRANSFORM_TRANSLATE,
//     TRANSFORM_TRANSLATE_RESET,
//     TRANSFORM_ROTATE,
//     TRANSFORM_ROTATE_RESET,
//     TRANSFORM_SCALE,
//     TRANSFORM_SCALE_RESET
// } TransformType;

class Group : public Object {
   public:
    Group() = default;
    Group(std::vector<Model*> objects) {
        this->objects = objects;
    };
    Group(std::vector<Model*> objects, std::vector<Group*> groups) {
        this->objects = objects;
        this->groups = groups;
    };

    // TODO: Read nested groups
    static Group* fromXML(XMLElement* xml) {
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
                    // GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "x", tx, float);
                    // GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "y", ty, float);
                    // GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "z", tz, float);

                    // translate = Vector3(tx, ty, tz);
                    // tfStack.push_back(TRANSFORM_TRANSLATE);

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
        if (translate.has_value()) retGroup->translation = translate;
        if (rotate.has_value()) retGroup->rotation = rotate;
        if (scale.has_value()) retGroup->setScale(scale.value());
        retGroup->setTfStack(tfStack);

        return retGroup;
    }

    void addModel(Model* object) {
        this->objects.push_back(object);
    }

    void addModelsFrom(std::vector<Model*> objects) {
        this->objects.insert(this->objects.end(), objects.begin(), objects.end());
    }

    void setModels(std::vector<Model*> objects) {
        this->objects = objects;
    }

    void addGroup(Group* group) {
        this->groups.push_back(group);
    }

    void addGroupsFrom(std::vector<Group*> groups) {
        this->groups.insert(this->groups.end(), groups.begin(), groups.end());
    }

    void setGroups(std::vector<Group*> groups) {
        this->groups = groups;
    }

    void load() {
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
    // void setPosition(float x, float y, float z) {
    //     this->translation = Vector3(x, y, z);
    // };

    // void setPosition(Vector3<float> pos) {
    //     this->translation = pos;
    // };

    // void moveTo(float x, float y, float z) {
    //     if (this->translation.has_value()) {
    //         this->translation.value() += Vector3(x, y, z);
    //     } else {
    //         this->translation = Vector3(x, y, z);
    //     }
    // };

    // void moveTo(Vector3<float> pos) {
    //     if (this->translation.has_value()) {
    //         this->translation.value() += pos;
    //     } else {
    //         this->translation = pos;
    //     }
    // };

    void setPosition(float x, float y, float z) {
        if (this->translation.has_value()) {
            if (!this->translation.value().isDynamic()) this->translation = ObjectTranslation(x, y, z);
        } else {
            this->translation = ObjectTranslation(x, y, z);
        }
    };

    void setPosition(Vector3<float> pos) {
        if (this->translation.has_value()) {
            if (!this->translation.value().isDynamic()) this->translation = ObjectTranslation(pos.first, pos.second, pos.third);
        } else {
            this->translation = ObjectTranslation(pos.first, pos.second, pos.third);
        }
    };

    void moveTo(float x, float y, float z) {
        if (this->translation.has_value()) {
            if (!this->translation.value().isDynamic()) this->translation.value() += Vector3<float>(x, y, z);
        } else {
            this->translation = ObjectTranslation(x, y, z);
        }
    };

    void moveTo(Vector3<float> pos) {
        if (this->translation.has_value()) {
            if (!this->translation.value().isDynamic()) this->translation.value() += pos;
        } else {
            this->translation = ObjectTranslation(pos.first, pos.second, pos.third);
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
    void setRotation(float axisX, float axisY, float axisZ) {
        // if (this->rotation.has_value()) {
        //     this->rotation.value().first  = axisX;
        //     this->rotation.value().second = axisY;
        //     this->rotation.value().third  = axisZ;
        // } else {
        //     this->rotation = Vector4(axisX, axisY, axisZ, 0.0f);
        // }

        this->rotation = ObjectRotation(0.0f, axisX, axisY, axisZ);
    }

    void rotate(float angle) {
        if (this->rotation.has_value()) {
            if (this->rotation.value().isDynamic())
                this->rotation.value().setTime(angle);
            else
                this->rotation.value().setAngle(angle);
        }
    }

    void rotateAlong(float axisX, float axisY, float axisZ, float angle) {
        // this->rotation = Vector4(axisX, axisY, axisZ, angle);
        if (this->rotation.has_value()) {
            if (!this->rotation.value().isDynamic()) this->rotation = ObjectRotation(angle, axisX, axisY, axisZ);
        } else {
            this->rotation = ObjectRotation(angle, axisX, axisY, axisZ);
        }
    }

    void rotateAlong(Vector4<float> vec) {
        if (this->rotation.has_value()) {
            if (!this->rotation.value().isDynamic()) {
                this->rotation = ObjectRotation(this->rotation.value().getAngle(), vec.first, vec.second, vec.third);
            }
        } else {
            this->rotation = ObjectRotation(0.0f, vec.first, vec.second, vec.third);
        }
    }

    void setScale(Vector3<float> sv) {
        this->scale = sv;
    }

    void scaleTo(Vector3<float> sv) {
        if (this->scale.has_value()) {
            this->scale.value() += sv;
        } else {
            this->scale = sv;
        }
    }

    void render() override {
        // TODO: Push transformation matrices and pop them at the end.
        glPushMatrix();
        // _TRANSFORM_IF3(glTranslatef, this->translation);
        // // _TRANSFORM_IF4(glRotatef, this->rotation);
        // if (this->rotation.has_value()) {
        //     glRotatef(
        //         this->rotation.value().fourth,
        //         this->rotation.value().first,
        //         this->rotation.value().second,
        //         this->rotation.value().third
        //     );
        // }
        // _TRANSFORM_IF3(glScalef, this->scale);

        for (auto tt : this->tfStack) {
            switch (tt) {
                case TRANSFORM_TRANSLATE: {
                    // _TRANSFORM_IF3(glTranslatef, this->translation);
                    if (this->translation.has_value()) {
                        // TODO: Dynamic translation
                        if (this->translation.value().isDynamic()) {
                            if (STATE.showCurves) {
                                std::cout << "As curvas nao devem aparecer" << std::endl;
                            } else {
                                std::cout << "As curvas devem aparecer" << std::endl;
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
                        // glRotatef(
                        //     this->rotation.value().fourth,
                        //     this->rotation.value().first,
                        //     this->rotation.value().second,
                        //     this->rotation.value().third
                        // );

                        if (this->rotation.value().isDynamic()) {
                            std::cout << "Can rotate yet" << std::endl;
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

   private:
    std::vector<Model*> objects;
    std::vector<Group*> groups;

    // // Transform
    // std::optional<Vector3<float>> translation;
    // std::optional<Vector4<float>> rotation;
    // std::optional<Vector3<float>> scale;
    // std::vector<TransformType> tfStack;

    // void setTfStack(std::vector<TransformType> tfStack) {
    //     this->tfStack = tfStack;
    // }

    // protected:
    //     // Transform
    //     std::optional<Vector3<float>> translation;
    //     std::optional<Vector4<float>> rotation;
    //     std::optional<Vector3<float>> scale;
    //     std::vector<TransformType> tfStack;

    //     void setTfStack(std::vector<TransformType> tfStack);
};