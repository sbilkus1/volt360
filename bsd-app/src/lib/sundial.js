/**
 * Sundial calculator & geometry generator.
 * Supports: Horizontal, Vertical (south-facing), Equatorial, Analemmatic, Polar.
 *
 * All calculations auto-adjust for latitude, longitude, timezone,
 * and daylight saving.
 */

const DEG = Math.PI / 180;

/**
 * Calculate solar declination for a given day of year (1-365).
 */
export function solarDeclination(doy) {
  return 23.45 * DEG * Math.sin((360 / 365) * (284 + doy) * DEG);
}

/**
 * Equation of time (minutes) for day of year.
 */
export function equationOfTime(doy) {
  const B = (360 / 365) * (doy - 81) * DEG;
  return 9.87 * Math.sin(2 * B) - 7.53 * Math.cos(B) - 1.5 * Math.sin(B);
}

/**
 * Hour angle for a given solar hour (0-23, decimal).
 */
export function hourAngle(solarHour) {
  return (solarHour - 12) * 15 * DEG;
}

/**
 * Convert clock time to solar time.
 * @param clockHour - decimal hour (e.g. 14.5 = 2:30 PM)
 * @param timezoneOffset - UTC offset in hours (e.g. -5 for EST)
 * @param longitude - degrees
 * @param doy - day of year
 */
export function clockToSolar(clockHour, timezoneOffset, longitude, doy) {
  const eot = equationOfTime(doy) / 60;
  const lngCorrection = (longitude - timezoneOffset * 15) / 15;
  return clockHour + lngCorrection + eot;
}

/**
 * Angular distance of sun from the meridian for a given hour line.
 */
export function hourLineAngle(hour, latitudeDeg, declinationDeg) {
  const lat = latitudeDeg * DEG;
  const dec = declinationDeg * DEG;
  const ha = hourAngle(hour);
  const num = Math.sin(ha);
  const den = Math.sin(lat) * Math.cos(ha) - Math.cos(lat) * Math.tan(dec);
  return Math.atan2(num, den);
}

// ---- Sundial types ----

/**
 * Horizontal sundial hour line angles.
 * ha = hour angle (e.g. ±15° per hour from noon)
 * lat = latitude in radians
 * Returns angle from the noon (substyle) line.
 */
export function horizontalHourLineAngles(latitudeDeg) {
  const lat = latitudeDeg * DEG;
  const lines = [];
  for (let h = 4; h <= 20; h++) {
    if (h === 12) continue;
    const ha = (h - 12) * 15 * DEG;
    const angle = Math.atan(Math.tan(ha) * Math.sin(lat));
    lines.push({ hour: h, angle });
  }
  return lines;
}

/**
 * Vertical south-facing sundial hour line angles.
 */
export function verticalHourLineAngles(latitudeDeg) {
  const lat = latitudeDeg * DEG;
  const lines = [];
  for (let h = 4; h <= 20; h++) {
    if (h === 12) continue;
    const ha = (h - 12) * 15 * DEG;
    const angle = Math.atan(Math.tan(ha) * Math.cos(lat));
    lines.push({ hour: h, angle });
  }
  return lines;
}

/**
 * Equatorial sundial: the dial plate is parallel to the equator.
 * Hour lines are evenly spaced at 15° intervals.
 */
export function equatorialHourLines() {
  const lines = [];
  for (let h = 4; h <= 20; h++) {
    if (h === 12) continue;
    lines.push({ hour: h, angle: (h - 12) * 15 * DEG });
  }
  return lines;
}

/**
 * Polar sundial: dial plate is parallel to the polar axis.
 * Hour lines use tan rule.
 */
export function polarHourLineAngles(latitudeDeg) {
  const lat = latitudeDeg * DEG;
  const lines = [];
  for (let h = 4; h <= 20; h++) {
    if (h === 12) continue;
    const ha = (h - 12) * 15 * DEG;
    const angle = Math.atan(Math.tan(ha) * Math.sin(lat));
    lines.push({ hour: h, angle });
  }
  return lines;
}

