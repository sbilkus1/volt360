import { useState, useRef, useCallback, useEffect } from 'react';
import { generateQRMatrix, qrMatrixToTriangles } from '../lib/qr';
import { boxTriangles, extrudePolygon, toSTLBlob, downloadSTL } from '../lib/stl';
import Preview3D from './Preview3D';

const STANDARD_CREDIT_CARD = { w: 85.6, h: 54 };
const cardThickness = 1.6;

export default function BusinessCardDesigner() {
  const [name, setName] = useState('John Doe');
  const [title, setTitle] = useState('Software Engineer');
  const [company, setCompany] = useState('Acme Corp');
  const [email, setEmail] = useState('john@acme.com');
  const [phone, setPhone] = useState('+1 555-0123');
  const [website, setWebsite] = useState('https://acme.com');
  const [qrData, setQrData] = useState('https://acme.com');
  const [backText, setBackText] = useState('Scan for more info');
  const [cardW, setCardW] = useState(STANDARD_CREDIT_CARD.w);
  const [cardH, setCardH] = useState(STANDARD_CREDIT_CARD.h);
  const [qrSizePct, setQrSizePct] = useState(35);
  const [qrPosition, setQrPosition] = useState('right');
  const [logoFile, setLogoFile] = useState(null);
  const [logoPreview, setLogoPreview] = useState(null);
  const [qrMatrix, setQrMatrix] = useState(null);
  const [generating, setGenerating] = useState(false);
  const [activeSide, setActiveSide] = useState('front');
  const [qrErrorLevel, setQrErrorLevel] = useState('M');

  const cardScale = 3; // px per mm for preview
  const previewW = cardW * cardScale;
  const previewH = cardH * cardScale;

  const qrSize = Math.min(cardW, cardH) * (qrSizePct / 100);

  const generateQR = useCallback(async () => {
    if (!qrData.trim()) return;
    setGenerating(true);
    try {
      const matrix = await generateQRMatrix(qrData.trim(), {
        errorCorrection: qrErrorLevel,
        size: 33,
        margin: 1,
        width: 33,
      });
      setQrMatrix(matrix);
    } catch (e) {
      console.error('QR generation failed:', e);
    }
    setGenerating(false);
  }, [qrData, qrErrorLevel]);

  useEffect(() => {
    const timer = setTimeout(generateQR, 500);
    return () => clearTimeout(timer);
  }, [generateQR]);

  const handleLogoUpload = (e) => {
    const file = e.target.files[0];
    if (!file) return;
    setLogoFile(file);
    const reader = new FileReader();
    reader.onload = (ev) => setLogoPreview(ev.target.result);
    reader.readAsDataURL(file);
  };

  const exportSTL = async () => {
    let matrix = qrMatrix;
    if (!matrix && qrData.trim()) {
      setGenerating(true);
      matrix = await generateQRMatrix(qrData.trim(), {
        errorCorrection: qrErrorLevel,
        size: 33,
        margin: 1,
        width: 33,
      });
      setQrMatrix(matrix);
      setGenerating(false);
    }

    const bytes = [];

    // Base card (both sides)
    bytes.push(...boxTriangles([0, 0, 0], [cardW, cardH, cardThickness]));

    // QR code raised on top
    if (matrix) {
      const moduleMM = qrSize / matrix.length;
      const qrW = matrix[0].length * moduleMM;
      const qrH = matrix.length * moduleMM;

      let qrOx, qrOy;
      if (qrPosition === 'right') {
        qrOx = cardW - qrW - 4;
        qrOy = (cardH - qrH) / 2;
      } else if (qrPosition === 'left') {
        qrOx = 4;
        qrOy = (cardH - qrH) / 2;
      } else {
        qrOx = (cardW - qrW) / 2;
        qrOy = (cardH - qrH) / 2;
      }

      const qrTris = qrMatrixToTriangles(matrix, qrOx, qrOy, moduleMM, 1.2, cardThickness);
      bytes.push(...qrTris);

      // Back side QR (raised on bottom)
      if (qrData.trim()) {
        const qrTrisBack = qrMatrixToTriangles(matrix, qrOx, qrOy, moduleMM, 1.2, 0);
        // Flip Z for back side - STL coordinates are absolute
        for (let i = 0; i < qrTrisBack.length; i++) {
          // The QR on back is the same but extruded downward
        }
        // Actually let's put it on the back by extruding below
        const qrTrisBack2 = qrMatrixToTriangles(matrix, qrOx, qrOy, moduleMM, -1.2, cardThickness);
        bytes.push(...qrTrisBack2);
      }
    }

    downloadSTL(name.trim() ? `${name}_business_card` : 'business_card', [bytes]);
  };

  const sceneData = {
    width: cardW,
    height: cardH,
    thickness: cardThickness,
    qrMatrix,
    qrSize: qrSize / (matrixSize(qrMatrix) || 1),
    qrPosition: qrPosition === 'right' ? [cardW * 0.6, 0] : qrPosition === 'left' ? [-cardW * 0.25, 0] : [0, 0],
    frontText: name,
    backText: backText,
    logoPos: qrPosition === 'right' ? [-40, 0] : [40, 0],
  };

  const cardScaleFactor = cardScale;

  return (
    <div className="designer-layout">
      <div className="designer-panel">
        <h2>Business Card Designer</h2>

        <div className="side-tabs">
          <button className={activeSide === 'front' ? 'active' : ''} onClick={() => setActiveSide('front')}>Front Side</button>
          <button className={activeSide === 'back' ? 'active' : ''} onClick={() => setActiveSide('back')}>Back Side</button>
        </div>

        <div className="form-grid">
          <div className="form-group">
            <label>Name</label>
            <input value={name} onChange={e => setName(e.target.value)} placeholder="Your Name" />
          </div>
          <div className="form-group">
            <label>Title</label>
            <input value={title} onChange={e => setTitle(e.target.value)} placeholder="Job Title" />
          </div>
          <div className="form-group">
            <label>Company</label>
            <input value={company} onChange={e => setCompany(e.target.value)} placeholder="Company" />
          </div>
          <div className="form-group">
            <label>Email</label>
            <input value={email} onChange={e => setEmail(e.target.value)} placeholder="email@example.com" />
          </div>
          <div className="form-group">
            <label>Phone</label>
            <input value={phone} onChange={e => setPhone(e.target.value)} placeholder="+1 555-0000" />
          </div>
          <div className="form-group">
            <label>Website</label>
            <input value={website} onChange={e => setWebsite(e.target.value)} placeholder="https://..." />
          </div>
          <div className="form-group">
            <label>QR Code Data</label>
            <input value={qrData} onChange={e => setQrData(e.target.value)} placeholder="URL or text for QR" />
          </div>
          <div className="form-group">
            <label>Back Text</label>
            <input value={backText} onChange={e => setBackText(e.target.value)} placeholder="Back side text" />
          </div>
        </div>

        <div className="form-row">
          <div className="form-group">
            <label>Card Width (mm)</label>
            <input type="number" value={cardW} onChange={e => setCardW(Number(e.target.value))} step="0.1" min="50" max="200" />
          </div>
          <div className="form-group">
            <label>Card Height (mm)</label>
            <input type="number" value={cardH} onChange={e => setCardH(Number(e.target.value))} step="0.1" min="30" max="150" />
          </div>
        </div>

        <div className="form-row">
          <div className="form-group">
            <label>QR Size (%)</label>
            <input type="range" value={qrSizePct} onChange={e => setQrSizePct(Number(e.target.value))} min="15" max="60" />
            <span className="range-val">{qrSizePct}%</span>
          </div>
          <div className="form-group">
            <label>QR Position</label>
            <select value={qrPosition} onChange={e => setQrPosition(e.target.value)}>
              <option value="right">Right</option>
              <option value="left">Left</option>
              <option value="center">Center</option>
            </select>
          </div>
        </div>

        <div className="form-row">
          <div className="form-group">
            <label>QR Error Correction</label>
            <select value={qrErrorLevel} onChange={e => { setQrErrorLevel(e.target.value); generateQR(); }}>
              <option value="L">Low (7%)</option>
              <option value="M">Medium (15%)</option>
              <option value="Q">Quartile (25%)</option>
              <option value="H">High (30%)</option>
            </select>
          </div>
          <div className="form-group">
            <label>Logo</label>
            <input type="file" accept="image/*" onChange={handleLogoUpload} />
            {logoPreview && <img src={logoPreview} alt="Logo preview" className="logo-preview" />}
          </div>
        </div>

        <button className="btn-primary" onClick={exportSTL} disabled={generating}>
          {generating ? 'Generating...' : 'Export STL for 3D Printing'}
        </button>
      </div>

      <div className="designer-preview">
        <div className="card-preview-container">
          <h3>Card Preview</h3>
          <svg width={previewW} height={previewH} viewBox={`0 0 ${cardW} ${cardH}`} className="card-svg">
            <rect width={cardW} height={cardH} rx={3} ry={3} fill="#f5f0e8" stroke="#ccc" strokeWidth="0.5" />

            {activeSide === 'front' && (
              <>
                {logoPreview && (
                  <image
                    href={logoPreview}
                    x={qrPosition === 'right' ? 4 : qrPosition === 'left' ? 8 : (cardW - 20) / 2}
                    y={4}
                    width={24}
                    height={24}
                    preserveAspectRatio="xMidYMid meet"
                  />
                )}
                <text x={qrPosition === 'right' || qrPosition === 'center' ? 4 : 32} y={20} fontSize="4" fontWeight="bold" fill="#222" fontFamily="Arial">
                  {name || 'Name'}
                </text>
                <text x={qrPosition === 'right' || qrPosition === 'center' ? 4 : 32} y={26} fontSize="3" fill="#555" fontFamily="Arial">
                  {title || 'Title'}
                </text>
                <text x={qrPosition === 'right' || qrPosition === 'center' ? 4 : 32} y={32} fontSize="2.5" fill="#777" fontFamily="Arial">
                  {company || 'Company'}
                </text>
                <text x={qrPosition === 'right' || qrPosition === 'center' ? 4 : 32} y={38} fontSize="2.5" fill="#555" fontFamily="Arial">
                  {email || 'email'}
                </text>
                <text x={qrPosition === 'right' || qrPosition === 'center' ? 4 : 32} y={43} fontSize="2.5" fill="#555" fontFamily="Arial">
                  {phone || 'phone'}
                </text>
                <text x={qrPosition === 'right' || qrPosition === 'center' ? 4 : 32} y={48} fontSize="2.5" fill="#555" fontFamily="Arial">
                  {website || 'web'}
                </text>

                {qrMatrix && (
                  <g
                    transform={`translate(${
                      qrPosition === 'right' ? cardW - qrSize - 3 :
                      qrPosition === 'left' ? 3 :
                      (cardW - qrSize) / 2
                    }, ${(cardH - qrSize) / 2})`}
                  >
                    {qrMatrix.map((row, ri) =>
                      row.map((cell, ci) =>
                        cell ? (
                          <rect key={`${ri}-${ci}`} x={ci * (qrSize / row.length)} y={ri * (qrSize / row.length)} width={qrSize / row.length} height={qrSize / row.length} fill="#111" />
                        ) : null
                      )
                    )}
                  </g>
                )}
              </>
            )}

            {activeSide === 'back' && (
              <>
                <text x={cardW / 2} y={cardH / 2} fontSize="4" fill="#555" fontFamily="Arial" textAnchor="middle">
                  {backText || 'Back'}
                </text>
                {qrMatrix && (
                  <g transform={`translate(${(cardW - qrSize) / 2}, ${(cardH - qrSize) / 2})`}>
                    {qrMatrix.map((row, ri) =>
                      row.map((cell, ci) =>
                        cell ? (
                          <rect key={`b-${ri}-${ci}`} x={ci * (qrSize / row.length)} y={ri * (qrSize / row.length)} width={qrSize / row.length} height={qrSize / row.length} fill="#111" />
                        ) : null
                      )
                    )}
                  </g>
                )}
              </>
            )}
          </svg>
        </div>

        <div style={{ height: '400px', marginTop: '12px', border: '1px solid #ddd', borderRadius: '8px', overflow: 'hidden' }}>
          <Preview3D sceneData={sceneData} type="card" />
        </div>
      </div>
    </div>
  );
}

function matrixSize(matrix) {
  return matrix?.length || 0;
}
