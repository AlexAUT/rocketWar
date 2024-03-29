#include <aw/graphics/core/bufferObject.hpp>

#include <aw/opengl/opengl.hpp>

#include <cassert>

namespace aw
{
GLenum toGL(BindType type);
GLbitfield toGL(UsageType type);

GPUBufferObject::GPUBufferObject(BindType type) : GPUBufferObject(type, UsageType::StaticDraw)
{
  GL_CHECK(glGenBuffers(1, &mId));
}

GPUBufferObject::GPUBufferObject(BindType type, UsageType usage) :
    mType(toGL(type)),
    mUsage(toGL(usage))
{
  GL_CHECK(glGenBuffers(1, &mId));
}

GPUBufferObject::~GPUBufferObject()
{
  GL_CHECK(glDeleteBuffers(1, &mId));
}

void GPUBufferObject::setData(const void* data, size_t size)
{
  GL_CHECK(glBufferData(mType, size, data, mUsage));
}

void GPUBufferObject::setSubData(const void* data, size_t size, ptrdiff offset)
{
  GL_CHECK(glBufferSubData(mType, offset, size, data));
}

void GPUBufferObject::bind() const
{
  GL_CHECK(glBindBuffer(mType, mId));
}

void GPUBufferObject::unbind() const
{
  GL_CHECK(glBindBuffer(mType, 0));
}

GLenum toGL(BindType type)
{
  switch (type)
  {
  case BindType::ArrayBuffer:
    return GL_ARRAY_BUFFER;
  case BindType::CopyReadBuffer:
    return GL_COPY_READ_BUFFER;
  case BindType::CopyWriteBuffer:
    return GL_COPY_WRITE_BUFFER;
  case BindType::ElementArrayBuffer:
    return GL_ELEMENT_ARRAY_BUFFER;
  case BindType::PixelPackBuffer:
    return GL_PIXEL_PACK_BUFFER;
  case BindType::PixelUnpackBuffer:
    return GL_PIXEL_UNPACK_BUFFER;
  case BindType::TransformFeedbackBuffer:
    return GL_TRANSFORM_FEEDBACK_BUFFER;
  case BindType::UniformBuffer:
    return GL_UNIFORM_BUFFER;
  default:
    assert(false && "Unsupported BindType");
    return 0;
  }
}

GLbitfield toGL(UsageType type)
{
  switch (type)
  {
  case UsageType::StreamDraw:
    return GL_STREAM_DRAW;
  case UsageType::StreamRead:
    return GL_STREAM_READ;
  case UsageType::StreamCopy:
    return GL_STREAM_COPY;
  case UsageType::StaticDraw:
    return GL_STATIC_DRAW;
  case UsageType::StaticRead:
    return GL_STATIC_READ;
  case UsageType::StaticCopy:
    return GL_STATIC_COPY;
  case UsageType::DynamicDraw:
    return GL_DYNAMIC_DRAW;
  case UsageType::DynamicRead:
    return GL_DYNAMIC_READ;
  case UsageType::DynamicCopy:
    return GL_DYNAMIC_COPY;
  default:
    assert(false && "Unsupported UsageType");
    return 0;
  }
}

} // namespace aw
