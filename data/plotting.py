from scipy.stats import gaussian_kde
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

def bc_plot(filepath):
  df = pd.read_csv(filepath)
  x = df['incident_edges'].to_numpy()
  y = df['betweenness_centrality'].to_numpy()
  xy = np.vstack([x, y])
  z = gaussian_kde(xy)(xy)

  plt.scatter(x, y, c=z)
  plt.xscale('log')
  plt.title('Betweenness Centrality as a\nFunction of Incident Edges')
  plt.xlabel('Incident Edges (log)')
  plt.ylabel('Betweenness Centrality')
  plt.colorbar()
  plt.show()

def dijkstras_plot(filepath):
  df = pd.read_csv(filepath)
  x = df['incident_edges'].to_numpy()
  y = df['distance'].to_numpy()
  xy = np.vstack([x, y])
  z = gaussian_kde(xy)(xy)

  start = df[df['distance'] == 0].iloc[0]['address']

  plt.scatter(x, y, c=z)
  plt.xscale('log')
  plt.title(f'Distance from Start Vertex with Address {start} as a\nFunction of Incident Edges')
  plt.xlabel('Incident Edges (log)')
  plt.ylabel('Distance')
  plt.colorbar()
  plt.show()

def print_centrality(filepath):
  df = pd.read_csv(filepath)
  df['centrality_pct'] = df['betweenness_centrality'] / 13021800219242089
  print(df.head(20))

dijkstras_plot('data/dijkstras_outfile.csv')
# bc_plot('data/30mb_data_betweenness_centrality.csv')
