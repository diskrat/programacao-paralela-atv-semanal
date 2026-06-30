import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation, PillowWriter

# 1. Carregue os dados do seu arquivo CSV
# Substitua 'seu_arquivo.csv' pelo nome do seu arquivo real
df = pd.read_csv('animacao_completa.csv')

# Obter os limites dos eixos e da cor para manter a consistência visual
x_min, x_max = df['x'].min(), df['x'].max()
y_min, y_max = df['y'].min(), df['y'].max()
z_min, z_max = df['z'].min(), df['z'].max()
mag_min, mag_max = df['magnitude'].min(), df['magnitude'].max()

# Obter a lista de frames únicos ordenados
frames = sorted(df['frame'].unique())
frame_inicial = frames[0]
frame_final = frames[-1]

# -------------------------------------------------------------------
# 2. NOVA PARTE: Função para salvar imagens estáticas
# -------------------------------------------------------------------
def salvar_frame_estatico(num_frame, nome_arquivo):
    # Criamos uma nova figura para não interferir na figura da animação
    fig_estatica = plt.figure(figsize=(10, 8))
    ax_estatico = fig_estatica.add_subplot(111, projection='3d')
    
    # Filtrar dados do frame específico
    df_frame = df[df['frame'] == num_frame]
    
    # Plotar
    scat_estatico = ax_estatico.scatter(df_frame['x'], df_frame['y'], df_frame['z'], 
                                        c=df_frame['magnitude'], cmap='plasma', 
                                        vmin=mag_min, vmax=mag_max, alpha=0.8)
    
    # Configurar eixos
    ax_estatico.set_xlim(x_min, x_max)
    ax_estatico.set_ylim(y_min, y_max)
    ax_estatico.set_zlim(z_min, z_max)
    ax_estatico.set_xlabel('Eixo X')
    ax_estatico.set_ylabel('Eixo Y')
    ax_estatico.set_zlabel('Eixo Z')
    ax_estatico.set_title(f'Simulação Navier-Stokes - Frame: {num_frame}')
    
    # Adicionar barra de cores
    cbar = fig_estatica.colorbar(scat_estatico, ax=ax_estatico, pad=0.1, shrink=0.7)
    cbar.set_label('Magnitude')
    
    # Salvar a imagem e fechar a figura para liberar memória
    plt.savefig(nome_arquivo, dpi=150, bbox_inches='tight')
    plt.close(fig_estatica)
    print(f"Imagem salva: {nome_arquivo}")

# Salvar o primeiro e o último frame
salvar_frame_estatico(frame_inicial, 'frame_inicial.png')
salvar_frame_estatico(frame_final, 'frame_final.png')


# -------------------------------------------------------------------
# 3. PARTE DA ANIMAÇÃO (GIF)
# -------------------------------------------------------------------
print("Iniciando a criação da animação...")

fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

scat = ax.scatter([], [], [], c=[], cmap='plasma', vmin=mag_min, vmax=mag_max, alpha=0.8)

ax.set_xlim(x_min, x_max)
ax.set_ylim(y_min, y_max)
ax.set_zlim(z_min, z_max)
ax.set_xlabel('Eixo X')
ax.set_ylabel('Eixo Y')
ax.set_zlabel('Eixo Z')

cbar = fig.colorbar(scat, ax=ax, pad=0.1, shrink=0.7)
cbar.set_label('Magnitude (Viscosidade)')

def update(frame_atual):
    df_frame = df[df['frame'] == frame_atual]
    
    # NOVO: Filtra os dados para remover pontos "mortos"
    # Você pode ajustar esse valor (0.1, 0.5, etc.) conforme necessário
    limiar = 0.1 
    df_visivel = df_frame[df_frame['magnitude'] > limiar]
    
    # Atualiza usando apenas os pontos visíveis
    scat._offsets3d = (df_visivel['x'], df_visivel['y'], df_visivel['z'])
    scat.set_array(df_visivel['magnitude'])
    
    ax.set_title(f'Simulação Navier-Stokes - Frame: {frame_atual}')
    return scat,
ani = FuncAnimation(fig, update, frames=frames, interval=100, blit=False)

nome_saida_gif = 'animacao_navier_stokes.gif'
ani.save(nome_saida_gif, writer=PillowWriter(fps=10))

print(f"Pronto! Animação salva como '{nome_saida_gif}'.")