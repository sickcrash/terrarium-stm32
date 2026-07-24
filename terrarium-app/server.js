const express = require('express');
const sqlite3 = require('sqlite3').verbose();
const cors = require('cors');
const path = require('path');

const app = express();
app.use(express.json());
app.use(cors());

// Espone la cartella public per servire la dashboard
app.use(express.static(path.join(__dirname, 'public')));

// Inizializza il DB nella cartella "data"
const dbPath = path.join(__dirname, 'data', 'terrarium.db');
const db = new sqlite3.Database(dbPath, (err) => {
    if (err) console.error("Errore apertura DB:", err);
    else console.log("DB SQLite connesso.");
});

// 1. Tabella ad alta frequenza (buffer temporaneo)
db.run(`CREATE TABLE IF NOT EXISTS telemetria_live (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    temperatura REAL,
    umidita REAL,
    ventola INTEGER
)`);

// 2. Tabella a bassa frequenza (storico a lungo termine)
db.run(`CREATE TABLE IF NOT EXISTS telemetria_storico (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    temperatura REAL,
    umidita REAL,
    ventola INTEGER
)`);

// --- JOB DI AGGREGAZIONE E PULIZIA (Ogni 20 minuti) ---
const MINUTI_AGGREGAZIONE = 1; // per scopo di testing lasciamo 1 minuto

setInterval(() => {
    // Calcola la media (e il picco della ventola) degli ultimi 20 minuti e salvali nello storico
    const sqlAggrega = `
        INSERT INTO telemetria_storico (temperatura, umidita, ventola)
        SELECT 
            ROUND(AVG(temperatura), 1), 
            ROUND(AVG(umidita), 1), 
            MAX(ventola) -- Se la ventola si è accesa anche solo per un attimo, lo vogliamo sapere
        FROM telemetria_live
        WHERE timestamp >= datetime('now', '-${MINUTI_AGGREGAZIONE} minutes')
        HAVING COUNT(*) > 0; -- Inserisce solo se ci sono stati campionamenti
    `;

    db.run(sqlAggrega, function(err) {
        if (err) console.error("Errore aggregazione dati:", err.message);
        else if (this.changes > 0) {
            console.log(`[ROLLUP] Dati degli ultimi ${MINUTI_AGGREGAZIONE} min aggregati nello storico.`);
            
            // Subito dopo, pulisce la tabella live eliminando i record più vecchi di 1 ora
            const sqlPulisci = `DELETE FROM telemetria_live WHERE timestamp < datetime('now', '-1 hour')`;
            db.run(sqlPulisci, function(errClean) {
                if (errClean) console.error("Errore pulizia tabella live:", errClean.message);
                else console.log(`[CLEANUP] Eliminati ${this.changes} vecchi record live.`);
            });
        }
    });
}, MINUTI_AGGREGAZIONE * 60 * 1000);

// --- API GESTIONE DATI ---

// Il demone Python invia i dati a raffica qui (vanno nel buffer live)
app.post('/api/telemetria', (req, res) => {
    const { temperatura, umidita, ventola } = req.body;
    db.run(
        `INSERT INTO telemetria_live (temperatura, umidita, ventola) VALUES (?, ?, ?)`,
        [temperatura, umidita, ventola],
        function(err) {
            if (err) return res.status(500).json({ error: err.message });
            res.json({ status: 'ok', id: this.lastID });
        }
    );
});

// La Dashboard chiede SOLO l'ultimo dato per i widget numerici (lettura istantanea)
app.get('/api/storico/recente', (req, res) => {
    db.get(`SELECT * FROM telemetria_live ORDER BY timestamp DESC LIMIT 1`, [], (err, row) => {
        if (err) return res.status(500).json({ error: err.message });
        res.json(row || {});
    });
});

// La Dashboard chiede lo storico aggregato per il grafico (es. ultimi 50 campioni da 20 min)
app.get('/api/storico/grafico', (req, res) => {
    db.all(`SELECT * FROM telemetria_storico ORDER BY timestamp DESC LIMIT 50`, [], (err, rows) => {
        if (err) return res.status(500).json({ error: err.message });
        res.json(rows.reverse()); 
    });
});

// --- API GESTIONE COMANDI ---
let comandoVentolaPendente = null;

app.post('/api/comando-ventola', (req, res) => {
    comandoVentolaPendente = req.body.pwm;
    res.json({ success: true });
});

app.get('/api/leggi-comandi', (req, res) => {
    res.json({ comando: comandoVentolaPendente });
    comandoVentolaPendente = null; 
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    console.log(`Server Backend attivo su http://localhost:${PORT}`);
});