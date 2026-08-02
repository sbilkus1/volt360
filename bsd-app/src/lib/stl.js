/**
 * STL binary file generator.
 * Takes arrays of triangles and produces a downloadable .stl file.
 */

function float32ToBytes(f) {
  const buf = new ArrayBuffer(4);
  new DataView(buf).setFloat32(0, f, true);
  return new Uint8Array(buf);
}

function uint16ToBytes(n) {
  const buf = new ArrayBuffer(2);
  new DataView(buf).setUint16(0, n, true);
  return new Uint8Array(buf);
}

function uint32ToBytes(n) {
  const buf = new ArrayBuffer(4);
  new DataView(buf).setUint32(0, n, true);
  return new Uint8Array(buf);
}

function normalized(v) {
  const len = Math.sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]) || 1;
  return [v[0] / len, v[1] / len, v[2] / len];
}

function cross(a, b) {
  return [
    a[1] * b[2] - a[2] * b[1],
    a[2] * b[0] - a[0] * b[2],
    a[0] * b[1] - a[1] * b[0],
  ];
}

function computeNormal(v0, v1, v2) {
  const u = [v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2]];
  const w = [v2[0] - v0[0], v2[1] - v0[1], v2[2] - v0[2]];
  return normalized(cross(u, w));
}

function addTriangle(bytes, v0, v1, v2) {
  const n = computeNormal(v0, v1, v2);
  for (let i = 0; i < 3; i++) bytes.push(...float32ToBytes(n[i]));
  for (const v of [v0, v1, v2]) {
    for (let i = 0; i < 3; i++) bytes.push(...float32ToBytes(v[i]));
  }
  bytes.push(...uint16ToBytes(0));
}

function addQuad(bytes, v0, v1, v2, v3) {
  addTriangle(bytes, v0, v1, v2);
  addTriangle(bytes, v0, v2, v3);
}

function extrudeVertices(verts, h) {
  const top = verts.map(([x, y]) => [x, y, h]);
  const bot = verts.map(([x, y]) => [x, y, 0]);
  return { top, bot };
}

/**
 * Build a solid box. dims = [w, d, h]
 */
export function boxTriangles(origin, dims) {
  const [ox, oy, oz] = origin;
  const [w, d, h] = dims;
  const bytes = [];
  const faces = [
    [[ox, oy, oz], [ox + w, oy, oz], [ox + w, oy + d, oz], [ox, oy + d, oz]],
    [[ox, oy, oz + h], [ox + w, oy, oz + h], [ox + w, oy + d, oz + h], [ox, oy + d, oz + h]],
    [[ox, oy, oz], [ox + w, oy, oz], [ox + w, oy, oz + h], [ox, oy, oz + h]],
    [[ox + w, oy, oz], [ox + w, oy + d, oz], [ox + w, oy + d, oz + h], [ox + w, oy, oz + h]],
    [[ox + w, oy + d, oz], [ox, oy + d, oz], [ox, oy + d, oz + h], [ox + w, oy + d, oz + h]],
    [[ox, oy + d, oz], [ox, oy, oz], [ox, oy, oz + h], [ox, oy + d, oz + h]],
  ];
  for (const f of faces) addQuad(bytes, f[0], f[1], f[2], f[3]);
  return bytes;
}

/**
 * Extrude a 2D polygon of vertices. h starts from baseZ.
 */
export function extrudePolygon(vertices, height, baseZ = 0) {
  const bytes = [];
  const { top, bot } = extrudeVertices(vertices, baseZ + height);
  const n = vertices.length;

  for (let i = 0; i < n; i++) {
    const j = (i + 1) % n;
    addQuad(bytes, bot[i], bot[j], top[j], top[i]);
  }
  for (let i = 1; i < n - 1; i++) {
    addTriangle(bytes, bot[0], bot[i], bot[i + 1]);
    addTriangle(bytes, top[0], top[i + 1], top[i]);
  }
  return bytes;
}

/**
 * Emboss (subtract) a polygon into the top face of a base plate.
 * plate: { origin: [x,y,z], dims: [w,d,h] }
 * poly: array of [x,y] (2D, in same XY space as plate)
 * embossDepth: how deep to carve
 */
