from draw_tapestries import draw_tapestries
import numpy as np

colors = [(i % 160, 0, 255) for i in range(160 * 6)]
draw_tapestries(colors)
