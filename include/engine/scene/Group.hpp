#pragma once

#include "common/geometry/BaseGeometry.hpp"
#include "common/util/xmlutil.hpp"
#include "engine/scene/Model.hpp"
#include "engine/scene/Object.hpp"

using namespace tinyxml2;

#define _TRANSFORM_IF3(transformer, source) if (source.has_value()) \
    transformer(source.value().first, source.value().second, source.value().third)

#define _TRANSFORM_IF4(transformer, source) if (source.has_value()) \
    transformer(source.value().first, source.value().second, source.value().third, source.value().fourth)

typedef enum transform_type {
    TRANSFORM_RESET_ALL,
    TRANSFORM_TRANSLATE,
    TRANSFORM_TRANSLATE_RESET,
    TRANSFORM_ROTATE,
    TRANSFORM_ROTATE_RESET,
    TRANSFORM_SCALE,
    TRANSFORM_SCALE_RESET
} TransformType;

class Group: public Object {
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
                    if (!_model) break; // Element does not exist in XML.
                    Model* model = Model::fromXML(_model);
                    if (!model) return nullptr; // Element exists, but has errors.1

                    models.push_back(model);
                } while ((_model = _model->NextSiblingElement("model")));
            }
            
            // Load all group children
            std::vector<Group*> groups;
            GET_XML_ELEMENT(xml, "group", _group);
            do {
                if (!_group) break; // Element does not exist in XML.
                Group* group = Group::fromXML(_group);
                if (!group) return nullptr; // Element exists, but has errors.

                groups.push_back(group);
            } while ((_group = _group->NextSiblingElement("group")));

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

                // GET_XML_ELEMENT(_transform, "translate", _translate);
                // if (_translate != NULL) {
                //     GET_XML_ELEMENT_ATTRIB_OR_FAIL(_translate, "x", tx, float);
                //     GET_XML_ELEMENT_ATTRIB_OR_FAIL(_translate, "y", ty, float);
                //     GET_XML_ELEMENT_ATTRIB_OR_FAIL(_translate, "z", tz, float);

                //     translate = Vector3(tx, ty, tz);
                // }

                // GET_XML_ELEMENT(_transform, "rotate", _rotate);
                // if (_rotate != NULL) {
                //     GET_XML_ELEMENT_ATTRIB_OR_FAIL(_rotate, "angle", rangle, float);
                //     GET_XML_ELEMENT_ATTRIB_OR_FAIL(_rotate, "x", rx, float);
                //     GET_XML_ELEMENT_ATTRIB_OR_FAIL(_rotate, "y", ry, float);
                //     GET_XML_ELEMENT_ATTRIB_OR_FAIL(_rotate, "z", rz, float);

                //     rotate = Vector4(rx, ry, rz, rangle);
                // }

                // GET_XML_ELEMENT(_transform, "scale", _scale);
                // if (_scale != NULL) {
                //     GET_XML_ELEMENT_ATTRIB_OR_FAIL(_scale, "x", sx, float);
                //     GET_XML_ELEMENT_ATTRIB_OR_FAIL(_scale, "y", sy, float);
                //     GET_XML_ELEMENT_ATTRIB_OR_FAIL(_scale, "z", sz, float);

                //     scale = Vector3(sx, sy, sz);
                // }
            }

            // Instantize a new instance
            Group* retGroup = new Group(models, groups);
            if (translate.has_value()) retGroup->setPosition(translate.value());
            if (rotate.has_value()) retGroup->rotateAlong(rotate.value());
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
                } findOut(std::string e) {
                    yeet e;
                }
            }

            for (Group* g : this->groups) {
                fuckAround {
                    g->load();
                } findOut(std::string e) {
                    yeet std::string("Unable to load subgroup: ") + e;
                }
            }
        }

        #pragma region ------- Overrides -------
        void setPosition(float x, float y, float z) {
            this->translation = Vector3(x, y, z);
        };

        void setPosition(Vector3<float> pos) {
            this->translation = pos;
        };

        void moveTo(float x, float y, float z) {
            if (this->translation.has_value()) {
                this->translation.value() += Vector3(x, y, z);
            } else {
                this->translation = Vector3(x, y, z);
            }
        };

        void moveTo(Vector3<float> pos) {
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
        void setRotation(float axisX, float axisY, float axisZ) {
            if (this->rotation.has_value()) {
                this->rotation.value().first  = axisX;
                this->rotation.value().second = axisY;
                this->rotation.value().third  = axisZ;
            } else {
                this->rotation = Vector4(axisX, axisY, axisZ, 0.0f);
            }
        }

        void rotate(float angle) {
            if (this->rotation.has_value()) {
                this->rotation.value().fourth = angle;
            }
        }

        void rotateAlong(float axisX, float axisY, float axisZ, float angle) {
            this->rotation = Vector4(axisX, axisY, axisZ, angle);
        }

        void rotateAlong(Vector4<float> vec) {
            this->rotation = vec;
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
            
            for (Model* m : this->objects) {
                m->render();
            }

            for (Group* g : this->groups) {
                g->render();
            }
            glPopMatrix();
        };
        #pragma endregion ------- Overrides -------

    private:
        std::vector<Model*> objects;
        std::vector<Group*> groups;
        
        // Transform
        std::optional<Vector3<float>> translation;
        std::optional<Vector4<float>> rotation;
        std::optional<Vector3<float>> scale;
        std::vector<TransformType> tfStack;

        void setTfStack(std::vector<TransformType> tfStack) {
            this->tfStack = tfStack;
        }
};