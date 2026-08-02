/**
 * QR code → 3D extruded geometry for 3D printing.
 * Uses the 'qrcode' library to generate the bitmap, then extrudes each module.
 */

import QRCodeLib from 'qrcode';

/**
 * Generate a QR code matrix (2D boolean array) from a string.
 * dark = true means filled module.
 */
export async function generateQRMatrix(text, options = {}) {
  const modules = await QRCodeLib.toCanvas(text, {
    errorCorrectionLevel: options.errorCorrection || 'M',
    width: options.size || 29,
    margin: options.margin || 1,
  });
  // Approach: use the canvas to extract the pixel data
  const size = options.size || 29;
  const canvas = document.createElement('canvas');
  canvas.width = size;
  canvas.height = size;
  const ctx = canvas.getContext('2d');

  // Draw QR at the exact module size
  await new Promise((resolve) => {
    QRCodeLib.toCanvas(canvas, text, {
      errorCorrectionLevel: options.errorCorrection || 'M',
      width: size,
      margin: 0,
    }, () => resolve());
  });

  const imgData = ctx.getImageData(0, 0, size, size);
  const matrix = [];
  for (let y = 0; y < size; y++) {
    const row = [];
    for (let x = 0; x < size; x++) {
      const idx = (y * size + x) * 4;
      row.push(imgData.data[idx] < 128); // dark = true
    }
    matrix.push(row);
  }
  return matrix;
}

/**
 * Convert a QR matrix into 3D extruded triangle byte arrays.
 * Each dark module becomes a small raised block.
 * @param matrix - 2D boolean array
 * @param ox, oy - origin X, Y
 * @param moduleSize - size of each module in mm
 * @param height - extrusion height in mm
 * @param baseHeight - base plate thickness embedding QR
 */
export function qrMatrixToTriangles(matrix, ox, oy, moduleSize, height, baseHeight = 0) {
  const bytes = [];
  const rows = matrix.length;
  const cols = matrix[0]?.length || 0;
  const ew = moduleSize * 0.85; // effective width for printability gap

  for (let r = 0; r < rows; r++) {
    for (let c = 0; c < cols; c++) {
      if (!matrix[r][c]) continue;
      const x = ox + c * moduleSize + (moduleSize - ew) / 2;
      const y = oy + r * moduleSize + (moduleSize - ew) / 2;
      const z = baseHeight;
      addBox(bytes, [x, y, z], [ew, ew, height]);
    }
  }
  return bytes;
}

function addBox(bytes, [ox, oy, oz], [w, d, h]) {
  const x1 = ox, x2 = ox + w;
  const y1 = oy, y2 = oy + d;
  const z1 = oz, z2 = oz + h;
  addQuad(bytes, [x1, y1, z1], [x2, y1, z1], [x2, y2, z1], [x1, y2, z1]);
  addQuad(bytes, [x1, y1, z2], [x2, y1, z2], [x2, y2, z2], [x1, y2, z2]);
  addQuad(bytes, [x1, y1, z1], [x2, y1, z1], [x2, y1, z2], [x1, y1, z2]);
  addQuad(bytes, [x2, y1, z1], [x2, y2, z1], [x2, y2, z2], [x2, y1, z2]);
  addQuad(bytes, [x2, y2, z1], [x1, y2, z1], [x1, y2, z2], [x2, y2, z2]);
  addQuad(bytes, [x1, y2, z1], [x1, y1, z1], [x1, y1, z2], [x1, y2, z2]);
}

function addQuad(bytes, v0, v1, v2, v3) {
  addTri(bytes, v0, v1, v2);
  addTri(bytes, v0, v2, v3);
}

function addTri(bytes, a, b, c) {
  const u = [b[0] - a[0], b[1] - a[1], b[2] - a[2]];
  const v = [c[0] - a[0], c[1] - a[1], c[2] - a[2]];
  const n = [
    u[1] * v[2] - u[2] * v[1],
    u[2] * v[0] - u[0] * v[2],
    u[0] * v[1] - u[1] * v[0],
  ];
  const len = Math.sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]) || 1;
  for (let i = 0; i < 3; i++) bytes.push(...f32(n[i] / len));
  for (const p of [a, b, c]) for (let i = 0; i < 3; i++) bytes.push(...f32(p[i]));
  bytes.push(0, 0); // attribute
}

function f32(f) {
  const buf = new ArrayBuffer(4);
  new DataView(buf).setFloat32(0, f, true);
  return new Uint8Array(buf);
}
