#!/bin/bash
make -j
for i in -0.73304 -0.10472 0.52360 1.15192


do
	echo "using t = " $i
	./bin/ray_tracer $i -a
done
