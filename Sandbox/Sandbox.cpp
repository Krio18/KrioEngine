#include "Sandbox.hpp"

#include "../src/ECS/Components/Transform/Transform.hpp"
#include "../src/ECS/Components/MeshRenderer/MeshRenderer.hpp"
#include "../src/Renderer/Camera/Camera.hpp"
#include "../src/Core/SceneManager/SceneManager.hpp"
#include "../src/Renderer/MaterialManager/MaterialManager.hpp"
#include "../src/Renderer/MeshManager/MeshManager.hpp"
#include "../src/Renderer/ShaderManager/ShaderManager.hpp"
#include "../src/Core/TimeManager/TimeManager.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace Voxel {

namespace {
    struct WandererParams { float r, ry, ax, ay, az, px, py, pz; };
    static constexpr WandererParams kWanderers[7] = {
        { 6.0f, 3.5f, 0.7f, 1.1f, 0.9f, 0.0f, 1.0f, 2.0f },
        { 7.5f, 2.0f, 1.3f, 0.8f, 1.5f, 1.5f, 2.5f, 0.5f },
        { 5.5f, 4.0f, 0.9f, 1.4f, 0.6f, 3.0f, 0.0f, 1.8f },
        { 8.0f, 3.0f, 0.5f, 0.9f, 1.2f, 0.8f, 3.5f, 2.2f },
        { 6.5f, 2.5f, 1.1f, 0.6f, 1.0f, 2.0f, 1.5f, 3.0f },
        { 9.0f, 3.5f, 0.8f, 1.3f, 0.7f, 1.2f, 2.0f, 0.8f },
        { 7.0f, 4.5f, 1.5f, 1.0f, 0.8f, 2.8f, 0.5f, 1.5f },
    };
}

Sandbox::Sandbox() : Engine() {}

