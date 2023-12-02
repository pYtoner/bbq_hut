import matplotlib.pyplot as plt
import numpy as np


def draw_tapestries(colors):
    def draw_trapezoid(ax, base1, base2, height, offset, colors, n_points=160):
        """
        Draw a trapezoid with a specified number of points on its edges.
        :param ax: Matplotlib axis to draw on.
        :param base1: Length of the top base of the trapezoid.
        :param base2: Length of the bottom base of the trapezoid.
        :param height: Height of the trapezoid.
        :param offset: Offset on the x-axis for positioning the trapezoid.
        :param n_points: Number of points to interpolate along the edges.
        """
        # Calculate coordinates of the trapezoid
        x1, y1 = offset, 2
        x2, y2 = x1 + base2, 2
        x3, y3 = x1 + (base2 - base1) / 2, height
        x4, y4 = x2 - (base2 - base1) / 2, height

        # Interpolate points along the edges
        left_side_x = np.linspace(x1, x3, n_points // 4)
        left_side_y = np.linspace(y1, y3, n_points // 4)
        right_side_x = np.linspace(x2, x4, n_points // 4)
        right_side_y = np.linspace(y2, y4, n_points // 4)
        top_side_x = np.linspace(x3, x4, n_points // 4)
        top_side_y = np.linspace(y3, y4, n_points // 4)
        bottom_side_x = np.linspace(x1, x2, n_points // 4)
        bottom_side_y = np.linspace(y1, y2, n_points // 4)

        # Combine points
        x_points = np.concatenate(
            [left_side_x, top_side_x, right_side_x, bottom_side_x])
        y_points = np.concatenate(
            [left_side_y, top_side_y, right_side_y, bottom_side_y])

        # Mark the interpolated points
        ax.scatter(x_points, y_points, s=10, c=colors)

    # Create a figure and axis
    fig, ax = plt.subplots()

    # Set limits and labels
    ax.set_xlim(0, 40)
    ax.set_ylim(0, 10)
    ax.set_xlabel('X-axis')
    ax.set_ylabel('Y-axis')
    ax.set_title('Trapezoids with 160 Points Each')

    # Common height for all trapezoids
    height = 7

    colors = [(r / 255, g / 255, b / 255) for (r, g, b) in colors]

    # Draw multiple trapezoids
    draw_trapezoid(ax, 3, 5, height, 0, colors[160 * 0: 160 * 1])
    draw_trapezoid(ax, 3, 5, height, 6, colors[160 * 1: 160 * 2])
    draw_trapezoid(ax, 3, 5, height, 12, colors[160 * 2: 160 * 3])
    draw_trapezoid(ax, 3, 5, height, 18, colors[160 * 3: 160 * 4])
    draw_trapezoid(ax, 3, 5, height, 24, colors[160 * 4: 160 * 5])
    draw_trapezoid(ax, 3, 5, height, 30, colors[160 * 5: 160 * 6])

    # Show plot
    plt.show()


colors = [(i % 160, 0, 255) for i in range(160 * 6)]
draw_tapestries(colors)
