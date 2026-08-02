import { useRef, useEffect, useMemo } from 'react';
import * as THREE from 'three';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';

function createSTLGeometry(voxelFunc, resolution = 64) {
  return null;
}

function buildCardMesh(width, height, thickness) {
  const group = new THREE.Group();

  const halfW = width / 2;
  const halfH = thickness / 2;
  const halfD = height / 2;

  const geo = new THREE.BoxGeometry(width, thickness, height, 1, 1, 1);
  const mat = new THREE.MeshStandardMaterial({ color: 0xe8d5b7, roughness: 0.4, metalness: 0.05 });
  const mesh = new THREE.Mesh(geo, mat);
  mesh.castShadow = true;
  mesh.receiveShadow = true;
  group.add(mesh);

  // Edge wireframe
  const edgeGeo = new THREE.EdgesGeometry(geo);
  const edgeMat = new THREE.LineBasicMaterial({ color: 0x444444 });
  group.add(new THREE.LineSegments(edgeGeo, edgeMat));

  return group;
}

function buildQRMesh(matrix, position, moduleSize, extrudeHeight, baseZ) {
  const group = new THREE.Group();
  const rows = matrix.length;
  const cols = matrix[0]?.length || 0;
  const totalW = cols * moduleSize;
  const totalH = rows * moduleSize;
  const [cx, cy] = position;

  const geo_template = new THREE.BoxGeometry(moduleSize * 0.85, extrudeHeight, moduleSize * 0.85);
  const mat = new THREE.MeshStandardMaterial({ color: 0x1a1a1a, roughness: 0.6, metalness: 0.2 });

  for (let r = 0; r < rows; r++) {
    for (let c = 0; c < cols; c++) {
      if (!matrix[r][c]) continue;
      const x = cx - totalW / 2 + c * moduleSize + moduleSize / 2;
      const z = cy - totalH / 2 + r * moduleSize + moduleSize / 2;
      const mesh = new THREE.Mesh(geo_template, mat.clone());
      mesh.position.set(x, baseZ + extrudeHeight / 2, z);
      mesh.castShadow = true;
      group.add(mesh);
    }
  }
  return group;
}

function buildTextMesh(text, position, fontSize, isBack = false) {
  const group = new THREE.Group();
  const canvas = document.createElement('canvas');
  canvas.width = 512;
  canvas.height = 128;
  const ctx = canvas.getContext('2d');
  ctx.fillStyle = '#ffffff';
  ctx.fillRect(0, 0, canvas.width, canvas.height);
  ctx.fillStyle = '#000000';
  ctx.font = 'bold 48px Arial';
  ctx.textAlign = 'center';
  ctx.textBaseline = 'middle';
  ctx.fillText(text, canvas.width / 2, canvas.height / 2);

  const texture = new THREE.CanvasTexture(canvas);
  texture.minFilter = THREE.LinearFilter;

  const geo = new THREE.PlaneGeometry(1, 0.25);
  const mat = new THREE.MeshStandardMaterial({
    map: texture,
    transparent: true,
    roughness: 0.8,
    side: THREE.DoubleSide,
  });
  const plane = new THREE.Mesh(geo, mat);
  plane.position.set(position[0], position[1], isBack ? -position[2] : position[2]);
  if (isBack) plane.rotation.y = Math.PI;
  group.add(plane);
  return group;
}

