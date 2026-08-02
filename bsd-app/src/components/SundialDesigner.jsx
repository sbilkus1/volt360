import { useState, useEffect, useCallback } from 'react';
import { downloadSTL } from '../lib/stl';
import {
  configureForLocation,
  horizontalDialGeometry,
  verticalDialGeometry,
  equatorialDialGeometry,
  analemmaticDialGeometry,
  polarDialGeometry,
} from '../lib/sundial';
import Preview3D from './Preview3D';

const SUNDIAL_TYPES = [
  { id: 'horizontal', label: 'Horizontal', desc: 'Flat on ground. Classic garden sundial. Gnomon angled at your latitude.' },
  { id: 'vertical', label: 'Vertical (South)', desc: 'Wall-mounted. Best on south-facing walls. Good for buildings.' },
  { id: 'equatorial', label: 'Equatorial', desc: 'Dial parallel to equator. Easy to read, hour lines evenly spaced.' },
  { id: 'analemmatic', label: 'Analemmatic', desc: 'Human sundial. Stand on the date marker — your shadow tells time.' },
  { id: 'polar', label: 'Polar', desc: 'Dial parallel to polar axis. Hour lines are parallel to each other.' },
];

function gmtimeOffset() {
  const d = new Date();
  return -d.getTimezoneOffset() / 60;
}