// ---------------------------------------------------------------------------
// onInit
// ---------------------------------------------------------------------------
void Sandbox::onInit() {
    Logger::info("Sandbox::onInit()");

    auto& shaderMgr  = getServiceLocator().getManager<ShaderManager>();
    auto& meshMgr    = getServiceLocator().getManager<MeshManager>();
    auto& matMgr     = getServiceLocator().getManager<MaterialManager>();
    auto& sceneMgr   = getServiceLocator().getManager<SceneManager>();

    shaderMgr.load("simple");
    meshMgr.load("cube");

    auto matGold    = matMgr.create("gold",    "simple");
    auto matCyan    = matMgr.create("cyan",    "simple");
    auto matMagenta = matMgr.create("magenta", "simple");
    auto matOrange  = matMgr.create("orange",  "simple");
    auto matWhite   = matMgr.create("white",   "simple");
    auto matTeal    = matMgr.create("teal",    "simple");

    if (matGold)    matGold   ->setColor(glm::vec4(1.00f, 0.75f, 0.10f, 1.0f));
    if (matCyan)    matCyan   ->setColor(glm::vec4(0.00f, 0.90f, 1.00f, 1.0f));
    if (matMagenta) matMagenta->setColor(glm::vec4(1.00f, 0.15f, 0.80f, 1.0f));
    if (matOrange)  matOrange ->setColor(glm::vec4(1.00f, 0.50f, 0.00f, 1.0f));
    if (matWhite)   matWhite  ->setColor(glm::vec4(0.95f, 0.95f, 0.95f, 1.0f));
    if (matTeal)    matTeal   ->setColor(glm::vec4(0.00f, 0.75f, 0.60f, 1.0f));

    _scene = sceneMgr.loadScene("vortex");

    const float aspect = static_cast<float>(getWindowWidth()) /
                         static_cast<float>(getWindowHeight());

    // --- Camera (Tag "MainCamera" picked up by CameraSystem) --------------
    {
        _cameraEntity = _scene->createEntity("MainCamera");

        Transform t;
        t.setPosition(glm::vec3(0.0f, 8.0f, 22.0f));
        t.setScale(glm::vec3(1.0f));
        _cameraEntity.addComponent<Transform>(t);

        Camera cam;
        cam.fovDegrees = 70.0f;
        cam.aspect     = aspect;
        cam.zNear      = 0.5f;
        cam.zFar       = 200.0f;
        _cameraEntity.addComponent<Camera>(cam);
    }

    // --- Central spire : 5 gold cubes, stacked, decreasing scale ----------
    {
        const float ys[]     = { 0.0f, 2.2f, 4.4f, 6.6f, 8.8f };
        const float scales[] = { 1.4f, 1.2f, 1.0f, 0.85f, 0.7f };
        for (int i = 0; i < 5; i++) {
            Entity e = _scene->createEntity("Spire_" + std::to_string(i));
            Transform t;
            t.setPosition(glm::vec3(0.0f, ys[i], 0.0f));
            t.setScale(glm::vec3(scales[i]));
            e.addComponent<Transform>(t);
            MeshRenderer mr;
            mr.meshName     = "cube";
            mr.materialName = "gold";
            e.addComponent<MeshRenderer>(mr);
            _spire.push_back(e);
        }
    }

    // --- Ring 1 : 14 cyan cubes, inner orbit, tilted 15° around X --------
    {
        constexpr int   N = 14;
        constexpr float R = 5.5f;
        for (int i = 0; i < N; i++) {
            float a = i * (glm::two_pi<float>() / N);
            Entity e = _scene->createEntity("Ring1_" + std::to_string(i));
            Transform t;
            t.setPosition(glm::vec3(R * std::cos(a), 3.0f, R * std::sin(a)));
            t.setScale(glm::vec3(0.30f));
            e.addComponent<Transform>(t);
            MeshRenderer mr;
            mr.meshName     = "cube";
            mr.materialName = "cyan";
            e.addComponent<MeshRenderer>(mr);
            _ring1.push_back(e);
        }
    }

    // --- Ring 2 : 20 magenta cubes, middle orbit, tilted 25° around Z ----
    {
        constexpr int   N = 20;
        constexpr float R = 8.5f;
        for (int i = 0; i < N; i++) {
            float a = i * (glm::two_pi<float>() / N);
            Entity e = _scene->createEntity("Ring2_" + std::to_string(i));
            Transform t;
            t.setPosition(glm::vec3(R * std::cos(a), 1.5f, R * std::sin(a)));
            t.setScale(glm::vec3(0.25f));
            e.addComponent<Transform>(t);
            MeshRenderer mr;
            mr.meshName     = "cube";
            mr.materialName = "magenta";
            e.addComponent<MeshRenderer>(mr);
            _ring2.push_back(e);
        }
    }

    // --- Ring 3 : 28 orange cubes, outer orbit, flat with vertical wave --
    {
        constexpr int   N = 28;
        constexpr float R = 12.5f;
        for (int i = 0; i < N; i++) {
            float a = i * (glm::two_pi<float>() / N);
            Entity e = _scene->createEntity("Ring3_" + std::to_string(i));
            Transform t;
            t.setPosition(glm::vec3(R * std::cos(a), 0.5f, R * std::sin(a)));
            t.setScale(glm::vec3(0.20f));
            e.addComponent<Transform>(t);
            MeshRenderer mr;
            mr.meshName     = "cube";
            mr.materialName = "orange";
            e.addComponent<MeshRenderer>(mr);
            _ring3.push_back(e);
        }
    }

    // --- Wanderers : 7 white cubes on independent 3D Lissajous paths -----
    for (int i = 0; i < 7; i++) {
        Entity e = _scene->createEntity("Wanderer_" + std::to_string(i));
        Transform t;
        t.setPosition(glm::vec3(kWanderers[i].r, 0.0f, 0.0f));
        t.setScale(glm::vec3(0.45f + (i % 3) * 0.15f));
        e.addComponent<Transform>(t);
        MeshRenderer mr;
        mr.meshName     = "cube";
        mr.materialName = "white";
        e.addComponent<MeshRenderer>(mr);
        _wanderers.push_back(e);
    }

    // --- Ground grid : 7×7 teal/white alternating, wave animated ----------
    for (int x = -3; x <= 3; x++) {
        for (int z = -3; z <= 3; z++) {
            std::string id = std::to_string(x + 3) + "_" + std::to_string(z + 3);
            Entity e = _scene->createEntity("Grid_" + id);
            Transform t;
            t.setPosition(glm::vec3(x * 2.5f, -5.0f, z * 2.5f));
            t.setScale(glm::vec3(0.40f));
            e.addComponent<Transform>(t);
            MeshRenderer mr;
            mr.meshName     = "cube";
            mr.materialName = ((x + z) % 2 == 0) ? "teal" : "white";
            e.addComponent<MeshRenderer>(mr);
            _grid.push_back(e);
        }
    }

    Logger::info("Sandbox::onInit() — " +
                 std::to_string(1 + 5 + 14 + 20 + 28 + 7 + 49) + " entities created");
}