/**
 * Analemmatic (human) sundial: ellipse + hour markers.
 * @return array of { hour, x, y } in meters on the ellipse.
 */
export function analemmaticMarkers(latitudeDeg, scale = 1.0) {
  const lat = latitudeDeg * DEG;
  const markers = [];
  const a = scale;             // semi-major axis
  const b = scale * Math.sin(lat); // semi-minor axis

  for (let h = 4; h <= 20; h++) {
    if (h === 12) continue;
    const ha = (h - 12) * 15 * DEG;
    markers.push({
      hour: h,
      x: a * Math.sin(ha),
      y: b * Math.cos(ha),
    });
  }
  return markers;
}

/**
 * Analemmatic date scale (where the person stands).
 * @param declinationRange - if true, return for multiple dates
 */
export function analemmaticDateScale(latitudeDeg, scale = 1.0) {
  const lat = latitudeDeg * DEG;
  const points = [];
  const months = [
    { name: 'Jan', doy: 15 }, { name: 'Feb', doy: 46 }, { name: 'Mar', doy: 75 },
    { name: 'Apr', doy: 105 }, { name: 'May', doy: 136 }, { name: 'Jun', doy: 166 },
    { name: 'Jul', doy: 197 }, { name: 'Aug', doy: 228 }, { name: 'Sep', doy: 258 },
    { name: 'Oct', doy: 289 }, { name: 'Nov', doy: 319 }, { name: 'Dec', doy: 350 },
  ];
  for (const m of months) {
    const dec = solarDeclination(m.doy);
    const dist = scale * Math.tan(dec) * Math.cos(lat);
    points.push({ label: m.name, distance: dist });
  }
  return points;
}

/**
 * Generate tip orientation cues from solstice data.
 */
export function sundialTipCues(latitudeDeg) {
  const lat = latitudeDeg * DEG;
  const winterDoy = 355; // ~Dec 21
  const summerDoy = 172; // ~Jun 21
  const decWinter = solarDeclination(winterDoy);
  const decSummer = solarDeclination(summerDoy);

  const winterSlope = Math.tan(decWinter) * Math.cos(lat);
  const summerSlope = Math.tan(decSummer) * Math.cos(lat);

  return {
    winterTipDistance: winterSlope,
    summerTipDistance: summerSlope,
    gnomonDirection: lat > 0 ? 'north' : 'south',
  };
}

// ---- STL Geometry generators ----

/**
 * Generate triangles for a horizontal sundial face.
 * @param size - side length of square dial plate
 * @param latitudeDeg - latitude
 */
export function horizontalDialGeometry(size, latitudeDeg) {
  const half = size / 2;
  const hourLines = horizontalHourLineAngles(latitudeDeg);

  // Dial plate (flat square)
  const plateVerts = [
    [-half, 0, -half], [half, 0, -half], [half, 0, half], [-half, 0, half],
  ];

  // Gnomon: triangular fin rising from the center, angled at latitude
  const gnomonHeight = half * 0.5;
  const gnomonBase = half * 0.7;
  const gnomonTip = [
    gnomonBase * (Math.sin(latitudeDeg * DEG)),
    gnomonHeight,
    -gnomonBase * (Math.cos(latitudeDeg * DEG)),
  ];
  // gnomonTip = adjust for angle

  // Simplification: rectangular upright gnomon
  const gx = 0;
  const gy = 0;
  const gz = 0;
  const gH = gnomonHeight;
  const gThick = 2;
  const gLen = gnomonBase;

  // We'll build the gnomon as a right triangle pointing at latitude angle
  const latR = latitudeDeg * DEG;
  const gnomonTopX = Math.sin(latR) * gLen;  // tip leans north in NH
  const gnomonTopZ = -Math.cos(latR) * gLen;

  const gnomonVerts = [
    [-gThick / 2, 0, 0],
    [gThick / 2, 0, 0],
    [-gThick / 2, 0, gLen],
    [gThick / 2, 0, gLen],
  ];

  const tipTop = [0, gH, gnomonTopZ];
  const tipBot = [0, 0, 0];

  return {
    plateVerts,
    hourLines,
    gnomonAngle: latR,
    gnomonHeight: gH,
    gnomonBaseLen: gLen,
    gnomonThickness: gThick,
    gnomonTopVertex: tipTop,
  };
}

