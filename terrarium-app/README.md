# Terrarium App

Terrarium App è un progetto full-stack IoT pensato per raccogliere dati ambientali da un dispositivo STM32 tramite Bluetooth Low Energy (BLE), salvarli in un database SQLite e mostrarli in una dashboard web.

Il progetto è composto da:
- un backend Node.js che espone API REST e salva i dati;
- un demone Python che parla con il dispositivo BLE;
- una pagina web statica per visualizzare i valori e controllare la ventola.

## Funzionalità principali

- Ricezione di misure di temperatura, umidità e stato della ventola
- Salvataggio dei dati in SQLite
- Aggregazione dei dati live nello storico
- Dashboard web con grafici e controlli remoti
- Invio di comandi alla ventola tramite interfaccia web

## Struttura del progetto

```text
.
├── gateway_daemon.py      # Demone BLE che comunica con l'STM32
├── package.json            # Dipendenze Node.js e script di avvio
├── server.js               # Backend Express + SQLite
├── public/                 # Dashboard web statica
│   ├── index.html
│   └── style.css
└── data/                   # Cartella per il database SQLite (creata localmente)
```

## Requisiti

Prima di iniziare, assicurati di avere installato:

- Node.js 18+
- Python 3.10+
- Bluetooth attivo sul computer
- Un dispositivo STM32 compatibile con il firmware che espone le caratteristiche BLE attese dal demone

## Installazione

1. Clona il repository e entra nella cartella:

```bash
cd terrarium-app
```

2. Installa le dipendenze Node.js:

```bash
npm install
```

3. Crea l'ambiente Python virtuale (consigliato):

```bash
python3 -m venv .venv
source .venv/bin/activate
```

4. Installa le dipendenze Python:

```bash
pip install requests bleak
```

5. Crea la cartella per il database:

```bash
mkdir -p data
```

## Avvio del progetto

### 1) Avvia il backend Node.js

```bash
npm start
```

Il server sarà disponibile su:

```text
http://localhost:3000
```

### 2) Avvia il demone Python

In un secondo terminale, con l'ambiente virtuale attivo:

```bash
python3 gateway_daemon.py
```

Il demone:
- cerca un dispositivo BLE compatibile;
- si connette all'STM32;
- riceve notifiche dalle caratteristiche BLE;
- invia i dati al backend Node.js;
- legge eventuali comandi dalla dashboard e li invia al dispositivo.

### 3) Apri la dashboard

Apri nel browser:

```text
http://localhost:3000
```

## Come funziona il progetto

### Backend Node.js

Il file [server.js](server.js) avvia un server Express che:
- serve la dashboard dalla cartella public;
- salva i dati ricevuti in SQLite;
- espone API REST per:
  - ricevere telemetria: POST /api/telemetria
  - leggere l'ultimo valore: GET /api/storico/recente
  - leggere lo storico aggregato: GET /api/storico/grafico
  - ricevere comandi per la ventola: POST /api/comando-ventola
  - leggere comandi pendenti: GET /api/leggi-comandi

### Demone Python

Il file [gateway_daemon.py](gateway_daemon.py) si occupa di:
- eseguire la scansione BLE;
- connettersi al dispositivo STM32;
- gestire notifiche BLE;
- inviare i dati al server Node.js;
- scrivere eventuali comandi per la ventola sul dispositivo.

### Dashboard web

La dashboard è statica e si trova nella cartella [public](public). Mostra:
- temperatura e umidità dell'ultimo campione;
- un grafico storico;
- un controllo slider per impostare la velocità della ventola.

## API principali

### POST /api/telemetria
Riceve un payload JSON con:

```json
{
  "temperatura": 24.5,
  "umidita": 56.2,
  "ventola": 40
}
```

### GET /api/storico/recente
Restituisce l'ultimo record ricevuto.

### GET /api/storico/grafico
Restituisce lo storico aggregato per il grafico.

### POST /api/comando-ventola
Riceve un valore PWM per la ventola:

```json
{
  "pwm": 75
}
```

### GET /api/leggi-comandi
Restituisce il comando pendente, se presente.

## Note importanti

- Il backend usa SQLite, quindi i dati vengono salvati localmente nel file [data/terrarium.db](data/terrarium.db).
- Il demone dipende dal nome del dispositivo BLE e dalle UUID delle caratteristiche del firmware STM32. Se il dispositivo non viene rilevato, verifica che:
  - il Bluetooth sia attivo;
  - il dispositivo sia visibile;
  - il firmware STM32 esponga le stesse UUID attese dal codice.

## Troubleshooting

### Il backend non parte
- Verifica che la cartella data esista.
- Controlla che Node.js sia installato correttamente.
- Ripeti l'installazione con npm install.

### Il demone non trova il dispositivo
- Verifica che il dispositivo BLE sia acceso e visibile.
- Controlla che il nome del dispositivo contenga STM o Terr, come previsto dal codice.
- Controlla che le UUID BLE nel file gateway_daemon.py corrispondano al firmware in uso.

### La dashboard mostra valori vuoti
- Assicurati che il demone Python sia in esecuzione e che il backend Node.js sia attivo.
- Controlla i log del terminale per eventuali errori di connessione o di decodifica dati.

## Sviluppo futuro possibile

- aggiungere autenticazione per la dashboard;
- sostituire SQLite con un database più scalabile;
- aggiungere una gestione più robusta dei comandi BLE;
- introdurre test automatici per API e demone.
