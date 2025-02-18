/*
 Copyright (C) 2010 Kristian Duske

 This file is part of TrenchBroom.

 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "render/MaterialIndexRangeMap.h"
#include "render/PrimType.h"
#include "render/VertexListBuilder.h"

#include <vector>

namespace tb::mdl
{
class Material;
}

namespace tb::render
{

/**
 * Builds an index range map and a corresponding vertex array by recording rendering
 * primitives. The recorded data can be used to create an vertex array that can be
 * uploaded to video card memory, and to render the recorded primitives using the index
 * ranges stored in the constructed index range map.
 */
template <typename VertexSpec>
class MaterialIndexRangeMapBuilder
{
public:
  using Vertex = typename VertexSpec::Vertex;
  using VertexList = std::vector<Vertex>;
  using Material = mdl::Material;

private:
  using IndexData = typename VertexListBuilder<VertexSpec>::Range;

private:
  VertexListBuilder<VertexSpec> m_vertexListBuilder;
  MaterialIndexRangeMap m_indexRange;

public:
  /**
   * Creates a new index range map builder that initializes its data structures to the
   * given sizes.
   *
   * @param vertexCount the total number of vertices to expect
   * @param indexRangeSize the size of the index range map to expect
   */
  MaterialIndexRangeMapBuilder(
    const size_t vertexCount, const MaterialIndexRangeMap::Size& indexRangeSize)
    : m_vertexListBuilder(vertexCount)
    , m_indexRange(indexRangeSize)
  {
  }

  /**
   * Returns the recorded vertices.
   *
   * @return the recorded vertices
   */
  const VertexList& vertices() const { return m_vertexListBuilder.vertices(); }

  /**
   * Returns the recorded vertices.
   *
   * @return the recorded vertices
   */
  VertexList& vertices() { return m_vertexListBuilder.vertices(); }

  /**
   * Returns the recorded index ranges.
   *
   * @return the recorded index ranges
   */
  const MaterialIndexRangeMap& indices() const { return m_indexRange; }

  /**
   * Returns the recorded index ranges.
   *
   * @return the recorded index ranges
   */
  MaterialIndexRangeMap& indices() { return m_indexRange; }

  /**
   * Adds a point primitive at the given position.
   *
   * @param material the material to use
   * @param v the position of the point to add
   */
  void addPoint(const Material* material, const Vertex& v)
  {
    add(material, render::PrimType::Points, m_vertexListBuilder.addPoint(v));
  }

  /**
   * Adds multiple point primitives at the given positions.
   *
   * @param material the material to use
   * @param vertices the positions of the points to add
   */
  void addPoints(const Material* material, const VertexList& vertices)
  {
    add(material, render::PrimType::Points, m_vertexListBuilder.addPoints(vertices));
  }

  /**
   * Adds a line with the given end points.
   *
   * @param material the material to use
   * @param v1 the position of the first end point
   * @param v2 the position of the second end point
   */
  void addLine(const Material* material, const Vertex& v1, const Vertex& v2)
  {
    add(material, render::PrimType::Lines, m_vertexListBuilder.addLine(v1, v2));
  }

  /**
   * Adds multiple lines with the given endpoints. Each line to be added consists of two
   * consecutive of the given list, so for each line, two elements of the list are used.
   *
   * @param material the material to use
   * @param vertices the end points of the lines to add
   */
  void addLines(const Material* material, const VertexList& vertices)
  {
    add(material, render::PrimType::Lines, m_vertexListBuilder.addLines(vertices));
  }

  /**
   * Adds a line strip with the given points.
   *
   * @param material the material to use
   * @param vertices the end points of the lines to add
   */
  void addLineStrip(const Material* material, const VertexList& vertices)
  {
    add(
      material, render::PrimType::LineStrip, m_vertexListBuilder.addLineStrip(vertices));
  }

  /**
   * Adds a line loop with the given points.
   *
   * @param material the material to use
   * @param vertices the end points of the lines to add
   */
  void addLineLoop(const Material* material, const VertexList& vertices)
  {
    add(material, render::PrimType::LineLoop, m_vertexListBuilder.addLineLoop(vertices));
  }

  /**
   * Adds a triangle with the given corners.
   *
   * @param material the material to use
   * @param v1 the position of the first corner
   * @param v2 the position of the second corner
   * @param v3 the position of the third corner
   */
  void addTriangle(
    const Material* material, const Vertex& v1, const Vertex& v2, const Vertex& v3)
  {
    add(
      material, render::PrimType::Triangles, m_vertexListBuilder.addTriangle(v1, v2, v3));
  }

  /**
   * Adds multiple triangles using the corner positions in the given list. For
   * each triangle, three positions are used.
   *
   * @param material the material to use
   * @param vertices the corner positions
   */
  void addTriangles(const Material* material, const VertexList& vertices)
  {
    add(
      material, render::PrimType::Triangles, m_vertexListBuilder.addTriangles(vertices));
  }

  /**
   * Adds a triangle fan using the positions of the vertices in the given list.
   *
   * @param material the material to use
   * @param vertices the vertex positions
   */
  void addTriangleFan(const Material* material, const VertexList& vertices)
  {
    add(
      material,
      render::PrimType::TriangleFan,
      m_vertexListBuilder.addTriangleFan(vertices));
  }

  /**
   * Adds a triangle strip using the positions of the vertices in the given list.
   *
   * @param material the material to use
   * @param vertices the vertex positions
   */
  void addTriangleStrip(const Material* material, const VertexList& vertices)
  {
    add(
      material,
      render::PrimType::TriangleStrip,
      m_vertexListBuilder.addTriangleStrip(vertices));
  }

  /**
   * Adds a quad with the given corners.
   *
   * @param material the material to use
   * @param v1 the position of the first corner
   * @param v2 the position of the second corner
   * @param v3 the position of the third corner
   * @param v4 the position of the fourth corner
   */
  void addQuad(
    const Material* material,
    const Vertex& v1,
    const Vertex& v2,
    const Vertex& v3,
    const Vertex& v4)
  {
    add(material, render::PrimType::Quads, m_vertexListBuilder.addQuad(v1, v2, v3, v4));
  }

  /**
   * Adds multiple quads using the corner positions in the given list. For each
   * quad, four positions are used.
   *
   * @param material the material to use
   * @param vertices the corner positions
   */
  void addQuads(const Material* material, const VertexList& vertices)
  {
    add(material, render::PrimType::Quads, m_vertexListBuilder.addQuads(vertices));
  }

  /**
   * Adds a quad strip using the positions of the vertices in the given list.
   *
   * @param material the material to use
   * @param vertices the vertex positions
   */
  void addQuadStrip(const Material* material, const VertexList& vertices)
  {
    add(
      material, render::PrimType::QuadStrip, m_vertexListBuilder.addQuadStrip(vertices));
  }

  /**
   * Adds a polygon with the given corners.
   *
   * @param material the material to use
   * @param vertices the croner positions
   */
  void addPolygon(const Material* material, const VertexList& vertices)
  {
    add(material, render::PrimType::Polygon, m_vertexListBuilder.addPolygon(vertices));
  }

private:
  void add(const Material* material, const PrimType primType, const IndexData& data)
  {
    m_indexRange.add(material, primType, data.index, data.count);
  }
};

} // namespace tb::render
