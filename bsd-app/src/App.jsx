import { useState } from 'react';
import BusinessCardDesigner from './components/BusinessCardDesigner';
import SundialDesigner from './components/SundialDesigner';

export default function App() {
  const [tab, setTab] = useState('cards');

  return (
    <div className="app">
      <header className="app-header">
        <h1>3D Print Designer</h1>
        <nav className="tab-nav">
          <button
            className={`tab-btn ${tab === 'cards' ? 'active' : ''}`}
            onClick={() => setTab('cards')}
          >
            Business Cards
          </button>
          <button
            className={`tab-btn ${tab === 'sundials' ? 'active' : ''}`}
            onClick={() => setTab('sundials')}
          >
            Sundials
          </button>
        </nav>
      </header>

      <main className="app-main">
        {tab === 'cards' && <BusinessCardDesigner />}
        {tab === 'sundials' && <SundialDesigner />}
      </main>
    </div>
  );
}
