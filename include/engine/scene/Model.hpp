#pragma once

#include <unordered_map>

#include "common/geometry/BaseGeometry.hpp"
#include "common/parser.hpp"
#include "common/util/maputil.hpp"
#include "common/util/xmlutil.hpp"
#include "engine/geometry/GeometryCatalog.hpp"
#include "engine/scene/Object.hpp"

using namespace tinyxml2;

/**
 * @class Model
 * @brief Represents a 3D model in the scene with transformation and rendering capabilities.
 */
class Model : public Object {
   public:
    /**
     * @brief Construct a new Model object from a source filename.
     * @param source Path to the model file.
     */
    Model(const char* source);

#pragma region------- Overrides -------
    /**
     * @brief Set the model's position.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param z Z coordinate.
     */
    void setPosition(float x, float y, float z) override;

    /**
     * @brief Set the model's position using a vector.
     * @param pos Position as a Vector3.
     */
    void setPosition(Vector3<float> pos) override;

    /**
     * @brief Move the model to a new position.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param z Z coordinate.
     */
    void moveTo(float x, float y, float z) override;

    /**
     * @brief Move the model to a new position using a vector.
     * @param pos Target position.
     */
    void moveTo(Vector3<float> pos) override;

    /**
     * @brief Set the model's rotation axis.
     * @param axisX Rotation axis X.
     * @param axisY Rotation axis Y.
     * @param axisZ Rotation axis Z.
     */
    void setRotation(float axisX, float axisY, float axisZ) override;

    /**
     * @brief Sets the transformation stack for the Model.
     *
     * This stack defines the order and types of transformations (translate, rotate, scale)
     * that should be applied when rendering this model.
     *
     * @param tfstack A vector of TransformType enums representing the transformation sequence.
     */
    void setTfStack(std::vector<TransformType> tfstack);

    /**
     * @brief Rotate the model along a specific axis and angle.
     * @param axisX Axis X.
     * @param axisY Axis Y.
     * @param axisZ Axis Z.
     * @param angle Rotation angle in degrees.
     */
    void rotateAlong(float axisX, float axisY, float axisZ, float angle) override;

    /**
     * @brief Rotate the model using a rotation vector.
     * @param vec Vector containing axis and angle (x, y, z, angle).
     */
    void rotateAlong(Vector4<float> vec) override;

    /**
     * @brief Set the model's scale.
     * @param sv Scale vector.
     */
    void setScale(Vector3<float> sv) override;

    /**
     * @brief Scale the model to the given size.
     * @param sv Target scale.
     */
    void scaleTo(Vector3<float> sv) override;

    /**
     * @brief Render the model.
     */
    void render() override;
    /// @}
#pragma endregion-- -- -- -Overrides-- -- -- -
    /**
     * @brief Get the model source path.
     * @return The model's source.
     */
    const char* getSource();

    /**
     * @brief Get the texture.
     */
    const char* getTexture();

    /**
     * @brief Set the texture.
     */
    Model* setTexture(const char* texture);

    /**
     * @brief Get the color.
     */
    const char* getColor();

    /**
     * @brief Set the color.
     */
    Model* setColor(const char* color);

    /**
     * @brief Load a Model from an XML element.
     * @param xml The XML element.
     * @return A new Model instance or nullptr on error.
     */
    static Model* fromXML(XMLElement* xml);

    /**
     * @brief Loads the model data into memory (e.g., VBO/IBO).
     */
    void load();

    /**
     * @brief Get the shared geometry cache (model file to geometry pointer).
     */
    static Map<GLuint, std::string>& getGeometryVBO();

    /**
     * @brief Get the VBO map.
     */
    static Map<GLuint, std::string>& getGeometryIBO();

    /**
     * @brief Get the IBO map.
     */
    static Map<BaseGeometry*, std::string> getGeometryCache();

   private:
    /**
     * @brief Source path of the model file.
     */
    std::string source;

    /**
     * @brief Texture file path, if any.
     */
    std::string texture;

    /**
     * @brief Color value as a string (could be name or hex code).
     */
    std::string color;

    /**
     * @brief Pointer to the loaded geometry object.
     */
    BaseGeometry* geometry;

    /**
     * @brief Indicates whether the model is already loaded.
     */
    bool _loaded;

    /**
     * @brief Global cache of model source to BaseGeometry pointer.
     */
    static Map<BaseGeometry*, std::string> _geometryCache;

    /**
     * @brief Global map of VBOs (Vertex Buffer Objects).
     */
    static Map<GLuint, std::string> _geometryVBO;

    /**
     * @brief Global map of IBOs (Index Buffer Objects).
     */
    static Map<GLuint, std::string> _geometryIBO;

    /**
     * @brief Retrieves geometry from cache or loads from file.
     * @param modelName Name or path of the model to load.
     * @return Pointer to BaseGeometry instance.
     */
    static BaseGeometry* getOrLoadModel(std::string modelName);

   protected:
};