// ---------------------------------------------------------------------------
// onUpdate
// ---------------------------------------------------------------------------
void Sandbox::onUpdate() {
    float dt = static_cast<float>(
        getServiceLocator().getManager<TimeManager>().getDeltaTime());
    _time += dt;

    // --- Camera : slow orbit, gentle pitch oscillation --------------------
    {
        float yaw  = _time * 0.15f;
        float pitch = glm::radians(20.0f + 14.0f * std::sin(_time * 0.28f));
        float dist  = 22.0f;
        glm::vec3 pos(
            dist * std::cos(pitch) * std::sin(yaw),
            dist * std::sin(pitch),
            dist * std::cos(pitch) * std::cos(yaw)
        );
        auto* t = _cameraEntity.getComponent<Transform>();
        if (t) {
            t->setPosition(pos);
            t->setRotation(glm::conjugate(glm::quat_cast(
                glm::lookAt(pos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)))));
        }
    }

    // --- Spire : each level spins faster than the one below ---------------
    for (int i = 0; i < static_cast<int>(_spire.size()); i++) {
        auto* t = _spire[i].getComponent<Transform>();
        if (!t) continue;
        float ySpeed = 0.4f + i * 0.30f;
        float xWobble = std::sin(_time * 0.2f * (i + 1)) * 0.25f;
        glm::quat q = glm::angleAxis(_time * ySpeed, glm::vec3(0.0f, 1.0f, 0.0f))
                    * glm::angleAxis(xWobble,         glm::vec3(1.0f, 0.0f, 0.0f));
        t->setRotation(q);
    }

    // --- Ring 1 : cyan, inner, tilted 15° around X (fast CW) -------------
    {
        constexpr int   N    = 14;
        constexpr float R    = 5.5f;
        constexpr float TILT = glm::pi<float>() / 12.0f; // 15°
        for (int i = 0; i < N; i++) {
            auto* t = _ring1[i].getComponent<Transform>();
            if (!t) continue;
            float a = _time * 1.4f + i * (glm::two_pi<float>() / N);
            glm::vec3 pos(
                R * std::cos(a),
                3.0f - R * std::sin(a) * std::sin(TILT)
                     + std::sin(_time * 1.8f + i * 0.45f) * 0.35f,
                R * std::sin(a) * std::cos(TILT)
            );
            t->setPosition(pos);
            t->setRotation(glm::angleAxis(
                _time * 3.0f + i * 0.45f,
                glm::normalize(glm::vec3(1.0f, 1.0f, 0.5f))));
        }
    }

    // --- Ring 2 : magenta, middle, tilted 25° around Z (slow CCW) --------
    {
        constexpr int   N    = 20;
        constexpr float R    = 8.5f;
        constexpr float TILT = glm::pi<float>() / 7.2f; // 25°
        for (int i = 0; i < N; i++) {
            auto* t = _ring2[i].getComponent<Transform>();
            if (!t) continue;
            float a = -_time * 0.9f + i * (glm::two_pi<float>() / N);
            glm::vec3 pos(
                R * std::cos(a) * std::cos(TILT),
                1.5f + R * std::cos(a) * std::sin(TILT),
                R * std::sin(a)
            );
            t->setPosition(pos);
            t->setRotation(glm::angleAxis(
                _time * 2.0f + i * 0.32f,
                glm::normalize(glm::vec3(0.5f, 1.0f, 0.5f))));
        }
    }

    // --- Ring 3 : orange, outer, flat with travelling vertical wave -------
    {
        constexpr int   N = 28;
        constexpr float R = 12.5f;
        for (int i = 0; i < N; i++) {
            auto* t = _ring3[i].getComponent<Transform>();
            if (!t) continue;
            float a = _time * 0.5f + i * (glm::two_pi<float>() / N);
            float y = 0.5f + std::sin(_time * 0.7f + i * (glm::two_pi<float>() / N)) * 1.8f;
            t->setPosition(glm::vec3(R * std::cos(a), y, R * std::sin(a)));
            t->setRotation(glm::angleAxis(
                _time * 1.5f + i * 0.22f,
                glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))));
        }
    }

    // --- Wanderers : independent 3D Lissajous paths -----------------------
    for (int i = 0; i < static_cast<int>(_wanderers.size()); i++) {
        auto* t = _wanderers[i].getComponent<Transform>();
        if (!t) continue;
        const auto& p = kWanderers[i];
        glm::vec3 pos(
            p.r  * std::cos(p.ax * _time + p.px),
            p.ry * std::sin(p.ay * _time + p.py),
            p.r  * std::sin(p.az * _time + p.pz)
        );
        t->setPosition(pos);
        t->setRotation(glm::angleAxis(
            _time * p.ax * 2.0f,
            glm::normalize(glm::vec3(p.px + 0.1f, p.py + 0.1f, p.pz + 0.1f))));
    }

    // --- Ground grid : travelling sine wave -------------------------------
    int idx = 0;
    for (int x = -3; x <= 3; x++) {
        for (int z = -3; z <= 3; z++) {
            auto* t = _grid[idx++].getComponent<Transform>();
            if (!t) continue;
            float phase = static_cast<float>(x + z) * 0.7f;
            float y     = -5.0f + std::sin(_time * 1.6f + phase) * 0.9f;
            t->setPosition(glm::vec3(x * 2.5f, y, z * 2.5f));
            t->setRotation(glm::angleAxis(_time * 0.4f + phase * 0.2f,
                                          glm::vec3(0.0f, 1.0f, 0.0f)));
        }
    }
}

void Sandbox::onShutdown() {
    Logger::info("Sandbox::onShutdown()");
}

} // namespace Voxel
