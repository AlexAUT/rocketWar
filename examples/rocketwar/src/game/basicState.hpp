#pragma once

#include <aw/engine/resources/staticMesh.hpp>
#include <aw/engine/stateMachine/state.hpp>
#include <aw/engine/window/event.hpp>
#include <aw/engine/window/eventSubscriber.hpp>
#include <aw/graphics/3d/orbitCameraController.hpp>
#include <aw/graphics/core/camera.hpp>
#include <aw/graphics/core/shaderProgram.hpp>

#include "networkHandler.hpp"
#include "player.hpp"
#include "shared/missle.hpp"
#include "shared/ship.hpp"
#include "shared/shipController.hpp"

#include <memory>

#include <mutex>

namespace aw::engine
{
class Engine;
}

class BasicState : public aw::engine::State, public aw::engine::WindowEventSubscriber
{
public:
  BasicState(aw::engine::Engine& engine);

  virtual void onShow() override;

  virtual void update(float dt) override;
  virtual void render() override;

  void receive(const aw::windowEvent::Closed& event) override;
  void receive(const aw::windowEvent::Resized& event) override;
  void receive(const aw::windowEvent::MouseMoved& event) override;
  void receive(const aw::windowEvent::MouseButtonPressed& event) override;
  void receive(const aw::windowEvent::MouseButtonReleased& event) override;
  void receive(const aw::windowEvent::MouseWheelScrolled& event) override;
  void receive(const aw::windowEvent::KeyPressed& event) override;
  void receive(const aw::windowEvent::KeyReleased& event) override;

private:
  aw::engine::Engine& mEngine;

  aw::engine::StaticMesh mShipMesh;
  aw::engine::StaticMesh mLevelMesh;
  aw::engine::StaticMesh mMissleMesh;

  aw::ShaderProgram mBasicShader;
  aw::Camera mCamera;
  aw::OrbitCameraController mCamController;

  NetworkHandler mNetworkHandler;

  bool mRightPressed{false};

  bool mPause{true};

  std::vector<std::unique_ptr<Missle>> mMissles;
  std::vector<Player> mPlayers;
};
