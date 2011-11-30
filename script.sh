#!/bin/bash
make -j
for i in -1.57080 -1.25664 -0.94248 -0.62832 -0.31416 0.00000 0.31416 0.62832 0.94248 1.25664 1.57080

do
	echo "using t = " $i
	./bin/ray_tracer $i -a
done
