import asyncio
import struct
import requests
from bleak import BleakClient, BleakScanner
from bleak.exc import BleakError

# backend node.js
SERVER_URL = "http://localhost:3000"

# UUIDs
SERVICE_UUID    = "00000000-0000-0000-0000-0000000000cd"
TEMP_CHAR_UUID  = "00000000-0000-0000-0000-000000000100"
HUMID_CHAR_UUID = "00000000-0000-0000-0000-000000000200"
FAN_CHAR_UUID   = "00000000-0000-0000-0000-000000000300"

# stato in memoria ram
stato_attuale = {"temperatura": None, "umidita": None, "ventola": None}

def temp_handler(sender, data):
    try:
        val = struct.unpack("<H", data)[0] / 10.0 if len(data) >= 2 else data[0]
        stato_attuale["temperatura"] = val
        print(f"🌡️ Temperatura aggiornata: {val}°C")
    except Exception as e:
        print(f"Errore temp: {e}")

def humid_handler(sender, data):
    try:
        val = struct.unpack("<H", data)[0] / 10.0 if len(data) >= 2 else data[0]
        stato_attuale["umidita"] = val
        print(f"💧 Umidità aggiornata: {val}%")
    except Exception as e:
        print(f"Errore umidità: {e}")

def fan_handler(sender, data):
    try:
        stato_attuale["ventola"] = data[0]
        print(f"🌀 Ventola aggiornata: {data[0]}%")
    except Exception as e:
        print(f"Errore ventola: {e}")

async def sync_con_server():
    """Invia lo stato attuale al Database ogni 5 secondi"""
    while True:
        await asyncio.sleep(5)
        
        # evitiamo di inserire record nulli se non abbiamo ancora letto nulla
        if stato_attuale["temperatura"] is not None:
            try:
                # Payload JSON
                res = requests.post(f"{SERVER_URL}/api/telemetria", json=stato_attuale, timeout=2)
                if res.status_code == 200:
                    print(f"[SYNC OK] Salvato su DB: {stato_attuale}")
            except requests.exceptions.RequestException:
                print("Impossibile contattare il backend")

async def ascolta_comandi(client):
    """Interroga il backend per vedere se la dashboard ha inviato comandi alla ventola"""
    while True:
        await asyncio.sleep(2) # Polling 
        try:
            res = requests.get(f"{SERVER_URL}/api/leggi-comandi", timeout=2)
            if res.status_code == 200:
                comando = res.json().get("comando")
                
                if comando is not None:
                    pwm_val = max(0, min(100, int(comando)))
                    print(f"\n>>> RICEVUTO COMANDO WEB: Set Ventola {pwm_val}%")
                    
                    # 💡 FIX FONDAMENTALE: Salva subito il valore nella RAM locale per il DB!
                    stato_attuale["ventola"] = pwm_val
                    
                    # Scrittura via BLE
                    await client.write_gatt_char(FAN_CHAR_UUID, bytearray([pwm_val]), response=True)
                    print(f"<<< COMANDO INVIATO a STM32\n")
                    
        except requests.exceptions.RequestException:
            pass
        except Exception as e:
            print(f"Errore scrittura BLE: {e}")

async def gestisci_dispositivo():
    """Ciclo principale di connessione e riconnessione"""
    while True:
        print("\nScansione dispositivi BLE in corso...")
        dispositivi = await BleakScanner.discover()
        
        indirizzo_stm32 = None
        for d in dispositivi:
            # Sostituisci in d.name con il nome esatto impostato su CubeMX
            if d.name and ("STM" in d.name or "Terr" in d.name): 
                indirizzo_stm32 = d.address
                print(f"Trovato: {d.name} ({d.address})")
                break

        if not indirizzo_stm32:
            print("Dispositivo non trovato. Riprovo tra 5 secondi...")
            await asyncio.sleep(5)
            continue

        try:
            print(f"Tentativo di connessione a {indirizzo_stm32}...")
            async with BleakClient(indirizzo_stm32) as client:
                print("--- CONNESSO CON SUCCESSO ---")
                
                # Attivazione Notifiche
                await client.start_notify(TEMP_CHAR_UUID, temp_handler)
                await client.start_notify(HUMID_CHAR_UUID, humid_handler)
                await client.start_notify(FAN_CHAR_UUID, fan_handler)
                
                print("Notifiche attivate. Demone in ascolto (Ctrl+C per uscire)")
                
                # Avvio Task per parlare con il server locale Node.js
                task_sync = asyncio.create_task(sync_con_server())
                task_comandi = asyncio.create_task(ascolta_comandi(client))
                
                # Il gather mantiene attiva la connessione finché non c'è un errore hardware
                await asyncio.gather(task_sync, task_comandi)
                
        except BleakError as e:
            print(f"\n--- DISCONNESSO: {e} ---")
            print("Tentativo di riconnessione automatico...")
            await asyncio.sleep(3)

if __name__ == "__main__":
    try:
        asyncio.run(gestisci_dispositivo())
    except KeyboardInterrupt:
        print("\nDemone arrestato dall'utente.")