function buildSundialMesh(geoData, type) {
  const group = new THREE.Group();

  if (type === 'horizontal') {
    const { plateVerts, hourLines, gnomonAngle, gnomonBaseLen, gnomonHeight, gnomonThickness } = geoData;

    // Plate
    const plateGeo = new THREE.BoxGeometry(plateVerts[1][0] - plateVerts[0][0], 2, plateVerts[2][2] - plateVerts[0][2]);
    const plateMat = new THREE.MeshStandardMaterial({ color: 0xd4c5a9, roughness: 0.5, metalness: 0.1 });
    const plate = new THREE.Mesh(plateGeo, plateMat);
    plate.position.y = 1;
    plate.castShadow = true;
    plate.receiveShadow = true;
    group.add(plate);

    // Hour lines
    for (const line of hourLines) {
      const len = (plateVerts[1][0] - plateVerts[0][0]) * 0.4;
      const dx = Math.sin(line.angle) * len;
      const dz = Math.cos(line.angle) * len;
      const points = [new THREE.Vector3(0, 2.1, 0), new THREE.Vector3(dx, 2.1, dz)];
      const lineGeo = new THREE.BufferGeometry().setFromPoints(points);
      const lineObj = new THREE.Line(lineGeo, new THREE.LineBasicMaterial({ color: 0x333333, linewidth: 1 }));
      group.add(lineObj);
    }

    // Gnomon
    const shape = new THREE.Shape();
    shape.moveTo(-gnomonThickness / 2, -0.5);
    shape.lineTo(gnomonThickness / 2, -0.5);
    shape.lineTo(gnomonThickness / 2, gnomonHeight);
    shape.lineTo(-gnomonThickness / 2, gnomonHeight);
    const extrudeSettings = { steps: 1, depth: gnomonBaseLen, bevelEnabled: false };
    const gnomonGeo = new THREE.ExtrudeGeometry(shape, extrudeSettings);
    gnomonGeo.rotateZ(gnomonAngle);
    const gnomonMat = new THREE.MeshStandardMaterial({ color: 0x888888, roughness: 0.3, metalness: 0.7 });
    const gnomon = new THREE.Mesh(gnomonGeo, gnomonMat);
    gnomon.position.y = 2;
    gnomon.rotation.x = -Math.PI / 2;
    gnomon.castShadow = true;
    group.add(gnomon);

    // Hour labels
    for (let h = 6; h <= 18; h++) {
      const line = hourLines.find(l => l.hour === h);
      if (!line && h !== 12) continue;
      const angle = h === 12 ? 0 : line.angle;
      const len = (plateVerts[1][0] - plateVerts[0][0]) * 0.35;
      const sx = Math.sin(angle) * len;
      const sz = Math.cos(angle) * len;
      const labelCanvas = document.createElement('canvas');
      labelCanvas.width = 64;
      labelCanvas.height = 64;
      const lctx = labelCanvas.getContext('2d');
      lctx.fillStyle = '#000';
      lctx.font = 'bold 32px Arial';
      lctx.textAlign = 'center';
      lctx.textBaseline = 'middle';
      lctx.fillText(String(h), 32, 32);
      const tex = new THREE.CanvasTexture(labelCanvas);
      tex.minFilter = THREE.LinearFilter;
      const spriteMat = new THREE.SpriteMaterial({ map: tex });
      const sprite = new THREE.Sprite(spriteMat);
      sprite.position.set(sx, 2.5, sz);
      sprite.scale.set(8, 8, 1);
      group.add(sprite);
    }
  }

  if (type === 'equatorial') {
    const { plateVerts, hourLines, tiltAngle, gnomonLength } = geoData;
    const halfW = (plateVerts[1][0] - plateVerts[0][0]) / 2;

    const plateGeo = new THREE.CylinderGeometry(halfW, halfW, 3, 32);
    const plateMat = new THREE.MeshStandardMaterial({ color: 0xd4c5a9, roughness: 0.5, metalness: 0.1 });
    const plate = new THREE.Mesh(plateGeo, plateMat);
    plate.rotation.z = tiltAngle;
    plate.castShadow = true;
    plate.receiveShadow = true;
    group.add(plate);

    // Hour lines on equatorial face
    for (const line of hourLines) {
      const len = halfW * 0.75;
      const angle = line.angle;
      const p1 = new THREE.Vector3(0, 0, 0);
      const p2 = new THREE.Vector3(Math.cos(angle) * len, 0, Math.sin(angle) * len);
      const lineGeo = new THREE.BufferGeometry().setFromPoints([p1, p2]);
      const lineObj = new THREE.Line(lineGeo, new THREE.LineBasicMaterial({ color: 0x333333 }));
      lineObj.rotation.z = tiltAngle;
      lineObj.position.y = 1.5;
      group.add(lineObj);
    }

    // Gnomon rod
    const rodGeo = new THREE.CylinderGeometry(1.5, 1.5, gnomonLength, 8);
    const rodMat = new THREE.MeshStandardMaterial({ color: 0x888888, roughness: 0.3, metalness: 0.7 });
    const rod = new THREE.Mesh(rodGeo, rodMat);
    rod.rotation.z = tiltAngle - Math.PI / 2;
    rod.position.y = gnomonLength / 2;
    group.add(rod);

    // Hour labels
    for (const line of hourLines) {
      const len = halfW * 0.85;
      const labelCanvas = document.createElement('canvas');
      labelCanvas.width = 64;
      labelCanvas.height = 64;
      const lctx = labelCanvas.getContext('2d');
      lctx.fillStyle = '#000';
      lctx.font = 'bold 32px Arial';
      lctx.textAlign = 'center';
      lctx.textBaseline = 'middle';
      lctx.fillText(String(line.hour), 32, 32);
      const tex = new THREE.CanvasTexture(labelCanvas);
      tex.minFilter = THREE.LinearFilter;
      const sprite = new THREE.Sprite(new THREE.SpriteMaterial({ map: tex }));
      const a = line.angle;
      sprite.position.set(Math.cos(a) * len, 2, Math.sin(a) * len);
      sprite.scale.set(6, 6, 1);
      sprite.rotation.z = tiltAngle;
      group.add(sprite);
    }
  }

  if (type === 'analemmatic') {
    const { markers, scaleX, scaleY, dateScale } = geoData;

    // Ground plate
    const plateGeo = new THREE.BoxGeometry(scaleX * 2.5, 1, scaleX * 2.5);
    const plateMat = new THREE.MeshStandardMaterial({ color: 0xd4c5a9, roughness: 0.5, metalness: 0.1 });
    const plate = new THREE.Mesh(plateGeo, plateMat);
    plate.position.y = 0.5;
    plate.castShadow = true;
    plate.receiveShadow = true;
    group.add(plate);

    // Ellipse
    const ellipsePts = [];
    for (let i = 0; i <= 64; i++) {
      const a = (i / 64) * Math.PI * 2;
      ellipsePts.push(new THREE.Vector3(Math.sin(a) * scaleX, 1.1, Math.cos(a) * scaleY));
    }
    const ellipseGeo = new THREE.BufferGeometry().setFromPoints(ellipsePts);
    const ellipseLine = new THREE.Line(ellipseGeo, new THREE.LineBasicMaterial({ color: 0x333333 }));
    group.add(ellipseLine);

    // Date scale (center line for standing positions)
    const cps = [new THREE.Vector3(0, 1.1, -scaleX), new THREE.Vector3(0, 1.1, scaleX)];
    const dateLineGeo = new THREE.BufferGeometry().setFromPoints(cps);
    group.add(new THREE.Line(dateLineGeo, new THREE.LineBasicMaterial({ color: 0xaa4444 })));

    // Hour markers
    for (const m of markers) {
      const markerGeo = new THREE.CylinderGeometry(1.5, 1.5, 10, 8);
      const marker = new THREE.Mesh(markerGeo, new THREE.MeshStandardMaterial({ color: 0x666666, roughness: 0.3, metalness: 0.5 }));
      marker.position.set(m.x, 6, m.y);
      marker.castShadow = true;
      group.add(marker);

      // Label
      const labelCanvas = document.createElement('canvas');
      labelCanvas.width = 64;
      labelCanvas.height = 64;
      const lctx = labelCanvas.getContext('2d');
      lctx.fillStyle = '#000';
      lctx.font = 'bold 32px Arial';
      lctx.textAlign = 'center';
      lctx.textBaseline = 'middle';
      lctx.fillText(String(m.hour), 32, 32);
      const tex = new THREE.CanvasTexture(labelCanvas);
      tex.minFilter = THREE.LinearFilter;
      const sprite = new THREE.Sprite(new THREE.SpriteMaterial({ map: tex }));
      sprite.position.set(m.x, 12, m.y);
      sprite.scale.set(6, 6, 1);
      group.add(sprite);
    }

    // Date ticks on center line
    for (const ds of dateScale) {
      const tickGeo = new THREE.BoxGeometry(2, 1, 3);
      const tick = new THREE.Mesh(tickGeo, new THREE.MeshStandardMaterial({ color: 0xaa4444 }));
      tick.position.set(0, 1.1, ds.distance);
      group.add(tick);
    }
  }

  if (type === 'vertical') {
    const { plateVerts, hourLines, gnomonHeight } = geoData;
    const w = plateVerts[1][0] - plateVerts[0][0];
    const h = plateVerts[2][1] - plateVerts[0][1];

    const plateGeo = new THREE.BoxGeometry(w, h, 2);
    const plateMat = new THREE.MeshStandardMaterial({ color: 0xd4c5a9, roughness: 0.5, metalness: 0.1 });
    const plate = new THREE.Mesh(plateGeo, plateMat);
    plate.castShadow = true;
    plate.receiveShadow = true;
    group.add(plate);

    // Gnomon
    const gnomonGeo = new THREE.ConeGeometry(3, gnomonHeight, 8);
    const gnomon = new THREE.Mesh(gnomonGeo, new THREE.MeshStandardMaterial({ color: 0x888888, roughness: 0.3, metalness: 0.7 }));
    gnomon.position.set(0, 0, gnomonHeight / 2 + 1);
    gnomon.rotation.x = Math.PI / 2;
    gnomon.castShadow = true;
    group.add(gnomon);

    // Hour lines
    for (const line of hourLines) {
      const len = w * 0.4;
      const dx = Math.sin(line.angle) * len;
      const dy = -Math.cos(line.angle) * len;
      const points = [new THREE.Vector3(0, 0, 1.1), new THREE.Vector3(dx, dy, 1.1)];
      const lineGeo = new THREE.BufferGeometry().setFromPoints(points);
      group.add(new THREE.Line(lineGeo, new THREE.LineBasicMaterial({ color: 0x333333 })));
    }

    // Labels
    for (const line of hourLines) {
      const len = w * 0.35;
      const labelCanvas = document.createElement('canvas');
      labelCanvas.width = 64;
      labelCanvas.height = 64;
      const lctx = labelCanvas.getContext('2d');
      lctx.fillStyle = '#000';
      lctx.font = 'bold 32px Arial';
      lctx.textAlign = 'center';
      lctx.textBaseline = 'middle';
      lctx.fillText(String(line.hour), 32, 32);
      const tex = new THREE.CanvasTexture(labelCanvas);
      tex.minFilter = THREE.LinearFilter;
      const sprite = new THREE.Sprite(new THREE.SpriteMaterial({ map: tex }));
      sprite.position.set(Math.sin(line.angle) * len, -Math.cos(line.angle) * len, 1.5);
      sprite.scale.set(6, 6, 1);
      group.add(sprite);
    }
  }

  if (type === 'polar') {
    const { plateSize, lines, tiltAngle, gnomonHeight } = geoData;
    const [pw, ph] = plateSize;

    const plateGeo = new THREE.BoxGeometry(pw, gnomonHeight * 4, ph);
    const plateMat = new THREE.MeshStandardMaterial({ color: 0xd4c5a9, roughness: 0.5, metalness: 0.1 });
    const plate = new THREE.Mesh(plateGeo, plateMat);
    plate.rotation.z = tiltAngle;
    plate.castShadow = true;
    plate.receiveShadow = true;
    group.add(plate);

    // Gnomon edge
    const edgeGeo = new THREE.BoxGeometry(pw, 3, 2);
    const edge = new THREE.Mesh(edgeGeo, new THREE.MeshStandardMaterial({ color: 0x888888, roughness: 0.3, metalness: 0.7 }));
    edge.position.y = gnomonHeight * 2;
    edge.rotation.z = tiltAngle;
    group.add(edge);

    // Hour lines (parallel)
    for (const line of lines) {
      const lineGeo = new THREE.BoxGeometry(1, 1, ph + 10);
      const lineObj = new THREE.Mesh(lineGeo, new THREE.MeshStandardMaterial({ color: 0x333333 }));
      lineObj.position.set(line.offset, gnomonHeight * 2 + 1, 0);
      lineObj.rotation.z = tiltAngle;
      group.add(lineObj);

      // Label
      const labelCanvas = document.createElement('canvas');
      labelCanvas.width = 64;
      labelCanvas.height = 64;
      const lctx = labelCanvas.getContext('2d');
      lctx.fillStyle = '#000';
      lctx.font = 'bold 32px Arial';
      lctx.textAlign = 'center';
      lctx.textBaseline = 'middle';
      lctx.fillText(String(line.hour), 32, 32);
      const tex = new THREE.CanvasTexture(labelCanvas);
      tex.minFilter = THREE.LinearFilter;
      const sprite = new THREE.Sprite(new THREE.SpriteMaterial({ map: tex }));
      sprite.position.set(line.offset, gnomonHeight * 2 + 3, 15);
      sprite.scale.set(6, 6, 1);
      group.add(sprite);
    }
  }

  return group;
}

