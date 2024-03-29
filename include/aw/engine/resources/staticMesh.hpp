#pragma once

#include <aw/graphics/core/bufferObject.hpp>
#include <aw/graphics/core/material.hpp>
#include <aw/graphics/core/transform.hpp>
#include <aw/graphics/core/vertexArrayObject.hpp>
#include <aw/util/math/vector.hpp>
#include <aw/util/types.hpp>

#include <vector>

namespace aw::engine
{
class StaticMesh
{
public:
  using Positions = std::vector<Vec3>;
  using Normals = std::vector<Vec3>;
  using UVChannel = std::vector<Vec2>;
  using Indices = std::vector<uint32>;
  using Materials = std::vector<Material>;

  struct SubMesh
  {
    uint32 indicesOffset;
    uint32 indicesCount;
    size_t materialIndex;
  };
  using SubMeshes = std::vector<SubMesh>;

public:
  // Those are on purpose by value, because normally you will only set them once
  void positions(Positions positions);
  void normals(Normals normals);
  size_t addUVChannel(UVChannel channel);
  void uvChannel(size_t index, UVChannel channel);
  void uvChannels(std::vector<UVChannel> channels);
  void indices(Indices indices);
  void materials(Materials materials);
  void subMeshes(SubMeshes subMeshes);

  size_t numUVChannels() const;
  size_t subMeshCount() const;

  const Positions& positions() const;
  const Normals& normals() const;
  const UVChannel& uvChannel(size_t index) const;
  const Indices& indices() const;
  const Materials& materials() const;
  const SubMeshes& subMeshes() const;
  Transform& transform();
  const Transform& transform() const;

  // This function will generate the GPU buffers
  void update();

  void bind();

private:
private:
  Positions mPositions;
  Normals mNormals;
  std::vector<UVChannel> mUVChannels;
  Transform mTransform;

  Indices mIndices;
  Materials mMaterials;

  SubMeshes mSubMeshes;

  VertexArrayObject mVAO;
  GPUBufferObject mVBO{BindType::ArrayBuffer};
  GPUBufferObject mIBO{BindType::ElementArrayBuffer};
};
} // namespace aw::engine
