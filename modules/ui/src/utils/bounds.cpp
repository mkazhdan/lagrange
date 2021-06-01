/*
 * Copyright 2020 Adobe. All rights reserved.
 * This file is licensed to you under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain a copy
 * of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under
 * the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS
 * OF ANY KIND, either express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 */

#include <lagrange/ui/components/Common.h>
#include <lagrange/ui/components/Selection.h>
#include <lagrange/ui/utils/bounds.h>
#include <lagrange/ui/utils/layer.h>


namespace lagrange {
namespace ui {


float get_nearest_bounds_distance(
    const Registry& ctx,
    const Eigen::Vector3f& from,
    const Layer& visible,
    const Layer& hidden)
{
    float min_dst = std::numeric_limits<float>::max();

    ctx.view<const Bounds>().each([&](Entity e, const Bounds& bounds) {
        if (!ui::is_visible_in(ctx, e, visible, hidden)) return;
        const auto& box = bounds.global;
        if (box.isEmpty()) return;
        const auto dst = box.squaredExteriorDistance(from);
        if (dst == 0.0f) return;
        min_dst = std::min(dst, min_dst);
    });

    min_dst = std::sqrt(min_dst);

    return min_dst;
}

float get_furthest_bounds_distance(
    const Registry& ctx,
    const Eigen::Vector3f& from,
    const Layer& visible,
    const Layer& hidden)
{
    float max_dst = std::numeric_limits<float>::min();

    ctx.view<const Bounds>().each([&](Entity e, const Bounds& bounds) {
        if (!ui::is_visible_in(ctx, e, visible, hidden)) return;
        const auto& box = bounds.global;
        if (box.isEmpty()) return;
        Eigen::Vector3f dmin = (from - box.min()).cwiseAbs();
        Eigen::Vector3f dmax = (from - box.max()).cwiseAbs();
        Eigen::Vector3f dmaxmax = dmin.cwiseMax(dmax);
        max_dst = std::max(max_dst, dmaxmax.maxCoeff());
    });

    return max_dst;
}

AABB get_scene_bounding_box(const Registry& registry)
{
    return get_scene_bounds(registry).global;
}

const Bounds& get_scene_bounds(const Registry& registry)
{
    return registry.ctx<Bounds>();
}

Bounds& get_scene_bounds(Registry& registry)
{
    return registry.ctx<Bounds>();
}

AABB get_selection_bounding_box(const Registry& registry)
{
    AABB result;

    registry.view<const Selected, const AABB>().each([&](const Selected& sel, const AABB& bb) {
        if (!bb.isEmpty()) result.extend(bb);
    });

    return result;
}

AABB get_bounding_box(const Registry& registry, Entity e)
{
    if (registry.has<Bounds>(e)) {
        return registry.get<Bounds>(e).global;
    }

    return AABB();
}

AABB get_bounding_box_local(const Registry& registry, Entity e)
{
    if (registry.has<Bounds>(e)) {
        return registry.get<Bounds>(e).local;
    }

    return AABB();
}

} // namespace ui
} // namespace lagrange