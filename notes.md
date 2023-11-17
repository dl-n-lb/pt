# Path Tracing
## first things first - outputting an image
Using the ppm format is an easy dev choice here; the format is simple
The header is just
```
P3
<width> <height>
255
```
Then the data follows in triples of (r, g, b) separated by new lines.

## next - vectors
c doesn't have overloading so the interface might be a bit awkward. I like subscripting so `v3` will be a union. I will add operations as I need them.

## Next, rays
