////////////////////////////////////////////////////////////////////////////////
#include <lagrange/compute_vertex_normal.h>
#include <lagrange/poisson/mesh_from_oriented_points.h>
#include <lagrange/testing/common.h>
#include <lagrange/views.h>

#include <catch2/catch_test_macros.hpp>
////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Poisson surface reconstruction", "[poisson]")
{
    using Scalar = double;
    using Index = uint32_t;

    auto input_mesh = lagrange::testing::load_surface_mesh<Scalar, Index>("open/core/ball.obj");
    lagrange::compute_vertex_normal(input_mesh);
    input_mesh.clear_facets();

    auto mesh1 = lagrange::poisson::mesh_from_oriented_points(input_mesh);
    auto mesh2 = lagrange::poisson::mesh_from_oriented_points(input_mesh);

    REQUIRE(mesh1.get_num_facets() > 0);

    REQUIRE(vertex_view(mesh1) == vertex_view(mesh2));
    REQUIRE(facet_view(mesh1) == facet_view(mesh2));
}