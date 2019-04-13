# RT-v1
"My first raytracer", evolved.
Nearly (but really not nearly at all) a full raytracer (for the "RT" project). But most importantly a nice testing ground for 'object oriented' C and SIMD programming.
The code that was turned in for the RT-v1 project can still be found in the "rendu_rtv1" branch.

### How to compile
```
git clone https://framagit.org/awoimbee_42/rt-v1.git
cd rt-v1
#(optional) git checkout 'wathever branch you want to use'
make all clean
```

### How to use
./rt-v1 <scene_file>
Exemple:
`./rtv1 ./scenes/all_obj.rt`

![Exemple of a simple render](https://i.imgur.com/uMlxKmu.png)

We are able to (partially) render at a locked 30fps :
![low res render when moving](https://i.imgur.com/B1Bed8M.png) 