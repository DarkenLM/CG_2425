#pragma once

#include "common/geometry/BaseGeometry.hpp"
#include "common/util/xmlutil.hpp"
#include "engine/scene/Model.hpp"
#include "engine/scene/Object.hpp"

using namespace tinyxml2;

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

            GET_XML_ELEMENT_OR_FAIL(xml, "models", modelsElem);
            std::vector<Model*> models;
            GET_XML_ELEMENT(modelsElem, "model", _model);
            do {
                if (!_model) break; // Element does not exist in XML.
                Model* model = Model::fromXML(_model);
                if (!model) return nullptr; // Element exists, but has errors.1

                models.push_back(model);
            } while ((_model = _model->NextSiblingElement("model")));

            std::vector<Group*> groups;
            GET_XML_ELEMENT(xml, "group", _group);
            do {
                if (!_group) break; // Element does not exist in XML.
                Group* group = Group::fromXML(_group);
                if (!group) return nullptr; // Element exists, but has errors.

                groups.push_back(group);
            } while ((_group = _group->NextSiblingElement("group")));

            Group* retGroup = new Group(models, groups);
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
        void render() override {
            for (Model* m : this->objects) {
                m->render();
            }

            for (Group* g : this->groups) {
                g->render();
            }
        };
        #pragma endregion ------- Overrides -------

    private:
        std::vector<Model*> objects;
        std::vector<Group*> groups;
};