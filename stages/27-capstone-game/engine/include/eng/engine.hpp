#pragma once

// ============================================================================
// libengine — umbrella header.
//
// A game includes THIS one header to get the whole engine. Including a single
// "everything" header is a convenience for the consumer (like `import pygame`
// pulling in the whole package); internally each subsystem is still its own
// small, independently-includable header so unit tests can grab just the pure
// bits (math, collision, tilemap, resource manager) without dragging in SFML.
//
// Subsystems, grouped:
//   - math / physics   : math.hpp, collision.hpp        (pure, SFML-free)
//   - rendering/window : application.hpp, camera.hpp, textures.hpp,
//                        tile_renderer.hpp, sfml_interop.hpp
//   - world data       : tilemap.hpp                    (pure), tile_renderer.hpp
//   - input            : input.hpp
//   - audio            : audio.hpp
//   - resources        : resource_manager.hpp           (pure template)
//   - structure        : scene.hpp, application.hpp
//   - effects          : particles.hpp
// ============================================================================

#include "eng/application.hpp"
#include "eng/audio.hpp"
#include "eng/camera.hpp"
#include "eng/collision.hpp"
#include "eng/font.hpp"
#include "eng/input.hpp"
#include "eng/math.hpp"
#include "eng/particles.hpp"
#include "eng/resource_manager.hpp"
#include "eng/scene.hpp"
#include "eng/sfml_interop.hpp"
#include "eng/textures.hpp"
#include "eng/tile_renderer.hpp"
#include "eng/tilemap.hpp"
