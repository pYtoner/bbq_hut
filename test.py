from draw_tapestries import draw_tapestries

n_leds = [
    160,
    160,
    160,
    160,
    160,
    160,
]

side_lengths = [
    40, 20, 40, 60,
]

N_LEDS = sum(n_leds)


def side_index(tapestry_idx, idx, side, direction):
    tapestry_padding = 0

    i = 0
    while i < tapestry_idx:
        tapestry_padding += n_leds[i]

        i += 1

    side_padding = 0
    i = 0
    while i < side:
        side_padding += side_lengths[i]

        i += 1

    if idx >= side_lengths[side]:
        return N_LEDS

    if direction == 0:
        return tapestry_padding + side_padding + idx
    else:
        return tapestry_padding + side_padding + (side_lengths[side] - idx)


colors = [(0, 0, 0) for _ in range(160 * 6 + 1)]

for tapestry_idx in range(6):
    for side in range(4):
        for idx in range(side_lengths[side]):
            i = side_index(tapestry_idx, idx, side, 0)
            colors[i] = (side * 255 / 4, 0, 0)

draw_tapestries(colors)
