import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import numpy as np
import colorsys

def hsv_to_rgb(hsv):
    """ Convert HSV to RGB. HSV values should be in the range [0, 1]. """
    return colorsys.hsv_to_rgb(*hsv)

def rgb_to_hsv(rgb):
    """ Convert RGB to HSV. RGB values should be in the range [0, 1]. """
    return colorsys.rgb_to_hsv(*rgb)

def interpolate_rgb(rgb1, rgb2, num_steps=10):
    """ Interpolate between two RGB colors. """
    r = np.linspace(rgb1[0], rgb2[0], num_steps)
    g = np.linspace(rgb1[1], rgb2[1], num_steps)
    b = np.linspace(rgb1[2], rgb2[2], num_steps)

    return [(ri, gi, bi) for ri, gi, bi in zip(r, g, b)]

# Convert HSV to RGB
start_color_hsv = (250/360, 1, 1)  # Normalized HSV
end_color_hsv = (55/360, 1, 1)     # Normalized HSV
start_color_rgb = hsv_to_rgb(start_color_hsv)
end_color_rgb = hsv_to_rgb(end_color_hsv)

# Interpolate in RGB
interpolated_colors_rgb = interpolate_rgb(start_color_rgb, end_color_rgb)

# Convert interpolated RGB colors back to HSV (if needed)
interpolated_colors_hsv = [rgb_to_hsv(rgb) for rgb in interpolated_colors_rgb]


# Plotting the interpolated colors
plt.figure(figsize=(10, 2))
for i, color in enumerate(interpolated_colors_rgb):
    plt.plot([i, i+1], [1, 1], color=color, lw=15)
plt.xlim(0, len(interpolated_colors_rgb))
plt.axis('off')
plt.title('Interpolation in HSV Space')
plt.show()
