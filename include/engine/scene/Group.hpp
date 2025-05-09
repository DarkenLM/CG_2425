#pragma once

#include <optional>
#include <string>
#include <vector>

#include "engine/scene/Model.hpp"

using namespace tinyxml2;

/**
 * @class Group
 * @brief Represents a group of 3D models and nested groups in a scene graph.
 *
 * Supports hierarchical transformations and animation via XML configuration.
 */
class Group : public Object {
   public:
    /**
     * @brief Default constructor.
     */
    Group();

    /**
     * @brief Constructs a Group with a given set of models.
     * @param objects Vector of Model pointers to include in the group.
     */
    Group(std::vector<Model*> objects);

    /**
     * @brief Constructs a Group with models and nested groups.
     * @param objects Vector of Model pointers.
     * @param groups Vector of Group pointers (nested groups).
     */
    Group(std::vector<Model*> objects, std::vector<Group*> groups);

    /**
     * @brief Constructs a Group from an XML element.
     * @param xml Pointer to the XML element defining the group.
     * @return Pointer to the created Group or nullptr on failure.
     */
    static Group* fromXML(XMLElement* xml);

    /**
     * @brief Gets the groups directly inside the object (this).
     * @return Const reference to vector of groups.
     */
    const std::vector<Group*>& getGroups() const;

    /**
     * @brief Gets the models contained in this group.
     * @return Const reference to vector of models.
     */
    const std::vector<Model*>& getObjects() const;

    /**
     * @brief Gets the model contained in this group with the specified id.
     * @return Const reference to the model, if it exists, nullptr otherwise.
     */
    const Model* getObjectById(const char* id) const;

    /**
     * @brief Gets the ordered list of transformations to apply.
     * @return Const reference to vector of transformation types.
     */
    const std::vector<TransformType>& getTransformations() const;

    /**
     * @brief Sets the translation transformation for the group.
     *
     * This replaces any existing translation, whether static or dynamic.
     *
     * @param t The new translation to apply. Use std::nullopt to remove the translation.
     */
    void setTranslation(std::optional<ObjectTranslation> t);

    /**
     * @brief Gets the current translation transformation of the group.
     *
     * @return An optional ObjectTranslation. std::nullopt if no translation is set.
     */
    std::optional<ObjectTranslation>& getTranslation();

    /**
     * @brief Sets the rotation transformation for the group.
     *
     * This replaces any existing rotation, whether static or dynamic.
     *
     * @param r The new rotation to apply. Use std::nullopt to remove the rotation.
     */
    void setRotation(std::optional<ObjectRotation> r);

    /**
     * @brief Gets the current rotation transformation of the group.
     *
     * @return An optional ObjectRotation. std::nullopt if no rotation is set.
     */
    std::optional<ObjectRotation>& getRotation();

    /**
     * @brief Sets the transformation stack for the group.
     *
     * This stack defines the order and types of transformations (translate, rotate, scale)
     * that should be applied when rendering this group.
     *
     * @param tfstack A vector of TransformType enums representing the transformation sequence.
     */
    void setTfStack(std::vector<TransformType> tfstack);

    /**
     * @brief Adds a model to the group.
     * @param object Pointer to the Model to add.
     */
    void addModel(Model* object);

    /**
     * @brief Adds multiple models to the group.
     * @param objects Vector of Model pointers to add.
     */
    void addModelsFrom(std::vector<Model*> objects);

    /**
     * @brief Replaces all models in the group.
     * @param objects Vector of Model pointers.
     */
    void setModels(std::vector<Model*> objects);

    /**
     * @brief Adds a nested group.
     * @param group Pointer to the Group to add.
     */
    void addGroup(Group* group);

    /**
     * @brief Adds multiple nested groups.
     * @param groups Vector of Group pointers to add.
     */
    void addGroupsFrom(std::vector<Group*> groups);

    /**
     * @brief Replaces all nested groups.
     * @param groups Vector of Group pointers.
     */
    void setGroups(std::vector<Group*> groups);

    /**
     * @brief Loads all models and nested groups, with error handling.
     * @throws std::string if a model or group fails to load.
     */
    void load();

    /**
     * @brief Sets static translation position.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param z Z coordinate.
     */
    void setPosition(float x, float y, float z);

    /**
     * @brief Sets static translation position.
     * @param pos 3D vector representing the new position.
     */
    void setPosition(Vector3<float> pos);

    /**
     * @brief Moves the group by a relative offset.
     * @param x Offset in X.
     * @param y Offset in Y.
     * @param z Offset in Z.
     */
    void moveTo(float x, float y, float z);

    /**
     * @brief Moves the group by a relative offset.
     * @param pos 3D vector representing the offset.
     */
    void moveTo(Vector3<float> pos);

    /**
     * @brief Sets the rotation axis with a zero angle (used for animation setup).
     * @param axisX X component of axis.
     * @param axisY Y component of axis.
     * @param axisZ Z component of axis.
     */
    void setRotation(float axisX, float axisY, float axisZ);

    /**
     * @brief Rotates the group by the given angle on the current axis.
     *        For animated rotations, updates the time-based rotation.
     * @param angle Rotation angle in degrees.
     */
    void rotate(float angle);

    /**
     * @brief Sets a new static rotation with angle and axis.
     * @param axisX X component of axis.
     * @param axisY Y component of axis.
     * @param axisZ Z component of axis.
     * @param angle Rotation angle in degrees.
     */
    void rotateAlong(float axisX, float axisY, float axisZ, float angle);

    /**
     * @brief Sets rotation using a 4D vector (angle + axis).
     * @param vec Vector containing (angle, x, y, z).
     */
    void rotateAlong(Vector4<float> vec);

    /**
     * @brief Sets the scale factors for the group.
     * @param sv Vector representing (scaleX, scaleY, scaleZ).
     */
    void setScale(Vector3<float> sv);

    /**
     * @brief Scales the group incrementally by a given vector.
     * @param sv Vector representing incremental scale values.
     */
    void scaleTo(Vector3<float> sv);

    // Override render method from Object
    void render() override;

   private:
    /**
     * @brief Models contained in this group.
     */
    std::vector<Model*> objects;

    /**
     * @brief Nested child groups.
     */
    std::vector<Group*> groups;

    /**
     * @brief Optional translation (static or dynamic).
     */
    std::optional<ObjectTranslation> translation = std::nullopt;

    /**
     * @brief Optional rotation (static or time-based).
     */
    std::optional<ObjectRotation> rotation = std::nullopt;

    /**
     * @brief Optional scaling vector.
     */
    std::optional<Vector3<float>> scale = std::nullopt;

    /**
     * @brief Ordered list of transformations to apply.
     */
    std::vector<TransformType> tfStack;
};