export default function Preview3D({ sceneData, type }) {
  const containerRef = useRef(null);
  const sceneRef = useRef(null);

  const group = useMemo(() => {
    if (!sceneData) return null;
    if (type === 'card') {
      const { width, height, thickness, qrMatrix, qrSize, qrPosition, frontText, backText, logoPos } = sceneData;
      const grp = new THREE.Group();
      const card = buildCardMesh(width, height, thickness);
      grp.add(card);

      if (qrMatrix) {
        const qrPos = qrPosition || [width * 0.25, 0];
        const qrMesh = buildQRMesh(qrMatrix, qrPos, qrSize || 2, 1.5, thickness / 2);
        grp.add(qrMesh);
      }
      if (frontText) {
        const txt = buildTextMesh(frontText, [0, thickness / 2 + 1, height * 0.3], 1);
        grp.add(txt);
      }
      if (backText) {
        const txt = buildTextMesh(backText, [0, thickness / 2 + 1, -height * 0.3], 1, true);
        grp.add(txt);
      }
      // Rotate card to lay flat (horizontal business card orientation)
      grp.rotation.x = -Math.PI / 2;
      return grp;
    }
    if (type === 'sundial') {
      return buildSundialMesh(sceneData.geoData, sceneData.sundialType);
    }
    return null;
  }, [sceneData, type]);

  useEffect(() => {
    if (!containerRef.current) return;
    const container = containerRef.current;
    const width = container.clientWidth;
    const height = container.clientHeight;

    const scene = new THREE.Scene();
    scene.background = new THREE.Color(0xf0f0f0);

    const camera = new THREE.PerspectiveCamera(45, width / height, 1, 500);
    camera.position.set(80, 60, 80);
    camera.lookAt(0, 0, 0);

    const renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(width, height);
    renderer.setPixelRatio(window.devicePixelRatio);
    renderer.shadowMap.enabled = true;
    container.appendChild(renderer.domElement);

    const controls = new OrbitControls(camera, renderer.domElement);
    controls.enableDamping = true;
    controls.dampingFactor = 0.1;
    controls.target.set(0, 10, 0);
    controls.update();

    // Lights
    const ambient = new THREE.AmbientLight(0xffffff, 0.6);
    scene.add(ambient);
    const directional = new THREE.DirectionalLight(0xffffff, 1.0);
    directional.position.set(50, 100, 50);
    directional.castShadow = true;
    directional.shadow.mapSize.set(1024, 1024);
    scene.add(directional);
    const dir2 = new THREE.DirectionalLight(0xffffff, 0.4);
    dir2.position.set(-50, 30, -50);
    scene.add(dir2);

    // Grid
    const gridHelper = new THREE.GridHelper(100, 20, 0xcccccc, 0xdddddd);
    scene.add(gridHelper);

    if (group) scene.add(group);

    sceneRef.current = { scene, camera, renderer, controls };

    const anim = () => {
      requestAnimationFrame(anim);
      controls.update();
      renderer.render(scene, camera);
    };
    anim();

    const onResize = () => {
      const w = container.clientWidth;
      const h = container.clientHeight;
      camera.aspect = w / h;
      camera.updateProjectionMatrix();
      renderer.setSize(w, h);
    };
    window.addEventListener('resize', onResize);

    return () => {
      window.removeEventListener('resize', onResize);
      renderer.dispose();
      controls.dispose();
      if (container.contains(renderer.domElement)) {
        container.removeChild(renderer.domElement);
      }
    };
  }, [group]);

  return <div ref={containerRef} style={{ width: '100%', height: '100%', minHeight: '400px' }} />;
}