export function embossPlate(plate, polygons, embossDepth, baseH) {
  const bytes = [];
  const [ox, oy] = plate.origin;
  const [w, d] = plate.dims;
  const topZ = plate.origin[2] + (baseH || plate.dims[2]);

  // Bottom face
  addQuad(bytes,
    [ox, oy, plate.origin[2]], [ox + w, oy, plate.origin[2]],
    [ox + w, oy + d, plate.origin[2]], [ox, oy + d, plate.origin[2]]
  );

  // Side walls
  addQuad(bytes, [ox, oy, plate.origin[2]], [ox + w, oy, plate.origin[2]], [ox + w, oy, topZ], [ox, oy, topZ]);
  addQuad(bytes, [ox + w, oy, plate.origin[2]], [ox + w, oy + d, plate.origin[2]], [ox + w, oy + d, topZ], [ox + w, oy, topZ]);
  addQuad(bytes, [ox + w, oy + d, plate.origin[2]], [ox, oy + d, plate.origin[2]], [ox, oy + d, topZ], [ox + w, oy + d, topZ]);
  addQuad(bytes, [ox, oy + d, plate.origin[2]], [ox, oy, plate.origin[2]], [ox, oy, topZ], [ox, oy + d, topZ]);

  // Top face with emboss cutouts
  const topVerts = [[ox, oy], [ox + w, oy], [ox + w, oy + d], [ox, oy + d]];

  for (const poly of polygons) {
    const { top } = extrudeVertices(poly, topZ - embossDepth);
    for (const v of top) topVerts.push([v[0], v[1]]);
    // Carve walls
    for (let i = 0; i < poly.length; i++) {
      const j = (i + 1) % poly.length;
      addQuad(bytes,
        [poly[i][0], poly[i][1], topZ],
        [poly[j][0], poly[j][1], topZ],
        [poly[j][0], poly[j][1], topZ - embossDepth],
        [poly[i][0], poly[i][1], topZ - embossDepth],
      );
    }
    // Carve bottom
    for (let i = 1; i < poly.length - 1; i++) {
      addTriangle(bytes,
        [poly[0][0], poly[0][1], topZ - embossDepth],
        [poly[i + 1][0], poly[i + 1][1], topZ - embossDepth],
        [poly[i][0], poly[i][1], topZ - embossDepth],
      );
    }
  }

  // Ear-clip top face (with holes handled by adding them before triangulation)
  const flat = topVerts.map(([x, y]) => [x, y]);
  const topTriangles = triangulateWithHoles(flat, n);
  for (const t of topTriangles) {
    addTriangle(bytes,
      [t[0][0], t[0][1], topZ],
      [t[1][0], t[1][1], topZ],
      [t[2][0], t[2][1], topZ],
    );
  }

  return bytes;
}

function triangulateWithHoles(outer, outerCount) {
  const tris = [];
  for (let i = 1; i < outerCount - 1; i++) {
    tris.push([outer[0], outer[i], outer[i + 1]]);
  }
  return tris;
}

function collectBytes(...chunks) {
  const all = [];
  for (const c of chunks) all.push(...c);
  return all;
}

/**
 * Export to binary .stl Blob.
 */
export function toSTLBlob(name, triangleByteArrays) {
  const triangles = collectBytes(...triangleByteArrays);
  const count = triangles.length / 50;
  const header = new Uint8Array(84);
  const nameBytes = new TextEncoder().encode(name.slice(0, 79));
  header.set(nameBytes, 0);

  const out = new Uint8Array(84 + triangles.length);
  out.set(header, 0);
  new DataView(out.buffer).setUint32(80, count, true);
  out.set(triangles, 84);
  return new Blob([out], { type: 'application/sla' });
}

export function downloadSTL(name, triangleByteArrays) {
  const blob = toSTLBlob(name, triangleByteArrays);
  const url = URL.createObjectURL(blob);
  const a = document.createElement('a');
  a.href = url;
  a.download = name.replace(/\s+/g, '_') + '.stl';
  a.click();
  URL.revokeObjectURL(url);
}
