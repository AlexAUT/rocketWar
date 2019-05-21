#include <aw/engine/resources/loaders/assimpLoader.hpp>

#include <aw/engine/resources/staticMesh.hpp>
#include <aw/util/file/converters.hpp>
#include <aw/util/file/inputStream.hpp>
#include <aw/util/file/path.hpp>
#include <aw/util/log/log.hpp>
#include <aw/util/math/vector.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace aw::log
{
inline Module assimpLogModule("AssimpLoader");
}

#define LOG_E(...)                                                                                 \
  aw::log::getDefaultLogger()->log(aw::log::assimpLogModule, __FILE__, __LINE__, __func__,         \
                                   aw::log::Level::Error, __VA_ARGS__);
#define LOG_W(...)                                                                                 \
  aw::log::getDefaultLogger()->log(aw::log::assimpLogModule, __FILE__, __LINE__, __func__,         \
                                   aw::log::Level::Warning, __VA_ARGS__);
#define LOG_D(...)                                                                                 \
  aw::log::getDefaultLogger()->log(aw::log::assimpLogModule, __FILE__, __LINE__, __func__,         \
                                   aw::log::Level::Debug, __VA_ARGS__);

namespace aw::engine
{
int configToImporterFlags(AssimpLoader::Config config);

AssimpLoader::AssimpLoader(Config config) : mConfig(config) {}

bool AssimpLoader::load(StaticMesh& mesh, const char* path)
{
  mStaticMesh = &mesh;

  aw::file::InputStream file(path);

  if (!file.isOpen())
  {
    LOG_D("Failed to open mesh file: {}\n", path);
    return false;
  }

  Assimp::Importer importer;

  // Extension is last element, so we can directly supply the stringview
  auto extensionV = filePath::extension(file.getPath());

  auto fileContent = aw::file::toUint8(file);
  importer.ReadFileFromMemory(fileContent.data(), fileContent.size(),
                              configToImporterFlags(mConfig), extensionV.data());

  const auto* scene = importer.GetScene();

  LOG_D("Num meshes: {}\n", scene->mNumMeshes);

  for (auto i = 0U; i < scene->mNumMeshes; i++)
  {
    assert(i < 1 && "Importing file with multiple objects not supported yet");
    if (!parseMesh(scene, i))
      return false;
  }

  mStaticMesh->update();
  return true;
}

bool AssimpLoader::parseMesh(const aiScene* scene, unsigned meshIndex)
{
  assert(meshIndex < scene->mNumMeshes);

  const auto* mesh = scene->mMeshes[meshIndex];
  assert(mesh->HasPositions());
  assert(mesh->HasNormals());
  assert(mesh->HasFaces());

  std::vector<math::Vec3> positions;
  positions.reserve(mesh->mNumVertices);

  std::vector<math::Vec3> normals;
  normals.reserve(mesh->mNumVertices);

  for (auto i = 0U; i < mesh->mNumVertices; i++)
  {
    positions.push_back(
        math::Vec3{mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z});
    normals.push_back(math::Vec3{mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z});
  }

  mStaticMesh->setPositions(std::move(positions));
  mStaticMesh->setNormals(std::move(normals));

  std::vector<unsigned> indices;
  for (auto i = 0U; i < mesh->mNumFaces; i++)
  {
    const auto& face = mesh->mFaces[i];
    assert(face.mNumIndices == 3);
    indices.emplace_back(face.mIndices[0]);
    indices.emplace_back(face.mIndices[1]);
    indices.emplace_back(face.mIndices[2]);
  }

  mStaticMesh->setIndices(std::move(indices));

  return true;
}

int configToImporterFlags(AssimpLoader::Config config)
{
  using T = AssimpLoader::Config::NormalType;
  return aiProcess_Triangulate | ((config.normals == T::Hard) ? aiProcess_GenNormals : 0) |
         ((config.normals == T::Smooth) ? aiProcess_GenSmoothNormals : 0);
}

} // namespace aw::engine
