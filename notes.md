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
https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection.html

## Light sources
Simple to implement - just set some materials as emissive and when they get hit break early with the color of the light

## AA + focus blur
AA is achieved by jittering the target pixels of rays 
focus blur is achieved by calculating a focus plane, jittering ray origins and fixing the point on the focus plane through which they pass. (this is not a physically accurate method).

FIX: (as in rtiow)
Model as a thin lens + focus plane:
lens approximated by taking random samples in disk (lens shape)
focus plane replaces previous image plane
## disney BSDF ?

## direct light sampling - PDF!

## BVH

## Wavefront methods?