export default function SundialDesigner() {
  const [latitude, setLatitude] = useState(40.7128);
  const [longitude, setLongitude] = useState(-74.006);
  const [timezone, setTimezone] = useState(gmtimeOffset());
  const [sundialType, setSundialType] = useState('horizontal');
  const [dialSize, setDialSize] = useState(120);
  const [config, setConfig] = useState(null);
  const [useCurrentLocation, setUseCurrentLocation] = useState(false);
  const [locationStatus, setLocationStatus] = useState('');

  const detectLocation = useCallback(() => {
    if (!navigator.geolocation) {
      setLocationStatus('Geolocation not supported');
      return;
    }
    setLocationStatus('Detecting...');
    navigator.geolocation.getCurrentPosition(
      (pos) => {
        setLatitude(parseFloat(pos.coords.latitude.toFixed(4)));
        setLongitude(parseFloat(pos.coords.longitude.toFixed(4)));
        setTimezone(gmtimeOffset());
        setLocationStatus(`Location set: ${pos.coords.latitude.toFixed(2)}, ${pos.coords.longitude.toFixed(2)}`);
        setUseCurrentLocation(true);
      },
      () => {
        setLocationStatus('Permission denied. Using defaults.');
      },
      { enableHighAccuracy: false, timeout: 10000 },
    );
  }, []);

  useEffect(() => {
    setConfig(configureForLocation(latitude, longitude, timezone));
  }, [latitude, longitude, timezone]);

  useEffect(() => {
    if (useCurrentLocation) detectLocation();
  }, [useCurrentLocation]);

  const geoData = (() => {
    if (!config) return null;
    switch (sundialType) {
      case 'horizontal': return horizontalDialGeometry(dialSize, latitude);
      case 'vertical': return verticalDialGeometry(dialSize, latitude);
      case 'equatorial': return equatorialDialGeometry(dialSize, latitude);
      case 'analemmatic': return analemmaticDialGeometry(dialSize, latitude);
      case 'polar': return polarDialGeometry(dialSize, latitude);
      default: return null;
    }
  })();

  const sceneData = { geoData, sundialType };

  const exportSTL = () => {
    const name = `${sundialType}_sundial_lat${Math.abs(latitude).toFixed(0)}${latitude >= 0 ? 'N' : 'S'}`;
    // For STL export we'd generate proper triangles, but let's use a placeholder
    // that describes the geometry for now — real STL would need full triangulation
    // of the dial plate, gnomon, and hour lines.
    const bytes = [];
    const half = dialSize / 2;

    // Simple plate
    const plateH = 4;
    const plateVerts = [
      [-half, 0, -half], [half, 0, -half], [half, 0, half], [-half, 0, half],
      [-half, plateH, -half], [half, plateH, -half], [half, plateH, half], [-half, plateH, half],
    ];

    const addQuad = (a, b, c, d) => {
      const tri = (p0, p1, p2) => {
        const u = [p1[0]-p0[0], p1[1]-p0[1], p1[2]-p0[2]];
        const v = [p2[0]-p0[0], p2[1]-p0[1], p2[2]-p0[2]];
        const n = [u[1]*v[2]-u[2]*v[1], u[2]*v[0]-u[0]*v[2], u[0]*v[1]-u[1]*v[0]];
        const len = Math.sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2])||1;
        for (let i=0;i<3;i++) bytes.push(...f32(n[i]/len));
        for (const p of [p0,p1,p2]) for (let i=0;i<3;i++) bytes.push(...f32(p[i]));
        bytes.push(0,0);
      };
      tri(a,b,c); tri(a,c,d);
    };

    const f32 = (f) => { const b=new ArrayBuffer(4); new DataView(b).setFloat32(0,f,true); return new Uint8Array(b); };

    // Bottom
    addQuad(plateVerts[3],plateVerts[2],plateVerts[1],plateVerts[0]);
    // Top
    addQuad(plateVerts[4],plateVerts[5],plateVerts[6],plateVerts[7]);
    // Sides
    addQuad(plateVerts[0],plateVerts[1],plateVerts[5],plateVerts[4]);
    addQuad(plateVerts[1],plateVerts[2],plateVerts[6],plateVerts[5]);
    addQuad(plateVerts[2],plateVerts[3],plateVerts[7],plateVerts[6]);
    addQuad(plateVerts[3],plateVerts[0],plateVerts[4],plateVerts[7]);

    if (sundialType === 'horizontal') {
      const gd = horizontalDialGeometry(dialSize, latitude);
      const gThick = gd.gnomonThickness;
      const gH = gd.gnomonHeight;
      const gLen = gd.gnomonBaseLen;
      const latR = latitude * Math.PI / 180;
      const tipX = Math.sin(latR) * gLen;
      const tipZ = -Math.cos(latR) * gLen;
      const gVerts = [
        [-gThick/2, plateH, 0], [gThick/2, plateH, 0],
        [-gThick/2, plateH, gLen], [gThick/2, plateH, gLen],
        [0, plateH + gH, tipZ],
      ];
      addQuad(gVerts[0],gVerts[1],gVerts[3],gVerts[2]); // base
      addQuad(gVerts[0],gVerts[1],gVerts[4],gVerts[4]); // front face
      addQuad(gVerts[2],gVerts[4],gVerts[4],gVerts[3]); // back face
      addQuad(gVerts[0],gVerts[2],gVerts[4],gVerts[4]); // left
      addQuad(gVerts[1],gVerts[4],gVerts[4],gVerts[3]); // right
    }

    const count = bytes.length / 50;
    const header = new Uint8Array(84);
    const nameBytes = new TextEncoder().encode(name.slice(0,79));
    header.set(nameBytes, 0);
    const out = new Uint8Array(84 + bytes.length);
    out.set(header, 0);
    new DataView(out.buffer).setUint32(80, count, true);
    out.set(bytes, 84);
    const blob = new Blob([out], { type: 'application/sla' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = name.replace(/\s+/g,'_') + '.stl';
    a.click();
    URL.revokeObjectURL(url);
  };

  return (
    <div className="designer-layout">
      <div className="designer-panel">
        <h2>Sundial Designer</h2>

        <div className="form-row">
          <div className="form-group">
            <label>Latitude (&deg;)</label>
            <input type="number" value={latitude} onChange={e => setLatitude(Number(e.target.value))} step="0.01" min="-90" max="90" />
          </div>
          <div className="form-group">
            <label>Longitude (&deg;)</label>
            <input type="number" value={longitude} onChange={e => setLongitude(Number(e.target.value))} step="0.01" min="-180" max="180" />
          </div>
          <div className="form-group">
            <label>Timezone (UTC)</label>
            <input type="number" value={timezone} onChange={e => setTimezone(Number(e.target.value))} step="0.5" min="-12" max="14" />
          </div>
        </div>

        <button className="btn-secondary" onClick={detectLocation}>
          {locationStatus || 'Use My Location'}
        </button>
        {locationStatus && <p className="status-msg">{locationStatus}</p>}

        {config && (
          <div className="config-info">
            <p><strong>Hemisphere:</strong> {config.isNorthernHemisphere ? 'Northern' : 'Southern'}</p>
            <p><strong>Gnomon points:</strong> {config.gnomonPoints}</p>
            <p><strong>Summer solstice declination:</strong> {config.summerSolsticeDeclination.toFixed(2)}&deg;</p>
            <p><strong>Winter solstice declination:</strong> {config.winterSolsticeDeclination.toFixed(2)}&deg;</p>
          </div>
        )}

        <h3>Select Sundial Type</h3>
        <div className="type-grid">
          {SUNDIAL_TYPES.map(t => (
            <button
              key={t.id}
              className={`type-card ${sundialType === t.id ? 'active' : ''}`}
              onClick={() => setSundialType(t.id)}
            >
              <strong>{t.label}</strong>
              <span>{t.desc}</span>
            </button>
          ))}
        </div>

        <div className="form-group" style={{ marginTop: '12px' }}>
          <label>Dial Size (mm) ({sundialType !== 'analemmatic' ? 'side/total' : 'major axis'})</label>
          <input type="number" value={dialSize} onChange={e => setDialSize(Number(e.target.value))} step="5" min="50" max="500" />
        </div>

        <button className="btn-primary" onClick={exportSTL}>
          Export STL for 3D Printing
        </button>
      </div>

      <div className="designer-preview">
        <div className="sundial-info">
          <h3>{SUNDIAL_TYPES.find(t => t.id === sundialType)?.label} Sundial</h3>
          <p>Latitude: {latitude}&deg; | Longitude: {longitude}&deg; | Size: {dialSize}mm</p>
        </div>
        <div style={{ height: '500px', border: '1px solid #ddd', borderRadius: '8px', overflow: 'hidden' }}>
          {geoData && <Preview3D sceneData={sceneData} type="sundial" />}
        </div>
      </div>
    </div>
  );
}
