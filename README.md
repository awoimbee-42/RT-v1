
# RT-v1
"My first raytracer", evolved.  
Nearly (but really not nearly at all) a full raytracer (for the "RT" project). But most importantly a nice testing ground for 'object oriented' C and SIMD programming.  
The code that was turned in for the RT-v1 project can still be found in the "rendu_rtv1" branch.

### How to compile
/!\ This project is meant to run on MacOS/Linux (but you might maybe be able to run it on windows with MinGW/Cygwin)
```
git clone https://framagit.org/awoimbee_42/rt-v1.git;
cd rt-v1;
make all clean
```

### How to use
./rt-v1 <scene_file>
Exemple:
`./rtv1 ./scenes/multiple_lights.rt`

![Exemple of a simple render](https://i.imgur.com/mGjcj5u.png)

We render at a minimum of 30fps, using lower resolutions if necessary :
![low res render when moving](https://i.imgur.com/B1Bed8M.png) 
