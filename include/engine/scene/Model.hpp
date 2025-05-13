#pragma once

#include <unordered_map>

#include "common/geometry/BaseGeometry.hpp"
#include "common/geometry/GeometryCatalog.hpp"
#include "common/parser.hpp"
#include "common/util/maputil.hpp"
#include "common/util/xmlutil.hpp"
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
    Model(const char* source, const char* id);

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

    // TODO: DOXYGEN
    void processNormals();

    void loadingMaterial();

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
    const char* getSource() const;

    /**
     * @brief Get the boolean defining if normals are visible or not.
     * @return A boolean value.
     */
    const bool getShowNormals() const;

    // TODO doxygen
    bool* getShowNormalsPtr();

    /**
     * @brief Set the boolean defining if normals are visible or not.
     * @return A boolean value.
     */
    const bool setShowNormals();

    /**
     * @brief Get the models transformation stack.
     * @return The model's tfStack.
     */
    std::vector<TransformType> getTrasnformations() const;

    /**
     * @brief Retrieves the object's translation component, if available.
     *
     * @return Optional containing ObjectTranslation if present.
     */
    std::optional<ObjectTranslation> getTranslation() const;

    /**
     * @brief Retrieves the object's rotation component, if available.
     *
     * @return Optional containing ObjectRotation if present.
     */
    std::optional<ObjectRotation> getRotation() const;

    /**
     * @brief Retrieves the object's scaling factor, if available.
     *
     * @return Optional containing a Vector3<float> if scale is present.
     */
    std::optional<Vector3<float>> getScale() const;

    /**
     * @brief Retrieves the object's material properties, if available.
     *
     * @return Optional containing ObjectMaterial if present.
     */
    std::optional<ObjectMaterial> getMaterial() const;

    /**
     * @brief Get the texture.
     */
    const char* getTexture();

    /**
     * @brief Set the texture.
     */
    Model* setTexture(const char* texture);

    // TODO:: DOXY
    unsigned int getTextureId() const;

    void setTextureId(unsigned int id);

    /**
     * @brief Get the color.
     */
    const char* getColor();

    /**
     * @brief Set the color.
     */
    Model* setColor(const char* color);

    /**
     * @brief Get the model id.
     * @return The model's id.
     * @warning This method does not return the id of the associated global geometry. It returns the id of the model
     * instance, as defined on the XML source.
     */
    std::string getId();

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
     * @brief Get the VBO map.
     */
    static Map<GLuint, std::string>& getGeometryVBO();

    /**
     * @brief Get the TBO map.
     */
    static Map<GLuint, std::string>& getGeometryTBO();

    /**
     * @brief Get the IBO map.
     */
    static Map<GLuint, std::string>& getGeometryIBO();

    /**
     * @brief Get the NBO map.
     */
    static Map<GLuint, std::string>& getGeometryNBO();

    /**
     * @brief Get the shared geometry cache (model file to geometry pointer).
     */
    static Map<BaseGeometry*, std::string> getGeometryCache();

    /**
     * @brief Texture Id, if any.
     */
    GLuint textureId;

   private:
    /**
     * @brief Source path of the model file.
     */
    bool showNormals;

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
     * @brief The unique identifier for this model.
     */
    std::string id;

    /**
     * @brief Global cache of model source to BaseGeometry pointer.
     */
    static Map<BaseGeometry*, std::string> _geometryCache;

    /**
     * @brief Global map of VBOs (Vertex Buffer Objects).
     */
    static Map<GLuint, std::string> _geometryVBO;

    /**
     * @brief Global map of TBOs (Texture coords Buffer Objects).
     */
    static Map<GLuint, std::string> _geometryTBO;

    /**
     * @brief Global map of NBOs (Normal Buffer Objects).
     */
    static Map<GLuint, std::string> _geometryNBO;

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