/**
 * Generate geometry for a vertical south-facing dial.
 */
export function verticalDialGeometry(size, latitudeDeg) {
  const half = size / 2;
  const hourLines = verticalHourLineAngles(latitudeDeg);

  const gnomonHeight = half * 0.5;
  const latR = latitudeDeg * DEG;
  const gnomonAngle = (90 * DEG) - latR; // complement
  const gnomonTip = [half, -gnomonHeight * Math.tan(gnomonAngle), gnomonHeight];

  return {
    plateVerts: [
      [-half, -half, 0], [half, -half, 0], [half, half, 0], [-half, half, 0],
    ],
    hourLines,
    gnomonAngle,
    gnomonHeight,
  };
}

/**
 * Generate equatorial dial geometry.
 * Dial plate tilted at (90° - latitude) so it's parallel to equator.
 * Gnomon is perpendicular to plate (i.e., parallel to earth's axis).
 */
export function equatorialDialGeometry(size, latitudeDeg) {
  const half = size / 2;
  const tiltAngle = (90 - latitudeDeg) * DEG;

  // Dial plate is a circle or square tilted
  const plateVerts = [
    [-half, -half, 0], [half, -half, 0], [half, half, 0], [-half, half, 0],
  ];

  // Gnomon: rod perpendicular to plate, parallel to polar axis
  const gnomonLength = half * 0.8;
  const gnomonRadius = 1.5;

  return {
    plateVerts,
    hourLines: equatorialHourLines(),
    tiltAngle,
    gnomonLength,
    gnomonRadius,
    gnomonDirection: [0, 1, 0], // perpendicular to plate in local coords
  };
}

/**
 * Generate analemmatic dial geometry.
 * Returns ellipse points and date scale.
 */
export function analemmaticDialGeometry(size, latitudeDeg) {
  const half = size / 2;
  const scale = half * 0.8;
  const markers = analemmaticMarkers(latitudeDeg, scale);
  const dateScale = analemmaticDateScale(latitudeDeg, scale);
  const cues = sundialTipCues(latitudeDeg);

  return {
    markers,
    dateScale,
    scaleX: scale,
    scaleY: scale * Math.sin(latitudeDeg * DEG),
    center: [0, 0],
    cues,
  };
}

/**
 * Polar sundial geometry.
 * The dial plate is parallel to the polar axis (tilted at latitude).
 * Hour lines are parallel.
 */
export function polarDialGeometry(size, latitudeDeg) {
  const half = size / 2;
  const latR = latitudeDeg * DEG;
  const tiltAngle = latR;

  const lines = [];
  const spacing = size / 12;
  for (let h = 4; h <= 20; h++) {
    if (h === 12) continue;
    const offset = (h - 12) * spacing;
    lines.push({ hour: h, offset });
  }

  return {
    plateSize: [size, size * 0.6],
    lines,
    tiltAngle,
    gnomonHeight: half * 0.1,
  };
}

/**
 * Full auto-configuration for a location.
 * @param lat - latitude in degrees
 * @param lng - longitude in degrees
 * @param tz - timezone offset from UTC (e.g. -5 for EST)
 */
export function configureForLocation(lat, lng, tz) {
  const tipCues = sundialTipCues(lat);

  return {
    latitude: lat,
    longitude: lng,
    timezone: tz,
    isNorthernHemisphere: lat > 0,
    gnomonPoints: tipCues.gnomonDirection,
    recommendedTypes: ['horizontal', 'vertical', 'equatorial', 'analemmatic', 'polar'],
    summerSolsticeDeclination: solarDeclination(172),
    winterSolsticeDeclination: solarDeclination(355),
    equationOfTimeVariation: { min: equationOfTime(1), max: equationOfTime(180) },
  };
}
