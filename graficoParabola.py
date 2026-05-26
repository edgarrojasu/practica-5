import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np
from collections import defaultdict

# Leer archivo
ruta = r"D:\Universidad\Info2\practicas lab\practica5\trayectoria_particulas.txt"

particulas = defaultdict(lambda: {'x': [], 'y': [], 'masa': []})
obstaculos = []
eventos_obstaculo = []
eventos_particulas = []

with open(ruta, 'r') as f:
    for linea in f:
        linea = linea.strip()
        if not linea or linea.startswith('#'):
            continue

        partes = linea.split()

        # Obstáculos fijos: OBS1 x y ancho alto
        if partes[0].startswith('OBS'):
            obstaculos.append({
                'id': partes[0],
                'x': float(partes[1]),
                'y': float(partes[2]),
                'ancho': float(partes[3]),
                'alto': float(partes[4])
            })

        # Posición de partícula: P1 x y velX velY masa
        elif partes[0].startswith('P') and len(partes) == 6:
            pid = partes[0]
            particulas[pid]['x'].append(float(partes[1]))
            particulas[pid]['y'].append(float(partes[2]))
            particulas[pid]['masa'].append(float(partes[5]))

        # Evento colisión obstáculo
        elif linea.startswith('COLISION_OBSTACULO'):
            eventos_obstaculo.append(linea)

        # Evento colisión entre partículas
        elif linea.startswith('COLISION_PARTICULAS'):
            eventos_particulas.append(linea)

# --- Graficar ---
colores = {'P1': 'red', 'P2': 'green', 'P3': 'gold', 'P4': 'cyan'}

fig, ax = plt.subplots(figsize=(12, 8))

# Dibujar obstáculos
for obs in obstaculos:
    rect = patches.Rectangle(
        (obs['x'], obs['y']), obs['ancho'], obs['alto'],
        linewidth=1.5, edgecolor='black', facecolor='steelblue', alpha=0.6
    )
    ax.add_patch(rect)
    ax.text(obs['x'] + obs['ancho']/2, obs['y'] + obs['alto']/2,
            obs['id'], ha='center', va='center', fontsize=8, color='white', fontweight='bold')

# Dibujar trayectorias
for pid, datos in particulas.items():
    color = colores.get(pid, 'black')
    ax.plot(datos['x'], datos['y'], '-', color=color, linewidth=1.2,
            label=f"{pid} (masa final: {datos['masa'][-1]:.2f})")
    # Marcar inicio
    ax.plot(datos['x'][0], datos['y'][0], 'o', color=color, markersize=8)
    # Marcar fin
    ax.plot(datos['x'][-1], datos['y'][-1], 's', color=color, markersize=8)

# Marcar eventos de colisión entre partículas
for evento in eventos_particulas:
    # Extraer x e y del evento
    try:
        pos_str = [p for p in evento.split() if p.startswith('pos=')][0]
        coords = pos_str.replace('pos=(','').replace(')','').split(',')
        ex, ey = float(coords[0]), float(coords[1])
        ax.plot(ex, ey, '*', color='magenta', markersize=15, zorder=5)
        ax.annotate('Fusión', (ex, ey), textcoords='offset points',
                    xytext=(8, 8), fontsize=7, color='magenta')
    except:
        pass

ax.set_xlim(0, 950)
ax.set_ylim(0, 700)
ax.invert_yaxis()
ax.set_title("Trayectoria de partículas — Simulación de colisiones", fontsize=13)
ax.set_xlabel("Posición X")
ax.set_ylabel("Posición Y")
ax.legend(loc='upper right')
ax.grid(True, alpha=0.3)

# Resumen de eventos en consola
print(f"Partículas graficadas: {list(particulas.keys())}")
print(f"Colisiones con obstáculos: {len(eventos_obstaculo)}")
print(f"Colisiones entre partículas: {len(eventos_particulas)}")
for e in eventos_particulas:
    print(" ", e)

plt.tight_layout()
plt.show()
