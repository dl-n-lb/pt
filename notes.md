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
rays are really simple, just an origin and a direction, with a single helper function to get the point some distance along the ray (this function assumes the direction is normalized).

## Camera time
Pinhole cameras are the most simple, so that's all we need for now
(TODO: diagram + explain where the calculations come from)

## Intersection Functions
### Sphere
Just a quadratic equation (TODO: diagram/working)
### Triangle
TODO

## Random vector
TODO: learn how this works lol
These might work, but I think too expensive?
https://stackoverflow.com/questions/6283080/random-unit-vector-in-multi-dimensional-space
https://en.wikipedia.org/wiki/Normal_distribution#Computational_methods
This one works:
https://math.stackexchange.com/questions/44689/how-to-find-a-random-axis-or-unit-vector-in-3d

## Shading
Using Monte Carlo + the rendering equation as:
https://en.wikipedia.org/wiki/Rendering_equation

## Triangle intersection

## Light sources

## BVH

## Wavefront methods?
